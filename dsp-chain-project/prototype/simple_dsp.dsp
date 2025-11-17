// Simple Compressor Prototype in Faust
// A basic compressor to reduce dynamic range

import("stdfaust.lib");

// Simple compressor: threshold -10dB, ratio 4:1, attack 0.1s, release 0.1s
compressor = co.compressor_mono(4, -10, 0.1, 0.1);

process = compressor;
