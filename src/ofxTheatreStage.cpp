#include "ofxTheatreStage.h"

//--------------------------------------------------------------
//CLASS ofxPlane
//--------------------------------------------------------------
ofxPlane::ofxPlane(std::string strPlaneName, unsigned char ucLevel, ofPoint oPos, int iWidth, int iHeight, bool bIsVisible)
	:ofxBaseElement(strPlaneName, ucLevel, oPos, bIsVisible)
{
	this->m_stElementBase.iWidth		= iWidth;
	this->m_stElementBase.iHeight   = iHeight;

	this->m_stElementBaseBackup		= this->m_stElementBase;
	this->m_oElementManager.resize(MAX_LEVEL);
}

//--------------------------------------------------------------
ofxPlane::~ofxPlane()
{
	//delete each element
	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
	{
		if(this->m_oElementManager[idx_].size() > 0)
		{
			ELEMENT_LEVEL_ITER oIter_ = this->m_oElementManager[idx_].begin();
			for(;oIter_ != this->m_oElementManager[idx_].end();++oIter_)
			{
				delete oIter_->second;
			}
		}
	}

	this->m_oElementNameMap.clear();
}

//--------------------------------------------------------------
void ofxPlane::update(double dDiffSecond)
{
	//execute update for each element
	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
	{
		if(this->m_oElementManager[idx_].size() > 0)
		{
			ELEMENT_LEVEL_ITER oIter_ = this->m_oElementManager[idx_].begin();
			for(;oIter_ != this->m_oElementManager[idx_].end();++oIter_)
			{
				oIter_->second->update(dDiffSecond);
			}
		}
	}
}

