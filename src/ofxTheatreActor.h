#ifndef OFX_THEATRE_ACTOR
#define OFX_THEATRE_ACTOR

#include "ofxTheatreProtocol.h"


//--------------------------------------------------------------
//Base Actor Class
//--------------------------------------------------------------
class ofxBaseActor
{
public:
	ofxBaseActor(std::string strActorName):
		m_strActorName(strActorName),
		m_ucActorType(eACTOR_BASIC_TYPE){};
	virtual	void Test(){};
public:
	std::string		m_strActorName;
	eActorType		m_ucActorType; //@see eActorType
};

//--------------------------------------------------------------
//Empty Actor
//--------------------------------------------------------------
class ofxEmptyActor : public ofxBaseActor
{
public:
	ofxEmptyActor(string strActorName, float fWidth, float fHeight):
		ofxBaseActor(strActorName), 
		_fWidth(fWidth),
		_fHeight(fHeight)
	{
		m_ucActorType = eACTOR_EMPTY_TYPE;
	}

public:
	float _fWidth, _fHeight;
};

//--------------------------------------------------------------
//Image Actor
//--------------------------------------------------------------
class ofxImageActor:public ofxBaseActor
{
public:
	ofxImageActor(std::string strActorName, std::string strImagePath, eBlendType eBlendType = eBLEND_DISABLE):
	ofxBaseActor(strActorName), _eBlendType(eBlendType)
	{
		this->m_ucActorType = eACTOR_IMAGE_TYPE;
		if(!this->m_oImage.loadImage(strImagePath))
		{
			ofLogError("Load image faild:" + strImagePath);
		}
	};
public:
	eBlendType	_eBlendType;
	ofImage		m_oImage;
};

//--------------------------------------------------------------
//Dynamic Image Actor
//--------------------------------------------------------------
class ofxDynamicImageActor:public ofxBaseActor
{
public:
	ofxDynamicImageActor(std::string strActorName, eBlendType eBlendType = eBLEND_DISABLE):
	ofxBaseActor(strActorName), _eBlendType(eBlendType)
	{
		this->m_ucActorType = eACTOR_DYNAMIC_IMAGE_TYPE;

	};
public:
	eBlendType	_eBlendType;
	ofImage		m_oImage;
};

//--------------------------------------------------------------
//Animation Image Actor
//--------------------------------------------------------------
class ofxAnimationImageActor:public ofxBaseActor
{
public:
	ofxAnimationImageActor(std::string strActorName, std::string strImageFolder, eBlendType eBlendType = eBLEND_DISABLE):
	ofxBaseActor(strActorName), _eBlendType(eBlendType)
	{
		this->m_ucActorType = eACTOR_ANIMATION_IMAGE_TYPE;

		ofDirectory	oDir_(strImageFolder);
		if(_eBlendType == eBLEND_ALPHA)
		{
			oDir_.allowExt("png");
		}
		else
		{
			oDir_.allowExt("jpg");
		}
		oDir_.listDir();

		if(oDir_.numFiles() > 0)
		{
			for(int idx_ = 0; idx_ < oDir_.numFiles(); ++idx_)
			{
				ofImage oImg_;
				oImg_.loadImage(oDir_.getPath(idx_));
				this->m_oImageList.push_back(oImg_);
			}
		}
		else
		{
			ofLog(OF_LOG_WARNING, "Does not found any animation in folder");
		}
	};

public:
	eBlendType					_eBlendType;
	std::vector<ofImage>		m_oImageList;
};

//--------------------------------------------------------------
//Video Actor
//--------------------------------------------------------------
class ofxVideoActor:public ofxBaseActor
{
public:
	ofxVideoActor(std::string strActorName, std::string strVideoPath):
		ofxBaseActor(strActorName), _eBlendType(eBLEND_DISABLE)
	{	
		this->m_ucActorType = eACTOR_VIDEO_TYPE;
		if(!this->m_oVideo.loadMovie(strVideoPath))
		{
			ofLogError("Load video faild:" + strVideoPath);
		}
	};

	ofxVideoActor(std::string strActorName, std::string strVideoPath, ofPtr<ofBaseVideoPlayer> pPlayer, eBlendType eBlendType = eBLEND_DISABLE)
		:ofxBaseActor(strActorName), _eBlendType(eBlendType)
	{
		this->m_ucActorType = eACTOR_VIDEO_TYPE;
		this->m_oVideo.setPlayer(pPlayer);
		if(_eBlendType == eBLEND_ALPHA)
		{
			this->m_oVideo.setPixelFormat(OF_PIXELS_RGBA);
		}
		if(!this->m_oVideo.loadMovie(strVideoPath))
		{
			ofLogError("Load video faild:" + strVideoPath);
		}
	};

public:
	eBlendType		_eBlendType;
	ofVideoPlayer	m_oVideo;
};


//--------------------------------------------------------------
//Webcam Actor
//--------------------------------------------------------------
class ofxWebcamActor:public ofxBaseActor
{
public:
	ofxWebcamActor(std::string strActorName, unsigned char ucDeviceID, unsigned char ucFPS, int iWebcamWidth, int iWebcamHeight):ofxBaseActor(strActorName)
	{
		this->m_bIsInitialSuccess = true;
		this->m_ucActorType = eACTOR_WEBCAM_TYPE;
		//WebCam Setting

		this->m_oVidGrabber.setVerbose(true);
		this->m_oVidGrabber.setDeviceID(ucDeviceID);
		this->m_oVidGrabber.setDesiredFrameRate(ucFPS);
		if(!this->m_oVidGrabber.initGrabber(iWebcamWidth, iWebcamHeight))
		{
			this->m_bIsInitialSuccess = false;
			ofLogError("Webcam initial failed!");
		}
	};

	void WebcamSetting()
	{
		this->m_oVidGrabber.videoSettings();
	};

public:
	bool			m_bIsInitialSuccess;
	ofVideoGrabber	m_oVidGrabber;
	
};

//--------------------------------------------------------------
//Sound Actor
//--------------------------------------------------------------
class ofxSoundActor:public ofxBaseActor
{
public:
	ofxSoundActor(std::string strActorName, std::string strSoundPath, bool bIsLoop = true):ofxBaseActor(strActorName)
	{
		this->m_ucActorType = eACTOR_SOUND_TYPE;
		
		//Sound Setting
		if(!this->m_oSoundPlayer.loadSound(strSoundPath))
		{
			ofLogError("Load sound faild:" + strSoundPath);
		}
	};

public:
	ofSoundPlayer	m_oSoundPlayer;
	
};
#endif //OFX_THEATRE_ACTOR