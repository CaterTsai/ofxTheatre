#include "ofxTheatreDirector.h"


//--------------------------------------------------------------
//CLASS ofxDirector
//--------------------------------------------------------------
ofxDirector::ofxDirector()
	:m_iNowScenesIndex(0), m_bIsStart(false)
{	
	this->m_bIsTransitions   = false;
	this->m_bIsPause         = false;
	this->m_fDiffAlpha      = MAX_ALPHA;
	this->m_fTransitionsTime = 0.5;
	this->m_iAlpah			 = 0;

	_NowVideo = nullptr;

}

//--------------------------------------------------------------
ofxDirector::~ofxDirector()
{	
	//Clear Stage
	for(int idx_ = 0; idx_ < this->m_oScenesList.size(); ++idx_)
	{
		delete this->m_oScenesList[idx_];
	}
	this->m_oScenesList.clear();

	//Clear Actor
	ACTOR_ITER oActorIter_ = this->m_oActorMgr.begin();
	for(;oActorIter_ != this->m_oActorMgr.end();++oActorIter_)
	{
		delete oActorIter_->second;
	}

}

//--------------------------------------------------------------
void ofxDirector::SetStageSize(int iWidth, int iHeight)
	{
		this->m_iStageWidth  = iWidth;
		this->m_iStageHeight = iHeight;
	}

//--------------------------------------------------------------
void ofxDirector::update(int iFrame)
{
	if(this->m_bIsStart)
	{
		double dDiffSecond_ = (double)(ofGetElapsedTimeMillis() - this->m_uint64Timer) / 1000;
		this->m_uint64Timer = ofGetElapsedTimeMillis();
		this->m_oScenesList[this->m_iNowScenesIndex]->update(dDiffSecond_, iFrame);

		//Transition
		this->TransitionUpdate(dDiffSecond_);
	}
}

//--------------------------------------------------------------
void ofxDirector::draw()
{
	if(this->m_bIsStart)
	{
		
		this->m_oScenesList[this->m_iNowScenesIndex]->draw();

		//Transition
		this->TransitionDraw();
	}
}

//--------------------------------------------------------------
void ofxDirector::Play(int iScenesIndex)
{
	this->m_oScenesList[iScenesIndex]->Play();
	this->m_iNowScenesIndex  = iScenesIndex;
	this->m_uint64Timer      = ofGetElapsedTimeMillis();
	this->m_bIsStart         = true;
}

//--------------------------------------------------------------
void ofxDirector::Pause()
{
	this->m_bIsStart			= false;
}

//--------------------------------------------------------------
void ofxDirector::Replay()
{
	this->m_bIsStart			= false;
	for(int idx_ = 0; idx_ < this->m_oScenesList.size(); ++idx_)
	{
		this->m_oScenesList[idx_]->Stop();
	}

	this->Play();
	this->ResetTimer();
}

//--------------------------------------------------------------
//Transitions
//--------------------------------------------------------------
bool ofxDirector::getIsTransitions()
{
	return this->m_bIsTransitions;
}

//--------------------------------------------------------------
bool ofxDirector::TransitTo(TRANSITION_TYPE eTransitionType, ofColor FadeoutColor, bool bIsPause, string strVideoName)
{
	if(this->m_iNowScenesIndex + 1 >= this->m_oScenesList.size())
	{
		this->m_iNextScenesIndex = 0;
	}
	else
	{
		this->m_iNextScenesIndex = this->m_iNowScenesIndex + 1;
	}
	
	if(eTransitionType == eTRANSITION_VIDEO)
	{
		auto Iter_ = _TransitionVideoMgr.find(strVideoName);
		if(Iter_ == _TransitionVideoMgr.end())
		{
			eTransitionType = eTRANSITION_NONE;
		}
		else
		{
			_NowVideo = &(Iter_->second);
			_NowVideo->Video.play();
		}
	}

	this->m_eTransitionType = eTransitionType;
	this->m_bIsIn           = true;
	this->m_bIsTransitions  = true;
	this->m_bIsPause        = bIsPause;
	this->m_iAlpah			= 0;
	this->m_FadeoutColor	= FadeoutColor;
	//(this->m_bIsPause == true)?this->m_oScenesList[this->m_iNowScenesIndex]->Pause():this->m_oScenesList[this->m_iNowScenesIndex]->Stop();
	return true;
}

