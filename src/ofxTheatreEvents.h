#ifndef OFX_THEATRE_EVENTS
#define OFX_THEATRE_EVENTS

#include "ofxTheatreProtocol.h"

class ofxTheatreEventArgs :public ofEventArgs
{
public:
	std::string		strMessage;
	static ofEvent<ofxTheatreEventArgs> TheatreEvent;
};


#endif //OFX_THEATRE_EVENTS

