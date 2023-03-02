#include "pch.h"
#include "XBuffComponent.h"
#include "buff/XBuff.h"
#include "buff/XBuffTemplateManager.h"
#include "entity/XObject.h"
#include "unit/unit.h"
#include "unit/enemy.h"
#include "battle/ptcg2c_buffnotify.h"
#include "scene/scene.h"
#include "XComponent.h"
#include "buff/XBuffTrigger.h"
#include "battle/ptcg2c_clientonlybuffnotify.h"
#include "unit/role.h"
#include "gamelogic/spriterecord.h"
#include "gamelogic/combateffect.h"

XBuffComponent::XBuffComponent()
	:_magic_num(0xEDCBA987)
{
	_ResetBuffState();
}

XBuffComponent::~XBuffComponent()
{
	_magic_num = 0;
}

void XBuffComponent::OnAttachToHost(XObject* pHost)
{
	m_bDestroying = false;
	m_bLeavingScene = false;
	m_bRefreshingQueue = false;

	XComponent::OnAttachToHost(pHost);
	if(pHost->GetUnit()->IsEnemy())
	{
		Enemy* pEnemy = dynamic_cast<Enemy*>(pHost->GetUnit());
		if (pEnemy != NULL)
			m_bNotifyClientWhenAddButExist = pEnemy->IsBoss();
		else
			m_bNotifyClientWhenAddButExist = false;
	}
	else
	{
		m_bNotifyClientWhenAddButExist = true;
	}
}

void XBuffComponent::OnDetachFromHost()
{
	m_bDestroying = true;
	ClearBuff();
	XComponent::OnDetachFromHost();
}

void XBuffComponent::Update(float fDeltaT)
{
	_RefreshQueue();

	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		XBuff *pBuff = *i;

		if (pBuff->IsValid())
		{
			pBuff->OnUpdate();
		}
	}
}

void XBuffComponent::ActionSubscribe()
{
	RegisterAction(XAction_BuffAdd, DelegateCombination(XBuffComponent, OnAddBuffAction, this));
	RegisterAction(XAction_BuffRemove, DelegateCombination(XBuffComponent, OnRemoveBuff, this));
	RegisterAction(XAction_RealDead, DelegateCombination(XBuffComponent, OnRealDead, this));
	RegisterAction(XAction_LeaveScene, DelegateCombination(XBuffComponent, OnLeaveScene, this));
	RegisterAction(XAction_AttributeChange, DelegateCombination(XBuffComponent, OnAttributeChanged, this));
	RegisterAction(XAction_ComboChange, DelegateCombination(XBuffComponent, OnComboChange, this));
	RegisterAction(XAction_BattleEnd, DelegateCombination(XBuffComponent, OnBattleEnd, this));
	RegisterAction(XAction_QTE_On, DelegateCombination(XBuffComponent, OnQTEStateChanged, this));
}

void XBuffComponent::SetAllBuffsInfo(KKSG::AllBuffsInfo* pAllBuffsInfo)
{
	pAllBuffsInfo->set_buffstate(_GetSpecialState());
	for	(int i = 0; i < XBuffType_Max; ++i)
	{
		if(m_stateParam[i] != 0)
		{
			pAllBuffsInfo->add_stateparamvalues(m_stateParam[i]);
			pAllBuffsInfo->add_stateparamindex(i);
		}
	}
}

void XBuffComponent::_SendAIEvent(XBuff *addbuff)
{
	if(addbuff->GetBuffInfo()->AIEvent != 0)
	{
		XAIEventArgs args;
		args.MsgTargetTemplateId = 0;
		args.EventArg = "buffstring";
		args.Templateid = addbuff->GetID();
		args.Pos = addbuff->GetEntity()->GetPosition_p();
		args.FloatArg = 0;
		Scene* pScene = addbuff->GetEntity()->GetUnit()->GetCurrScene();
		if (pScene != NULL)
			pScene->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);
	}
}

