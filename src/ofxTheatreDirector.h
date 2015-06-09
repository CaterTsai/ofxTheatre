#ifndef OFX_THEATRE_DIRECTOR
#define OFX_THEATRE_DIRECTOR

#include "ofxTheatreProtocol.h"
#include "ofxTheatreStage.h"
#include "ofxTheatreTransitions.h"

//--------------------------------------------------------------
//Director Class
//--------------------------------------------------------------
typedef std::map<std::string, ofxBaseActor*>				ACTOR_MAP;
typedef std::map<std::string, ofxBaseActor*>::iterator		ACTOR_ITER;
typedef std::map<std::string, ofxBaseElement*>				ELEMENT_MAP;
typedef std::map<std::string, ofxBaseElement*>::iterator	ELEMENT_ITER;
typedef std::map<std::string, ofxPlane*>					PLANE_NAME_MAP;
typedef std::map<std::string, ofxPlane*>::iterator			PLANE_NAME_ITER;

typedef enum eTransitionsType
{
	eTRANSITION_NONE,
	eTRANSITION_FADE,
	eTRANSITION_VIDEO,

}TRANSITION_TYPE;

typedef struct _stTransitionVideo
{
	float				fPosition;
	ofPoint				DrawPos;
	ofVideoPlayer		Video;
	eBlendType			BlendType;

	_stTransitionVideo(ofVideoPlayer& Video, ofPoint DrawPos, float fPosition, eBlendType eType)
		:Video(Video)
		,DrawPos(DrawPos)
		,fPosition(fPosition)
		,BlendType(eType)
	{}
}stTransitionVideo;

class ofxDirector
{
public:
	ofxDirector();
	~ofxDirector();

	void SetStageSize(int iWidth, int iHeight);

public:
	void update(int iFrame = 0);
	void draw();

public:
	void Play(int iScenesIndex = 0);
	void Pause();
	void Replay();

private:
	int		m_iStageWidth, m_iStageHeight;

//---------
// Transitions
//---------
public:
	bool getIsTransitions();
	bool TransitTo(TRANSITION_TYPE eTransitionType = eTRANSITION_FADE, ofColor FadeoutColor = ofColor(255), bool bIsPause = false, string strVideoName = "");
	bool TransitTo(int iScenesIndex, TRANSITION_TYPE eTransitionType = eTRANSITION_FADE, ofColor FadeoutColor = ofColor(255), bool bIsPause = false, string strVideoName = "");
	bool TransitTo(std::string strScenesName, TRANSITION_TYPE eTransitionType = eTRANSITION_FADE, ofColor FadeoutColor = ofColor(255), bool bIsPause = false, string strVideoName = "");
	
	bool TransitByVideo(std::string strScenesName, string strVideoName);

	void AddTransititonVideo(string strName, string strFilePath, ofPtr<ofBaseVideoPlayer> pPlayer, float fPosition = 0.5, eBlendType bBlendType = eBLEND_ALPHA);

private:
	void TransitionUpdate(double dDiffSecond);
	void TransitionDraw();
	void PlayTheScene();

private:
	bool				m_bIsTransitions;
	TRANSITION_TYPE		m_eTransitionType;
	int					m_iNextScenesIndex;
	bool				m_bIsPause;
	float				m_fTransitionsTime;
	bool				m_bIsIn;

	//fade
	ofColor				m_FadeoutColor;
	float				m_fDiffAlpha;
	int					m_iAlpah;

	//video
	stTransitionVideo*					_NowVideo;
	map<string, stTransitionVideo>		_TransitionVideoMgr;

//---------
// Actor
//---------
public:
	bool AddActor(ofxBaseActor* pActorPtr);
	ofxBaseActor* GetActor(std::string strActorName);

private:
	ACTOR_MAP				m_oActorMgr;

//---------
// Stage
//---------
public:
	int AddScenes(std::string strScenesName);
	bool AddPlane(std::string strScneesName, std::string strPlaneName, unsigned char ucLevel);
	bool AddPlane(std::string strScneesName, std::string strPlaneName, unsigned char ucLevel, bool bIsVisible);
	bool AddPlane(std::string strScneesName, std::string strPlaneName, unsigned char ucLevel, ofPoint oPos, int iWidth, int iHeight, bool bIsVisible = true, bool bShowRange = false);
	bool AddElement(std::string strElementName, std::string strPlaneName, std::string strActorName, unsigned char ucLevel = 0, ofPoint oPos = ofPoint(0,0), bool bIsVisible = true);
	
	bool GetScenesPtr(std::string strScenesName, ofxScenes*& pScenesPtr);
	bool GetPlanePtr(std::string strPlaneName, ofxPlane*& pPlanePtr);
	template <typename T>
	bool GetElementPtr(std::string strName, T*& Ptr)
	{
		Ptr = nullptr;
		auto Iter_ = this->m_oElementMap.find(strName);
		if(Iter_ == this->m_oElementMap.end())
		{
			ofLogError("Can't find the element:" + strName);
			return false;
		}
		else
		{
			Ptr = dynamic_cast<T*>(Iter_->second);
			return true;
		}
	}

	bool GetElementPtr(std::string strName, ofxBaseElement*& Ptr)
	{
		Ptr = nullptr;
		auto Iter_ = this->m_oElementMap.find(strName);
		if(Iter_ == this->m_oElementMap.end())
		{
			ofLogError("Can't find the element:" + strName);
			return false;
		}
		else
		{
			Ptr = Iter_->second;
			return true;
		}
	}
	
	ofxScenes* GetNowScenes();

private:
	
	ofxBaseElement* CreateElement(ofxBaseActor* pActorPtr, std::string strElementName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible);

private:
	bool					m_bIsStart;
	std::vector<ofxScenes*>	m_oScenesList;
	PLANE_NAME_MAP			m_oPlaneMap;
	ELEMENT_MAP				m_oElementMap;
	int						m_iNowScenesIndex;

public:
	//Timer
	unsigned long long  m_uint64Timer;
	void ResetTimer();

//---------
// Animation
//---------
public:
	void AddAnimation(std::string strScenesName, unsigned char ucPriority, ofxBaseAnimation* pAnimation);
	void AddAnimation(unsigned char ucPriority, ofxBaseAnimation* pAnimation);

	void AddVideoAnimation(std::string strScenesName, ofxBaseAnimation* pAnimation);
};


#endif //OFX_THEATRE_DIRECTOR