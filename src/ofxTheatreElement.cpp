#include "ofxTheatreElement.h"

#pragma region ofxBaseElement

//--------------------------------------------------------------
//CLASS ofxBaseElement
//--------------------------------------------------------------
ofxBaseElement::ofxBaseElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible)
{
	//Name
	this->m_strElementName = strName;

	//for reset
	this->m_stElementBase.ucLevel	        = ucLevel;
	this->m_stElementBase.oPos              = oPos;
	this->m_stElementBase.fDegree           = 0.0;
	this->m_stElementBase.fScale            = 1.0;
	this->m_stElementBase.fAlpah				= MAX_ALPHA;
	this->m_stElementBase.bIsVisible        = bIsVisible;
	this->m_stElementBase.bIsDrawSubsection = false;
	this->m_stElementBase.bIsMirror			= false;
	this->m_stElementBase.oStartPos			= ofPoint(0,0);
	this->m_stElementBase.iSubWidth			= 0;
	this->m_stElementBase.iSubHeight			= 0;

	this->m_stElementBase.bFadeIn	 = false;
	this->m_stElementBase.bFadeOut   = false;
	this->m_stElementBase.bIsMove	 = false;
	this->m_stElementBase.bIsRotate	 = false;
	this->m_stElementBase.bIsScale	 = false;
	this->m_stElementBase.bIsShake	 = false;
	
	
	this->m_stElementBaseBackup		 = this->m_stElementBase;
	
	this->m_pPlanePtr	  = NULL;
	this->m_pBaseActor    = NULL;
}

//--------------------------------------------------------------
void ofxBaseElement::Stop()
{
	this->m_stElementBase = this->m_stElementBaseBackup;
}

//--------------------------------------------------------------
void ofxBaseElement::SetVisible(const bool bValue)
{
	this->m_stElementBase.bIsVisible = bValue;
}

//--------------------------------------------------------------
void ofxBaseElement::SetPos(const ofPoint oNewPos)
{
	this->m_stElementBase.oPos = oNewPos;
}


//--------------------------------------------------------------
int ofxBaseElement::GetWidth()
{
	return this->m_stElementBase.iWidth;
}

//--------------------------------------------------------------
int ofxBaseElement::GetHeight()
{
	return this->m_stElementBase.iHeight;
}
//--------------------------------------------------------------
void ofxBaseElement::SetParentsPlane(ofxBaseElement* pPlanePtr)
{
	this->m_pPlanePtr = pPlanePtr;
}

//--------------------------------------------------------------
ofxBaseElement* ofxBaseElement::GetParentsPlane()
{
	return this->m_pPlanePtr;
}
#pragma endregion

#pragma region CLASS ofxImageElement
//--------------------------------------------------------------
//CLASS ofxImageElement
//--------------------------------------------------------------
ofxImageElement::ofxImageElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxImageActor* pActor)
	:ofxBaseElement(strName, ucLevel, oPos, bIsVisible)
{
	this->m_pImageActor = pActor;
	this->m_pBaseActor = this->m_pImageActor;
	
	this->m_stElementBase.iHeight = this->m_pImageActor->m_oImage.height;
	this->m_stElementBase.iWidth  = this->m_pImageActor->m_oImage.width;
	this->m_stElementBaseBackup   = this->m_stElementBase;

};

