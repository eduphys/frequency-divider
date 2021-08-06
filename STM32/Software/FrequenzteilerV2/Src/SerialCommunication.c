/*
 * SerialConnection.c
 *
 * The operation works as follows.
 * SerialCommunication_Received is called by
 * usbd_cdc_if.c as interrupt routine.
 * It checks the buffer for end of line,
 * otherwise it fills the cmd_buffer and
 * returns.
 *
 * The main program checks for
 * endline_reached.  *
 *
 *  First created on: Nov 5, 2019
 *  Rewritten: Many times
 *  Author: simon
 *
 * <h2><center>&copy; Copyright (c) 2020 Eduphys
 * All rights reserved.</center></h2>
 * License: Apache License, see https://www.apache.org/licenses/LICENSE-2.0
 */

//#include "usbd_cdc_if.h"
//#include "HMCWriter.h"

#include "SerialCommunication.h"

# define MYBUFFERSIZE 256
uint8_t cmd_buff[MYBUFFERSIZE];
uint32_t cmd_buff_len;

unsigned int endline_found = 0;

//Private Defines


unsigned int BufferingStopped() {
	return endline_found;
}


// public define

/**
  * @brief  This functions handles the serial connection over the USB of the Frequency Divider.
  *
  *         @note
  *         Gets called in CDC_Receive_FS
  *         Serial connection is independent of the packages.
  * 		For example, within the same buffer, two commands could be contained.
  * 		Furthermore, an end of line can arrive in a different buffer.
  *
  * 		The protocol implemented is as follows
  *
  * 		Received 		| Action
  * 		"d\n"			| Sends value of the current divider
  * 		"D<integer>\n" 	| Sets divider value to given integer and sends the value of the new divider
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  */
void SerialComm_Interrupt(uint8_t* Buf, uint32_t Len) {

	// ignore packages longer than MYBUFFERSIZE bytes
	if (Len >= MYBUFFERSIZE) {
		return;
	}


	// buffer fill
	for (int i = 0; i < Len; ++i) {

		if (BufferingStopped())
			return;

		// buffer overflow -> resets the buffer
		if (cmd_buff_len+1 >= MYBUFFERSIZE ) {
			//reset buffer
			cmd_buff_len = 0;
		}

		// fill cmd buffer
		cmd_buff[cmd_buff_len++] = Buf[i];

		switch (Buf[i]) {
		case '\n':
			SerialComm_StopBuffering();
		}

	}

}


uint8_t* SerialComm_Cmd_Buffer() {
	return cmd_buff;
}

uint32_t SerialComm_Cmd_Buffer_Len() {
	return cmd_buff_len;
}

int SerialComm_EndReached() {
	return endline_found;
}

void SerialComm_StopBuffering() {
	endline_found = 1;
}

void SerialComm_StartBuffering() {
	endline_found = 0;
	cmd_buff_len = 0;
}


