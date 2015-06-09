#ifndef OFX_THEATRE_STAGE
#define OFX_THEATRE_STAGE

#include "ofxTheatreProtocol.h"
#include "ofxTheatreActor.h"
#include "ofxTheatreElement.h"
#include "ofxTheatreAnimator.h"

//--------------------------------------------------------------
//Plane Class
//--------------------------------------------------------------
typedef std::vector<std::map<std::string, ofxBaseElement*>>		ELEMENT_MANAGER;
typedef std::map<std::string, ofxBaseElement*>::iterator		ELEMENT_LEVEL_ITER;
typedef std::map<std::string, unsigned char>					ELEMENT_NAME_MAP;
typedef std::map<std::string, unsigned char>::iterator			ELEMENT_NAME_ITER;
class ofxPlane: public ofxBaseElement
{
public:
	ofxPlane(std::string strPlaneName, unsigned char ucLevel, ofPoint oPos, int iWidth, int iHeight, bool bIsVisible = true);
	~ofxPlane();

public:
	void update(double dDiffSecond);
	void draw(int iWidth, int iHeight, float fScale = 1.0);
	void Play();
	void Pause();
	void Stop();
	//void FadeIn();
	//void FadeOut();


	void SetShowRange(bool bShowRange);
	
public:
	bool AddElement(ofxBaseElement* pElement);
	ofxBaseElement* GetElement(std::string strElementName);
	bool DelElement(std::string strElementName);

private:
	bool				m_bShowRange;
	ELEMENT_MANAGER		m_oElementManager;
	ELEMENT_NAME_MAP	m_oElementNameMap;
};

//--------------------------------------------------------------
//Scenes Class
//--------------------------------------------------------------
typedef std::vector<ofxPlane*>					PLANE_MANAGER;
typedef std::vector<ofxPlane*>::iterator		PLANE_ITER;

class ofxScenes
{
public:
	ofxScenes(std::string strScenesName, int iWidth, int iHeight)
	  :m_strScenesName(strScenesName), m_iWidth(iWidth), m_iHeight(iHeight)
	{
		this->m_oPlaneManager.resize(MAX_LEVEL);
		this->m_pAnimator = new ofxAnimator(strScenesName);
	};
	~ofxScenes();

public:
	void update(double dDiffSecond, int iFrame = 0);
	void draw();
	void Play();
	void Pause();
	void Stop();

public:
	bool AddPlane(ofxPlane* pPlane);
	ofxPlane* GetPlane(std::string strPlaneName);
	std::string GetScenesName();

public:
	void AddAnimation(unsigned char ucAnimationPriority, ofxBaseAnimation* pAnimation);

	void AddVideoAnimation(ofxBaseAnimation* pAnimation);

private:
	ofxAnimator*	m_pAnimator;
	std::string		m_strScenesName;
	int				m_iWidth, m_iHeight;
	PLANE_MANAGER	m_oPlaneManager;
	

};

#endif //OFX_THEATRE_STAGE