//--------------------------------------------------------------
void ofxImageElement::draw(int iWidth, int iHeight, float fScale)
{
	if(!this->m_stElementBase.bIsVisible)
	{
		return;
	}

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
	int iTmpWidth_, iTmpHeight_;
	if(this->m_stElementBase.bIsDrawSubsection)
	{
		iTmpWidth_  = this->m_stElementBase.iSubWidth * this->m_stElementBase.fScale * fScale;
		iTmpHeight_ = this->m_stElementBase.iSubHeight * this->m_stElementBase.fScale * fScale;
	}
	else
	{
		iTmpWidth_  = this->m_stElementBase.iWidth * this->m_stElementBase.fScale * fScale;
		iTmpHeight_ = this->m_stElementBase.iHeight * this->m_stElementBase.fScale * fScale;
	}

	//Mirror
	if(this->m_stElementBase.bIsMirror)
	{
		ofTranslate(iTmpWidth_, 0);
		ofScale(-1, 1);
	}

	switch(this->m_pImageActor->_eBlendType)
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

	//Fade In or Fade Out
	if(this->m_stElementBase.bFadeIn || this->m_stElementBase.bFadeOut)
	{
		ofEnableAlphaBlending();
		ofSetColor(255,255,255,(int)this->m_stElementBase.fAlpah);
			
		if(this->m_stElementBase.bIsDrawSubsection)
		{
			this->m_pImageActor->m_oImage.drawSubsection(0, 0, iTmpWidth_, iTmpHeight_, this->m_stElementBase.oStartPos.x, this->m_stElementBase.oStartPos.y, this->m_stElementBase.iSubWidth, this->m_stElementBase.iSubHeight);
		}
		else
		{
			this->m_pImageActor->m_oImage.draw(0, 0, iTmpWidth_, iTmpHeight_);
		}
		ofDisableAlphaBlending();
		ofSetColor(255,255,255);
			
	}
	else
	{
		if(this->m_stElementBase.bIsDrawSubsection)
		{
			this->m_pImageActor->m_oImage.drawSubsection(0, 0, iTmpWidth_, iTmpHeight_, this->m_stElementBase.oStartPos.x, this->m_stElementBase.oStartPos.y, this->m_stElementBase.iSubWidth, this->m_stElementBase.iSubHeight);
		}
		else
		{
			this->m_pImageActor->m_oImage.draw(0, 0, iTmpWidth_, iTmpHeight_);
		}
	}
	ofDisableAlphaBlending();
	ofPopMatrix();
	ofPopStyle();
	
}
#pragma endregion

#pragma region CLASS ofxDynamicImageElement
//--------------------------------------------------------------
//CLASS ofxDynamicImageElement
//--------------------------------------------------------------
ofxDynamicImageElement::ofxDynamicImageElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxDynamicImageActor* pActor)
	:ofxBaseElement(strName, ucLevel, oPos, bIsVisible)
{
	this->m_pImageActor = pActor;
	this->m_pBaseActor = this->m_pImageActor;
};