void XBuffComponent::_NotifyAddBuff(XBuff *addbuff)
{
	if (!addbuff->IsMustNotify())
		return;
	Unit *pUnit = GetHost()->GetUnit();
	if (pUnit && pUnit->GetCurrScene())
	{
		PtcG2C_BuffNotify oPtc;
		oPtc.m_Data.set_uid(GetHost()->GetID());

		KKSG::BuffInfo* pBuffInfo = oPtc.m_Data.mutable_addbuff();
		pBuffInfo->set_buffid(addbuff->GetID());
		pBuffInfo->set_bufflevel(addbuff->GetLevel());
		pBuffInfo->set_lefttime(addbuff->GetDuration());
		if(addbuff->GetEffectData()->MobID > 0)
			pBuffInfo->set_mobuid(addbuff->GetEffectData()->MobID);
		if(addbuff->GetEffectData()->bReduceCD)
			pBuffInfo->set_breducecd(addbuff->GetEffectData()->bReduceCD);
		if(addbuff->GetEffectData()->TransformID != 0)
			pBuffInfo->set_transformid(addbuff->GetEffectData()->TransformID);
		pBuffInfo->set_curhp(addbuff->GetHP());
		pBuffInfo->set_maxhp(addbuff->GetMaxHP());
		pBuffInfo->set_stackcount(addbuff->GetStackCount());

		KKSG::AllBuffsInfo* pAllBuffsInfo = oPtc.m_Data.mutable_allbuffsinfo();
		SetAllBuffsInfo(pAllBuffsInfo);

		pUnit->GetCurrScene()->Broadcast(pUnit, oPtc);

		//SSInfo << "HostID: " << GetHost()->GetID() << ", Buff ID " << addbuff->GetID() << ", Buff Level " << addbuff->GetLevel() << END;
	}
}

void XBuffComponent::NotifyUpdateBuff(XBuff *updateBuff)
{
	if (!updateBuff->IsMustNotify())
		return;
	Unit *pUnit = GetHost()->GetUnit();
	if (pUnit && pUnit->GetCurrScene())
	{
		PtcG2C_BuffNotify oPtc;
		oPtc.m_Data.set_uid(GetHost()->GetID());

		KKSG::BuffInfo* pBuffInfo = oPtc.m_Data.mutable_updatebuff();
		pBuffInfo->set_buffid(updateBuff->GetID());
		pBuffInfo->set_bufflevel(updateBuff->GetLevel());
		pBuffInfo->set_lefttime(updateBuff->GetLeftTime());
		pBuffInfo->set_mobuid(updateBuff->GetEffectData()->MobID);
		pBuffInfo->set_breducecd(updateBuff->GetEffectData()->bReduceCD);
		pBuffInfo->set_transformid(updateBuff->GetEffectData()->TransformID);
		pBuffInfo->set_curhp(updateBuff->GetHP());
		pBuffInfo->set_maxhp(updateBuff->GetMaxHP());
		pBuffInfo->set_stackcount(updateBuff->GetStackCount());

		KKSG::AllBuffsInfo* pAllBuffsInfo = oPtc.m_Data.mutable_allbuffsinfo();
		SetAllBuffsInfo(pAllBuffsInfo);

		pUnit->GetCurrScene()->Broadcast(pUnit, oPtc);

		//SSInfo << "HostID: " << GetHost()->GetID() << ", Buff ID " << updateBuff->GetID() << ", Buff Level " << updateBuff->GetLevel() << END;
	}
}

void XBuffComponent::_NotifyRemoveBuff(XBuff *removeBuff)
{
	if (!removeBuff->IsMustNotify())
		return;
	Unit *pUnit = GetHost()->GetUnit();
	if (pUnit && pUnit->GetCurrScene() && !m_bLeavingScene && !m_bDestroying)
	{
		PtcG2C_BuffNotify oPtc;
		oPtc.m_Data.set_uid(GetHost()->GetID());

		KKSG::BuffInfo* pBuffInfo = oPtc.m_Data.mutable_removebuff();
		pBuffInfo->set_buffid(removeBuff->GetID());
		pBuffInfo->set_bufflevel(removeBuff->GetLevel());
		if(removeBuff->GetEffectData()->bReduceCD)
			pBuffInfo->set_breducecd(removeBuff->GetEffectData()->bReduceCD);

		//>变身ID为负时，死亡的时候不恢复原型，所以当服务器发变身ID时，客户端才变回原形
		if (removeBuff->GetEffectData()->TransformID != 0)
		{
			if (removeBuff->GetEffectData()->TransformID > 0 || !pUnit->IsDead())
				pBuffInfo->set_transformid(removeBuff->GetEffectData()->TransformID);
		}

		KKSG::AllBuffsInfo* pAllBuffsInfo = oPtc.m_Data.mutable_allbuffsinfo();
		SetAllBuffsInfo(pAllBuffsInfo);

		pUnit->GetCurrScene()->Broadcast(pUnit, oPtc);

		//SSInfo << "HostID: " << GetHost()->GetID() << ", Buff ID " << removeBuff->GetID() << ", Buff Level " << removeBuff->GetLevel() << END;
	}
}