//--------------------------------------------------------------
bool ofxDirector::TransitTo(int iScenesIndex, TRANSITION_TYPE eTransitionType, ofColor FadeoutColor, bool bIsPause, string strVideoName)
{
	if(iScenesIndex < 0 || iScenesIndex >= this->m_oScenesList.size())
	{
		return false;
	}

	if(eTransitionType == eTRANSITION_VIDEO)
	{
		auto Iter_ = _TransitionVideoMgr.find(strVideoName);
		if(Iter_ == _TransitionVideoMgr.end())
		{
			eTransitionType = eTRANSITION_NONE;
		}
		else
		{
			_NowVideo = &(Iter_->second);
			_NowVideo->Video.play();
		}
	}

	this->m_iNextScenesIndex = iScenesIndex;
	this->m_eTransitionType  = eTransitionType;
	this->m_bIsIn            = true;
	this->m_bIsTransitions   = true;
	this->m_bIsPause         = bIsPause;
	this->m_iAlpah			 = 0;
	this->m_FadeoutColor	= FadeoutColor;
	//(this->m_bIsPause == true)?this->m_oScenesList[this->m_iNowScenesIndex]->Pause():this->m_oScenesList[this->m_iNowScenesIndex]->Stop();
	return true;
}

//--------------------------------------------------------------
bool ofxDirector::TransitTo(std::string strScenesName, TRANSITION_TYPE eTransitionType,  ofColor FadeoutColor, bool bIsPause, string strVideoName)
{
	if(strScenesName == this->m_oScenesList[this->m_iNowScenesIndex]->GetScenesName())
	{
		return false;
	}
	
	bool bIsFound_ = false;
	for(int idx_ = 0; idx_ < this->m_oScenesList.size(); ++idx_)
	{
		if(this->m_oScenesList[idx_]->GetScenesName() == strScenesName)
		{
			this->m_iNextScenesIndex = idx_;
			bIsFound_ = true;
			break;
		}
	}

	if(bIsFound_ == false)
	{
		return false;
	}

	if(eTransitionType == eTRANSITION_VIDEO)
	{
		auto Iter_ = _TransitionVideoMgr.find(strVideoName);
		if(Iter_ == _TransitionVideoMgr.end())
		{
			eTransitionType = eTRANSITION_NONE;
		}
		else
		{
			_NowVideo = &(Iter_->second);
			_NowVideo->Video.play();
		}
	}

	this->m_eTransitionType = eTransitionType;
	this->m_bIsIn           = true;
	this->m_bIsTransitions  = true;
	this->m_bIsPause        = bIsPause;
	this->m_iAlpah			= 0;
	this->m_FadeoutColor	= FadeoutColor;
	//(this->m_bIsPause == true)?this->m_oScenesList[this->m_iNowScenesIndex]->Pause():this->m_oScenesList[this->m_iNowScenesIndex]->Stop();
	return true;
}

//--------------------------------------------------------------
bool ofxDirector::TransitByVideo(std::string strScenesName, string strVideoName)
{
	if(strScenesName == this->m_oScenesList[this->m_iNowScenesIndex]->GetScenesName())
	{
		return false;
	}
	
	bool bIsFound_ = false;
	for(int idx_ = 0; idx_ < this->m_oScenesList.size(); ++idx_)
	{
		if(this->m_oScenesList[idx_]->GetScenesName() == strScenesName)
		{
			this->m_iNextScenesIndex = idx_;
			bIsFound_ = true;
			break;
		}
	}

	if(bIsFound_ == false)
	{
		return false;
	}

	auto Iter_ = _TransitionVideoMgr.find(strVideoName);
	_NowVideo = &(Iter_->second);
	_NowVideo->Video.play();
	this->m_eTransitionType = eTRANSITION_VIDEO;
	this->m_bIsIn           = true;
	this->m_bIsTransitions  = true;
	this->m_bIsPause        = false;
	this->m_iAlpah			= 0;
	return true;
}

