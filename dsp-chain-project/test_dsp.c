#include "core/dsp_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Simple WAV header structure (minimal for test)
typedef struct {
    char riff[4];
    int file_size;
    char wave[4];
    char fmt[4];
    int fmt_size;
    short format;
    short channels;
    int sample_rate;
    int byte_rate;
    short block_align;
    short bits_per_sample;
    char data[4];
    int data_size;
} WAVHeader;

// Generate a test sine wave
void generate_sine(float *buffer, int num_samples, float freq, float sample_rate) {
    for (int i = 0; i < num_samples; i++) {
        buffer[i] = sinf(2.0f * M_PI * freq * i / sample_rate);
    }
}

// Write WAV file (mono, 16-bit)
void write_wav(const char *filename, float *data, int num_samples, int sample_rate) {
    FILE *file = fopen(filename, "wb");
    if (!file) return;

    WAVHeader header = {
        {'R', 'I', 'F', 'F'}, 0, {'W', 'A', 'V', 'E'},
        {'f', 'm', 't', ' '}, 16, 1, 1, sample_rate,
        sample_rate * 2, 2, 16, {'d', 'a', 't', 'a'}, num_samples * 2
    };
    header.file_size = 36 + num_samples * 2;

    fwrite(&header, sizeof(WAVHeader), 1, file);

    for (int i = 0; i < num_samples; i++) {
        short sample = (short)(data[i] * 32767.0f);
        fwrite(&sample, sizeof(short), 1, file);
    }

    fclose(file);
}

int main() {
    const int sample_rate = 44100;
    const int num_samples = sample_rate * 2; // 2 seconds
    float *input = (float *)malloc(num_samples * sizeof(float));
    float *output = (float *)malloc(num_samples * sizeof(float));

    // Generate test signal (1kHz sine)
    generate_sine(input, num_samples, 1000.0f, sample_rate);

    // Test Biquad Low Shelf
    BiquadFilter filter;
    biquad_init_type(&filter, LOW_SHELF, 500.0f, 6.0f, 0.707f, sample_rate); // Boost 500Hz by 6dB
    biquad_process(&filter, input, output, num_samples);
    write_wav("test_low_shelf.wav", output, num_samples, sample_rate);

    // Test Gain Staging
    apply_gain(output, -3.0f, num_samples); // -3dB
    write_wav("test_gain.wav", output, num_samples, sample_rate);

    // Test Soft Clipping
    soft_clip(output, num_samples);
    write_wav("test_soft_clip.wav", output, num_samples, sample_rate);

    // Test Limiter
    limiter_process(output, 0.8f, num_samples); // Threshold at 0.8
    write_wav("test_limiter.wav", output, num_samples, sample_rate);

    // Test RMS Detector (print RMS level)
    RMSDetector rms;
    rms_init(&rms, 0.1f); // Alpha for smoothing
    float total_rms = 0.0f;
    for (int i = 0; i < num_samples; i++) {
        total_rms += rms_process(&rms, input[i]);
    }
    printf("Average RMS: %.4f\n", total_rms / num_samples);

    // Test FIR Filter (simple low-pass coefficients)
    float fir_coeffs[5] = {0.1f, 0.2f, 0.4f, 0.2f, 0.1f}; // Example coeffs
    FIRFilter fir;
    fir_init(&fir, fir_coeffs, 5);
    fir_process(&fir, input, output, num_samples);
    write_wav("test_fir.wav", output, num_samples, sample_rate);

    // Test FFT Convolution (simple IR)
    float ir[8] = {1.0f, 0.5f, 0.25f, 0.125f, 0.0f, 0.0f, 0.0f, 0.0f}; // Simple decaying IR
    float *conv_output = (float *)malloc((num_samples + 8 - 1) * sizeof(float));
    fft_convolve(input, ir, conv_output, num_samples, 8);
    write_wav("test_fft_conv.wav", conv_output, num_samples + 8 - 1, sample_rate);
    free(conv_output);

    free(input);
    free(output);

    printf("Test files generated: test_*.wav\n");
    printf("Use aplay or audacity to listen and verify.\n");

    return 0;
}
