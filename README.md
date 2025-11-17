# Project name
DSP Chain Project

Overview
This project processes audio on Kali Linux. It supports gain, limiter, soft clip, filters, and convolution. It uses C and C plus plus. It builds correctly on Kali with CMake.

Supported on Kali
You can build and run this project on Kali. Kali supports gcc, g plus plus, cmake, and pkg config.

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
• WAV audio player
• Standard C library

Install guide for Kali Linux

Open a terminal.

Update packages.

sudo apt update

@ Install build tools.

sudo apt install build essential cmake pkg config

Clone the repository.

git clone https colon slash slash github dot com slash SuvankarRv slash dsp chain project dot git

Enter the project folder.

cd dsp chain project
cd dsp chain project

Create a build folder.

mkdir build

Enter the build folder.

cd build

Run CMake.

cmake two dots

Build the project.

make

Verify the output files.

ls

Folder info
• core contains DSP algorithms
• plugin builds the shared library
• test dsp runs processing
• WAV files are included for testing

Usage on Kali

Enter the build folder.

cd build

Run the test program with a WAV file.

./test dsp test input.wav

The program writes a processed WAV file in the same folder.

Play the output with any audio player in Kali.

Run examples

Gain test

./test dsp test gain.wav

FIR filter test

./test dsp test fir.wav

FFT convolution test

./test dsp test fft conv.wav

Limiter test

./test dsp test limiter.wav

Low shelf filter test

./test dsp test low shelf.wav

Soft clip test

./test dsp test soft clip.wav

Purpose
• Learn DSP
• Test audio effects
• Study code
• Build small audio tools
