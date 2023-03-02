#ifndef __IAIINTERFACE_H__
#define __IAIINTERFACE_H__
#include <string>

struct AIDataArgs
{
	float mSpeed;
	float mNormalAttackProb;
	float mEnterFightRange;
	bool mIsWander;
	float mActionGap;
	float mAIStartTime;
	bool mIsFixedInCd;
	float mFightTogetherDis;
	int mType;
	float mRatioLeft;
	float mRatioRight;
	float mRatioIdle;
	float mRatioDistance;
	float mRatioSkill;
	float mRatioExp;
};

class IAIInterface
{
public:
	IAIInterface(){};
	virtual ~IAIInterface(){};

public:
	virtual std::string GetAIString() = 0;
	virtual void GetAIArgs(AIDataArgs &args) = 0;
};

#endif

