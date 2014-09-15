// MAVLinkParser.h
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

#define MAVLINK_HEADER_MESSAGE_LENGTH 6
#define MAVLINK_CHECKSUM_MESSAGE_LENGTH 2
#define MAVLINK_MAX_PAYLOAD_MESSAGE_LENGTH 256
#define MAVLINK_MAX_MESSAGE_LENGTH (MAVLINK_HEADER_MESSAGE_LENGTH + MAVLINK_MAX_PAYLOAD_MESSAGE_LENGTH + MAVLINK_CHECKSUM_MESSAGE_LENGTH)

typedef enum { 	
	MAVLINK_VALIDATION_STATUS_SUCCESS, 
	MAVLINK_VALIDATION_STATUS_INCOMPLETE_HEADER,
	MAVLINK_VALIDATION_STATUS_INCOMPLETE_MSG, 
	MAVLINK_VALIDATION_STATUS_INVALID_PACKET, 
	MAVLINK_VALIDATION_STATUS_INVALID_LENGTH, 
	MAVLINK_VALIDATION_STATUS_INVALID_CHECKSUM
} mavlink_validation_status_t;

class MAVLinkParser {
	public:
		MAVLinkParser();
		MAVLinkParser(uint64_t size);
		
		void						parseChar(char c, char ** msg, uint16_t * len);
		mavlink_validation_status_t validateMessage(char * buffer, uint16_t length);
	private:
		char * _buffer;
		uint32_t _length;
		uint16_t _offset;

		void						_swiftMessage(uint16_t offset);
};