UINT32 XBuffComponent::_GetSpecialState()
{
	UINT32 state = 0;
	for(int i = 0; i < XBuffType_Max; ++i)
	{
		if(IsBuffStateOn((XBuffType)i))
			state |= (1 << i);
	}
	return state;
}

bool XBuffComponent::RemoveBuff(XBuff *buff, XBuffRemoveReason reason, bool notifyclient)
{
	buff->OnRemove(reason);

	if (notifyclient)
	{
		_NotifyRemoveBuff(buff);
	}

	return true;
}

void XBuffComponent::_AppendBuff(XBuff* pExistBuff)
{
	CombatEffectHelper effectHelper(GetEntity()->GetUnit(), pExistBuff->GetCaster(), pExistBuff->GetBuffInfo());

	pExistBuff->Append(&effectHelper);
	NotifyUpdateBuff(pExistBuff);
	pExistBuff->OnAppend();
}

void XBuffComponent::_AddBuff(const BuffDesc& buffDesc, bool notifyclient, UINT64 caster)
{
	Scene* pScene = GetEntity()->GetUnit()->GetCurrScene();
	BuffTable::RowData* pBuffData = XBuffTemplateManager::Instance()->GetBuffInfo(buffDesc.buffID, buffDesc.BuffLevel, pScene);
	if (pBuffData == NULL)
		return;

	if (!_PreAddBuff(pBuffData))
		return;

	if (caster == 0)
		caster = GetEntity()->GetUnit()->GetID();

	CombatEffectHelper effectHelper(GetEntity()->GetUnit(), caster, pBuffData);

	XBuff *pBuff = XBuffTemplateManager::Instance()->CreateBuff(buffDesc, &effectHelper);

	if (pBuff != NULL)
	{
		pBuff->SetCaster(caster);
		pBuff->OnAdd(this->GetEntity(), &effectHelper);
		_PostAddBuff(pBuff);
		m_buffList.push_back(pBuff);

		if (notifyclient)
		{
			_NotifyAddBuff(pBuff);
		}

		_SendAIEvent(pBuff);

		if(pBuff->HasTag(XBuffTag_SpriteEffect) && GetEntity()->GetUnit()->IsRole())
		{
			Role* pRole = static_cast<Role*>(GetEntity()->GetUnit());
			pRole->Get<CSpriteRecord>()->OnSkillCast();
		}
	}
}

void XBuffComponent::_CheckRelatedBuffs(const BuffTable::RowData* rowData, bool& bCanAdd, std::vector<XBuff*>& buffsShouldRemove)
{
	bCanAdd = true;
	for (auto it = m_buffList.begin(); it != m_buffList.end(); ++it)
	{
		XBuff* existBuff = *it;
		if (!existBuff->IsValid())
			continue;

		if (!existBuff->GetExclusive().CanAdd(rowData->BuffClearType))
		{
			bCanAdd = false;
			return;
		}

		//if (existBuff->GetExclusive().IsExcluded(rowData))
		//{
		//	buffsShouldRemove.push_back(existBuff);
		//}
	}

	bCanAdd = XBuff::CanBuffAdd(GetEntity(), rowData);
}

bool XBuffComponent::_PreAddBuff(const BuffTable::RowData* pRowData)
{
	bool bCanBuffAdd;
	std::vector<XBuff*> buffsShouldRemove;
	_CheckRelatedBuffs(pRowData, bCanBuffAdd, buffsShouldRemove);
	if (!bCanBuffAdd)
	{
		return false;
	}
	if (buffsShouldRemove.size() != 0)
	{
		for (auto it = buffsShouldRemove.begin(); it != buffsShouldRemove.end(); ++it)
		{
			RemoveBuff(*it, XBuffRemoveReason_Manual, true);
		}
	}

	return true;
}

