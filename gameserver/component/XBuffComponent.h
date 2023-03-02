#ifndef __XBUFFCOMPONENT_H__
#define __XBUFFCOMPONENT_H__

#include "component/XComponent.h"
#include "buff/XBuffEnum.h"
#include "buff/XBuffTemplateManager.h"

class XEntity;
class XBuff;
class XBuffChangeAttribute;
struct HurtInfo;
struct ProjectDamageResult;
class XTriggerCondition;

typedef void(*BuffCB)(void *arg, const XBuff* pBuff);
class XBuffComponent : public XComponent
{
public:

	XBuffComponent();
	~XBuffComponent();

	static const UINT32 uuID = XBuff_Component;

	virtual UINT32 ID() { return XBuffComponent::uuID; }

	virtual void OnAttachToHost(XObject* pHost);
	virtual void OnDetachFromHost();

	virtual void Update(float fDeltaT);
	virtual void ActionSubscribe();

	void IterateBuff(BuffCB cb, void *arg);
	void SetAllBuffsInfo(KKSG::AllBuffsInfo* pAllBuffsInfo);

	XBuff *GetBuffByID(int bid);
	
	bool RemoveBuff(XBuff *buff, XBuffRemoveReason reason, bool notifyclient);
	void ClearBuff();
	void OnBuffEffect(const HurtInfo &hurtInfo, ProjectDamageResult &result);

	inline bool IsBuffStateOn(XBuffType type) { return m_buffCount[type] > 0; }
	inline int GetStateParam(XBuffType type) { return m_stateParam[type]; }
	void IncBuffState(XBuffType type, int param);
	void DecBuffState(XBuffType type, int param);
	void SetStateParam(XBuffType type, int param) { m_stateParam[type] = param;}

	XEntity *GetEntity();
	const std::list<XBuff*>& GetBuffList() { return m_buffList; }

	bool OnAddBuffAction(IArgs* pargs, void *);
	bool OnRealDead(IArgs* pargs, void *);
	bool OnLeaveScene(IArgs* pargs, void *);
	bool OnRemoveBuff(IArgs* pargs, void *);
	bool OnAttributeChanged(IArgs* pargs, void *);
	bool OnComboChange(IArgs* pargs, void *);
	bool OnBattleEnd(IArgs* pargs, void *);
	bool OnQTEStateChanged(IArgs* pargs, void *);

	//double ModifySkillCoolDown();
	double ModifySkillCost();
	double ModifySkillCastDamage();
	double ModifyIncReceivedDamage();
	double ModifyDecReceivedDamage();
	double ChangeSkillDamage(UINT32 skillID);

	void NotifyUpdateBuff(XBuff *updateBuff);

	XTriggerCondition* GetTriggerState(BuffTable::RowData* pInfo);
	void ClearTriggerStates();
	void ReApplyChangeAttributeEffect();

	void OnHurt(const HurtInfo &hurtInfo, ProjectDamageResult &result);
	void OnCastDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result);
	void OnCastSkill(const HurtInfo &hurtInfo);

	void MakeSingleEffect(XBuff* pBuff);

	inline bool IsDestroying() { return m_bDestroying; }
	inline bool IsLeavingScene() { return m_bLeavingScene; }
private:
	void _AddBuff(const BuffDesc& buffDesc, bool notifyclient, UINT64 caster);
	void _AppendBuff(XBuff* pExistBuff);
	void _ResetBuffState();
	void _CheckBuffState();
	void _NotifyAddBuff(XBuff *addbuff);
	void _NotifyRemoveBuff(XBuff *removeBuff);
	void _RefreshQueue();
	UINT32 _GetSpecialState();
	void _SendAIEvent(XBuff *addbuff);
	void _CheckRelatedBuffs(const BuffTable::RowData* rowData, bool& bCanAdd, std::vector<XBuff*>& buffsShouldRemove);
	bool _PreAddBuff(const BuffTable::RowData* pRowData);
	void _PostAddBuff(XBuff* pNewBuff);
private:
	UINT32 _magic_num;

	std::list<XBuff*> m_buffList;
	std::list<BuffDesc> m_addBuff;
	short m_buffCount[XBuffType_Max];
	int m_stateParam[XBuffType_Max];
	std::map<int, XTriggerCondition*> m_GlobalTriggerState;
	bool m_bNotifyClientWhenAddButExist;
	bool m_bRefreshingQueue;

	bool m_bDestroying;
	bool m_bLeavingScene;
};

#endif // __XBUFFCOMPONENT_H__