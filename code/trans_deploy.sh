#!/bin/bash

./build trans

if [ -d "/media/leander/RPI-RP2" ]
then
    sudo cp ./pico/bin/transceiver.uf2 /media/leander/RPI-RP2/
else
    sudo mkdir /media/leander/RPI-RP2
    sudo mount /dev/sdd1 /media/leander/RPI-RP2
    sudo cp ./pico/bin/transceiver.uf2 /media/leander/RPI-RP2/
    sudo umount /media/leander/RPI-RP2
    sudo rm -rf /media/leander/RPI-RP2
fi
