#ifndef DSP_CORE_H
#define DSP_CORE_H

#include <stdint.h>

// DSP Core Structures
typedef struct {
    float *buffer;
    int size;
    int write_pos;
} DelayBuffer;

typedef enum {
    LOW_SHELF,
    HIGH_SHELF,
    PEAKING
} BiquadType;

typedef struct {
    float a0, a1, a2, b1, b2; // Filter coefficients
    float x1, x2, y1, y2;     // Filter state
    float inv_sample_rate;    // Precomputed 1/sample_rate for optimization
} BiquadFilter;

typedef struct {
    float *coeffs;
    int length;
    float *state;
    int state_len;
} FIRFilter;

typedef struct {
    float rms;
    float alpha;
} RMSDetector;

// Function prototypes
void delay_init(DelayBuffer *delay, int max_delay_samples);
void delay_process(DelayBuffer *delay, float *input, float *output, int delay_samples, float feedback, int num_samples);

void biquad_init_type(BiquadFilter *filter, BiquadType type, float freq, float gain_db, float q, float sample_rate);
void biquad_process(BiquadFilter *filter, float *input, float *output, int num_samples);

void fir_init(FIRFilter *filter, float *coeffs, int length);
void fir_process(FIRFilter *filter, float *input, float *output, int num_samples);

void rms_init(RMSDetector *rms, float alpha);
float rms_process(RMSDetector *rms, float input);

void apply_gain(float *buffer, float gain_db, int num_samples);
void soft_clip(float *buffer, int num_samples);
void limiter_process(float *buffer, float threshold, int num_samples);

void fft_convolve(float *input, float *ir, float *output, int input_len, int ir_len);

#endif // DSP_CORE_H
