#ifndef OFX_THEATRE_ANIMATOR
#define OFX_THEATRE_ANIMATOR

#include "ofxTheatreProtocol.h"
#include "ofxTheatreElement.h"
#include "ofxTheatreEvents.h"
//--------------------------------------------------------------
//Base Animation Class
//--------------------------------------------------------------
class ofxBaseAnimation
{
public:
	ofxBaseAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, eAnimationType eType, float fAnimationTime, float fWaitStart, float fDelay):
	  m_strAnimationName(strAnimationName),
	  m_eType(eType),
	  m_fWaitStart(fWaitStart),
	  m_fDelay(fDelay),
	  m_fAnimationTime(fAnimationTime),
	  m_bCanBreak(false),
	  m_eAnimationState(eANIMATION_WAIT_INITIAL),
	  m_pElementPtr(pElementPtr),
	  m_bIsVideoAnimation(false){};

	ofxBaseAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, eAnimationType eType, float fAnimationTime, int iStartFrame):
	  m_strAnimationName(strAnimationName),
	  m_eType(eType),
	  m_fAnimationTime(fAnimationTime),
	  m_iStartFrame(iStartFrame),
	  m_bCanBreak(false),
	  m_eVideoAnimationState(eVIDEO_ANIMATION_WAIT_FRAME),
	  m_pElementPtr(pElementPtr),
	  m_bIsVideoAnimation(true){};

	eAnimationState GetAnimationState();
	eVideoAnimationState GetVideoAnimationState();

	virtual void WaitInitial(double dDiffSecond);
	virtual void Initial(){};
	virtual void Run(double dDiffSecond){};
	virtual void WaitFinish(double dDiffSecond);
	virtual void Finish(){};

	virtual void WaitFrame(int iFrame);
	
public:
	std::string				m_strAnimationName;
	eAnimationState			m_eAnimationState;
	eVideoAnimationState	m_eVideoAnimationState;
	
	float				m_fWaitStart;
	float				m_fDelay;
	float				m_fAnimationTime;
	eAnimationType		m_eType;
	bool				m_bCanBreak;
	int					m_iStartFrame;
	ofxBaseElement*		m_pElementPtr;
	bool				m_bIsVideoAnimation;
};

//--------------------------------------------------------------
//Empty Animation Class
//--------------------------------------------------------------
class ofxEmptyAnimation : public ofxBaseAnimation
{
public:	
	ofxEmptyAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, float fAnimationTime = 0.5, float fWaitStart = 0.0, float fDelay = 0.0):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_EMPTY_TYPE, fAnimationTime, fWaitStart, fDelay){}
	
	ofxEmptyAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, int iStartFrame, float fAnimationTime = 0.5):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_EMPTY_TYPE, fAnimationTime, iStartFrame){}

	virtual void Initial()
	{
		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_RUN;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_RUN;
		}
	};
	virtual void Run(double dDiffSecond)
	{
		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_WAIT_FINISH;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_FINISH;
		}
	};
};
//--------------------------------------------------------------
//Move Animation Class
//--------------------------------------------------------------
class ofxMoveAnimation : public ofxBaseAnimation
{
public:
	ofxMoveAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, ofPoint oTargetPos, float fAnimationTime = 0.5, float fWaitStart = 0.0, float fDelay = 0.0):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_MOVE_TYPE, fAnimationTime, fWaitStart, fDelay),
	  m_oEndPoint(oTargetPos){};
	
	ofxMoveAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, ofPoint oTargetPos, int iStartFrame, float fAnimationTime = 0.5):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_MOVE_TYPE, fAnimationTime, iStartFrame),
	  m_oEndPoint(oTargetPos){};

	virtual void Initial();
	virtual void Run(double dDiffSecond);
	virtual void Finish();
private:
	ofPoint			m_oEndPoint;
	ofPoint			m_oVelocity;
};

