<<<<<<< HEAD
# DSP Chain Project

This project demonstrates building a DSP (Digital Signal Processing) chain for audio processing, from prototype to production plugin.

## Structure

- `prototype/`: Faust prototype for quick testing
- `core/`: C implementation of DSP algorithms
- `plugin/`: LV2 plugin wrapper

## Building and Testing

### Prerequisites

Install required packages:
```bash
sudo apt update
sudo apt install faust lv2-dev cmake build-essential pkg-config
```

### Build the Plugin

```bash
cd dsp-chain-project
mkdir build
cd build
cmake ..
make
sudo make install
```

### Test the Prototype

Use Faust to compile and test the prototype:
```bash
faust -a alsa-gtk prototype/simple_dsp.dsp -o simple_dsp.cpp
g++ -std=c++11 simple_dsp.cpp -o simple_dsp `pkg-config --cflags --libs jack gtk+-2.0`
./simple_dsp
```

### Load Plugin in EasyEffects

1. Open EasyEffects
2. Add a new effect
3. Search for "DSP Chain" or the plugin URI
4. Adjust parameters: Cutoff Frequency, Delay Time, Feedback

## DSP Chain Description

The DSP chain consists of:
1. Low-pass filter (adjustable cutoff)
2. Delay effect with feedback

Parameters:
- Cutoff Frequency: 20-20000 Hz
- Delay Time: 0-2 seconds
- Feedback: 0-0.99

## Optimization Notes

- Core algorithms are implemented in C for performance
- LV2 plugin ensures real-time capability
- Use with PipeWire for low-latency audio routing
=======
# dsp-chain-project
>>>>>>> 8b7f18da34839c9c021d956b60cb1e481dbbedcd
