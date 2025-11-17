#include "dsp_core.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Delay Effect Implementation
void delay_init(DelayBuffer *delay, int max_delay_samples) {
    delay->buffer = (float *)malloc(max_delay_samples * sizeof(float));
    memset(delay->buffer, 0, max_delay_samples * sizeof(float));
    delay->size = max_delay_samples;
    delay->write_pos = 0;
}

void delay_process(DelayBuffer *delay, float *input, float *output, int delay_samples, float feedback, int num_samples) {
    for (int i = 0; i < num_samples; i++) {
        int read_pos = (delay->write_pos - delay_samples + delay->size) % delay->size;
        float delayed_sample = delay->buffer[read_pos];
        output[i] = input[i] + delayed_sample * feedback;
        delay->buffer[delay->write_pos] = input[i] + delayed_sample * feedback;
        delay->write_pos = (delay->write_pos + 1) % delay->size;
    }
}

// Biquad Filter Implementation
void biquad_init_type(BiquadFilter *filter, BiquadType type, float freq, float gain_db, float q, float sample_rate) {
    filter->inv_sample_rate = 1.0f / sample_rate; // Precompute for optimization
    float omega = 2.0f * M_PI * freq * filter->inv_sample_rate;
    float sin_omega = sinf(omega);
    float cos_omega = cosf(omega);
    float alpha = sin_omega / (2.0f * q);
    float A = powf(10.0f, gain_db / 40.0f); // Linear gain from dB

    float a0, a1, a2, b0, b1, b2;

    switch (type) {
        case LOW_SHELF:
            b0 = A * ((A + 1.0f) - (A - 1.0f) * cos_omega + 2.0f * sqrtf(A) * alpha);
            b1 = 2.0f * A * ((A - 1.0f) - (A + 1.0f) * cos_omega);
            b2 = A * ((A + 1.0f) - (A - 1.0f) * cos_omega - 2.0f * sqrtf(A) * alpha);
            a0 = (A + 1.0f) + (A - 1.0f) * cos_omega + 2.0f * sqrtf(A) * alpha;
            a1 = -2.0f * ((A - 1.0f) + (A + 1.0f) * cos_omega);
            a2 = (A + 1.0f) + (A - 1.0f) * cos_omega - 2.0f * sqrtf(A) * alpha;
            break;
        case HIGH_SHELF:
            b0 = A * ((A + 1.0f) + (A - 1.0f) * cos_omega + 2.0f * sqrtf(A) * alpha);
            b1 = -2.0f * A * ((A - 1.0f) + (A + 1.0f) * cos_omega);
            b2 = A * ((A + 1.0f) + (A - 1.0f) * cos_omega - 2.0f * sqrtf(A) * alpha);
            a0 = (A + 1.0f) - (A - 1.0f) * cos_omega + 2.0f * sqrtf(A) * alpha;
            a1 = 2.0f * ((A - 1.0f) - (A + 1.0f) * cos_omega);
            a2 = (A + 1.0f) - (A - 1.0f) * cos_omega - 2.0f * sqrtf(A) * alpha;
            break;
        case PEAKING:
            b0 = 1.0f + alpha * A;
            b1 = -2.0f * cos_omega;
            b2 = 1.0f - alpha * A;
            a0 = 1.0f + alpha / A;
            a1 = -2.0f * cos_omega;
            a2 = 1.0f - alpha / A;
            break;
        default:
            // Default to low-pass if invalid type
            a0 = 1.0f + alpha;
            b0 = (1.0f - cos_omega) / 2.0f;
            b1 = 1.0f - cos_omega;
            b2 = (1.0f - cos_omega) / 2.0f;
            a1 = -2.0f * cos_omega;
            a2 = 1.0f - alpha;
            break;
    }

    filter->a0 = b0 / a0;
    filter->a1 = b1 / a0;
    filter->a2 = b2 / a0;
    filter->b1 = a1 / a0;
    filter->b2 = a2 / a0;

    filter->x1 = filter->x2 = filter->y1 = filter->y2 = 0.0f;
}