//--------------------------------------------------------------
//Rotate Animation Class
//--------------------------------------------------------------
class ofxRotateAnimation : public ofxBaseAnimation
{
public:
	ofxRotateAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, float fRotateDegree, float fAnimationTime = 0.5, float fWaitStart = 0.0, float fDelay = 0.0):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_ROTATE_TYPE, fAnimationTime, fWaitStart, fDelay),
	  m_fEndRotateDegree(fRotateDegree){};

	ofxRotateAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, float fRotateDegree, int iStartFrame, float fAnimationTime = 0.5):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_ROTATE_TYPE, fAnimationTime, iStartFrame),
	  m_fEndRotateDegree(fRotateDegree){};
	
	virtual void Initial();
	virtual void Run(double dDiffSecond);
	virtual void Finish();

private:
	float			m_fEndRotateDegree;
	float			m_fRotateVelocity;
};

//--------------------------------------------------------------
//Scale Animation Class
//--------------------------------------------------------------
class ofxScaleAnimation : public ofxBaseAnimation
{
public:
	ofxScaleAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, float fTargetScale, float fAnimationTime = 0.5, float fWaitStart = 0.0, float fDelay = 0.0):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_SCALE_TYPE, fAnimationTime, fWaitStart, fDelay),
	  m_fEndScale(fTargetScale){};

	ofxScaleAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, float fTargetScale, int iStartFrame, float fAnimationTime = 0.5):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_SCALE_TYPE, fAnimationTime, iStartFrame),
	  m_fEndScale(fTargetScale){};
	
	virtual void Initial();
	virtual void Run(double dDiffSecond);
	virtual void Finish();

private:
	float			m_fEndScale;
	float			m_fScaleVelocity;
};

//--------------------------------------------------------------
//Enter Animation Class
//--------------------------------------------------------------
class ofxEnterAnimation : public ofxBaseAnimation
{
public:
	ofxEnterAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, ENTER_TYPE eEnterType, int iWidth, int iHeight, float fAnimationTime = 0.5, float fWaitStart = 0.0, float fDelay = 0.0):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_ENTER_TYPE, fAnimationTime, fWaitStart, fDelay),
      m_eEnterType(eEnterType),
	  m_iWindowsWidth(iWidth),
	  m_iWindowsHeight(iHeight){};

	ofxEnterAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, ENTER_TYPE eEnterType, int iWidth, int iHeight, int iStartFrame, float fAnimationTime = 0.5):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_ENTER_TYPE, fAnimationTime, iStartFrame),
      m_eEnterType(eEnterType),
	  m_iWindowsWidth(iWidth),
	  m_iWindowsHeight(iHeight){};

	virtual void Initial();
	virtual void Run(double dDiffSecond);
	virtual void Finish();

private:
	ENTER_TYPE		m_eEnterType;
	int				m_iWindowsWidth, m_iWindowsHeight;
	ofPoint			m_oSourcePoint;
	ofPoint			m_oVelocity;
};

//--------------------------------------------------------------
//Exit Animation Class
//--------------------------------------------------------------
class ofxExitAnimation : public ofxBaseAnimation
{
public:
	ofxExitAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, EXIT_TYPE eExitType, int iWidth, int iHeight, float fAnimationTime = 0.5, float fWaitStart = 0.0, float fDelay = 0.0):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_EXIT_TYPE, fAnimationTime, fWaitStart, fDelay),
      m_eExitType(eExitType),
	  m_iWindowsWidth(iWidth),
	  m_iWindowsHeight(iHeight){};

	ofxExitAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, EXIT_TYPE eExitType, int iWidth, int iHeight, int iStartFrame, float fAnimationTime = 0.5):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_EXIT_TYPE, fAnimationTime, iStartFrame),
	  m_eExitType(eExitType),
	  m_iWindowsWidth(iWidth),
	  m_iWindowsHeight(iHeight){};

	virtual void Initial();
	virtual void Run(double dDiffSecond);
	virtual void Finish();

private:
	EXIT_TYPE		m_eExitType;
	int				m_iWindowsWidth, m_iWindowsHeight;
	ofPoint			m_oTargetPoint;
	ofPoint			m_oVelocity;
};

