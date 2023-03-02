#ifndef __XBUFFTRIGGERBYCASTSKILL_H__
#define __XBUFFTRIGGERBYCASTSKILL_H__

#include "XBuffTrigger.h"

class XBuffTriggerByCastSkill : public XBuffTrigger
{
public:

	XBuffTriggerByCastSkill(XBuff *pBuff);
	~XBuffTriggerByCastSkill();

	virtual bool OnCastSkill(const HurtInfo& hurtInfo);
	virtual bool CheckTriggerCondition();
private:
	const std::unordered_set<UINT32>* m_SpecifiedSkillsSet;
	const HurtInfo* m_pRawInput;
};


#endif // __XBUFFTRIGGERWHENREMOVE_H__