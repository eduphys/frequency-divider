
/**
  ******************************************************************************
  * @file           : SerialCommunication.h
  * @version        : v2.0_Cube
  * @brief          : This file implements the USB Serial connections
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 Eduphys
  * All rights reserved.</center></h2>
  *
  * 
  *
  ******************************************************************************
  */



#include <stdint.h>
// Interrupt
void SerialComm_Interrupt(uint8_t* Buf, uint32_t Len);


void SerialComm_ProcessBuffer();
int SerialComm_EndReached();
void SerialComm_StopBuffering();
void SerialComm_StartBuffering();

uint8_t* SerialComm_Cmd_Buffer();

uint32_t SerialComm_Cmd_Buffer_Len();
