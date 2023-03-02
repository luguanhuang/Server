#ifndef __XBUFFLIFEADDATTACK_H__
#define __XBUFFLIFEADDATTACK_H__

#include "XBuff.h"
#include "unit/combatattrdef.h"
#include "XBuffReduceDamage.h"

class XBuff;
class XEntity;

class XBuffLifeAddAttack : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffLifeAddAttack(const BuffTable::RowData *pBuffInfo);
	~XBuffLifeAddAttack();

	virtual bool OnCastDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result);

	virtual XBuffEffectPrioriy GetPriority();

	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);

	//virtual void OnRemove(XBuffRemoveReason reason);

	//virtual void OnAttributeChanged(CombatAttrDef attr);

private:
	//void CheckHP();
	//void AddEffect();
	//void RemoveEffect();

	//bool m_IsEffect;
	//double m_HPTriggerPercent;
	//double m_AttrValue;
	//CombatAttrDef m_AttrID;
	XEntity *m_Entity;

	//XBuff *m_pBuff;

	XPieceWiseDataMgr m_Data;
};

#endif // __XBUFFLIFEADDATTACK_H__