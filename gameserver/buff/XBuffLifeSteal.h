#ifndef __XBUFFLIFESTEAL_H__
#define __XBUFFLIFESTEAL_H__

#include "XBuff.h"

class XBuffLifeSteal : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffLifeSteal(XBuff* pBuff, const Sequence<float, 2>& data);
	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual bool OnCastDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result);

private:
	float m_Ratio;

	float m_UpperBoundRatio;
	double m_StealUpperBound;
	XBuff* m_pBuff;
};

#endif // __XBUFFCLEAR_H__