void XBuffComponent::_PostAddBuff(XBuff* pNewBuff)
{
	if (pNewBuff == NULL)
		return;

	for (auto it = m_buffList.begin(); it != m_buffList.end(); ++it)
	{
		XBuff* pExistBuff = *it;
		if (!pExistBuff->IsValid() || pExistBuff == pNewBuff)
			continue;

		if (!pNewBuff->GetExclusive().ShouldClear(pExistBuff->GetClearType()))
		{
			RemoveBuff(pExistBuff, XBuffRemoveReason_Manual, true);
		}
	}
}

void XBuffComponent::MakeSingleEffect(XBuff* pBuff)
{
	if (pBuff == NULL)
		return;

	for (auto it = m_buffList.begin(); it != m_buffList.end(); ++it)
	{
		XBuff* pExistBuff = *it;
		if (!pExistBuff->IsValid() || pExistBuff == pBuff)
			continue;

		if (pBuff->GetExclusive().IsSingleEffectConflict(pExistBuff->GetExclusive()))
		{
			RemoveBuff(pExistBuff, XBuffRemoveReason_Manual, true);
		}
	}
}

XBuff * XBuffComponent::GetBuffByID(int bid)
{
	XBuff *pBuff = NULL;
	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		pBuff = *i;

		if (pBuff->IsValid() && pBuff->GetID() == bid)
		{
			return pBuff;
		}
	}

	return NULL;
}

void XBuffComponent::ClearBuff()
{
	std::list<XBuff*> temp;
	temp.swap(m_buffList);
	for (auto i = temp.begin(); i != temp.end(); ++i)
	{
		XBuff *pBuff = *i;
		if (pBuff->IsValid())
			RemoveBuff(pBuff, XBuffRemoveReason_Manual, true);
		delete pBuff;
	}

	_CheckBuffState();
	_ResetBuffState();
	m_addBuff.clear();
}

void XBuffComponent::OnHurt(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	XBuff* pBuff;
	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		pBuff = *i;

		if (!pBuff->IsValid())
		{
			continue;
		}

		pBuff->OnBuffEffect(hurtInfo, result, XBuff::OnHurt);
	}
}
void XBuffComponent::OnCastDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	XBuff* pBuff;
	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		pBuff = *i;

		if (!pBuff->IsValid())
		{
			continue;
		}

		pBuff->OnBuffEffect(hurtInfo, result, XBuff::OnCastDamage);
	}
}


void XBuffComponent::IncBuffState(XBuffType type, int param)
{
	++m_buffCount[type];

	switch(type)
	{
	case XBuffType_Transform:
	case XBuffType_Scale:
		m_stateParam[type] = param;
		break;
	default:
		m_stateParam[type] += param;
		break;
	}
}

void XBuffComponent::DecBuffState(XBuffType type, int param)
{
	--m_buffCount[type];

	switch(type)
	{
	case XBuffType_Transform:
	case XBuffType_Scale:
		if(m_stateParam[type] == param)
			m_stateParam[type] = 0;
		break;
	default:
		m_stateParam[type] -= param;
		break;
	}
}

XEntity * XBuffComponent::GetEntity()
{
	return (XEntity *)GetHost();
}

