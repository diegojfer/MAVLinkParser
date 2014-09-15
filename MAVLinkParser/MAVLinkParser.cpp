// MAVLinkParser.cpp
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
#include "MAVLinkParser.h"
#include "MAVLinkParserMsg.h"

#include <stdlib.h>
#include <string.h>
#include <PRAWUtils.h>
#include <CRC_16_CCITT.h>

#define MAVLINKPARSER_MIN_BUFFER_SIZE 266

MAVLinkParser::MAVLinkParser(){
	_buffer = (char *)malloc(sizeof(char) * MAVLINKPARSER_MIN_BUFFER_SIZE);
	_length = MAVLINKPARSER_MIN_BUFFER_SIZE;
	_offset = 0;
}

MAVLinkParser::MAVLinkParser(uint64_t size){
	if (size > MAVLINKPARSER_MIN_BUFFER_SIZE){
		_buffer = (char *)malloc(sizeof(char) * MAVLINKPARSER_MIN_BUFFER_SIZE);
		_length = MAVLINKPARSER_MIN_BUFFER_SIZE;
		_offset = 0;
	}else{
		_buffer = (char *)malloc(sizeof(char) * size);
		_length = size;
		_offset = 0;
	}
}

void MAVLinkParser::_swiftMessage(uint16_t offset){
	if (offset < 1) offset = 1;
	
	char * 		buf = _buffer + offset;
	uint16_t 	len = _length - offset;

	while ( *buf != (char)0xFE && len != 0 ){
		buf++;
		len--;
	}

	if (len == 0){
		memset(_buffer, 0x00, _length);
		
		_offset = 0;
	}else{
		memcpy(_buffer, buf, len);
		memset(_buffer + len, 0x00, _length - len);
		
		_offset = len - (_length - _offset);
	}
}

mavlink_validation_status_t MAVLinkParser::validateMessage(char * buffer, uint16_t length){
	// check minimun message length
	if ( length < MAVLINK_HEADER_MESSAGE_LENGTH + MAVLINK_CHECKSUM_MESSAGE_LENGTH ) return MAVLINK_VALIDATION_STATUS_INCOMPLETE_HEADER;
	
	// check start packet sign
	if ( *buffer != (char)0xFE ) return MAVLINK_VALIDATION_STATUS_INVALID_PACKET;
	
	// check message length
	uint16_t payload_length = utils_praw_uint8((uint8_t *)buffer, 1);
	if ( length < MAVLINK_HEADER_MESSAGE_LENGTH + payload_length + MAVLINK_CHECKSUM_MESSAGE_LENGTH) 
		return MAVLINK_VALIDATION_STATUS_INCOMPLETE_MSG;
	if ( length > MAVLINK_HEADER_MESSAGE_LENGTH + payload_length + MAVLINK_CHECKSUM_MESSAGE_LENGTH)
		return MAVLINK_VALIDATION_STATUS_INVALID_LENGTH;

	// check message checksum
	uint8_t message_id = utils_praw_uint8((uint8_t *)buffer, 5);
	uint16_t calc_checksum = crc16ccitt_checksum((uint8_t *)buffer + 1, MAVLINK_HEADER_MESSAGE_LENGTH + payload_length - 1);
	_crc16ccitt_accumulate(&calc_checksum, _mavlink_crc_extra(message_id));
	
	uint16_t readed_checksum = utils_praw_uint16((uint8_t *)buffer, MAVLINK_HEADER_MESSAGE_LENGTH + payload_length, CPU_LITTLE_ENDIAN);
	
	if (readed_checksum != calc_checksum)
		return MAVLINK_VALIDATION_STATUS_INVALID_CHECKSUM;
	
	// if pass, return success
	return MAVLINK_VALIDATION_STATUS_SUCCESS;
}

void MAVLinkParser::parseChar(char c, char ** msg, uint16_t * len){
	*msg = NULL;
	*len = 0;
	
	// only save valid char
	if ( _offset == 0 && c != (char)0xFE )
		return;
	
	// avoid mem overflow 
	// never will run... ¿?
	if (_offset > _length) {
		memset(_buffer, 0x00, _length);
		return;
	}
	
	// save parsed char
	_buffer[_offset] = c;
	
	mavlink_validation_status_t validation = validateMessage(_buffer, ++_offset);
	if (validation == MAVLINK_VALIDATION_STATUS_SUCCESS){
		uint16_t message_length = MAVLINK_HEADER_MESSAGE_LENGTH + utils_praw_uint8((uint8_t *)_buffer, 1) + MAVLINK_CHECKSUM_MESSAGE_LENGTH;
		*msg = (char *)malloc(sizeof(char) * message_length);
		memcpy(*msg, _buffer, _offset);
		*len = message_length;
		
		_swiftMessage(_offset);
	
		return;
	}else if (validation == MAVLINK_VALIDATION_STATUS_INCOMPLETE_HEADER){
		// do nothing - wait for more data
		return;
	}else if (validation == MAVLINK_VALIDATION_STATUS_INCOMPLETE_MSG){
		// do nothing - wait for more data
		return;
	}else if (validation == MAVLINK_VALIDATION_STATUS_INVALID_PACKET){
		uint16_t message_length = MAVLINK_HEADER_MESSAGE_LENGTH + utils_praw_uint8((uint8_t *)_buffer, 1) + MAVLINK_CHECKSUM_MESSAGE_LENGTH;
		_swiftMessage(message_length);
		return;
	}else if (validation == MAVLINK_VALIDATION_STATUS_INVALID_LENGTH){
		uint16_t message_length = MAVLINK_HEADER_MESSAGE_LENGTH + utils_praw_uint8((uint8_t *)_buffer, 1) + MAVLINK_CHECKSUM_MESSAGE_LENGTH;
		_swiftMessage(message_length);
		return;
	}else if (validation == MAVLINK_VALIDATION_STATUS_INVALID_CHECKSUM){
		uint16_t message_length = MAVLINK_HEADER_MESSAGE_LENGTH + utils_praw_uint8((uint8_t *)_buffer, 1) + MAVLINK_CHECKSUM_MESSAGE_LENGTH;
		_swiftMessage(message_length);
		return;
	}
		
	
	return;
}