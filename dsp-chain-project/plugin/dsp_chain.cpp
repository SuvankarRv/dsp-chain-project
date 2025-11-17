#include <lv2/core/lv2.h>
#include <lv2/atom/atom.h>
#include <lv2/atom/forge.h>
#include <lv2/midi/midi.h>
#include <lv2/urid/urid.h>
#include <lv2/log/log.h>
#include <lv2/log/logger.h>
#include <lv2/core/lv2_util.h>

#include "../core/dsp_core.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

// Plugin URI
#define DSP_CHAIN_URI "http://example.org/dsp-chain"

// Plugin structure
typedef struct {
    // LV2 ports
    const float* input;
    float* output;
    const float* cutoff_freq;
    const float* delay_time;
    const float* feedback;

    // DSP components
    BiquadFilter filter;
    DelayBuffer delay;

    // Sample rate
    double sample_rate;
} DSPChain;

// LV2 descriptor functions
static LV2_Handle instantiate(const LV2_Descriptor* descriptor, double rate, const char* bundle_path, const LV2_Feature* const* features) {
    DSPChain* plugin = (DSPChain*)malloc(sizeof(DSPChain));
    if (!plugin) return NULL;

    plugin->sample_rate = rate;

    // Initialize DSP components
    biquad_init_type(&plugin->filter, LOW_SHELF, 1000.0f, 0.0f, 0.707f, rate); // Default low shelf at 1000Hz, 0dB gain, Q=0.707
    delay_init(&plugin->delay, (int)(2.0 * rate)); // Max 2 seconds delay

    return (LV2_Handle)plugin;
}

static void connect_port(LV2_Handle instance, uint32_t port, void* data) {
    DSPChain* plugin = (DSPChain*)instance;

    switch (port) {
        case 0: plugin->input = (const float*)data; break;
        case 1: plugin->output = (float*)data; break;
        case 2: plugin->cutoff_freq = (const float*)data; break;
        case 3: plugin->delay_time = (const float*)data; break;
        case 4: plugin->feedback = (const float*)data; break;
    }
}

static void activate(LV2_Handle instance) {
    // Nothing to do here
}

static void run(LV2_Handle instance, uint32_t n_samples) {
    DSPChain* plugin = (DSPChain*)instance;

    // Update filter if cutoff changed
    if (*plugin->cutoff_freq != plugin->filter.a0) { // Simple check, could be improved
        biquad_init_type(&plugin->filter, LOW_SHELF, *plugin->cutoff_freq, 0.0f, 0.707f, plugin->sample_rate);
    }

    // Temporary buffer for intermediate processing
    float* temp = (float*)malloc(n_samples * sizeof(float));
    if (!temp) return;

    // Apply filter first
    biquad_process(&plugin->filter, (float*)plugin->input, temp, n_samples);

    // Apply delay
    int delay_samples = (int)(*plugin->delay_time * plugin->sample_rate);
    delay_process(&plugin->delay, temp, plugin->output, delay_samples, *plugin->feedback, n_samples);

    free(temp);
}

static void deactivate(LV2_Handle instance) {
    // Nothing to do here
}

static void cleanup(LV2_Handle instance) {
    DSPChain* plugin = (DSPChain*)instance;

    free(plugin->delay.buffer);
    free(plugin);
}

static const void* extension_data(const char* uri) {
    return NULL;
}

// LV2 descriptor
static const LV2_Descriptor descriptor = {
    DSP_CHAIN_URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};

LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index) {
    switch (index) {
        case 0: return &descriptor;
        default: return NULL;
    }
}