bool XBuffComponent::OnAddBuffAction(IArgs* pargs, void *)
{
	assert(_magic_num == 0xEDCBA987);

	if (GetHost()->GetUnit()->IsDead())
	{
		return true;
	}

	XAddBuffActionArgs *AddBuffArgs = (XAddBuffActionArgs *)pargs;

	if(AddBuffArgs->singleBuff.buffID == 0 && AddBuffArgs->vecBuffs.size() == 0)
		return true;

	if(AddBuffArgs->bClientOnly)
	{
		PtcG2C_ClientOnlyBuffNotify oPtc;
		oPtc.m_Data.set_roleid(GetHost()->GetID());
		oPtc.m_Data.set_casterid(AddBuffArgs->Caster);

		if(AddBuffArgs->singleBuff.buffID != 0)
		{
			KKSG::Buff* pBuff = oPtc.m_Data.add_buffs();
			pBuff->set_buffid(AddBuffArgs->singleBuff.buffID);
			pBuff->set_bufflevel(AddBuffArgs->singleBuff.BuffLevel);
			if(AddBuffArgs->singleBuff.skillID != 0)
				pBuff->set_skillid(AddBuffArgs->singleBuff.skillID);
			if(AddBuffArgs->singleBuff.IsTimeSpecified())
				pBuff->set_effecttime(AddBuffArgs->singleBuff.effecttime);
		}
		for(auto it = AddBuffArgs->vecBuffs.begin(); it != AddBuffArgs->vecBuffs.end(); ++it)
		{
			BuffDesc& desc = (*it);
			KKSG::Buff* pBuff = oPtc.m_Data.add_buffs();
			pBuff->set_buffid(desc.buffID);
			pBuff->set_bufflevel(desc.BuffLevel);
			if(desc.skillID != 0)
				pBuff->set_skillid(desc.skillID);
			if(desc.IsTimeSpecified())
				pBuff->set_effecttime(desc.effecttime);
		}

		GetHost()->GetUnit()->Send(oPtc);

		return true;
	}

	/*
	BuffDesc desc;
	if(AddBuffArgs->BuffID != 0)
	{	
		desc.buffID = AddBuffArgs->BuffID;
		desc.BuffLevel = AddBuffArgs->BuffLevel;
		desc.casterID = AddBuffArgs->Caster;
		m_addBuff.push_back(desc);
	}

	int size = AddBuffArgs->BuffIDs.size();

	for (int i = 0; i < size; ++i)
	{
		BuffDesc desc;
		desc.buffID = AddBuffArgs->BuffIDs[i];
		desc.BuffLevel = AddBuffArgs->BuffLevels[i];
		desc.casterID = AddBuffArgs->Caster;
		m_addBuff.push_back(desc);
	}
	*/

	if(AddBuffArgs->singleBuff.buffID != 0)
	{
		if(AddBuffArgs->Caster != 0)
			AddBuffArgs->singleBuff.casterID = AddBuffArgs->Caster;

		m_addBuff.push_back(AddBuffArgs->singleBuff);
	}

	for(auto it = AddBuffArgs->vecBuffs.begin(); it != AddBuffArgs->vecBuffs.end(); ++it)
	{
		BuffDesc& desc = (*it);
		if(AddBuffArgs->Caster != 0)
			desc.casterID = AddBuffArgs->Caster;
		m_addBuff.push_back(desc);
	}

	if(AddBuffArgs->EffectImm) _RefreshQueue();

	return true;
}

bool XBuffComponent::OnRealDead(IArgs* pargs, void *)
{
	assert(_magic_num == 0xEDCBA987);

	XRealDeadActionArgs *Args = (XRealDeadActionArgs*)pargs;

	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		XBuff *pBuff = *i;

		if (pBuff->IsValid())
		{
			pBuff->OnRealDead(Args);
		}
	}

	ClearBuff();
	return true;
}

void XBuffComponent::IterateBuff(BuffCB cb, void *arg)
{
	assert(_magic_num == 0xEDCBA987);

	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		XBuff *pBuff = *i;

		if (pBuff->IsValid())
		{
			cb(arg, pBuff);
		}
	}
}

bool XBuffComponent::OnLeaveScene(IArgs* pargs, void *)
{
	assert(_magic_num == 0xEDCBA987);
	
	m_bLeavingScene = true;

	ClearBuff();
	ClearTriggerStates();

	m_bLeavingScene = false;

	return true;
}

void XBuffComponent::_ResetBuffState()
{
	for (int i = 0; i < XBuffType_Max; ++i)
	{
		m_buffCount[i] = 0;
		m_stateParam[i] = 0;
	}
}

void XBuffComponent::_CheckBuffState()
{
	for (int i = 0; i < XBuffType_Max; ++i)
	{
		if (m_buffCount[i] != 0)
		{
			LogWarn("Clear buff, state %d is not 0", i);
		}
	}
}

