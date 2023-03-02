#ifndef __XBUFFTRIGGERBYBEHIT_H__
#define __XBUFFTRIGGERBYBEHIT_H__

#include "XBuffTrigger.h"

class XBuffTriggerByBeHit : public XBuffTrigger
{
public:

	XBuffTriggerByBeHit(XBuff *pBuff);
	~XBuffTriggerByBeHit();

	virtual bool OnBuffEffect(const HurtInfo &hurtInfo, ProjectDamageResult &result);
	virtual XBuffEffectPrioriy GetPriority();
	virtual bool CheckTriggerCondition();

private:
	int m_Type;
	int m_Param0;

	const HurtInfo* m_pRawInput;
	const ProjectDamageResult* m_pResult;
};


#endif // __XBUFFTRIGGERBYBEHIT_H__