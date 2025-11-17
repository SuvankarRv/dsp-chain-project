# Project name
DSP Chain Project

Overview
This project runs audio processing on Kali Linux. It includes gain, limiter, soft clip, filters, and convolution. The code uses C and C plus plus. The project builds correctly on Kali with CMake.

Supported on Kali
You can build and run the project on Kali without extra setup. Kali supports gcc, g plus plus, cmake, and pkg config.

Features
• Gain processing
• FIR convolution
• FFT convolution
• Limiter
• Soft clip
• Low shelf filter
• Shared library plugin
• Test WAV files included

Requirements for Kali
• GCC and G plus plus
• CMake
• Pkg config
• Any WAV audio player
• Standard C library

Install on Kali

Update packages
sudo apt update

Install tools
sudo apt install build essential cmake pkg config

Clone the repo
git clone https colon slash slash github dot com slash SuvankarRv slash dsp chain project dot git

Enter the project folder
cd dsp chain project slash dsp chain project

Create build folder
mkdir build
cd build

Configure
cmake two dots

Build
make

Folder info
• core contains DSP algorithms
• plugin creates the shared library
• test dsp runs the DSP chain
• WAV files are provided for testing

Usage on Kali
• Move to the build folder
• Run test dsp with a WAV file
./test dsp test input.wav
• The program creates a new processed WAV file
• Play the output WAV with any audio player in Kali

Purpose
• Learn simple DSP
• Test audio effects
• Experiment with code
• Build shared library plugins
