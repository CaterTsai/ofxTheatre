#include "ofxTheatreAnimator.h"
#include "ofxTheatreElement.h"

//--------------------------------------------------------------
//Animator Class
//--------------------------------------------------------------

ofxAnimator::~ofxAnimator()
{
	this->ClearAnimation();
}

//--------------------------------------------------------------
void ofxAnimator::AnimatorUpdate(double dDiffSecond)
{
	for(int idx_ = 0; idx_ < ANIMATION_MAX_PRIORITY; ++idx_)
	{
		if(this->m_aoAnimationMgr[idx_].size() > 0)
		{
			ANIMATION_LIST_ITER oListIter_ = this->m_aoAnimationMgr[idx_].begin();

			eAnimationState eAnimationState_ = (*oListIter_)->GetAnimationState();

			switch(eAnimationState_)
			{
			case eANIMATION_WAIT_INITIAL:
				{
					(*oListIter_)->WaitInitial(dDiffSecond);
					break;
				}
			case eANIMATION_INITIAL:
				{
					(*oListIter_)->Initial();
					break;
				}
			case eANIMATION_RUN:
				{
					(*oListIter_)->Run(dDiffSecond);
					break;
				}
			case eANIMATION_WAIT_FINISH:
				{
					(*oListIter_)->WaitFinish(dDiffSecond);
					break;
				}
			case eANIMATION_FINISH:
				{
					(*oListIter_)->Finish();
					this->DelAnimationInMap((*oListIter_)->m_strAnimationName);

					ofxTheatreEventArgs oEvent_;
					oEvent_.strMessage = (*oListIter_)->m_strAnimationName;
					delete (*oListIter_);
					this->m_aoAnimationMgr[idx_].erase(oListIter_);

					this->m_uiAnimationCounter--;
					ofNotifyEvent(ofxTheatreEventArgs::TheatreEvent, oEvent_);
					break;
				}
			default:
				{
					break;
				}
			}


		}
	}
}

//--------------------------------------------------------------
bool ofxAnimator::AddAnimation(unsigned char ucAnimationPriority, ofxBaseAnimation* pAnimation)
{
	if(pAnimation->m_eType >= eANIMATE_MAXIMUM)
	{
		ofLogWarning("Unknow Animation type:" + pAnimation->m_eType);
		delete pAnimation;
		return false;
	}

	if(ucAnimationPriority >= ANIMATION_MAX_PRIORITY)
	{
		ofLogWarning("Over the priority range");
		delete pAnimation;
		return false;
	}

	ANIMATION_MAP_ITER oMapIter_ = this->m_oAnimationMap.find(pAnimation->m_strAnimationName);
	if(oMapIter_ != this->m_oAnimationMap.end())
	{
		ofLogWarning("This Animation Name is already been used : " + pAnimation->m_strAnimationName);
		delete pAnimation;
		return false;
	}


	this->m_uiAnimationCounter++;
	this->m_aoAnimationMgr[ucAnimationPriority].push_back(pAnimation);
	this->m_oAnimationMap.insert(::make_pair(pAnimation->m_strAnimationName, ucAnimationPriority));


	return true;
}

//--------------------------------------------------------------
bool ofxAnimator::DelAnimationInMap(std::string strAnimationName)
{
	ANIMATION_MAP_ITER oMapIter_ = this->m_oAnimationMap.find(strAnimationName);
	if(oMapIter_ == this->m_oAnimationMap.end())
	{
		ofLogWarning("Can't found this animation in map : " + strAnimationName);
		return false;
	}

	this->m_oAnimationMap.erase(strAnimationName);
	return true;
}

//--------------------------------------------------------------
void ofxAnimator::ClearAnimation()
{
	for(int idx_ = 0; idx_ < ANIMATION_MAX_PRIORITY; ++idx_)
	{
		if(this->m_aoAnimationMgr[idx_].size() > 0)
		{
			ANIMATION_LIST_ITER oListIter_ = this->m_aoAnimationMgr[idx_].begin();
			for(;oListIter_ != this->m_aoAnimationMgr[idx_].end(); ++oListIter_)
			{
				delete (*oListIter_);
			}
			this->m_aoAnimationMgr[idx_].clear();
		}
	}

	this->m_oAnimationMap.clear();
}

