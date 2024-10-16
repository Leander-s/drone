Requirements:
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

Init:
    run init.py

Build:
    from "drone/code/general/" directory run "python build.py b" to build the output
    from "drone/code/general/" directory run "python build.py r" to rebuild the files 
        (deletes all previously built files and rebuilds them)

    3 Output files:
        drone/code/general/drone/build -> *.uf2 goes in the drones pico
        drone/code/general/ground/pc/build -> *.exe/*(unix executable) for pc
        drone/code/general/ground/pico/build -> *.uf2 goes in pico connected to pc as transceiver

    right now, all the subprojects produce static libraries that go in the relevant lib folders.
    For example drone/code/general/drone/lib for drone files

    there is a build.py and rebuild.py in every subproject directory. 
    To build only one subprojects and tests (if given), run build.py or rebuild.py
    in the subprojects folder.

IMPORTANT:
    as of right now, you MUST manually set the COM port/dev file for the usb connection to the pico transceiver
    otherwise, it might just not be the right port
    the port can be set in drone/code/general/communications/ground/pc_side/src/main.c

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
transmissions per second needs to be averaged and updated every second or something so its actually readable

make FloatBytes something other than a union. Union is too breakable.

Windows compatibility (done?):
    connection.h/.c
    groundreceiver (path_to_port only)
    build scripts/git script
        make all scripts in python

Foamboard frame

Pico hat maximal 10 Mhz übertragungsrate also nicht ausreichend für video.
