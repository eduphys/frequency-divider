#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jun 30 11:09:01 2020

@author: simon
License: Apache License, see https://www.apache.org/licenses/LICENSE-2.0
"""

#import threading
# we need the library provided by pyserial
# e.g get it with pip3 install --user pyserial
import serial 

import time

class SerialCom():
    """ Impelents the communication with
    the FrequencyDivider
    """
    
  
    
    def __init__(self, port,debug=True):
        """self.ser = serial.Serial(
           port=port,
           baudrate = 9600,
           parity=serial.PARITY_NONE,
           stopbits=serial.STOPBITS_ONE,
           bytesize=serial.EIGHTBITS,
           timeout=2
           )"""
        
        self.serial = serial.Serial()
        self.serial.baudrate = 9600
        self.serial.port = port
        self.serial.timeout=0.8
        
        self.debug = debug
        
        self.divider = 0
        
        
    def __del__(self):
        if self.is_open():
            self.close()
           
    def open(self):
        self.serial.open()
        
    def close(self):
        self.serial.close()
        
    def is_open(self):
        return self.serial.is_open
    
    def read_divider(self):
        self.serial.read(1000)
        bytestring_read=b'd\n'
        self.serial.write(bytestring_read)
        self.serial.flush()
        return_bytestring = self.serial.read_until()
        if return_bytestring == b'':
            self.serial.read(1000) #MAGIC. Clears somehow the buffer
            self.serial.write(bytestring_read)
            self.serial.flush()
            return_bytestring = self.serial.read_until()
            
        self.divider = int(return_bytestring)
        return self.divider
    
    def set_divider(self, value):
        
        bytestring = "D{}\n".format(value).encode('ASCII')
        self.serial.read(1000) #MAGIC. Clears somehow the buffer
        self.serial.write(bytestring)
        self.serial.flush()
        return_bytestring = self.serial.read_until()
        self.divider = int(return_bytestring)
        return self.divider
        
    
    def _clearbuffer(self):
        self.serial.reset_input_buffer()
        self.serial.reset_output_buffer()
        
        


            
            
                
    
