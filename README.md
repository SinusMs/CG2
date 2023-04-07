# Computer Graphics 2 - exercise repository

This repository contains the code which you are going to work with over the course of the CG2 exercises. It contains the CGV Framework as a submodule, so be sure to clone the repository using the `--recursive` flag, like so:
>`git clone https://bitbucket.org/cgvtud/cg2.git --recursive`

If you forget adding the flag when cloning, you can run
>`git submodule init`

>`git submodule update --remote`

from within your local repository root later on.

## Development environment

The CGV Framework is cross-platform and supports Windows and Linux (MacOS is supported in theory, but completely untested and likely to not work without adjustments). For the exercise however, only building on Windows and Visual Studio is officially supported. CMake build files are included in the exercise and you are free to develop on the OS of your choice, but due to the vast multitude of different configurations and setups, we _cannot_ and _*will not*_ provide assistance if you encounter trouble building or running on anything other than Windows.

## Building the exercises

Refer to the file HowToBuild.pdf in the repository root for illustrated instructions on how to set up the official Windows build environment and build the exercise code.
Advanced users may opt to build with CMake – however, the Framework only supports Visual Studio generators (VS2017 or later) on Windows.

## Linux pointers

To solve the exercise in a productive way under Linux, we do recommend using an IDE. Many current offerings (like the open source Visual Studio Code editor) support opening a CMake-enabled source tree directly. Currently, the Framework CMake build system will automatically generate launch and debug configurations for VS Code. But if you use any other IDE, you'll have to create launch/debug configs yourself. Shell scripts for launching the exercises will be created in your chosen CMake build directory, named `run_CG2_exercise1.sh`, `run_CG2_exercise2.sh` and `run_CG2_exercise45.sh`, respectively. You can inspect them to find out how to launch the `cgv_viewer` binary and provide it with the necessary command line arguments for each exercise, which you can then adapt for your IDE.
