#ifndef OFX_THEATRE_PROTOCOL
#define OFX_THEATRE_PROTOCOL

#include "ofMain.h"
#define MAX_ALPHA 255
#define MAX_LEVEL 100
#define ANIMATION_MAX_PRIORITY	10


//--------------------------------------------------------------
//Actor Type
//--------------------------------------------------------------
enum eActorType
{
	//Visible
	eACTOR_BASIC_TYPE		=	0,
	eACTOR_EMPTY_TYPE,
	eACTOR_IMAGE_TYPE,
	eACTOR_DYNAMIC_IMAGE_TYPE,
	eACTOR_ANIMATION_IMAGE_TYPE,
	eACTOR_VIDEO_TYPE,
	eACTOR_WEBCAM_TYPE,
	
	//Invisible
	eACTOR_INVISIBLE_TYPE	=	100,
	eACTOR_SOUND_TYPE,
	eACTOR_MAXIMUM,
};

//--------------------------------------------------------------
//Blending Type
//--------------------------------------------------------------
enum eBlendType
{
	eBLEND_DISABLE	=	0,
	eBLEND_ALPHA,
	eBLEND_ADD,
	eBLEND_SCREEN,
	eBLEND_NUM
};
//--------------------------------------------------------------
//Animation Type
//--------------------------------------------------------------
enum eAnimationType
{
	//For event
	eANIMATE_EMPTY_TYPE,

	//ACTION
	eANIMATE_MOVE_TYPE,
	eANIMATE_ROTATE_TYPE,
	eANIMATE_SCALE_TYPE,
	eANIMATE_ENTER_TYPE,
	eANIMATE_EXIT_TYPE,
	
	//DISPLAY
	eANIMATE_FADEIN_TYPE,
	eANIMATE_FADEOUT_TYPE,
	eANIMATE_SHAKE_TYPE,

	eANIMATE_MAXIMUM,
};

//--------------------------------------------------------------
//Animation State
//--------------------------------------------------------------
enum eAnimationState
{
	eANIMATION_WAIT_INITIAL,
	eANIMATION_INITIAL,
	eANIMATION_RUN,
	eANIMATION_WAIT_FINISH,
	eANIMATION_FINISH,
};

//--------------------------------------------------------------
//Video Animation State
//--------------------------------------------------------------
enum eVideoAnimationState
{
	eVIDEO_ANIMATION_WAIT_FRAME,
	eVIDEO_ANIMATION_INITIAL,
	eVIDEO_ANIMATION_RUN,
	eVIDEO_ANIMATION_FINISH,
};

//--------------------------------------------------------------
//Animation Enter Type
//--------------------------------------------------------------
typedef enum eEnterType
{
	eENTER_FROM_UP,
	eENTER_FROM_RIGHT,
	eENTER_FROM_DOWN,
	eENTER_FROM_LEFT,
}ENTER_TYPE;

//--------------------------------------------------------------
//Animation Exit Type
//--------------------------------------------------------------
typedef enum eExitType
{
	eEXIT_TO_UP,
	eEXIT_TO_RIGHT,
	eEXIT_TO_DOWN,
	eEXIT_TO_LEFT,
}EXIT_TYPE;

//--------------------------------------------------------------

#endif //OFX_THEATRE_PROTOCOL