//--------------------------------------------------------------
void ofxAnimator::VideoAnimatorUpdate(double dDiffSecond, int iFrameIndex)
{
	ANIMATION_LIST_ITER oListIter_ = this->m_oVideoAnimationList.begin();
		
	for(;oListIter_ != this->m_oVideoAnimationList.end(); ++oListIter_)
	{
		eVideoAnimationState eVideoAnimationState_ = (*oListIter_)->GetVideoAnimationState();

		switch(eVideoAnimationState_)
		{
		case eVIDEO_ANIMATION_WAIT_FRAME:
			{
				(*oListIter_)->WaitFrame(iFrameIndex);
				break;
			}
		case eVIDEO_ANIMATION_INITIAL:
			{
				(*oListIter_)->Initial();
				break;
			}
		case eVIDEO_ANIMATION_RUN:
			{
				(*oListIter_)->Run(dDiffSecond);
				break;
			}
		case eVIDEO_ANIMATION_FINISH:
			{
				(*oListIter_)->Finish();
				this->DelVideoAnimationInMap((*oListIter_)->m_strAnimationName);

				ofxTheatreEventArgs oEvent_;
				oEvent_.strMessage = (*oListIter_)->m_strAnimationName;
				ofNotifyEvent(ofxTheatreEventArgs::TheatreEvent, oEvent_);

				delete (*oListIter_);
				(*oListIter_) = NULL;
				break;
			}
		}
	}

	this->m_oVideoAnimationList.erase(std::remove_if(this->m_oVideoAnimationList.begin(), this->m_oVideoAnimationList.end(), ofxAnimator::IsFinish), this->m_oVideoAnimationList.end() );
}

//--------------------------------------------------------------
bool ofxAnimator::AddVideoAnimation(ofxBaseAnimation* pAnimation)
{
	if(pAnimation->m_eType >= eANIMATE_MAXIMUM)
	{
		ofLogWarning("Unknow Animation type:" + pAnimation->m_eType);
		delete pAnimation;
		return false;
	}

	ANIMATION_MAP_ITER oMapIter_ = this->m_oAnimationMap.find(pAnimation->m_strAnimationName);
	if(oMapIter_ != this->m_oAnimationMap.end())
	{
		ofLogWarning("This Animation Name is already been used : " + pAnimation->m_strAnimationName);
		delete pAnimation;
		return false;
	}

	this->m_oVideoAnimationList.push_back(pAnimation);
	this->m_oVideoAnimationMap.insert(::make_pair(pAnimation->m_strAnimationName, 0));


	return true;
}

//--------------------------------------------------------------
bool ofxAnimator::DelVideoAnimationInMap(std::string strAnimationName)
{
	ANIMATION_MAP_ITER oMapIter_ = this->m_oVideoAnimationMap.find(strAnimationName);
	if(oMapIter_ == this->m_oVideoAnimationMap.end())
	{
		ofLogWarning("Can't found this animation in video map : " + strAnimationName);
		return false;
	}

	this->m_oVideoAnimationMap.erase(strAnimationName);
	return true;
}

//--------------------------------------------------------------
void ofxAnimator::ClearVideoAnimation()
{

	ANIMATION_LIST_ITER oListIter_ = this->m_oVideoAnimationList.begin();
	for(;oListIter_ != this->m_oVideoAnimationList.end(); ++oListIter_)
	{
		delete (*oListIter_);
	}
	this->m_oVideoAnimationList.clear();

	this->m_oVideoAnimationMap.clear();
}

//--------------------------------------------------------------
//Base Animation Class
//--------------------------------------------------------------
eAnimationState ofxBaseAnimation::GetAnimationState()
{
	return this->m_eAnimationState;
}

//--------------------------------------------------------------
eVideoAnimationState ofxBaseAnimation::GetVideoAnimationState()
{
	return this->m_eVideoAnimationState;
}

//--------------------------------------------------------------
void ofxBaseAnimation::WaitInitial(double dDiffSecond)
{
	if(this->m_fWaitStart <= 0)
	{
		this->m_eAnimationState = eANIMATION_INITIAL;
	}
	else
	{
		this->m_fWaitStart -= dDiffSecond;
	}
}

//--------------------------------------------------------------
void ofxBaseAnimation::WaitFinish(double dDiffSecond)
{
	if(this->m_fDelay <= 0)
	{
		this->m_eAnimationState = eANIMATION_FINISH;
	}
	else
	{
		this->m_fDelay -= dDiffSecond;
	}
}