bool XBuffComponent::OnRemoveBuff(IArgs* pargs, void *)
{
	assert(_magic_num == 0xEDCBA987);

	XRemoveBuffActionArgs *Args = (XRemoveBuffActionArgs *)pargs;
	XBuff *pExistBuff = GetBuffByID(Args->nBuffId);
	if (pExistBuff != NULL)
	{
		RemoveBuff(pExistBuff, XBuffRemoveReason_Manual, true);
	}

	///> 删掉待添加队列中的buff
	for(auto it = m_addBuff.begin(); it != m_addBuff.end();)
	{
		if (it->buffID == Args->nBuffId)
			it = m_addBuff.erase(it);
		else
			++it;
	}
	return true;
}

bool XBuffComponent::OnAttributeChanged(IArgs* pargs, void *)
{
	assert(_magic_num == 0xEDCBA987);

	XAttributeChangedEventArgs *Args = (XAttributeChangedEventArgs*)pargs;

	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		XBuff *pBuff = *i;

		if (pBuff->IsValid())
		{
			pBuff->OnAttributeChanged((CombatAttrDef)Args->AttrID);
		}
	}

	return true;
}

bool XBuffComponent::OnQTEStateChanged(IArgs* pargs, void *)
{
	assert(_magic_num == 0xEDCBA987);

	XOnQTEOnEventArgs *Args = (XOnQTEOnEventArgs*)pargs;

	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		XBuff *pBuff = *i;

		if (pBuff->IsValid())
		{
			pBuff->OnQTEStateChanged(Args);
		}
	}

	return true;
}

bool XBuffComponent::OnComboChange(IArgs* pargs, void *)
{
	assert(_magic_num == 0xEDCBA987);

	XComboChangeArgs *Args = (XComboChangeArgs*)pargs;

	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		XBuff *pBuff = *i;

		if (pBuff->IsValid())
		{
			pBuff->OnComboChange(Args->GetComboCount());
		}
	}

	return true;
}

bool XBuffComponent::OnBattleEnd(IArgs* pargs, void *)
{
	assert(_magic_num == 0xEDCBA987);

	XBattleEndArgs *Args = (XBattleEndArgs*)pargs;

	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		XBuff *pBuff = *i;

		if (pBuff->IsValid())
		{
			pBuff->OnBattleEnd();
		}
	}

	return true;
}

//double XBuffComponent::ModifySkillCoolDown()
//{
//	double ret = 1;
//	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
//	{
//		XBuff *pBuff = *i;
//		ret *= pBuff->ModifySkillCoolDown();
//	}
//
//	return ret;
//}

double XBuffComponent::ModifySkillCost()
{
	double ret = 0;
	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		XBuff *pBuff = *i;
		ret += pBuff->ModifySkillCost();
	}
	ret = XClamp(ret,  GetGlobalConfig().MpCostChangeLowerBound,  GetGlobalConfig().MpCostChangeUpperBound);
	return ret;
}

double XBuffComponent::ModifySkillCastDamage()
{
	double ret = 0;
	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		XBuff *pBuff = *i;
		ret += pBuff->ModifySkillCastDamage();
	}
	ret = XClamp(ret,  GetGlobalConfig().BuffCastDamageChangeLowerBound,  GetGlobalConfig().BuffCastDamageChangeUpperBound);
	return ret;
}

double XBuffComponent::ModifyIncReceivedDamage()
{
	double ret = 0;
	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		XBuff *pBuff = *i;
		ret += pBuff->ModifyIncReceivedDamage();
	}
	if (ret >  GetGlobalConfig().BuffReceivedDamageChangeUpperBound)
		ret =  GetGlobalConfig().BuffReceivedDamageChangeUpperBound;
	return ret;
}

double XBuffComponent::ModifyDecReceivedDamage()
{
	double ret = 0;
	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		XBuff *pBuff = *i;
		ret += pBuff->ModifyDecReceivedDamage();
	}
	if (ret <  GetGlobalConfig().BuffReceivedDamageChangeLowerBound)
		ret =  GetGlobalConfig().BuffReceivedDamageChangeLowerBound;
	return ret;
}

