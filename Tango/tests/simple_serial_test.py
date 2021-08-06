#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Aug  3 11:46:06 2020

@author: simon
License: Apache License, see https://www.apache.org/licenses/LICENSE-2.0
"""

import unittest
import serial

import time

PORT='/dev/ttyACM0'

class Simple_Test(unittest.TestCase):
    """ Test the communication with
    the FrequencyDivider
    """
        
        
    def setUp(self):
        self.serial = serial.Serial()
        self.serial.baudrate = 9600
        self.serial.port = PORT
        self.serial.timeout=0.6
        self.serial.open()
        time.sleep(0.2)
        
        
    def tearDown(self):
        self.serial.close()
        
    #@unittest.skip("Skip read")
    def test_read(self):
        print("Test read divider..")
        self.serial.read(1000)
        for i in range(10):
            self.serial.read(1000) #MAGIC. Clears somehow the buffer
            self.serial.write(b'd\n')
            self.serial.flush()
            bytestring = self.serial.read_until()
            print("\tRead ",i,"th time", bytestring)
            
    def test_write(self):
        print("Test write divider")
        self.serial.read(10000)
        val = 16
        bytestring = "D{}\n".format(val).encode('ASCII')
        print("\tWrite",bytestring)
        self.serial.write(bytestring)
        self.serial.flush()
        return_bytestring = self.serial.read_until()
        print("Divider is",return_bytestring)
        self.assertEqual(int(return_bytestring), val)
        
    #@unittest.skip("Skip multiple_read_write")
    def test_multiple_read_write(self):
        print("Test multiple read/write divider")
        #self.serial.read(1000)
        values = [2,4,8,16,32]
        for val in values:
            bytestring = "D{}\n".format(val).encode('ASCII')
            print("\tWrite",bytestring)
            self.serial.read(1000) #MAGIC. Clears somehow the buffer
            self.serial.write(bytestring)
            self.serial.flush()
            return_bytestring = self.serial.read_until()
            if return_bytestring == b'':
                print("\tRead/Write failed, try read")
                self.serial.write(b'd\n')
                self.serial.flush()
                self.serial.reset_input_buffer()
                self.serial.reset_output_buffer()
                return_bytestring = self.serial.read(100)
            if return_bytestring == b'':
                print("\tRead failed, try last time")
                self.serial.write(b'd\n')
                self.serial.flush()
                return_bytestring = self.serial.read(100)
            if return_bytestring == b'':
                print("\tRead failed, try very last time")
                self.serial.write(b'd\n')
                self.serial.flush()
                return_bytestring = self.serial.read(100)
            if return_bytestring == b'':
                print("\tFailed completely")
            else:
                print("\tRead",return_bytestring)
            divider = int(return_bytestring)
            self.assertEqual(divider,val)
        
if __name__ == '__main__':
    unittest.main(verbosity=2)
