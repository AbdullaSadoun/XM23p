# XM23p
Emulator for the xm23p cpu

# Loader
## Background
This portion of the system is responsible for loading the instructions taken from the s-records (.xme) file which is accquired after assembeling the .asm file, and it seperates, decodes, and saves the content of these s-records. In my CPU's architecture (HARVARD). The instructions are saved in a different memory space, seperate from the data. 

# Overview
A comprehensive CPU emulator that simulates the XM23P processor architecture, providing a platform for understanding computer architecture and assembly programming.

# The Challenge
Accurately implementing the complex instruction set while maintaining cycle-accurate timing and proper memory management.

# Solution
Developed a modular architecture with separate components for instruction decode, execute, and memory management, ensuring accurate processor behavior.

# Key Features
- Full instruction set implementation
- Memory management and addressing
- Register simulation
- Debugging interface
- Step-by-step execution

# Tech Stack

# Results
Successfully created a functional CPU emulator that serves as both an educational tool and a platform for testing XM23P assembly code.
