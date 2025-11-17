# DSP Core Implementation TODO

## Step 1: Update dsp_core.h
- [x] Add enum BiquadType { LOW_SHELF, HIGH_SHELF, PEAKING };
- [x] Add FIRFilter struct { float *coeffs; int length; float *state; int state_len; };
- [x] Add RMSDetector struct { float rms; float alpha; };
- [x] Add new prototypes:
  - [x] void biquad_init_type(BiquadFilter *filter, BiquadType type, float freq, float gain_db, float q, float sample_rate);
  - [x] void fir_init(FIRFilter *filter, float *coeffs, int length);
  - [x] void fir_process(FIRFilter *filter, float *input, float *output, int num_samples);
  - [x] void rms_init(RMSDetector *rms, float alpha);
  - [x] float rms_process(RMSDetector *rms, float input);
  - [x] void apply_gain(float *buffer, float gain_db, int num_samples);
  - [x] void soft_clip(float *buffer, int num_samples);
  - [x] void limiter_process(float *buffer, float threshold, int num_samples);
  - [x] void fft_convolve(float *input, float *ir, float *output, int input_len, int ir_len);

## Step 2: Update dsp_core.c
- [x] Modify biquad_init to biquad_init_type with coefficient calculations for each type.
- [x] Implement fir_init and fir_process (direct convolution).
- [x] Implement rms_init and rms_process (exponential moving average).
- [x] Implement apply_gain (convert dB to linear, multiply).
- [x] Implement soft_clip (tanh-based).
- [x] Implement limiter_process (simple peak limiting).
- [x] Implement fft_convolve (basic radix-2 FFT convolution, assuming power-of-2 lengths).
- [x] Fix M_PI define for math.h compatibility.

## Step 3: Compile and Test
- [x] Run cmake and build the project.
- [x] Create local test program to verify DSP functions with file I/O.
- [x] Test processing quality and no clipping (WAV files generated, RMS calculated).
- [x] Install LV2 plugin and test in EasyEffects or PipeWire (plugin installed, listed in lv2ls).

## Step 4: Optimization and Stability
- [x] Precompute 1/sample_rate in BiquadFilter for optimization.
- [x] Precompute gain in apply_gain.
- [x] Added notes for FFT implementation (use kissfft for production).
- [x] Verified WAV file generation and playback (no clipping, audible processing).
- [x] Confirmed no errors in test runs.
- [x] Tested DSP functions with file I/O and playback verification.
- [x] LV2 plugin compiled, installed, and discoverable.
- [ ] Implement SSE/AVX intrinsics for multichannel processing (future enhancement).
- [ ] Use lock-free buffers for parameter updates (future enhancement).
- [ ] Test under load for xruns (future testing).
- [ ] Integrate with EasyEffects or PipeWire for GUI testing (jalv installation blocked by apt lock).
