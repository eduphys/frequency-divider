#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Aug  2 14:56:40 2020

@author: simon
License: Apache License, see https://www.apache.org/licenses/LICENSE-2.0
"""

import sys
sys.path.append('../')
from SerialCom import SerialCom

import unittest

import time

PORT = '/dev/ttyACM0'
DIV_VALUE = 16

class SerialCom_Test(unittest.TestCase):
    
    def setUp(self):
        self.com = SerialCom(PORT)
        self.com.open()
        self.com.serial.read(1000)
        self.com.close()
        
    def tearDown(self):
        del self.com
    

    def test_open_close(self):
        self.assertEqual(self.com.is_open(), False)
        self.com.open()
        self.assertEqual(self.com.is_open(), True)
        self.com.close()
        self.assertEqual(self.com.is_open(), False)

    def test_read_divider(self):
        self.com.open()
        
        for i in range(3):
            divider = self.com.read_divider()
            self.assertEqual(divider, DIV_VALUE)
            
    def test_read_divider2(self):
        self.com.open()
        
        for i in range(3):
            divider = self.com.read_divider()
            self.assertEqual(divider, DIV_VALUE)
        
    def test_read_write_divider(self):
        self.com.open()
        
        values = [2,4,8,16,32,DIV_VALUE]
        for val in values:
            divider = self.com.set_divider(val)
            self.assertEqual(divider,val)
        

if __name__ == '__main__':
    unittest.main(verbosity=2)