//--------------------------------------------------------------
void ofxBaseAnimation::WaitFrame(int iFrame)
{
	if(this->m_iStartFrame <= iFrame)
	{
		this->m_eVideoAnimationState = eVIDEO_ANIMATION_INITIAL;
	}
}

//--------------------------------------------------------------
//Move Animation Class
//--------------------------------------------------------------
void ofxMoveAnimation::Initial()
{
	this->m_pElementPtr->m_stElementBase.bIsVisible = true;
	
	if(!this->m_pElementPtr->m_stElementBase.bIsMove)
	{
		this->m_pElementPtr->m_stElementBase.bIsMove = true;
		this->m_oVelocity                            = (this->m_oEndPoint - this->m_pElementPtr->m_stElementBase.oPos) / this->m_fAnimationTime;
		this->m_bCanBreak                            = false;

		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState	= eANIMATION_RUN;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_RUN;
		}
	}
}

//--------------------------------------------------------------
void ofxMoveAnimation::Run(double dDiffSecond)
{
	if(this->m_pElementPtr->m_stElementBase.bIsMove)
	{
		ofPoint oDiffPos_ = (this->m_oVelocity * dDiffSecond);
		ofPoint oNewPos_ = this->m_pElementPtr->m_stElementBase.oPos + oDiffPos_;
		if( abs(oNewPos_.x - this->m_oEndPoint.x) < abs(oDiffPos_.x) ||
			abs(oNewPos_.y - this->m_oEndPoint.y) < abs(oDiffPos_.y))
		{
			this->m_pElementPtr->m_stElementBase.oPos	= this->m_oEndPoint;

			if(!this->m_bIsVideoAnimation)
			{
				this->m_eAnimationState = eANIMATION_WAIT_FINISH;
			}
			else
			{
				this->m_eVideoAnimationState = eVIDEO_ANIMATION_FINISH;
			}
		}
		else
		{
			this->m_pElementPtr->m_stElementBase.oPos = oNewPos_;
		}
	}
}

//--------------------------------------------------------------
void ofxMoveAnimation::Finish()
{
	this->m_pElementPtr->m_stElementBase.bIsMove = false;
}

//--------------------------------------------------------------
//Rotate Animation Class
//--------------------------------------------------------------
void ofxRotateAnimation::Initial()
{
	this->m_pElementPtr->m_stElementBase.bIsVisible = true;

	if(!this->m_pElementPtr->m_stElementBase.bIsRotate)
	{
		this->m_pElementPtr->m_stElementBase.bIsRotate = true;
		this->m_fRotateVelocity                        = (this->m_fEndRotateDegree - this->m_pElementPtr->m_stElementBase.fDegree) / this->m_fAnimationTime;
		this->m_bCanBreak                              = false;
		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_RUN;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_RUN;
		}
	}
}

//--------------------------------------------------------------
void ofxRotateAnimation::Run(double dDiffSecond)
{
	float fDiffDegree_ = this->m_fRotateVelocity * dDiffSecond;
	float fTmpDegree_  = this->m_pElementPtr->m_stElementBase.fDegree + fDiffDegree_;
	if(abs(fTmpDegree_ - this->m_fEndRotateDegree) <= abs(fDiffDegree_))
	{
		this->m_pElementPtr->m_stElementBase.fDegree	 = this->m_fEndRotateDegree;

		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_WAIT_FINISH;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_FINISH;
		}
	}
	else
	{
		this->m_pElementPtr->m_stElementBase.fDegree	= fTmpDegree_;
	}

}

//--------------------------------------------------------------
void ofxRotateAnimation::Finish()
{
	this->m_pElementPtr->m_stElementBase.bIsRotate = false;
}

//--------------------------------------------------------------
//Scale Animation Class
//--------------------------------------------------------------
void ofxScaleAnimation::Initial()
{
	this->m_pElementPtr->m_stElementBase.bIsVisible = true;

	if(!this->m_pElementPtr->m_stElementBase.bIsScale)
	{
		this->m_pElementPtr->m_stElementBase.bIsScale = true;
		this->m_fScaleVelocity	= (this->m_fEndScale - this->m_pElementPtr->m_stElementBase.fScale) / this->m_fAnimationTime;
		this->m_bCanBreak       = false;
		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_RUN;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_RUN;
		}
	}
}

