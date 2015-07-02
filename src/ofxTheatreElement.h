#ifndef OFX_THEATRE_ELEMENT
#define OFX_THEATRE_ELEMENT

#include "ofxTheatreProtocol.h"
#include "ofxTheatreActor.h"
#include "ofxTheatreEvents.h"

//--------------------------------------------------------------
//Element's base variable
//--------------------------------------------------------------
typedef struct
{
	unsigned char	ucLevel;
	ofPoint			oPos;
	float			fDegree;
	float			fScale;
	float			fAlpah;
	int				iWidth, iHeight;
	bool			bIsVisible;
	
	bool			bIsMirror;
	bool			bIsDrawSubsection;
	ofPoint			oStartPos;
	int				iSubWidth, iSubHeight;

	//Element Action state
	bool			bIsMove;
	bool			bIsRotate;
	bool			bIsScale;
	bool			bFadeIn;
	bool			bFadeOut;
	bool			bIsShake;

}BASE_ELEMENT;

//--------------------------------------------------------------
//Base Element Class
//--------------------------------------------------------------
class ofxBaseElement
{
public:
	ofxBaseElement(string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible);
	virtual void update(double dDiffSecond){};
	virtual void draw(int iWidth, int iHeight, float fScale){};

	virtual void Play(){};
	virtual void Pause(){};
	virtual void Stop();

public:
	//Basic
	void SetVisible(const bool bValue);	
	void SetPos(ofPoint oNewPos);
	int GetWidth();
	int GetHeight();
	
	void SetParentsPlane(ofxBaseElement* pPlanePtr);
	ofxBaseElement* GetParentsPlane();


public:
	string		m_strElementName;
	ofxBaseElement* m_pPlanePtr;
	ofxBaseActor*	m_pBaseActor;
	
	BASE_ELEMENT	m_stElementBase;
	BASE_ELEMENT	m_stElementBaseBackup;

};

//--------------------------------------------------------------
//Empty Element Class (for button)
//--------------------------------------------------------------
class ofxEmptyElement:public ofxBaseElement
{
public:
	ofxEmptyElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxEmptyActor* pActor)
		:ofxBaseElement(strName, ucLevel, oPos, bIsVisible)
	{
		this->m_stElementBase.iHeight = pActor->_fHeight;
		this->m_stElementBase.iWidth  = pActor->_fWidth;
		this->m_stElementBaseBackup   = this->m_stElementBase;
	}
};

//--------------------------------------------------------------
//Image Element
//--------------------------------------------------------------
class ofxImageElement:public ofxBaseElement
{
public:
	ofxImageElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxImageActor* pActor);
public:
	void draw(int iWidth, int iHeight, float fScale);
private:
	ofxImageActor*	m_pImageActor;

};

//--------------------------------------------------------------
//Dynamic Image Element
//--------------------------------------------------------------
class ofxDynamicImageElement:public ofxBaseElement
{
public:
	ofxDynamicImageElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxDynamicImageActor* pActor);

public:
	void draw(int iWidth, int iHeight, float fScale);
	void updateImg(ofImage& oNewImg);
	void getImage(ofImage& refImg);

	void updateSize();
private:
	ofxDynamicImageActor*	m_pImageActor;

};

//--------------------------------------------------------------
//Animation Image Element
//--------------------------------------------------------------
class ofxAnimationImageElement:public ofxBaseElement
{
public:
	ofxAnimationImageElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxAnimationImageActor* pActor, float fSPF = 0.033);

public:
	void update(double dDiffSecond);
	void draw(int iWidth, int iHeight, float fScale);

	void Play();

public:
	void PlayAnimation();
	void SetSPF(float fSPF);
	void SetLoop(bool bIsLoop);
	void SetEvent(bool bHaveEvent);

private:
	int	GetFrameNum();

private:
	bool						m_bIsPlay;
	bool						m_bIsLoop;
	bool						m_bCallEvent, m_bIsCallEventIsTriggle, m_bIsDone;
	float						m_fTimer;
	int							m_iFrameNo;
	float						m_fSPF;
	ofxAnimationImageActor*		m_pAnimationActor;
};
//--------------------------------------------------------------
//Video Element
//--------------------------------------------------------------
class ofxVideoElement:public ofxBaseElement
{
public:
	ofxVideoElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxVideoActor* pActor, bool bIsAutoPlay = true);

public:
	void update(double dDiffSecond);
	void draw(int iWidth, int iHeight, float fScale);
	void Play();
	void Pause();
	void Stop();

	void ChangeVideoActor(ofxVideoActor* pNewVideoActor);

public:
	void SetVideoLoop(bool bIsLoop);
	void SetVideoAutoPlay(bool bIsAutoPlay);

	void SetVideoSubloop(int iStartFrame, int iEndFrame);
	void SetVideoSubloop(int iFrame, bool bIsFromStart = true);
	
	void ExitSubloop();

	//Video Control
	void PlayVideo();
	void PauseVideo(bool bValue);
	void StopVideo();
	int GetFrame();
	void SetFrame(int iFrame);
	void StartEvent();

protected:
	bool			m_bIsLoop;
	bool			m_bIsPlay, m_bIsPause;

	//Sublooping
	bool			m_bSubloop, m_bSubloopClick;
	int				m_iStartFrame, m_iEndFrame;

	float			m_fPausePosition;
	ofxVideoActor*	m_pVideoActor;
	bool			m_bIsAutoPlay;
	bool			m_bCallEvent;
	bool			m_bIsCallEventIsTriggle;
};

//--------------------------------------------------------------
//Webcam Element
//--------------------------------------------------------------
class ofxWebcamElement:public ofxBaseElement
{
public:
	ofxWebcamElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxWebcamActor* pActor);

public:
	void update(double dDiffSecond);
	void draw(int iWidth, int iHeight, float fScale);

public:
	void getImage(ofImage&);
private:

	bool			m_bMirror;
	ofxWebcamActor*	m_pWebcamActor;
	ofImage			m_oImage;
};

//--------------------------------------------------------------
//Sound Element
//--------------------------------------------------------------
class ofxSoundElement:public ofxBaseElement
{
public:
	ofxSoundElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxSoundActor* pActor);

public:
	void update(double dDiffSecond);
	void draw(int iWidth, int iHeight);
	void Play();
	void Pause();
	void Stop();

public:
	//Sound Control
	void PlaySound(float m_fVolume);
	void PauseSound(bool bValue);


private:
	ofxSoundActor*	m_pSoundActor;
	bool			m_bIsFullPlay;
	bool			m_bIsLoop;
	bool			m_bIsPlay, m_bIsPause;
	float			m_fBeginPosition, m_fEndPosition;
	float			m_fPausePosition;

	float			m_fVolume;
};


#endif //OFX_THEATRE_ELEMENT