//--------------------------------------------------------------
void ofxDirector::AddTransititonVideo(string strName, string strFilePath,  ofPtr<ofBaseVideoPlayer> pPlayer, float fPosition, eBlendType bBlendType)
{
	if(_TransitionVideoMgr.find(strName) != _TransitionVideoMgr.end())
	{
		ofLogWarning("Transition Video is duplicate :" + strName);
		return;
	}

	ofVideoPlayer	VideoPlayer_;
	VideoPlayer_.setPlayer(pPlayer);
	VideoPlayer_.setLoopState(ofLoopType::OF_LOOP_NONE);
	if(bBlendType == eBLEND_ALPHA)
	{
		VideoPlayer_.setPixelFormat(ofPixelFormat::OF_PIXELS_RGBA);
	}

	if(!VideoPlayer_.loadMovie(strFilePath))
	{
		ofLogWarning("Load transition video failed :" + strName);
		return;
	}

	stTransitionVideo	NewTransitionVideo(VideoPlayer_, ofPoint(0, 0), fPosition, bBlendType);
	_TransitionVideoMgr.insert(make_pair(strName, NewTransitionVideo));
}



//--------------------------------------------------------------
void ofxDirector::TransitionUpdate(double dDiffSecond)
{
	if(this->m_bIsTransitions)
	{
		if(this->m_bIsIn)
		{			
			switch(this->m_eTransitionType)
			{
			case eTRANSITION_NONE:
				{					
					this->m_bIsIn           = false;
					(this->m_bIsPause == true)?this->m_oScenesList[this->m_iNowScenesIndex]->Pause():this->m_oScenesList[this->m_iNowScenesIndex]->Stop();
					this->m_iNowScenesIndex = this->m_iNextScenesIndex;
					this->PlayTheScene();

					ofxTheatreEventArgs oEvent_;
					oEvent_.strMessage = this->m_oScenesList[this->m_iNowScenesIndex]->GetScenesName();
					ofNotifyEvent(ofxTheatreEventArgs::TheatreEvent, oEvent_);
					break;
				}
			case eTRANSITION_FADE:
				{
					if(this->m_iAlpah >= MAX_ALPHA)
					{			
						this->m_bIsIn  = false;
						this->m_iAlpah = MAX_ALPHA;
						(this->m_bIsPause == true)?this->m_oScenesList[this->m_iNowScenesIndex]->Pause():this->m_oScenesList[this->m_iNowScenesIndex]->Stop();
						this->m_iNowScenesIndex = this->m_iNextScenesIndex;
						this->PlayTheScene();

						ofxTheatreEventArgs oEvent_;
						oEvent_.strMessage = this->m_oScenesList[this->m_iNowScenesIndex]->GetScenesName();
						ofNotifyEvent(ofxTheatreEventArgs::TheatreEvent, oEvent_);
					}
					else
					{
						this->m_iAlpah += floor(this->m_fDiffAlpha * dDiffSecond);
					}
					break;
				}
			case eTRANSITION_VIDEO:
				{
					if(_NowVideo == nullptr)
					{
						return;
					}
					_NowVideo->Video.update();

					if(_NowVideo->Video.getPosition() >= _NowVideo->fPosition)
					{
						this->m_bIsIn  = false;
						(this->m_bIsPause == true)?this->m_oScenesList[this->m_iNowScenesIndex]->Pause():this->m_oScenesList[this->m_iNowScenesIndex]->Stop();
						this->m_iNowScenesIndex = this->m_iNextScenesIndex;
						this->PlayTheScene();

						ofxTheatreEventArgs oEvent_;
						oEvent_.strMessage = this->m_oScenesList[this->m_iNowScenesIndex]->GetScenesName();
						ofNotifyEvent(ofxTheatreEventArgs::TheatreEvent, oEvent_);
					}

					break;
				}
			default:
				{
					ofLogWarning("Unknown transition type :" + ofToString(this->m_eTransitionType));
					break;
				}
			}
		}
		else
		{
			switch(this->m_eTransitionType)
			{
			case eTRANSITION_NONE:
				{
					this->m_bIsTransitions = false;
					break;
				}
			case eTRANSITION_FADE:
				{
					if(this->m_iAlpah <= 0)
					{
						this->m_iAlpah = 0;
						this->m_bIsTransitions = false;
					}
					else
					{
						this->m_iAlpah -= floor(this->m_fDiffAlpha * dDiffSecond);
					}
					break;
				}
			case eTRANSITION_VIDEO:
				{
					if(_NowVideo == nullptr)
					{
						return;
					}

					_NowVideo->Video.update();
					if(_NowVideo->Video.getIsMovieDone())
					{
						this->m_bIsTransitions = false;
						_NowVideo->Video.setFrame(0);
						_NowVideo->Video.update();
						_NowVideo->Video.stop();
					}
					break;
				}
			default:
				{
					ofLogWarning("Unknown transition type :" + ofToString(this->m_eTransitionType));
					break;
				}
			}

		}
	}
}