void ofxScaleAnimation::Run(double dDiffSecond)
{

	float fDiffScale_ = this->m_fScaleVelocity * dDiffSecond;
	float fTmpScale_  = this->m_pElementPtr->m_stElementBase.fScale + fDiffScale_;
	if(abs(fTmpScale_ - this->m_fEndScale) <= abs(fDiffScale_))
	{
		this->m_pElementPtr->m_stElementBase.fScale	= this->m_fEndScale;
		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_WAIT_FINISH;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_FINISH;
		}
	}
	else
	{
		this->m_pElementPtr->m_stElementBase.fScale = fTmpScale_;
	}

}

//--------------------------------------------------------------
void ofxScaleAnimation::Finish()
{
	this->m_pElementPtr->m_stElementBase.bIsScale = false;
}

//--------------------------------------------------------------
//Enter Animation Class
//--------------------------------------------------------------
void ofxEnterAnimation::Initial()
{
	if(!this->m_pElementPtr->m_stElementBase.bIsMove)
	{	
		ofPoint oPlanePos_ = this->m_pElementPtr->m_pPlanePtr->m_stElementBase.oPos;
		if(!this->m_pElementPtr->m_stElementBase.bIsVisible)
		{
			this->m_pElementPtr->m_stElementBase.bIsVisible = true;
		}

		this->m_oSourcePoint = this->m_pElementPtr->m_stElementBase.oPos;
		switch(this->m_eEnterType)
		{
		case eENTER_FROM_UP:
			{
				this->m_pElementPtr->m_stElementBase.oPos.y = 0 - this->m_pElementPtr->m_stElementBase.iHeight - oPlanePos_.y;
				break;
			}
		case eENTER_FROM_RIGHT:
			{
				this->m_pElementPtr->m_stElementBase.oPos.x = this->m_iWindowsWidth + oPlanePos_.x;
				break;
			}
		case eENTER_FROM_DOWN:
			{
				this->m_pElementPtr->m_stElementBase.oPos.y = this->m_iWindowsHeight + oPlanePos_.y;
				break;
			}
		case eENTER_FROM_LEFT:
			{
				this->m_pElementPtr->m_stElementBase.oPos.x = 0 - this->m_pElementPtr->m_stElementBase.iWidth - oPlanePos_.x;
				break;
			}
		default:
			{
				ofLogWarning("Unknow enter type :" + ofToString(this->m_eEnterType));
				return;
			}
		}

		this->m_pElementPtr->m_stElementBase.bIsMove = true;
		this->m_oVelocity = (this->m_oSourcePoint - this->m_pElementPtr->m_stElementBase.oPos) / this->m_fAnimationTime;
		this->m_bCanBreak = false;

		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_RUN;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_RUN;
		}
	}
}

//--------------------------------------------------------------
void ofxEnterAnimation::Run(double dDiffSecond)
{
	ofPoint oDiffPos_ = (this->m_oVelocity * dDiffSecond);
	ofPoint oNewPos_ = this->m_pElementPtr->m_stElementBase.oPos + oDiffPos_;
	if( abs(oNewPos_.x - this->m_oSourcePoint.x) < abs(oDiffPos_.x) ||
		abs(oNewPos_.y - this->m_oSourcePoint.y) < abs(oDiffPos_.y))
	{
		this->m_pElementPtr->m_stElementBase.oPos	= this->m_oSourcePoint;
		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_WAIT_FINISH;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_FINISH;
		}
	}
	else
	{
		this->m_pElementPtr->m_stElementBase.oPos = oNewPos_;
	}
}

//--------------------------------------------------------------
void ofxEnterAnimation::Finish()
{
	this->m_pElementPtr->m_stElementBase.bIsMove = false;
}

