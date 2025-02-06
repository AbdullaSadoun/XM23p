# XM23P CPU Emulator
A sophisticated emulator that simulates the XM23P processor architecture, providing a robust platform for computer architecture education and assembly programming development.

## Overview
This project implements a complete CPU emulator that accurately simulates the XM23P processor architecture. It features a Harvard architecture design with separate instruction and data memory spaces, making it an excellent tool for understanding advanced computer architecture concepts and developing assembly programs.

## Core Features
- **Harvard Architecture Implementation**: Separate instruction (IMEM) and data (DMEM) memory spaces
- **Complete Instruction Set**: Full implementation of the XM23P instruction set
- **Advanced Memory Management**: Sophisticated addressing modes and memory operations
- **Register System**: Accurate simulation of processor registers and PSW (Program Status Word)
- **Comprehensive Debugging Suite**:
  - Breakpoint functionality
  - Step-by-step execution
  - Memory inspection and modification
  - Register state monitoring
  - Cycle-accurate timing simulation

## Technical Architecture
### Loader
- Processes S-record (.xme) files from assembled code
- Implements intelligent memory mapping for Harvard architecture
- Handles memory segmentation and address space management

### Core Components
1. **Instruction Decoder**
   - Precise instruction parsing and decoding
   - Support for multiple addressing modes
   - Efficient opcode handling

2. **Execution Unit**
   - Cycle-accurate instruction execution
   - ALU operations simulation
   - Branch and control flow handling

3. **Memory Controller**
   - Separate IMEM and DMEM management
   - Memory access synchronization
   - Address space protection

4. **Debug Interface**
   - Real-time system state monitoring
   - Interactive debugging capabilities
   - Memory and register modification

## Technical Stack
- Language: C
- Architecture: Harvard
- Build System: GCC
- Development Tools: Make, GDB

## Development Highlights
- Implemented complex instruction timing and execution cycles
- Created sophisticated memory management systems
- Developed comprehensive debugging capabilities
- Ensured cycle-accurate simulation
- Built modular and maintainable architecture

## Usage
```bash
gcc -o main main.c debugger.c decode.c execute.c fetch.c loader.c xm23p.c run.c psw.c BRANCHinstructions.c MEMinstructions.c REGinstructions.c cex.c
./main

# Key Technical Achievements
Successfully implemented Harvard architecture with separate memory spaces
Achieved cycle-accurate instruction execution
Created comprehensive debugging system with multiple viewing modes
Developed flexible loader system for S-record handling
Implemented full XM23P instruction set with accurate timing
Educational Value
This emulator serves as both an educational tool and a practical platform for:

# Understanding computer architecture principles
Learning assembly programming
Studying processor design
Exploring instruction set architectures
Debugging and testing assembly code
Future Enhancements
Performance optimization for larger programs
Enhanced debugging features
GUI interface development
Extended instruction set support
Cross-platform compatibility