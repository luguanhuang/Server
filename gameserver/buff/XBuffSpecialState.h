#ifndef __XBUFFSPECIALSTATE_H__
#define __XBUFFSPECIALSTATE_H__

#include "XBuff.h"

class XObject;

class XBuffSpecialState : public BuffEffect
{
public:
	enum TransformResult
	{
		TR_Success,
		TR_Error,
		TR_NoEffect,
	};

	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffSpecialState(XBuff *pBuff, int buffState);
	~XBuffSpecialState();

	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnRemove(XBuffRemoveReason reason);
	virtual bool OnBuffEffect(const HurtInfo &hurtInfo, ProjectDamageResult &result);
	virtual void OnBattleEnd();
	virtual void OnUpdate();
	virtual XBuffEffectPrioriy GetPriority();
	virtual void OnResetTime();

	static void InitHandlerFun();
	static double GetCantDieDamage(double originalDeltaValue, XObject* pEntity);
	static double GetActualChangeAttr(CombatAttrDef attr, double deltaValue, XObject* pEntity, bool bIgnoreImmortal, bool bForceCantDie);
	static TransformResult TryTransform(XEntity* pEntity, int buffID, UINT32 transformID, bool bTransform);
	static bool CanBuffAdd(XEntity* pEntity, const BuffTable::RowData* pRowData);
	
private:
	typedef void (XBuffSpecialState::*Func)(const HurtInfo &hurtInfo, ProjectDamageResult &result);

	void OnBati(const HurtInfo &hurtInfo, ProjectDamageResult &result);
	void OnDefense(const HurtInfo &hurtInfo, ProjectDamageResult &result);
	void OnImmortal(const HurtInfo &hurtInfo, ProjectDamageResult &result);
	void OnCantDie(const HurtInfo &hurtInfo, ProjectDamageResult &result);
	void OnShield(const HurtInfo &hurtInfo, ProjectDamageResult &result);
	void OnTrapped(const HurtInfo &hurtInfo, ProjectDamageResult &result);
	void RestoreStateParam();
	XBuff *m_pBuff;
	XBuffType m_nBuffType;
	UINT64 m_nToken;
	XEntity *m_pEntity;
	int m_OldStateParam;
	int m_MyStateParam;
	bool m_bUpdated;

	static Func sStateHandler[XBuffType_Max];
};

#endif // __XBUFFSPECIALSTATE_H__