#ifndef __XBUFFDAMAGEREFLECTION_H__
#define __XBUFFDAMAGEREFLECTION_H__

#include "XBuff.h"

class XBuffDamageReflection : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffDamageReflection(float ratio, XBuff* _Buff);
	virtual bool OnBuffEffect(const HurtInfo& rawInput, ProjectDamageResult& result);
	virtual XBuffEffectPrioriy GetPriority();

private:
	float m_Ratio;
	XBuff* m_pBuff;
};

#endif // __XBUFFDAMAGEREFLECTION_H__