//--------------------------------------------------------------
void ofxDirector::TransitionDraw()
{
	ofPushStyle();
	if(this->m_bIsTransitions)
	{
		switch(this->m_eTransitionType)
		{
		case eTRANSITION_FADE:
			{
				ofEnableAlphaBlending();
				ofSetColor(this->m_FadeoutColor, this->m_iAlpah);
				ofFill();
				ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
				ofDisableAlphaBlending();
				break;
			}
		case eTRANSITION_VIDEO:
			{
				switch(_NowVideo->BlendType)
				{
				case eBLEND_ALPHA:
					{
						ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);
					}
					break;
				case eBLEND_ADD:
					{
						ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
					}
					break;
				case eBLEND_SCREEN:
					{
						ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_SCREEN);
					}
					break;
				}
				ofSetColor(255);
				if(_NowVideo != nullptr)
				{
					_NowVideo->Video.draw(_NowVideo->DrawPos);
				}
				ofDisableBlendMode();
				break;
			}
		case eTRANSITION_NONE:
		default:
			{
				break;
			}
		}
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofxDirector::PlayTheScene()
{
	this->m_oScenesList[this->m_iNowScenesIndex]->Play();
	this->ResetTimer();
}

//--------------------------------------------------------------
//Actor
//--------------------------------------------------------------
bool ofxDirector::AddActor(ofxBaseActor* pActorPtr)
{
	ACTOR_ITER oMapIter_;
	oMapIter_ = this->m_oActorMgr.find(pActorPtr->m_strActorName);

	if(oMapIter_ ==  this->m_oActorMgr.end())
	{
		this->m_oActorMgr.insert(::make_pair(pActorPtr->m_strActorName, pActorPtr));
		return true;
	}
	else
	{
		ofLogError("This actor is already exist:" + pActorPtr->m_strActorName);
		return false;
	}
}

//--------------------------------------------------------------
ofxBaseActor* ofxDirector::GetActor(std::string strActorName)
{
	ACTOR_ITER oMapIter_;

	oMapIter_ = this->m_oActorMgr.find(strActorName);

	if(oMapIter_ ==  this->m_oActorMgr.end())
	{
		ofLogError("Can't find the actor:" + strActorName);
		return NULL;
	}
	else
	{		
		return oMapIter_->second;
	}

}

//--------------------------------------------------------------
//Stage
//--------------------------------------------------------------
int ofxDirector::AddScenes(std::string strScenesName)
{
	ofxScenes* pScenesPtr_ = NULL;
	if(this->GetScenesPtr(strScenesName, pScenesPtr_))
	{
		ofLogError("This scenes is already exist:" + strScenesName);
		return -1;
	}
	else
	{
		pScenesPtr_ = new ofxScenes(strScenesName, this->m_iStageWidth, this->m_iStageHeight);
		this->m_oScenesList.push_back(pScenesPtr_);
		return this->m_oScenesList.size();
	}	
}

//--------------------------------------------------------------
bool ofxDirector::AddPlane(std::string strScenesName, std::string strPlaneName, unsigned char ucLevel)
{
	return this->AddPlane(strScenesName, strPlaneName, ucLevel, ofPoint(0, 0), m_iStageWidth, m_iStageHeight, true, false);
}

