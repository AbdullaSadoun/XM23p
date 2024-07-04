# XM23p
Emulator for the xm23p cpu

# Loader
## Background
This portion of the system is responsible for loading the instructions taken from the s-records (.xme) file which is accquired after assembeling the .asm file, and it seperates, decodes, and saves the content of these s-records. In my CPU's architecture (HARVARD). The instructions are saved in a different memory space, seperate from the data. 