//--------------------------------------------------------------
void ofxPlane::draw(int iWidth, int iHeight, float fScale)
{
	if(this->m_stElementBase.bIsVisible)
	{
		ofPushStyle();
		//Translate
		ofPushMatrix();
		ofTranslate(this->m_stElementBase.oPos);
		
		//Rotate
		ofPoint oCenterPoint(this->m_stElementBase.iWidth/2, this->m_stElementBase.iHeight/2);
		ofTranslate(oCenterPoint);
		ofRotateZ(this->m_stElementBase.fDegree);
		ofTranslate(-oCenterPoint);

		//Scale
		int iTmpWidth_  = this->m_stElementBase.iWidth * this->m_stElementBase.fScale;
		int iTmpHeight_ = this->m_stElementBase.iHeight * this->m_stElementBase.fScale;

		////Check ragne
		//if(this->m_stElementBase.oPos.x + iTmpWidth_ > iWidth)
		//{
		//	this->m_stElementBase.oPos.x = iWidth - iTmpWidth_;
		//}

		//if(this->m_stElementBase.oPos.y + iTmpHeight_ > iHeight)
		//{
		//	this->m_stElementBase.oPos.y = iHeight - iTmpHeight_;
		//}
		
		//Show plane range
		if(this->m_bShowRange)
		{
			ofSetHexColor(0xff0000);
			ofNoFill();
			ofRect(0, 0, iTmpWidth_, iTmpHeight_);
			ofSetHexColor(0xffffff);
		}
		
		if(this->m_stElementBase.bFadeIn || this->m_stElementBase.bFadeOut)
		{
			ofEnableAlphaBlending();
			ofSetColor(255,255,255,(int)this->m_stElementBase.fAlpah);
		}
		else
		{
			ofSetColor(255);
		}
		
		//execute draw for each element
		for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
		{
			if(this->m_oElementManager[idx_].size() > 0)
			{
				ELEMENT_LEVEL_ITER oIter_ = this->m_oElementManager[idx_].begin();
				for(;oIter_ != this->m_oElementManager[idx_].end();++oIter_)
				{
					oIter_->second->draw(this->m_stElementBase.iWidth, this->m_stElementBase.iHeight, this->m_stElementBase.fScale);
				}
			}
		}
		
		if(this->m_stElementBase.bFadeIn || this->m_stElementBase.bFadeOut)
		{
			ofDisableAlphaBlending();
			ofSetColor(255,255,255);
		}

		ofPopMatrix();
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofxPlane::Play()
{
	ofxBaseElement::Play();

	//execute Play for each element
	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
	{
		if(this->m_oElementManager[idx_].size() > 0)
		{
			ELEMENT_LEVEL_ITER oIter_ = this->m_oElementManager[idx_].begin();
			for(;oIter_ != this->m_oElementManager[idx_].end();++oIter_)
			{
				oIter_->second->Play();
			}
		}
	}
}

//--------------------------------------------------------------
void ofxPlane::Pause()
{
	ofxBaseElement::Pause();

	//execute Pause for each element
	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
	{
		if(this->m_oElementManager[idx_].size() > 0)
		{
			ELEMENT_LEVEL_ITER oIter_ = this->m_oElementManager[idx_].begin();
			for(;oIter_ != this->m_oElementManager[idx_].end();++oIter_)
			{
				oIter_->second->Pause();
			}
		}
	}
}

//--------------------------------------------------------------
void ofxPlane::Stop()
{
	ofxBaseElement::Stop();

	//execute Stop for each element
	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
	{
		if(this->m_oElementManager[idx_].size() > 0)
		{
			ELEMENT_LEVEL_ITER oIter_ = this->m_oElementManager[idx_].begin();
			for(;oIter_ != this->m_oElementManager[idx_].end();++oIter_)
			{
				oIter_->second->Stop();
			}
		}
	}
}
//
////--------------------------------------------------------------
//void ofxPlane::FadeIn()
//{
//	//About visible element
//	ofxBaseElement::FadeIn();
//
//	//About invisible element
//	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
//	{
//		if(this->m_oElementManager[idx_].size() > 0)
//		{
//			ELEMENT_LEVEL_ITER oIter_ = this->m_oElementManager[idx_].begin();
//			for(;oIter_ != this->m_oElementManager[idx_].end();++oIter_)
//			{
//				if(oIter_->second->m_pBaseActor->m_ucActorType > eACTOR_INVISIBLE_TYPE)
//				{
//					oIter_->second->FadeIn();
//				}
//			}
//		}
//	}
//}
//
////--------------------------------------------------------------
//void ofxPlane::FadeOut()
//{
//	//About visible element
//	ofxBaseElement::FadeOut();
//
//	//About invisible element
//	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
//	{
//		if(this->m_oElementManager[idx_].size() > 0)
//		{
//			ELEMENT_LEVEL_ITER oIter_ = this->m_oElementManager[idx_].begin();
//			for(;oIter_ != this->m_oElementManager[idx_].end();++oIter_)
//			{
//				if(oIter_->second->m_pBaseActor->m_ucActorType > eACTOR_INVISIBLE_TYPE)
//				{
//					oIter_->second->FadeOut();
//				}
//			}
//		}
//	}
//}

//--------------------------------------------------------------
void ofxPlane::SetShowRange(bool bShowRange)
{
	this->m_bShowRange = bShowRange;
}

//--------------------------------------------------------------
bool ofxPlane::AddElement(ofxBaseElement* pElement)
{
	ELEMENT_NAME_ITER oMapIter_;
	oMapIter_ = this->m_oElementNameMap.find(pElement->m_strElementName);
	
	if(oMapIter_ != this->m_oElementNameMap.end())
	{
		ofLogError("Element name is duplicate:" + pElement->m_strElementName);
		delete pElement;
		return false;
	}
	else
	{
		if(pElement->m_stElementBase.ucLevel >= MAX_LEVEL)
		{
			ofLogWarning("Out of maximum level!! reset to maximum levels");
			pElement->m_stElementBase.ucLevel = MAX_LEVEL - 1;
		}
		
		this->m_oElementNameMap.insert(make_pair(pElement->m_strElementName, pElement->m_stElementBase.ucLevel));
		this->m_oElementManager[pElement->m_stElementBase.ucLevel].insert(make_pair(pElement->m_strElementName, pElement));
		return true;
	}
}

//--------------------------------------------------------------
ofxBaseElement* ofxPlane::GetElement(std::string strActorName)
{
	//Find this element level
	ELEMENT_NAME_ITER oMapIter_;
	oMapIter_ = this->m_oElementNameMap.find(strActorName);

	if(oMapIter_ == this->m_oElementNameMap.end())
	{
		ofLogError("This element was not be inset yet:" + strActorName);
		return NULL;
	}
	else
	{
		ELEMENT_LEVEL_ITER oLevelIter_ = this->m_oElementManager[oMapIter_->second].find(strActorName);
		if(oLevelIter_ == this->m_oElementManager[oMapIter_->second].end())
		{
			ofLogError("Manager is not match Name Map");
			return NULL;
		}
		
		return oLevelIter_->second;
	}

}

//--------------------------------------------------------------
bool ofxPlane::DelElement(std::string strElementName)
{
	return true;
}

//--------------------------------------------------------------
//CLASS ofxScenes
//--------------------------------------------------------------
ofxScenes::~ofxScenes()
{
	delete this->m_pAnimator;
	
	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
	{
		if(this->m_oPlaneManager[idx_])
		{
			delete this->m_oPlaneManager[idx_];
		}
	}
}

//--------------------------------------------------------------
void ofxScenes::update(double dDiffSecond, int iFrame)
{
	this->m_pAnimator->AnimatorUpdate(dDiffSecond);
	this->m_pAnimator->VideoAnimatorUpdate(dDiffSecond, iFrame);
	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
	{
		if(this->m_oPlaneManager[idx_])
		{
			this->m_oPlaneManager[idx_]->update(dDiffSecond);
		}
	}
}

//--------------------------------------------------------------
void ofxScenes::draw()
{
	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
	{
		if(this->m_oPlaneManager[idx_])
		{
			this->m_oPlaneManager[idx_]->draw(this->m_iWidth, this->m_iHeight);
		}
	}
}

//--------------------------------------------------------------
void ofxScenes::Play()
{
	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
	{
		if(this->m_oPlaneManager[idx_])
		{
			this->m_oPlaneManager[idx_]->Play();
		}
	}
}

//--------------------------------------------------------------
void ofxScenes::Pause()
{
	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
	{
		if(this->m_oPlaneManager[idx_])
		{
			this->m_oPlaneManager[idx_]->Pause();
		}
	}
}

//--------------------------------------------------------------
void ofxScenes::Stop()
{
	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
	{
		if(this->m_oPlaneManager[idx_])
		{
			this->m_oPlaneManager[idx_]->Stop();
		}
	}

	this->m_pAnimator->ClearAnimation();
	this->m_pAnimator->ClearVideoAnimation();
}

//--------------------------------------------------------------
bool ofxScenes::AddPlane(ofxPlane* pPlane)
{
	unsigned char ucTmpLevel_ = pPlane->m_stElementBase.ucLevel;

	if(!this->m_oPlaneManager[ucTmpLevel_])
	{
		this->m_oPlaneManager[ucTmpLevel_] = pPlane;
		return true;
	}
	else
	{
		ofLogError("Plane's level in this sence is not empty");
		delete pPlane;
		return false;
	}
}

//--------------------------------------------------------------
ofxPlane* ofxScenes::GetPlane(std::string strPlaneName)
{
	std::string strTmpName_;
	ofxPlane*	pPlanePtr_ = NULL;
	for(int idx_ = 0; idx_ < MAX_LEVEL; ++idx_)
	{
		if(this->m_oPlaneManager[idx_])
		{
			strTmpName_ = this->m_oPlaneManager[idx_]->m_strElementName;

			if(strTmpName_ == strPlaneName)
			{
				pPlanePtr_ = this->m_oPlaneManager[idx_];
			}
		}
	}
	return pPlanePtr_;
}

//--------------------------------------------------------------
std::string ofxScenes::GetScenesName()
{
	
	return this->m_strScenesName;
}

//--------------------------------------------------------------
void ofxScenes::AddAnimation(unsigned char ucAnimationPriority, ofxBaseAnimation* pAnimation)
{
	this->m_pAnimator->AddAnimation(ucAnimationPriority, pAnimation);
}

//--------------------------------------------------------------
void ofxScenes::AddVideoAnimation(ofxBaseAnimation* pAnimation)
{
	this->m_pAnimator->AddVideoAnimation(pAnimation);
}