//--------------------------------------------------------------
bool ofxDirector::AddPlane(std::string strScneesName, std::string strPlaneName, unsigned char ucLevel, bool bIsVisible)
{
	return this->AddPlane(strScneesName, strPlaneName, ucLevel, ofPoint(0, 0), m_iStageWidth, m_iStageHeight, bIsVisible, false);
}

//--------------------------------------------------------------
bool ofxDirector::AddPlane(std::string strScenesName, std::string strPlaneName, unsigned char ucLevel, ofPoint oPos, int iWidth, int iHeight, bool bIsVisible, bool bShowRange)
{
	ofxScenes* pScenesPtr_ = NULL;
	if(!this->GetScenesPtr(strScenesName, pScenesPtr_))
	{
		//Don't exist, Create it!
		int iScenesIndex_ = this->AddScenes(strScenesName);
		pScenesPtr_ = this->m_oScenesList[iScenesIndex_];
	}

	ofxPlane* pPlanePtr_ = new ofxPlane(strPlaneName, ucLevel, oPos, iWidth, iHeight, bIsVisible);
	pPlanePtr_->SetShowRange(bShowRange);
	bool bResult_ = pScenesPtr_->AddPlane(pPlanePtr_);

	if(bResult_)
	{
		this->m_oPlaneMap.insert(::make_pair(strPlaneName, pPlanePtr_));
	}

	return bResult_;
}

//--------------------------------------------------------------
bool ofxDirector::AddElement(std::string strElementName, std::string strPlaneName, std::string strActorName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible)
{
	//Get Actor
	ofxBaseActor* pActorPtr_;
	pActorPtr_ = this->GetActor(strActorName);
	if(pActorPtr_ == NULL)
	{
		ofLogError("This actor is not exist:" + strActorName);
		return false;
	}

	//Get Plane
	PLANE_NAME_ITER oPlaneIter_;
	oPlaneIter_ = this->m_oPlaneMap.find(strPlaneName);
	if(oPlaneIter_ == this->m_oPlaneMap.end())
	{
		ofLogError("This plane is not exist:" + strPlaneName);
		return false;
	}

	//Create Element
	ofxBaseElement* pElementPtr_ =  this->CreateElement(pActorPtr_, strElementName, ucLevel, oPos, bIsVisible);
	if(pElementPtr_ == NULL)
	{
		return false;
	}

	pElementPtr_->m_pPlanePtr = oPlaneIter_->second;

	//Insert into ELEMENT_MAP and plane
	bool bResult_ = false;
	ELEMENT_ITER oElementIter_  = this->m_oElementMap.find(strElementName);
	if(oElementIter_ == this->m_oElementMap.end())
	{
		this->m_oElementMap.insert(::make_pair(strElementName, pElementPtr_));
		bResult_ = oPlaneIter_->second->AddElement(pElementPtr_);
	}

	return bResult_;
}

//--------------------------------------------------------------
bool ofxDirector::GetScenesPtr(std::string strScenesName, ofxScenes*& pScenesPtr)
{
	pScenesPtr = NULL;

	bool bIsFound_ = false;
	for(int idx_ = 0; idx_ < this->m_oScenesList.size(); ++idx_)
	{
		if(this->m_oScenesList[idx_]->GetScenesName() == strScenesName)
		{
			pScenesPtr = this->m_oScenesList[idx_];
			bIsFound_ = true;
			break;
		}
	}

	return bIsFound_;
}

//--------------------------------------------------------------
bool ofxDirector::GetPlanePtr(std::string strPlaneName, ofxPlane*& pPlanePtr)
{
	pPlanePtr = NULL;

	//Get Plane
	PLANE_NAME_ITER oPlaneIter_;
	oPlaneIter_ = this->m_oPlaneMap.find(strPlaneName);
	if(oPlaneIter_ == this->m_oPlaneMap.end())
	{
		ofLogError("This plane is not exist:" + strPlaneName);
		return false;
	}

	pPlanePtr = oPlaneIter_->second;

	return true;
}

