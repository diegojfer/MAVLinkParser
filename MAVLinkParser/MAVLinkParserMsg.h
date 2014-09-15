// MAVLinkParserMessages.h
// 
// Copyright (c) 2014 Diego Jose Fernandez Hernandez
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#include <stdint.h>

#define MAVLINK_DEFAULT_CRC_EXTRA 			0x01

#define MAVLINK_ATTITUDE_MSG 				30
#define MAVLINK_ATTITUDE_CRC_EXTRA 			0x27

#define MAVLINK_LOCATION_MSG 				33
#define MAVLINK_LOCATION_CRC_EXTRA 			0x68

#define MAVLINK_GPS_RAW_INT_MSG 			24
#define MAVLINK_GPS_RAW_INT_CRC_EXTRA 		0x18

#define MAVLINK_HEARTBEAT_MSG 				0
#define MAVLINK_HEARTBEAT_CRC_EXTRA 		0x32

uint8_t _mavlink_crc_extra(uint8_t message_id){
	if (message_id == MAVLINK_ATTITUDE_MSG){
		return MAVLINK_ATTITUDE_CRC_EXTRA;
	}else if (message_id == MAVLINK_LOCATION_MSG){
		return MAVLINK_LOCATION_CRC_EXTRA;
	}else if (message_id == MAVLINK_GPS_RAW_INT_MSG){
		return MAVLINK_GPS_RAW_INT_CRC_EXTRA;
	}else if (message_id == MAVLINK_HEARTBEAT_MSG){
		return MAVLINK_HEARTBEAT_CRC_EXTRA;
	}
	
	return MAVLINK_DEFAULT_CRC_EXTRA;
}