# README

This Tango server connectos th the Frequency divider.
The frequency divider is connected via an USB cable to the computer.
The software on the divider recieves the divider setting over the serial protocol

## Setup

### Permission
In order to be able to connect to the serial port, the linux user needs the permissions

  sudo usermod -a -G dialout $USER

Log out and log in again

Find the serial port, best with 

  dmesg | grep tty

### Packages

The python programm nees the pyserial libraries


## Debug

## Serial GUI
To debug the serial connection, one can use the programm cutecom.

## Permission denied
You could try

   sudo chmod 666 /dev/ttyACM0

Which will give you read/write permissions for the serial port (thanks to https://discourse.nodered.org/t/serial-port-issue-permission-denied-cannot-open-dev-ttyacm0/4500/2)