//--------------------------------------------------------------
//Exit Animation Class
//--------------------------------------------------------------
void ofxExitAnimation::Initial()
{	
	if(!this->m_pElementPtr->m_stElementBase.bIsMove)
	{	
		ofPoint oPlanePos_ = this->m_pElementPtr->m_pPlanePtr->m_stElementBase.oPos;
		this->m_oTargetPoint = this->m_pElementPtr->m_stElementBase.oPos;
		switch(this->m_eExitType)
		{
		case eEXIT_TO_UP:
			{

				this->m_oTargetPoint.y = 0 - this->m_pElementPtr->m_stElementBase.iHeight;
				break;
			}
		case eEXIT_TO_RIGHT:
			{
				this->m_oTargetPoint.x = this->m_iWindowsWidth;

				break;
			}
		case eEXIT_TO_DOWN:
			{
				this->m_oTargetPoint.y = this->m_iWindowsHeight;
				break;
			}
		case eEXIT_TO_LEFT:
			{
				this->m_oTargetPoint.x = 0 - this->m_pElementPtr->m_stElementBase.iWidth;
				break;
			}
		default:
			{
				ofLogWarning("Unknow enter type :" + ofToString(this->m_eExitType));
				return;
			}
		}


		this->m_oVelocity = (this->m_oTargetPoint - this->m_pElementPtr->m_stElementBase.oPos) / this->m_fAnimationTime;
		this->m_bCanBreak = false;

		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_RUN;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_RUN;
		}
	}
}

//--------------------------------------------------------------
void ofxExitAnimation::Run(double dDiffSecond)
{
	ofPoint oDiffPos_ = (this->m_oVelocity * dDiffSecond);
	ofPoint oNewPos_ = this->m_pElementPtr->m_stElementBase.oPos + oDiffPos_;
	if( abs(oNewPos_.x - this->m_oTargetPoint.x) < abs(oDiffPos_.x) ||
		abs(oNewPos_.y - this->m_oTargetPoint.y) < abs(oDiffPos_.y))
	{
		this->m_pElementPtr->m_stElementBase.oPos	= this->m_oTargetPoint;
		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_WAIT_FINISH;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_FINISH;
		}
	}
	else
	{
		this->m_pElementPtr->m_stElementBase.oPos = oNewPos_;
	}

}

//--------------------------------------------------------------
void ofxExitAnimation::Finish()
{
	this->m_pElementPtr->m_stElementBase.bIsMove = false;
	this->m_pElementPtr->m_stElementBase.bIsVisible = false;
	this->m_pElementPtr->m_stElementBase.oPos = this->m_pElementPtr->m_stElementBaseBackup.oPos;

}

//--------------------------------------------------------------
//Fade In Animation Class
//--------------------------------------------------------------
void ofxFadeInAnimation::Initial()
{
	if(!this->m_pElementPtr->m_stElementBase.bFadeIn)
	{
		if(this->m_fAnimationTime == 0)
		{
			this->m_pElementPtr->m_stElementBase.bIsVisible = true;
			this->m_pElementPtr->m_stElementBase.fAlpah		= MAX_ALPHA;
			this->m_eAnimationState                         = eANIMATION_WAIT_FINISH;
			
			if(!this->m_bIsVideoAnimation)
			{
				this->m_eAnimationState = eANIMATION_WAIT_FINISH;
			}
			else
			{
				this->m_eVideoAnimationState = eVIDEO_ANIMATION_FINISH;
			}

		}
		else
		{
			this->m_pElementPtr->m_stElementBase.bIsVisible = true;
			this->m_pElementPtr->m_stElementBase.bFadeIn    = true;
			this->m_pElementPtr->m_stElementBase.fAlpah		= 0.0;
			this->m_fDiffAlpha								= MAX_ALPHA / this->m_fAnimationTime;
			if(!this->m_bIsVideoAnimation)
			{
				this->m_eAnimationState = eANIMATION_RUN;
			}
			else
			{
				this->m_eVideoAnimationState = eVIDEO_ANIMATION_RUN;
			}
		}
	}
}

//--------------------------------------------------------------
void ofxFadeInAnimation::Run(double dDiffSecond)
{
	if(this->m_pElementPtr->m_stElementBase.fAlpah >= MAX_ALPHA)
	{
		this->m_pElementPtr->m_stElementBase.fAlpah	=	MAX_ALPHA;
		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_WAIT_FINISH;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_FINISH;
		}
	}
	else
	{
		this->m_pElementPtr->m_stElementBase.fAlpah += (this->m_fDiffAlpha * dDiffSecond);
	}
}

//--------------------------------------------------------------
void ofxFadeInAnimation::Finish()
{
	this->m_pElementPtr->m_stElementBase.bFadeIn		= false;
}