//--------------------------------------------------------------
void ofxDynamicImageElement::draw(int iWidth, int iHeight, float fScale)
{
	
	if(this->m_stElementBase.bIsVisible && this->m_pImageActor->m_oImage.bAllocated())
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
		int iTmpWidth_, iTmpHeight_;
		if(this->m_stElementBase.bIsDrawSubsection)
		{
			iTmpWidth_  = this->m_stElementBase.iSubWidth * this->m_stElementBase.fScale * fScale;
			iTmpHeight_ = this->m_stElementBase.iSubHeight * this->m_stElementBase.fScale * fScale;
		}
		else
		{
			iTmpWidth_  = this->m_stElementBase.iWidth * this->m_stElementBase.fScale * fScale;
			iTmpHeight_ = this->m_stElementBase.iHeight * this->m_stElementBase.fScale * fScale;
		}

		//Mirror
		if(this->m_stElementBase.bIsMirror)
		{
			ofTranslate(iTmpWidth_, 0);
			ofScale(-1, 1);
		}

		switch(this->m_pImageActor->_eBlendType)
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

		//Fade In or Fade Out
		if(this->m_stElementBase.bFadeIn || this->m_stElementBase.bFadeOut)
		{
			ofEnableAlphaBlending();
			ofSetColor(255,255,255,(int)this->m_stElementBase.fAlpah);
			if(this->m_stElementBase.bIsDrawSubsection)
			{
				this->m_pImageActor->m_oImage.drawSubsection(0, 0, iTmpWidth_, iTmpHeight_, this->m_stElementBase.oStartPos.x, this->m_stElementBase.oStartPos.y, this->m_stElementBase.iSubWidth, this->m_stElementBase.iSubHeight);
			}
			else
			{
				this->m_pImageActor->m_oImage.draw(0, 0, iTmpWidth_, iTmpHeight_);
			}
			ofDisableAlphaBlending();
			//ofSetColor(255,255,255);
			
		}
		else
		{
			if(this->m_stElementBase.bIsDrawSubsection)
			{
				this->m_pImageActor->m_oImage.drawSubsection(0, 0, iTmpWidth_, iTmpHeight_, this->m_stElementBase.oStartPos.x, this->m_stElementBase.oStartPos.y, this->m_stElementBase.iSubWidth, this->m_stElementBase.iSubHeight);
			}
			else
			{
				this->m_pImageActor->m_oImage.draw(0, 0, iTmpWidth_, iTmpHeight_);
			}

		}

		ofDisableAlphaBlending();
		ofPopMatrix();

		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofxDynamicImageElement::updateImg(ofImage& oNewImg)
{
	this->m_pImageActor->m_oImage.clone(oNewImg);
	this->m_stElementBase.iHeight = oNewImg.height;
	this->m_stElementBase.iWidth = oNewImg.width;
}
#pragma endregion

#pragma region CLASS ofxAnimationImageElement
//--------------------------------------------------------------
//CLASS ofxAnimationImageElement
//--------------------------------------------------------------
ofxAnimationImageElement::ofxAnimationImageElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxAnimationImageActor* pActor, float fSPF)
	:ofxBaseElement(strName, ucLevel, oPos, bIsVisible),
	 m_iFrameNo(0),
	 m_bIsLoop(false),
	 m_bCallEvent(false),
	 m_bIsCallEventIsTriggle(false),
	 m_bIsDone(false),
	 m_fSPF(fSPF),
	 m_fTimer(0.0),
	 m_pAnimationActor(pActor)
{};

//--------------------------------------------------------------
void ofxAnimationImageElement::update(double dDiffSecond)
{
	if(this->m_bIsPlay)
	{
		this->m_fTimer += dDiffSecond;
		this->m_iFrameNo = this->GetFrameNum();
		
		this->m_stElementBase.iWidth = this->m_pAnimationActor->m_oImageList[this->m_iFrameNo].getWidth();
		this->m_stElementBase.iHeight = this->m_pAnimationActor->m_oImageList[this->m_iFrameNo].getHeight();

		if(!this->m_bIsCallEventIsTriggle && !this->m_bIsLoop && this->m_bIsDone)
		{
			//Animation done
			ofxTheatreEventArgs oEvent_;
			oEvent_.strMessage = this->m_strElementName;
			ofNotifyEvent(ofxTheatreEventArgs::TheatreEvent, oEvent_);
			this->m_bIsCallEventIsTriggle = true;
		}
	}
}