////--------------------------------------------------------------
//bool ofxDirector::GetElementPtr(std::string strElementName, ofxBaseElement*& pElementPtr)
//{
//	pElementPtr = NULL;
//
//	ELEMENT_ITER oElementIter_ = this->m_oElementMap.find(strElementName);
//	if(oElementIter_ == this->m_oElementMap.end())
//	{
//		ofLogError("Can't find the element:" + strElementName);
//		return false;
//	}
//	else
//	{
//		pElementPtr = oElementIter_->second;
//		return true;
//	}
//}
//
////--------------------------------------------------------------
//bool ofxDirector::GetVideoPtr(std::string strVideoName, ofxVideoElement*& pVideoPtr)
//{
//	ofxBaseElement* pElementPtr_;
//	if(this->GetElementPtr(strVideoName, pElementPtr_))
//	{
//		pVideoPtr = dynamic_cast<ofxVideoElement*>(pElementPtr_);
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}


//--------------------------------------------------------------
ofxScenes* ofxDirector::GetNowScenes()
{
	
	return this->m_oScenesList[this->m_iNowScenesIndex];
}

//--------------------------------------------------------------
ofxBaseElement* ofxDirector::CreateElement(ofxBaseActor* pActorPtr, std::string strElementName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible)
{
	ofxBaseElement* pBaseElementPtr_ = NULL;
	switch(pActorPtr->m_ucActorType)
	{
	case eACTOR_EMPTY_TYPE:
		{
			pBaseElementPtr_ = new ofxEmptyElement(strElementName, ucLevel, oPos, bIsVisible, static_cast<ofxEmptyActor*>(pActorPtr));
			break;
		}
	case eACTOR_IMAGE_TYPE:
		{
			pBaseElementPtr_ = new ofxImageElement(strElementName, ucLevel, oPos, bIsVisible, static_cast<ofxImageActor*>(pActorPtr));
			break;
		}
	case eACTOR_DYNAMIC_IMAGE_TYPE:
		{
			pBaseElementPtr_ = new ofxDynamicImageElement(strElementName, ucLevel, oPos, bIsVisible, static_cast<ofxDynamicImageActor*>(pActorPtr));
			break;
		}
	case eACTOR_ANIMATION_IMAGE_TYPE:
		{
			pBaseElementPtr_ = new ofxAnimationImageElement(strElementName, ucLevel, oPos, bIsVisible, static_cast<ofxAnimationImageActor*>(pActorPtr));
			break;
		}
	case eACTOR_VIDEO_TYPE:
		{
			pBaseElementPtr_ = new ofxVideoElement(strElementName, ucLevel, oPos, bIsVisible, static_cast<ofxVideoActor*>(pActorPtr));
			break;
		}
	case eACTOR_WEBCAM_TYPE:
		{
			pBaseElementPtr_ = new ofxWebcamElement(strElementName, ucLevel, oPos, bIsVisible, static_cast<ofxWebcamActor*>(pActorPtr));
			break;
		}
	case eACTOR_SOUND_TYPE:
		{
			pBaseElementPtr_ = new ofxSoundElement(strElementName, ucLevel, oPos, bIsVisible, static_cast<ofxSoundActor*>(pActorPtr));
			break;
		}
	default:
		{
			ofLogError("Unknows Actor Type" + ofToString(pActorPtr->m_ucActorType));
			break;
		}
	}
	
	return pBaseElementPtr_;
}

//--------------------------------------------------------------
void ofxDirector::ResetTimer()
{
	this->m_uint64Timer     = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void ofxDirector::AddAnimation(std::string strScenesName, unsigned char ucPriority, ofxBaseAnimation* pAnimation)
{
	ofxScenes* pScenesPtr_;
	this->GetScenesPtr(strScenesName, pScenesPtr_);
	pScenesPtr_->AddAnimation(ucPriority, pAnimation);
}


//--------------------------------------------------------------
void ofxDirector::AddAnimation(unsigned char ucPriority, ofxBaseAnimation* pAnimation)
{
	ofxScenes* pNowScenesPtr_ =  this->GetNowScenes();
	pNowScenesPtr_->AddAnimation(ucPriority, pAnimation);
}

//--------------------------------------------------------------
void ofxDirector::AddVideoAnimation(std::string strScenesName, ofxBaseAnimation* pAnimation)
{
	ofxScenes* pScenesPtr_;
	this->GetScenesPtr(strScenesName, pScenesPtr_);
	pScenesPtr_->AddVideoAnimation(pAnimation);
}