//--------------------------------------------------------------
//Fade Out Animation Class
//--------------------------------------------------------------
void ofxFadeOutAnimation::Initial()
{
	if(!this->m_pElementPtr->m_stElementBase.bFadeOut)
	{
		if(this->m_fAnimationTime == 0)
		{
			this->m_pElementPtr->m_stElementBase.fAlpah		= 0;
			this->m_pElementPtr->m_stElementBase.bIsVisible = false;

			if(!this->m_bIsVideoAnimation)
			{
				this->m_eAnimationState = eANIMATION_WAIT_FINISH;
			}
			else
			{
				this->m_eVideoAnimationState = eVIDEO_ANIMATION_FINISH;
			}
		}
		else
		{
			this->m_pElementPtr->m_stElementBase.bFadeOut   = true;
			this->m_pElementPtr->m_stElementBase.fAlpah		= 255.0;
			this->m_fDiffAlpha								= MAX_ALPHA / this->m_fAnimationTime;

			if(!this->m_bIsVideoAnimation)
			{
				this->m_eAnimationState = eANIMATION_RUN;
			}
			else
			{
				this->m_eVideoAnimationState = eVIDEO_ANIMATION_RUN;
			}
		}
	}

	if(!this->m_pElementPtr->m_stElementBase.bIsVisible)
	{
		this->m_eAnimationState	= eANIMATION_FINISH;
	}
}

//--------------------------------------------------------------
void ofxFadeOutAnimation::Run(double dDiffSecond)
{
	if(this->m_pElementPtr->m_stElementBase.fAlpah <= 0)
	{
		this->m_pElementPtr->m_stElementBase.fAlpah	= 0;
		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_WAIT_FINISH;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_FINISH;
		}
	}
	else
	{
		this->m_pElementPtr->m_stElementBase.fAlpah -= (this->m_fDiffAlpha * dDiffSecond);
		if(this->m_pElementPtr->m_stElementBase.fAlpah < 0)
		{
			this->m_pElementPtr->m_stElementBase.fAlpah = 0;
		}
	}
}

//--------------------------------------------------------------
void ofxFadeOutAnimation::Finish()
{
	this->m_pElementPtr->m_stElementBase.bFadeOut = false;
	this->m_pElementPtr->m_stElementBase.bIsVisible = false;
}

//--------------------------------------------------------------
//Fade Out Animation Class
//--------------------------------------------------------------
void ofxShakeAnimation::Initial()
{
	if(!this->m_pElementPtr->m_stElementBase.bIsShake)
	{
		if(this->m_fAnimationTime == 0)
		{
			if(!this->m_bIsVideoAnimation)
			{
				this->m_eAnimationState = eANIMATION_WAIT_FINISH;
			}
			else
			{
				this->m_eVideoAnimationState = eVIDEO_ANIMATION_FINISH;
			}
		}
		else
		{
			this->m_pElementPtr->m_stElementBase.bIsShake   = true;
			_oBackupPos = this->m_pElementPtr->m_stElementBase.oPos;
			if(!this->m_bIsVideoAnimation)
			{
				this->m_eAnimationState = eANIMATION_RUN;
			}
			else
			{
				this->m_eVideoAnimationState = eVIDEO_ANIMATION_RUN;
			}
		}
	}

	if(!this->m_pElementPtr->m_stElementBase.bIsVisible)
	{
		this->m_eAnimationState	= eANIMATION_FINISH;
	}
}

//--------------------------------------------------------------
void ofxShakeAnimation::Run(double dDiffSecond)
{
	this->m_fAnimationTime -= dDiffSecond;
	if(this->m_fAnimationTime <= 0)
	{
		this->m_pElementPtr->m_stElementBase.oPos = _oBackupPos;

		if(!this->m_bIsVideoAnimation)
		{
			this->m_eAnimationState = eANIMATION_WAIT_FINISH;
		}
		else
		{
			this->m_eVideoAnimationState = eVIDEO_ANIMATION_FINISH;
		}
	}
	else
	{
		ofPoint oShakePos_ = _oBackupPos;
		oShakePos_.x += ofRandom(-_fShakeX, _fShakeX);
		oShakePos_.y += ofRandom(-_fShakeY, _fShakeY);
		this->m_pElementPtr->m_stElementBase.oPos = oShakePos_;
	}
}

//--------------------------------------------------------------
void ofxShakeAnimation::Finish()
{
	this->m_pElementPtr->m_stElementBase.bIsShake = false;
}