//--------------------------------------------------------------
void ofxAnimationImageElement::draw(int iWidth, int iHeight, float fScale)
{
	ofPushStyle();
	if(this->m_stElementBase.bIsVisible)
	{
		
		//Translate
		ofPushMatrix();
		ofTranslate(this->m_stElementBase.oPos);
		
		//Rotate
		ofPoint oCenterPoint(this->m_stElementBase.iWidth/2, this->m_stElementBase.iHeight/2);
		ofTranslate(oCenterPoint);
		ofRotateZ(this->m_stElementBase.fDegree);
		ofTranslate(-oCenterPoint);

		//Scale
		int iTmpWidth_, iTmpHeight_;
		if(this->m_stElementBase.bIsDrawSubsection)
		{
			iTmpWidth_  = this->m_stElementBase.iSubWidth * this->m_stElementBase.fScale * fScale;
			iTmpHeight_ = this->m_stElementBase.iSubHeight * this->m_stElementBase.fScale * fScale;
		}
		else
		{
			iTmpWidth_  = this->m_stElementBase.iWidth * this->m_stElementBase.fScale * fScale;
			iTmpHeight_ = this->m_stElementBase.iHeight * this->m_stElementBase.fScale * fScale;
		}

		//Mirror
		if(this->m_stElementBase.bIsMirror)
		{
			ofTranslate(iTmpWidth_, 0);
			ofScale(-1, 1);
		}

		switch(this->m_pAnimationActor->_eBlendType)
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
		
		//Fade In or Fade Out
		if(this->m_stElementBase.bFadeIn || this->m_stElementBase.bFadeOut)
		{
			ofEnableAlphaBlending();
			ofSetColor(255,255,255,(int)this->m_stElementBase.fAlpah);
			this->m_pAnimationActor->m_oImageList[this->m_iFrameNo].draw(0, 0, iTmpWidth_, iTmpHeight_);
			ofDisableAlphaBlending();
			ofSetColor(255,255,255);
			
		}
		else
		{
			//ofSetColor(255,255,255);
			this->m_pAnimationActor->m_oImageList[this->m_iFrameNo].draw(0, 0, iTmpWidth_, iTmpHeight_);
		}
		
		ofDisableAlphaBlending();
		ofPopMatrix();
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofxAnimationImageElement::Play()
{
	this->m_bIsPlay = false;
	this->m_iFrameNo = 0;
	this->m_fTimer = 0.0;
	this->m_bIsCallEventIsTriggle = false;
	this->m_bIsDone = false;
}

//--------------------------------------------------------------
void ofxAnimationImageElement::PlayAnimation()
{
	if(!this->m_bIsPlay)
	{
		this->m_bIsPlay = true;
	}
}

//--------------------------------------------------------------
void ofxAnimationImageElement::SetSPF(float fSPF)
{
	this->m_fSPF = fSPF;
}

//--------------------------------------------------------------
void ofxAnimationImageElement::SetLoop(bool bIsLoop)
{
	this->m_bIsLoop = bIsLoop;
}

//--------------------------------------------------------------
void ofxAnimationImageElement::SetEvent(bool bHaveEvent)
{
	this->m_bCallEvent = bHaveEvent;

	if(bHaveEvent)
	{
		this->SetLoop(false);
	}
}

//--------------------------------------------------------------
int	ofxAnimationImageElement::GetFrameNum()
{	
	int iFrameNo_ 	= 0;
	if(!m_bIsPlay)
	{
		return 0;
	}
	else
	{
		iFrameNo_ = (int)floor(this->m_fTimer / this->m_fSPF) ;
		if(m_bIsLoop)
		{
			return iFrameNo_ % this->m_pAnimationActor->m_oImageList.size();
		}
		else
		{
			if(iFrameNo_ >= this->m_pAnimationActor->m_oImageList.size())
			{
				m_bIsDone = true;
			}
			return MIN(iFrameNo_, this->m_pAnimationActor->m_oImageList.size() - 1);
		}
	}
}
#pragma endregion

#pragma region CLASS ofxVideoElement
//--------------------------------------------------------------
//CLASS ofxVideoElement
//--------------------------------------------------------------
ofxVideoElement::ofxVideoElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxVideoActor* pActor, bool bIsAutoPlay)
	:ofxBaseElement(strName, ucLevel, oPos, bIsVisible)
{
	this->m_pVideoActor = pActor;
	this->m_pBaseActor = this->m_pVideoActor;
	
	this->m_stElementBase.iHeight = this->m_pVideoActor->m_oVideo.height;
	this->m_stElementBase.iWidth  = this->m_pVideoActor->m_oVideo.width;
	this->m_stElementBaseBackup   = this->m_stElementBase;
	
	this->m_bIsLoop		=	true;
	this->m_bIsPlay		=	bIsAutoPlay;
	this->m_bIsPause		=	false;
	this->m_bSubloop = this->m_bSubloopClick	=	false;
	this->m_iStartFrame	=	this->m_iEndFrame = -1;

	this->m_fPausePosition = 0.0;
	this->m_bIsAutoPlay =	bIsAutoPlay;

	this->m_bCallEvent	=	false;
	this->m_bIsCallEventIsTriggle	=	false;
};

//--------------------------------------------------------------
void ofxVideoElement::update(double dDiffSecond)
{
	this->m_pVideoActor->m_oVideo.update();

	if(this->m_bSubloopClick)
	{
		int iFrame_ = this->m_pVideoActor->m_oVideo.getCurrentFrame();
		if(this->m_iEndFrame <= iFrame_)
		{
			this->m_pVideoActor->m_oVideo.setFrame(this->m_iStartFrame);
		}
	}

	if(this->m_bCallEvent && !this->m_bIsLoop && this->m_pVideoActor->m_oVideo.getIsMovieDone() && !this->m_bIsCallEventIsTriggle)
	{
		//Video is done
		ofxTheatreEventArgs oEvent_;
		oEvent_.strMessage = this->m_strElementName;
		ofNotifyEvent(ofxTheatreEventArgs::TheatreEvent, oEvent_);
		this->m_bIsCallEventIsTriggle = true;
	}
}

//--------------------------------------------------------------
void ofxVideoElement::draw(int iWidth, int iHeight, float fScale)
{
	if(this->m_stElementBase.bIsVisible && this->m_bIsPlay && this->m_pVideoActor->m_oVideo.isPlaying())
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
		int iTmpWidth_, iTmpHeight_;
		if(this->m_stElementBase.bIsDrawSubsection)
		{
			iTmpWidth_  = this->m_stElementBase.iSubWidth * this->m_stElementBase.fScale * fScale;
			iTmpHeight_ = this->m_stElementBase.iSubHeight * this->m_stElementBase.fScale * fScale;
		}
		else
		{
			iTmpWidth_  = this->m_stElementBase.iWidth * this->m_stElementBase.fScale * fScale;
			iTmpHeight_ = this->m_stElementBase.iHeight * this->m_stElementBase.fScale * fScale;
		}

		//Mirror
		if(this->m_stElementBase.bIsMirror)
		{
			ofTranslate(iTmpWidth_, 0);
			ofScale(-1, 1);
		}

		switch(this->m_pVideoActor->_eBlendType)
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

		//Fade In or Fade Out
		if(this->m_stElementBase.bFadeIn || this->m_stElementBase.bFadeOut)
		{
			ofEnableAlphaBlending();
			ofSetColor(255,255,255,(int)this->m_stElementBase.fAlpah);
			this->m_pVideoActor->m_oVideo.draw(0, 0, iTmpWidth_, iTmpHeight_);
			ofDisableAlphaBlending();
			ofSetColor(255,255,255);
		}
		else
		{
			//ofSetColor(255);
			this->m_pVideoActor->m_oVideo.draw(0, 0, iTmpWidth_, iTmpHeight_);
		}
		ofDisableAlphaBlending();

		ofPopStyle();
		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void ofxVideoElement::Play()
{
	//Set loop
	if(this->m_bIsLoop)
	{
		this->m_pVideoActor->m_oVideo.setLoopState(OF_LOOP_NORMAL);
	}
	else
	{
		this->m_pVideoActor->m_oVideo.setLoopState(OF_LOOP_NONE);
	}
	
	if(this->m_bIsPlay)
	{
		this->PlayVideo();
	}

	this->PauseVideo(this->m_bIsPause);
}

//--------------------------------------------------------------
void ofxVideoElement::Pause()
{
	this->m_fPausePosition = this->m_pVideoActor->m_oVideo.getPosition();
}

//--------------------------------------------------------------
void ofxVideoElement::Stop()
{
	ofxBaseElement::Stop();
	this->m_fPausePosition	=	0;
	this->m_bIsPlay			=	this->m_bIsAutoPlay;
	this->m_bIsPause			=	false;
	this->m_bIsCallEventIsTriggle = false;

	this->m_pVideoActor->m_oVideo.setFrame(0);
	this->m_pVideoActor->m_oVideo.update();
	this->m_pVideoActor->m_oVideo.stop();
}

//--------------------------------------------------------------
void ofxVideoElement::ChangeVideoActor(ofxVideoActor* pNewVideoActor)
{
	m_pVideoActor->m_oVideo.stop();
	m_pVideoActor = pNewVideoActor;

	this->m_stElementBase.iHeight = this->m_pVideoActor->m_oVideo.height;
	this->m_stElementBase.iWidth  = this->m_pVideoActor->m_oVideo.width;
	this->m_stElementBaseBackup   = this->m_stElementBase;

	if(this->m_bIsLoop)
	{
		this->m_pVideoActor->m_oVideo.setLoopState(OF_LOOP_NORMAL);
	}
	else
	{
		this->m_pVideoActor->m_oVideo.setLoopState(OF_LOOP_NONE);
	}
}

//--------------------------------------------------------------
void ofxVideoElement::SetVideoLoop(bool bIsLoop)
{
	this->m_bIsLoop = bIsLoop;
	//this->m_pVideoActor->m_oVideo.setLoopState(OF_LOOP_NONE);
}

//--------------------------------------------------------------
void ofxVideoElement::SetVideoAutoPlay(bool bIsAutoPlay)
{
	this->m_bIsAutoPlay = bIsAutoPlay;
	this->m_bIsPlay = bIsAutoPlay;
}

//--------------------------------------------------------------
void ofxVideoElement::SetVideoSubloop(int iStartFrame, int iEndFrame)
{
	int iTotalFrame_ = this->m_pVideoActor->m_oVideo.getTotalNumFrames();
	if(iStartFrame > iTotalFrame_ || iEndFrame > iTotalFrame_)
	{
		return;
	}

	if(iStartFrame > iEndFrame)
	{
		swap(iStartFrame, iEndFrame);
	}

	this->m_bSubloop = true;
	this->m_iStartFrame = iStartFrame;
	this->m_iEndFrame = iEndFrame;
}

//--------------------------------------------------------------
void ofxVideoElement::SetVideoSubloop(int iFrame, bool bIsFromStart)
{
	int iTotalFrame_ = this->m_pVideoActor->m_oVideo.getTotalNumFrames();
	if(iFrame >= iTotalFrame_)
	{
		return;
	}
	
	if(bIsFromStart)
	{
		this->m_iStartFrame = 0;
		this->m_iEndFrame = iFrame;
	}
	else
	{
		this->m_iStartFrame = iFrame;
		this->m_iEndFrame = iTotalFrame_ - 2;
	}

	this->m_bSubloop = true;
}

//--------------------------------------------------------------
void ofxVideoElement::ExitSubloop()
{
	this->m_bSubloopClick = false;
}

//--------------------------------------------------------------
//Video Control
//--------------------------------------------------------------
void ofxVideoElement::PlayVideo()
{
	this->m_bIsPlay = true;

	this->m_pVideoActor->m_oVideo.play();
	this->m_pVideoActor->m_oVideo.setFrame(0);

	this->m_bSubloopClick = this->m_bSubloop;
}

//--------------------------------------------------------------
void ofxVideoElement::PauseVideo(bool bValue)
{
	if(this->m_bIsPlay)
	{
		this->m_bIsPause		=	bValue;
		this->m_pVideoActor->m_oVideo.setPaused(bValue);
	}
}

//--------------------------------------------------------------
void ofxVideoElement::StopVideo()
{
	this->m_bIsPlay = false;
	this->m_pVideoActor->m_oVideo.stop();
}

//--------------------------------------------------------------
int ofxVideoElement::GetFrame()
{
	return this->m_pVideoActor->m_oVideo.getCurrentFrame();
}

//--------------------------------------------------------------
void ofxVideoElement::SetFrame(int iFrame)
{
	this->m_pVideoActor->m_oVideo.setFrame(iFrame);
}

//--------------------------------------------------------------
void ofxVideoElement::StartEvent()
{
	this->m_bIsLoop = false;
	this->m_bCallEvent = true;
}
#pragma endregion

#pragma region CLASS ofxWebcam Element
//--------------------------------------------------------------
//CLASS ofxWebcamElement
//--------------------------------------------------------------
ofxWebcamElement::ofxWebcamElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxWebcamActor* pActor)
	:ofxBaseElement(strName, ucLevel, oPos, bIsVisible)
{
	this->m_pWebcamActor = pActor;
	this->m_pBaseActor = this->m_pWebcamActor;
	
	this->m_stElementBase.iHeight = this->m_pWebcamActor->m_oVidGrabber.height;
	this->m_stElementBase.iWidth  = this->m_pWebcamActor->m_oVidGrabber.width;
	this->m_stElementBaseBackup   = this->m_stElementBase;
	
};

//--------------------------------------------------------------
void ofxWebcamElement::update(double dDiffSecond)
{
	if(this->m_pWebcamActor->m_bIsInitialSuccess)
	{
		this->m_pWebcamActor->m_oVidGrabber.update();
		this->m_oImage.setFromPixels(this->m_pWebcamActor->m_oVidGrabber.getPixels(), this->m_stElementBase.iWidth, this->m_stElementBase.iHeight, OF_IMAGE_COLOR);
	}
}

//--------------------------------------------------------------
void ofxWebcamElement::draw(int iWidth, int iHeight, float fScale)
{
	if(this->m_stElementBase.bIsVisible && this->m_pWebcamActor->m_bIsInitialSuccess)
	{
		//Translate
		ofPushMatrix();
		ofTranslate(this->m_stElementBase.oPos);
		
		//Rotate
		ofPoint oCenterPoint(this->m_stElementBase.iWidth/2, this->m_stElementBase.iHeight/2);
		ofTranslate(oCenterPoint);
		ofRotateZ(this->m_stElementBase.fDegree);
		ofTranslate(-oCenterPoint);

		//Scale
		int iTmpWidth_, iTmpHeight_;
		if(this->m_stElementBase.bIsDrawSubsection)
		{
			iTmpWidth_  = this->m_stElementBase.iSubWidth * this->m_stElementBase.fScale * fScale;
			iTmpHeight_ = this->m_stElementBase.iSubHeight * this->m_stElementBase.fScale * fScale;
		}
		else
		{
			iTmpWidth_  = this->m_stElementBase.iWidth * this->m_stElementBase.fScale * fScale;
			iTmpHeight_ = this->m_stElementBase.iHeight * this->m_stElementBase.fScale * fScale;
		}

		//Mirror
		if(this->m_stElementBase.bIsMirror)
		{
			ofTranslate(iTmpWidth_, 0);
			ofScale(-1, 1);
		}

		//Fade In or Fade Out
		if(this->m_stElementBase.bFadeIn || this->m_stElementBase.bFadeOut)
		{
			ofEnableAlphaBlending();
			ofSetColor(255,255,255,(int)this->m_stElementBase.fAlpah);
			
			if(this->m_stElementBase.bIsDrawSubsection)
			{
				this->m_oImage.drawSubsection(0, 0, iTmpWidth_, iTmpHeight_, this->m_stElementBase.oStartPos.x, this->m_stElementBase.oStartPos.y, this->m_stElementBase.iSubWidth, this->m_stElementBase.iSubHeight);
			}
			else
			{
				//this->m_oImage.draw(0, 0, iTmpWidth_, iTmpHeight_);
				this->m_pWebcamActor->m_oVidGrabber.draw(0, 0, iTmpWidth_, iTmpHeight_);
			}
			
			ofDisableAlphaBlending();
			ofSetColor(255,255,255);
			
		}
		else
		{
			if(this->m_stElementBase.bIsDrawSubsection)
			{
				this->m_oImage.drawSubsection(0, 0, iTmpWidth_, iTmpHeight_, this->m_stElementBase.oStartPos.x, this->m_stElementBase.oStartPos.y, this->m_stElementBase.iSubWidth, this->m_stElementBase.iSubHeight);
			}
			else
			{
				//this->m_oImage.draw(0, 0, iTmpWidth_, iTmpHeight_);
				this->m_pWebcamActor->m_oVidGrabber.draw(0, 0, iTmpWidth_, iTmpHeight_);
			}
		}

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void ofxWebcamElement::getImage(ofImage& oRefImage)
{
	oRefImage.clone(this->m_oImage);
}
#pragma endregion

#pragma region CLASS ofxSoundElement
//--------------------------------------------------------------
//CLASS ofxSoundElement
//--------------------------------------------------------------
ofxSoundElement::ofxSoundElement(std::string strName, unsigned char ucLevel, ofPoint oPos, bool bIsVisible, ofxSoundActor* pActor)
	:ofxBaseElement(strName, ucLevel, oPos, bIsVisible)
{
	this->m_pSoundActor = pActor;
	this->m_pBaseActor  = this->m_pSoundActor;
	this->m_fVolume		=	0.75f;

	this->m_bIsFullPlay	=	true;
	this->m_bIsLoop		=	true;
	this->m_bIsPlay		=	false;
	this->m_bIsPause		=	false;
	this->m_fBeginPosition =	 this->m_fEndPosition = 0.0;
	this->m_fPausePosition =	 0.0;
	this->m_pSoundActor->m_oSoundPlayer.setVolume(this->m_fVolume);

};

//--------------------------------------------------------------
void ofxSoundElement::update(double dDiffSecond)
{
	if(this->m_pSoundActor->m_oSoundPlayer.isLoaded())
	{
		ofSoundUpdate();
	}
	
}

//--------------------------------------------------------------
void ofxSoundElement::draw(int iWidth, int iHeight)
{
}

//--------------------------------------------------------------
void ofxSoundElement::Play()
{
	//Set loop
	this->m_pSoundActor->m_oSoundPlayer.setLoop(this->m_bIsLoop);
	
	if(this->m_bIsPlay)
	{
		this->m_pSoundActor->m_oSoundPlayer.setVolume(this->m_fVolume);
		this->m_pSoundActor->m_oSoundPlayer.play();
	}

	//Set Position
	this->m_pSoundActor->m_oSoundPlayer.setPosition(this->m_fPausePosition);

	this->PauseSound(this->m_bIsPause);
}

//--------------------------------------------------------------
void ofxSoundElement::Pause()
{
	this->m_fPausePosition = this->m_pSoundActor->m_oSoundPlayer.getPosition();
	this->m_pSoundActor->m_oSoundPlayer.stop();
}

//--------------------------------------------------------------
void ofxSoundElement::Stop()
{
	ofxBaseElement::Stop();
	this->m_fPausePosition	=	0;
	this->m_bIsPlay			=	false;
	this->m_bIsPause			=	false;
	this->m_pSoundActor->m_oSoundPlayer.stop();

}

//--------------------------------------------------------------
//Sound Control
//--------------------------------------------------------------
void ofxSoundElement::PlaySound(float fVolume)
{	
	this->m_pSoundActor->m_oSoundPlayer.setVolume(fVolume);
	
	this->m_fVolume = fVolume;
	this->m_bIsPlay = true;
	this->m_pSoundActor->m_oSoundPlayer.play();
}

//--------------------------------------------------------------
void ofxSoundElement::PauseSound(bool bValue)
{
	this->m_bIsPause = bValue;
	this->m_pSoundActor->m_oSoundPlayer.setPaused(bValue);
}
#pragma endregion