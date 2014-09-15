#MAVLinkParser
MAVLinkParser is an open-source library to decode MAVLink raw messages. 

![MAVLink Logo](https://raw.githubusercontent.com/diegojfer/MAVLinkParser/master/mavlinklogo.png)

## Installation 
To install the library, copy MAVLinkParser to libraries folder and select the library and dependencies (PRAWUtils and CRC\_16\_CCITT) from **Sketch > Import Library in Arduino IDE**.

![Import libraries](https://raw.githubusercontent.com/diegojfer/MAVLinkParser/master/importlib.png)

## Usage
To decode a message, create a **MAVLinkParser** instace and call function **MAVLinkParser:parseChar**.
```c
	MAVLinkParser uavLink();
	
	char * msg = NULL;
	uint16_t len = 0;

	uavLink.parseChar('a', &msg, &len);
      
	if (msg != NULL) {
   		uint8_t message_id = utils_praw_uint8((uint8_t *)msg, 5);
       
    	free(msg);
   		msg = NULL;
   		len = 0;
	}
```