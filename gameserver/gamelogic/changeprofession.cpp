#include "pch.h"
#include "unit/role.h"
#include "table/ProfessionMgr.h"
#include "changeprofession.h"
#include "table/globalconfig.h"
#include "network/mslink.h"
#include "gamelogic/skilllevel.h"
#include "role/rpcg2m_changeprofessionms.h"
#include "gamelogic/rolemisc.h"
#include "gamelogic/bagtransition.h"
#include "time.h"
#include "gamelogic/titlerecord.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/shoprecord.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "table/PlayerLevelManager.h"
#include "spriterecord.h"
#include "utility/tlogger.h"

INSTANCE_SINGLETON(ChangeProfessionMgr);

ChangeProfessionMgr::ChangeProfessionMgr()
{

}

ChangeProfessionMgr::~ChangeProfessionMgr()
{

}

bool ChangeProfessionMgr::Init()
{
	return true;
}

void ChangeProfessionMgr::Uninit()
{

}

KKSG::ErrorCode ChangeProfessionMgr::Check(Role* role, UINT32 pro)
{
	RoleMisc* record = role->Get<RoleMisc>();
	// level 
	if ((UINT32)role->GetLevel() < GetGlobalConfig().ProfessionChangeLevel)
	{
		return KKSG::ERR_CHANGEPRO_LEVEL;
	}
	// time
	if (record->ChangeProTimeLimit())
	{
		return KKSG::ERR_CHANGEPRO_TIME;
	}
	// count
	if (record->ChangeProCountLimit())
	{
		return KKSG::ERR_CHANGEPRO_COUNT;
	}

	///> �Ƿ�ְҵ
	if (pro == 0 || pro > GetGlobalConfig().MaxRole)
	{
		return KKSG::ERR_CHANGEPRO_INVALID;
	}
	///> ְͬҵ
	if (ProfessionMgr::Instance()->GetBasicProfession(role->GetProfession()) == pro)
	{
		return KKSG::ERR_CHANGEPRO_SAMEPRO;
	}
	///> ����
	//BagTakeItemTransition trans(role);
	//trans.SetReason(ItemFlow_ChangePro, ItemFlow_ChangePro_Cost);
	//if (!trans.TakeItem(GetGlobalConfig().ProfessionChangeUseItem[0], GetGlobalConfig().ProfessionChangeUseItem[1]))
	//{
	//	trans.RollBack();
	//	return KKSG::ERR_CHANGEPRO_ITEMLIMIT;
	//}
	//trans.NotifyClient();
	UINT32 count = role->Get<Bag>()->CountItemInBag(GetGlobalConfig().ProfessionChangeUseItem[0]);
	if (count < GetGlobalConfig().ProfessionChangeUseItem[1])
	{
		return KKSG::ERR_CHANGEPRO_ITEMLIMIT;
	}

	RoleChangeProInfo info;
	info.pro = pro;
	info.lastpro = ProfessionMgr::Instance()->GetBasicProfession(role->GetProfession());
	m_roleid2info[role->GetID()] = info;

	return KKSG::ERR_SUCCESS;
}

void ChangeProfessionMgr::BeginChange(Role* role, UINT32 pro)
{
	///> ��������
	RpcG2M_ChangeProfessionMs* rpc = RpcG2M_ChangeProfessionMs::CreateRpc();
	rpc->m_oArg.set_roleid(role->GetID());
	rpc->m_oArg.set_pro(pro);
	MSLink::Instance()->SendTo(*rpc);
	
	// mark
	//role->SetChangingPro(true);

	return;
}

