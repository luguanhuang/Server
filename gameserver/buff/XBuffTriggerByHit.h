#ifndef __XBUFFTRIGGERBYHIT_H__
#define __XBUFFTRIGGERBYHIT_H__

#include "XBuffTrigger.h"

class XBuffTriggerByHit : public XBuffTrigger
{
public:

	XBuffTriggerByHit(XBuff *pBuff);
	~XBuffTriggerByHit();

	virtual bool OnCastDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result);
	virtual bool CheckTriggerCondition();
	virtual XBuffEffectPrioriy GetPriority();

protected:
	virtual void _OnTrigger();
private:
	int m_Type;

	const HurtInfo* m_pRawInput;
	ProjectDamageResult* m_pResult;

	const std::unordered_set<UINT32>* m_SpecifiedSkillsSet;
	int m_SpecifiedBuffID;

	double m_CriticalChangeRatio;
	int m_KillType;
};


#endif // __XBUFFTRIGGERBYHIT_H__