//--------------------------------------------------------------
//Fade In Animation Class
//--------------------------------------------------------------
class ofxFadeInAnimation : public ofxBaseAnimation
{
public:
	ofxFadeInAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, float fAnimationTime = 0.5, float fWaitStart = 0.0, float fDelay = 0.0):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_FADEIN_TYPE, fAnimationTime, fWaitStart, fDelay){};
	
	ofxFadeInAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, int iStartFrame, float fAnimationTime = 0.5):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_FADEIN_TYPE, fAnimationTime, iStartFrame){};

	virtual void Initial();
	virtual void Run(double dDiffSecond);
	virtual void Finish();

private:
	float	m_fDiffAlpha;
};

//--------------------------------------------------------------
//Fade Out Animation Class
//--------------------------------------------------------------
class ofxFadeOutAnimation : public ofxBaseAnimation
{
public:
	ofxFadeOutAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, float fAnimationTime = 0.5, float fWaitStart = 0.0, float fDelay = 0.0):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_FADEOUT_TYPE, fAnimationTime, fWaitStart, fDelay){};
	
	ofxFadeOutAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, int iStartFrame, float fAnimationTime = 0.5):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_FADEOUT_TYPE, fAnimationTime, iStartFrame){};

	virtual void Initial();
	virtual void Run(double dDiffSecond);
	virtual void Finish();

private:
	float	m_fDiffAlpha;
};

//--------------------------------------------------------------
//Shake Animation Class
//--------------------------------------------------------------
class ofxShakeAnimation : public ofxBaseAnimation
{
public:
	ofxShakeAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, float fShakeX, float fShakeY, float fAnimationTime = 0.5, float fWaitStart = 0.0, float fDelay = 0.0):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_SHAKE_TYPE, fAnimationTime, fWaitStart, fDelay),
	  _fShakeX(fShakeX), _fShakeY(fShakeY), _oBackupPos(0,0){};
	
	ofxShakeAnimation(std::string strAnimationName, ofxBaseElement* pElementPtr, float fShakeX, float fShakeY, int iStartFrame, float fAnimationTime = 0.5):
	  ofxBaseAnimation(strAnimationName, pElementPtr, eANIMATE_SHAKE_TYPE, fAnimationTime, iStartFrame),
	  _fShakeX(fShakeX), _fShakeY(fShakeY), _oBackupPos(0,0){};

	virtual void Initial();
	virtual void Run(double dDiffSecond);
	virtual void Finish();

private:
	float _fShakeX, _fShakeY;
	ofPoint _oBackupPos;
};

//--------------------------------------------------------------
//Animator Class
//--------------------------------------------------------------
typedef std::list<ofxBaseAnimation*>					ANIMATION_LIST;
typedef std::list<ofxBaseAnimation*>::iterator			ANIMATION_LIST_ITER;
typedef std::map<std::string, unsigned char>			ANIMATION_MAP;
typedef std::map<std::string, unsigned char>::iterator	ANIMATION_MAP_ITER;
	
class ofxAnimator
{
public:
	ofxAnimator(std::string strScenesName):
	  m_uiAnimationCounter(0),
	  m_strScenesName(strScenesName)
	  {};
	~ofxAnimator();

private:
	std::string				m_strScenesName;

//Timer Animator
public:
	void AnimatorUpdate(double dDiffSecond);
	bool AddAnimation(unsigned char ucAnimationPriority, ofxBaseAnimation* pAnimation);
	bool DelAnimationInMap(std::string strAnimationName);
	void ClearAnimation();

private:	
	ANIMATION_MAP			m_oAnimationMap;
	ANIMATION_LIST				m_aoAnimationMgr[ANIMATION_MAX_PRIORITY];
	unsigned int					m_uiAnimationCounter;
	float									m_fTimer;

//Video Animator
public:
	void VideoAnimatorUpdate(double dDiffSecond, int iFrameIndex);
	bool AddVideoAnimation(ofxBaseAnimation* pAnimation);
	bool DelVideoAnimationInMap(std::string strAnimationName);
	void ClearVideoAnimation();

private:
	ANIMATION_MAP			m_oVideoAnimationMap;
	ANIMATION_LIST				m_oVideoAnimationList;

private:
	static bool IsFinish(ofxBaseAnimation* pBaseAnimation){return pBaseAnimation == NULL;};
};


#endif //OFX_THEATRE_ANIMATOR