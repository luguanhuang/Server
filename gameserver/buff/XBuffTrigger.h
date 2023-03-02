#ifndef __XBUFFTRIGGER_H__
#define __XBUFFTRIGGER_H__

#include "XBuff.h"

class XBuff;
class XEntity;

class XTriggerCondition
{
public:
	XTriggerCondition(BuffTable::RowData* pInfo);
	bool CanTrigger();
	void OnTrigger();
	void OnRandFail();
private:
	bool _IsTriggerBuffCD();
	bool _HasTriggerCount();

private:
	UINT64 m_lastTriggerTime;
	UINT64 m_triggerCD;
	UINT32 m_triggerCount;

	bool m_bCDWhenRandFail;
};

class XBuffTrigger : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffTrigger(XBuff *pBuff);
	virtual ~XBuffTrigger();

	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnUpdate();
	inline XEntity *GetEntity() { return m_entity; }
	void Trigger();

protected:
	virtual bool CheckTriggerCondition() { return true; }
	float _GetTriggerParam(const BuffTable::RowData* buffInfo, int index);
	string _GetTriggerParamStr(const BuffTable::RowData* buffInfo, int index);
	int _GetTriggerParamInt(const BuffTable::RowData* buffInfo, int index);
	XTriggerCondition * m_pActiveTriggerCondition;
	virtual void _OnTrigger() {}
	void _SetTarget(XEntity* pEntity) { m_pTarget = pEntity; }

	bool m_bIsTriggerImm;
private:
	void AddTriggerBuff();
	void CastTriggerSkill();
protected:
	double m_triggerRate;

	bool   m_bRandomTriggerBuff;
	uint   m_triggerSkill;
	XEntity *m_entity;
	XBuff  *m_pBuff;
	XEntity *m_pTarget;
	XTriggerCondition * m_pTriggerCondition;
	bool   m_bSkillTriggered;
};

#endif // __XBUFFTRIGGER_H__