void biquad_process(BiquadFilter *filter, float *input, float *output, int num_samples) {
    for (int i = 0; i < num_samples; i++) {
        float x0 = input[i];
        float y0 = filter->a0 * x0 + filter->a1 * filter->x1 + filter->a2 * filter->x2 -
                   filter->b1 * filter->y1 - filter->b2 * filter->y2;

        output[i] = y0;

        filter->x2 = filter->x1;
        filter->x1 = x0;
        filter->y2 = filter->y1;
        filter->y1 = y0;
    }
}

// FIR Filter Implementation
void fir_init(FIRFilter *filter, float *coeffs, int length) {
    filter->coeffs = (float *)malloc(length * sizeof(float));
    memcpy(filter->coeffs, coeffs, length * sizeof(float));
    filter->length = length;
    filter->state = (float *)malloc(length * sizeof(float));
    memset(filter->state, 0, length * sizeof(float));
    filter->state_len = length;
}

void fir_process(FIRFilter *filter, float *input, float *output, int num_samples) {
    for (int i = 0; i < num_samples; i++) {
        // Shift state
        for (int j = filter->state_len - 1; j > 0; j--) {
            filter->state[j] = filter->state[j - 1];
        }
        filter->state[0] = input[i];

        // Convolution
        output[i] = 0.0f;
        for (int j = 0; j < filter->length; j++) {
            output[i] += filter->coeffs[j] * filter->state[j];
        }
    }
}

// RMS Detector Implementation
void rms_init(RMSDetector *rms, float alpha) {
    rms->rms = 0.0f;
    rms->alpha = alpha;
}

float rms_process(RMSDetector *rms, float input) {
    rms->rms = rms->alpha * rms->rms + (1.0f - rms->alpha) * input * input;
    return sqrtf(rms->rms);
}

// Gain Staging
void apply_gain(float *buffer, float gain_db, int num_samples) {
    float gain_linear = powf(10.0f, gain_db / 20.0f); // Precompute gain
    for (int i = 0; i < num_samples; i++) {
        buffer[i] *= gain_linear;
    }
}

// Soft Clipping
void soft_clip(float *buffer, int num_samples) {
    for (int i = 0; i < num_samples; i++) {
        buffer[i] = tanhf(buffer[i]);
    }
}

// Limiter
void limiter_process(float *buffer, float threshold, int num_samples) {
    for (int i = 0; i < num_samples; i++) {
        if (buffer[i] > threshold) {
            buffer[i] = threshold;
        } else if (buffer[i] < -threshold) {
            buffer[i] = -threshold;
        }
    }
}

// FFT Convolution (Basic implementation, assumes power-of-2 lengths)
// Note: For production, integrate kissfft or similar for real FFT implementation
static void fft(float *data, int n, int is_inverse) {
    // Placeholder: Implement radix-2 FFT here for real-time use
    // This stub does no processing; replace with actual FFT code
}

void fft_convolve(float *input, float *ir, float *output, int input_len, int ir_len) {
    int n = 1;
    while (n < input_len + ir_len - 1) n <<= 1; // Next power of 2

    float *fft_input = (float *)malloc(2 * n * sizeof(float));
    float *fft_ir = (float *)malloc(2 * n * sizeof(float));
    float *fft_result = (float *)malloc(2 * n * sizeof(float));

    memset(fft_input, 0, 2 * n * sizeof(float));
    memset(fft_ir, 0, 2 * n * sizeof(float));

    // Copy input and IR (real parts, imaginary set to 0)
    for (int i = 0; i < input_len; i++) fft_input[2 * i] = input[i];
    for (int i = 0; i < ir_len; i++) fft_ir[2 * i] = ir[i];

    // FFT (stub - no-op)
    fft(fft_input, n, 0);
    fft(fft_ir, n, 0);

    // Multiply in frequency domain (complex multiplication)
    for (int i = 0; i < n; i++) {
        float real1 = fft_input[2 * i], imag1 = fft_input[2 * i + 1];
        float real2 = fft_ir[2 * i], imag2 = fft_ir[2 * i + 1];
        fft_result[2 * i] = real1 * real2 - imag1 * imag2;
        fft_result[2 * i + 1] = real1 * imag2 + imag1 * real2;
    }

    // IFFT (stub - no-op)
    fft(fft_result, n, 1);

    // Copy result and normalize
    for (int i = 0; i < input_len + ir_len - 1; i++) {
        output[i] = fft_result[2 * i] / n;
    }

    free(fft_input);
    free(fft_ir);
    free(fft_result);
}
