// MAVLink_ArduinoTelemetryPort.ino
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
#include <Arduino.h>
#include <stdint.h>

#include <PRAWUtils.h>
#include <CRC_16_CCITT.h>
#include <MAVLinkParser.h>

MAVLinkParser uavLink(320);

void setup(){
  Serial.begin(57600);
  pinMode(13, OUTPUT);
}

void loop(){
  while ( Serial.available() > 0 ) {
      char * msg = NULL;
      uint16_t len = 0;
      
      uavLink.parseChar(Serial.read(), &msg, &len);
      
      if (msg != NULL) {
        uint8_t message_id = utils_praw_uint8((uint8_t *)msg, 5);
        if (message_id == 0){
	  digitalWrite(13, HIGH);
	}
        
        free(msg);
        msg = NULL;
        len = 0;
      }
  }
}