double XBuffComponent::ChangeSkillDamage(UINT32 skillID)
{
	double ret = 0;
	for(auto it = m_buffList.begin(); it != m_buffList.end(); ++it)
	{
		ret += (*it)->ChangeSkillDamage(skillID);
	}
	return ret;
}

void XBuffComponent::_RefreshQueue()
{
	if (m_bRefreshingQueue)
		return;

	m_bRefreshingQueue = true;

	for (auto i = m_buffList.begin(); i != m_buffList.end();)
	{
		XBuff *pBuff = *i;
		if (pBuff->IsValid())
		{
			++i;
		}
		else
		{
			delete pBuff;
			m_buffList.erase(i++);
		}
	}

	std::list<BuffDesc> addbuffTemp;
	addbuffTemp.swap(m_addBuff);

	for (auto j = addbuffTemp.begin(); j != addbuffTemp.end(); ++j)
	{
		BuffTable::RowData* pBuffInfo = XBuffTemplateManager::Instance()->GetBuffInfo(j->buffID, j->BuffLevel, GetEntity()->GetUnit()->GetCurrScene());
		if (pBuffInfo != NULL)
		{
			XBuff *pExistBuff = GetBuffByID(j->buffID);
			if (pExistBuff != NULL)
			{
				if (pExistBuff->GetLevel() > pBuffInfo->BuffLevel)
				{
					continue;
				}

				if (pExistBuff->GetLevel() < pBuffInfo->BuffLevel)
				{
					RemoveBuff(pExistBuff, XBuffRemoveReason_Manual, m_bNotifyClientWhenAddButExist);
					_AddBuff(*j, m_bNotifyClientWhenAddButExist, j->casterID);
					continue;
				}

				switch(pExistBuff->GetMergeType())
				{
				case XBuffMergeType_Replace:
					{
						RemoveBuff(pExistBuff, XBuffRemoveReason_Replace, m_bNotifyClientWhenAddButExist);
						_AddBuff(*j, m_bNotifyClientWhenAddButExist, j->casterID);
						break;
					}
				case XBuffMergeType_ExtendTime:
					{
						pExistBuff->AddBuffTime(pExistBuff->GetOriginalDuration());
						NotifyUpdateBuff(pExistBuff);
						break;
					}
				case XBuffMergeType_Stacking:
					{
						_AppendBuff(pExistBuff);
						//_AddBuff(j->buffID, j->BuffLevel, true, j->casterID);
						break;
					}
				case XBuffMergeType_Reset:
					{
						if (pExistBuff->ResetBuffTime())
							NotifyUpdateBuff(pExistBuff);
						break;
					}
				default:
					break;
				}
			}
			else
			{
				_AddBuff(*j, true, j->casterID);
			}
		}
	}

	m_bRefreshingQueue = false;
}

XTriggerCondition* XBuffComponent::GetTriggerState(BuffTable::RowData* pInfo)
{
	std::map<int, XTriggerCondition*>::iterator itFind = m_GlobalTriggerState.find(pInfo->BuffID);
	if(itFind == m_GlobalTriggerState.end())
	{
		XTriggerCondition* newCond = new XTriggerCondition(pInfo);
		m_GlobalTriggerState[pInfo->BuffID] = newCond;
		return newCond;
	}
	else
	{
		return itFind->second;
	}
}

void XBuffComponent::ClearTriggerStates()
{
	for( auto i = m_GlobalTriggerState.begin(); i != m_GlobalTriggerState.end(); i++ )
	{
		delete i->second;
	}

	m_GlobalTriggerState.clear();
}

void XBuffComponent::ReApplyChangeAttributeEffect()
{
	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		XBuff *pBuff = *i;

		if (pBuff->IsValid())
		{
			CombatEffectHelper effectHelper(GetEntity()->GetUnit(), pBuff->GetCaster(), pBuff->GetBuffInfo());
			pBuff->ReApplyChangeAttributeEffect(&effectHelper);
		}
	}
}

void XBuffComponent::OnCastSkill(const HurtInfo &hurtInfo)
{
	XBuff* pBuff;
	for (auto i = m_buffList.begin(); i != m_buffList.end(); ++i)
	{
		pBuff = *i;

		if (!pBuff->IsValid())
		{
			continue;
		}

		pBuff->OnCastSkill(hurtInfo);
	}
}
