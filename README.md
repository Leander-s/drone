# Requirements:
    Windows:
        python
        MinGW
        cmake
        arm-none-eabi compilers (see raspberrypi pico setup)
        gcc
    Linux:
        python
        cmake
        arm-none-eabi compilers (see raspberrypi pico setup)
        gcc

# Init:
    run init.py
    (Does not reliably work on windows atm)

# Build:
    Pico:
        from "drone/code/general/" directory run "python build.py b" to build the output
        from "drone/code/general/" directory run "python build.py r" to rebuild the files 
            (deletes all previously built files and rebuilds them)

        3 Output files:
            drone/code/general/drone/build -> *.uf2 goes in the drones pico
            drone/code/general/ground/pc/build -> *.exe or a unix executable for pc
            drone/code/general/ground/pico/build -> *.uf2 goes in pico connected to pc as transceiver

<!--
drone/code/pico/*:
    This is where some tests for the pico and the pico-sdk are

drone/code/general/communications/nrf24:
    communications is the subproject for the communications between pc/drone
    nrf24 is the library that provides for a way to send data using nrf24 modules
    and the pico

nrf24:
    This is how the module should be wired https://coffeebreakpoint.com/wp-content/uploads/2021/05/pico-nrf24L01_wiring-768x763.png.
    
pico read timouts are an issue. The nrf24 read on the pico transceiver times out.
Both pico seem to be stuck on read to make this happen. Need to sync better somehow.
        
TODO:
OpenGL implementation (need to do all the rendering again)
    - depth buffer to get actual sides instead of lines
    - better model
    - maybe some lighting to get a sense of the orientation
this shouldnt be that hard but it sounds like a lot so I will take a day

Better cmake setup

Everything math related needs to be handled by glm (vectors, matrices) -> get
rid of my math code

transmissions per second needs to be averaged and updated every second or something so its actually readable

make FloatBytes something other than a union. Union is too breakable.

Windows compatibility (done?):
    connection.h/.c
    groundreceiver (path_to_port only)
    build scripts/git script
        make all scripts in python

Foamboard frame

Pico hat maximal 10 Mhz übertragungsrate also nicht ausreichend für video.
-->