KKSG::ErrorCode ChangeProfessionMgr::DoChange(Role* role)
{
	auto i = m_roleid2info.find(role->GetID());
	if (i == m_roleid2info.end())
	{
		SSWarn<<"not find roleid:"<<role->GetID()<<END;
		return KKSG::ERR_FAILED;
	}

	///> ����
	BagTakeItemTransition trans(role);
	trans.SetReason(ItemFlow_ChangePro, ItemFlow_ChangePro_Cost);
	if (!trans.TakeItem(GetGlobalConfig().ProfessionChangeUseItem[0], GetGlobalConfig().ProfessionChangeUseItem[1]))
	{
		SSWarn<<"take change pro item failed"<<role->GetID()<<END;
		trans.RollBack();
		return KKSG::ERR_CHANGEPRO_ITEMLIMIT;
	}
	trans.NotifyClient();

	RoleMisc* record = role->Get<RoleMisc>();
	record->AddChangeProCount();
	record->SetChangeProTime((UINT32)time(NULL));
	if (GetGlobalConfig().ProfessionChangeSkill)
	{
		role->Get<RoleSkillSet>()->SetFreeResetSkill(true);
	}

	RoleChangeProInfo& info = i->second;
	///> תְҵ + ����
	role->Get<RoleSkillSet>()->ResetSkill();
	role->Get<RoleSkillSet>()->ResetSkillTwo();
	role->Get<RoleSkillSet>()->ResetInitSkill();

	role->SetProfession(info.pro);

	InitSkill *pInitSkill = XEntityInfoLibrary::Instance()->GetInitSkill(info.pro);
	if (pInitSkill)
	{
		role->Get<RoleSkillSet>()->LearnInitSkill(pInitSkill);
		role->SetSkillPoint(SKILL_POINT, PlayerLevelManager::Instance()->GetSkillPoint(role->GetLevel(), true));
		role->SetSkillPoint(SKILL_POINT_TWO, PlayerLevelManager::Instance()->GetSkillPoint(role->GetLevel(), true));
	}

	role->SetTemplateData();

	///> ��Ʒ
	///> װ�� �����ݹ���ת����
	///> �������ã�
	///> ��������ж��
	role->Get<Bag>()->ChangeProfession(info.lastpro, info.pro);
	role->Get<RoleFashion>()->ChangeProfession(info.lastpro, info.pro);

	///> ��������ս��+�������а�
	role->Get<CSpriteRecord>()->OnChangeProfession();
	role->Get<CSpriteRecord>()->UpdateMSRankList();

	///> ��������ս��+�������а�
	role->Get<PetSys>()->UpdateAllPetRankList();

	///> �̵�
	for (auto i = GetGlobalConfig().ProfessionChangeShop.begin(); i != GetGlobalConfig().ProfessionChangeShop.end(); ++i)
	{
		role->Get<CShopRecord>()->Refresh(*i, role->GetLevel(), role->GetProfession(), false, false, true);
	}

	RoleSummaryMgr::Instance()->UpdateRoleSummary(role, SUMMARY_ALL);

	role->RecalcuateAttributeAndNotify(RecalAttr_ChangeProfession);

	SSInfo<<"change pro succeed, roleid:"<<role->GetID()<<" from:"<<info.lastpro<<" to:"<<info.pro<<END;

	role->SetChangingPro(true);

	DoTxLog(role, info.lastpro, info.pro);
	return KKSG::ERR_SUCCESS;
}

bool ChangeProfessionMgr::Fill(UINT64 roleid, bool hasslotrole, const KKSG::RoleBriefInfo& _slotrole, const KKSG::HiddenRoles& _hiddenroles)
{
	auto i = m_roleid2info.find(roleid);
	if (i == m_roleid2info.end())
	{
		SSWarn<<"not find roleid:"<<roleid<<END;
		return false;
	}
	i->second.hasslotrole = hasslotrole;
	i->second.slotrole = _slotrole;
	for (int j = 0; j < _hiddenroles.roles_size(); ++j)
	{
		i->second.hiddenroles[_hiddenroles.roles(j).slot()] = _hiddenroles.roles(j);
	}
	return true;
}

RoleChangeProInfo* ChangeProfessionMgr::GetInfo(UINT64 roleid)
{
	auto i = m_roleid2info.find(roleid);
	if (i == m_roleid2info.end())
	{
		return NULL;
	}
	return &i->second;
}

void ChangeProfessionMgr::DelInfo(UINT64 roleid)
{
	auto i = m_roleid2info.find(roleid);
	if (i != m_roleid2info.end())
	{
		m_roleid2info.erase(i);
	}
}

void ChangeProfessionMgr::DoTxLog(Role* role, INT32 lastpro, INT32 pro)
{
	if (NULL == role)return;
	TChangeProFlow oLog(role);
	oLog.m_PrePro = lastpro;
	oLog.m_CurPro = pro;
	oLog.Do();
}
