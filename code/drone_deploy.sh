#!/bin/bash

./build drone

if [ -d "/media/leander/RPI-RP2" ]
then
    sudo cp ./bin/drone.uf2 /media/leander/RPI-RP2/
else
    sudo mkdir /media/leander/RPI-RP2
    sudo mount /dev/sdd1 /media/leander/RPI-RP2
    sudo cp ./bin/drone.uf2 /media/leander/RPI-RP2/
    sudo umount /media/leander/RPI-RP2
    sudo rm -rf /media/leander/RPI-RP2
fi
