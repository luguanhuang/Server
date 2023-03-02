#include "pch.h"
#include "rolenethandler.h"
#include "unit/role.h"
#include "scene/ptcg2c_leavescenentf.h"
#include "scene/ptcg2c_enterscenentf.h"
#include "role/ptcg2c_levelchangentf.h"
#include "common/ptcg2c_completeachivement.h"
#include "common/ptcg2d_updatefield.h"
#include "XSpawnInfoGenerator.h"
#include "scene/scene.h"
#include "rolesummarymgr.h"
#include "dblink.h"
#include "LogInit.h"
#include "rolefashion.h"
#include "guild/guildmgr.h"
#include "guild/guildchat.h"
#include "guild/guild.h"
#include "gamelogic/XLevelWave.h"
#include "teammgr.h"
#include "team.h"
#include "role/rpcg2d_saveroledata.h"
#include "entity/XRole.h"
#include "util/dbutil.h"
#include "network/rslink.h"
#include "network/mslink.h"
#include "utildb/roledatafields.h"
#include "gamelogic/changeprofession.h"
#include "role/ptcg2m_levelchangentfms.h"
#include "scene/scenemorphinfo.h"

RoleAttrChangeNotify::RoleAttrChangeNotify(Role* temprole)
{
	role = temprole;
	lastlevel = role->GetLevel();

	CombatAttrDef NotifyAttr[] = {BASIC_Strength, BASIC_Agility, BASIC_Intelligence, BASIC_Vitality};
	int N = sizeof(NotifyAttr)/sizeof(NotifyAttr[0]);
	for (int i = 0; i < N; ++i)
	{
		ntf.m_Data.add_attrid(NotifyAttr[i]);
		ntf.m_Data.add_attroldvalue((UINT32)role->GetAttr(NotifyAttr[i]));
	}
}
	
void RoleAttrChangeNotify::Send()
{
	if ((int)lastlevel == role->GetLevel())
	{
		return;
	}
	CombatAttrDef NotifyAttr[] = {BASIC_Strength, BASIC_Agility, BASIC_Intelligence, BASIC_Vitality};
	int N = sizeof(NotifyAttr)/sizeof(NotifyAttr[0]);
	for (int i = 0; i < N; ++i)
	{
		ntf.m_Data.add_attrnewvalue((UINT32)role->GetAttr(NotifyAttr[i]));
	}
	ntf.m_Data.set_level(role->GetLevel());
	ntf.m_Data.set_exp(role->GetExp());
	ntf.m_Data.set_maxexp(role->GetMaxExp());

	role->Send(ntf);

	PtcG2M_LevelChangeNtfMs ntfms;
	ntfms.m_Data.set_roleid(role->GetID());
	ntfms.m_Data.set_level(role->GetLevel());
	MSLink::Instance()->SendTo(ntfms);
}

RoleNetSender::RoleNetSender(Role *pRole)
{
	m_pRole = pRole;
}

RoleNetSender::~RoleNetSender()
{

}

void RoleNetSender::SendExpAndLevelNtf()
{
	PtcG2C_LevelChangeNtf oLevelupPtc;
	oLevelupPtc.m_Data.set_level(m_pRole->GetLevel());
	oLevelupPtc.m_Data.set_exp(m_pRole->m_exp);
	oLevelupPtc.m_Data.set_maxexp(m_pRole->m_maxExp);
	m_pRole->Send(oLevelupPtc);
}

void RoleNetSender::SendEnterScene(Scene *pScene, bool isforce)
{
	PtcG2C_EnterSceneNtf oEnterPtc;
	oEnterPtc.m_Data.set_sceneid(pScene->GetSceneTemplateID());
	oEnterPtc.m_Data.set_syncmode(pScene->SyncMode());
	oEnterPtc.m_Data.set_iswatcher(m_pRole->IsWatcher());
	oEnterPtc.m_Data.set_canmorph(pScene->GetSceneMorphInfo()->CanMorph());

	// remove arena
	//if (pScene->SyncMode() == MIXMODE)
	//{
	//	XSpawnInfoGenerator::Instance()->GenerateWaveData(m_pRole, pScene, &oEnterPtc.m_Data);
	//}

	if (pScene->SyncMode() != SERVERMODE)
	{
		pScene->FillDoodadInfo(&oEnterPtc.m_Data);
	}
	else
	{
		if (pScene->GetSpawner())
		{
			pScene->GetSpawner()->FillPreloadInfo(oEnterPtc.m_Data);
		}
	}

	if (pScene->GetSceneType() == KKSG::SCENE_FAMILYGARDEN)
	{
		oEnterPtc.m_Data.set_ownerid(pScene->GetOwnerID());
	}

	m_pRole->Send(oEnterPtc);

	// store for reconnect
	m_pRole->SetEnterNtf(oEnterPtc, pScene->GetSceneTemplateID(), pScene->GetSceneID());
}

void RoleNetSender::SendLeaveScene()
{
	PtcG2C_LeaveSceneNtf oLeaveNtfPtc;
	m_pRole->Send(oLeaveNtfPtc);
}


void RoleNetSender::SendAchivementNtf(int aid, int state)
{
	PtcG2C_CompleteAchivement AchPtc;
	AchPtc.m_Data.set_achivementid(aid);
	AchPtc.m_Data.set_state(state);
	m_pRole->Send(AchPtc);
}

void RoleNetSender::SendRoleDataToDB(UINT32 dwParam, bool bSaveAll)
{
	m_pRole->DoAllSave();
	RpcG2D_SaveRoleData* rpc = RpcG2D_SaveRoleData::CreateRpc();
	rpc->m_sessionID = m_pRole->GetSessionID();
	rpc->m_oArg.set_roleid(m_pRole->GetID());

	if(bSaveAll)
	{
		CRoleDataFields oFields(m_pRole->GetRoleAllInfoPtr());
		if(!CRoleDataSaveUtil::SaveAll(oFields, rpc->m_oArg))
		{
			LogError("Pack role data failed, roleid=%llu data maybe lost", m_pRole->GetID());
			delete rpc;
			return;
		}
	}
	else
	{
		//save changed
		const CDataFieldSet& roChanged = m_pRole->GetFieldChanged();
		for(CDataFieldSet::const_iterator it = roChanged.begin(); it != roChanged.end(); ++it)
		{
			if(!CRoleDataSaveUtil::SaveField(*it, rpc->m_oArg))
			{
				LogError("Pack role data failed, roleid=%llu data maybe lost", m_pRole->GetID());
				delete rpc;
				return;
			}
		}
	}

	m_pRole->ClearFieldChanged();

	//account
	if (m_pRole->GetFlag(RSF_ISAccountBriefChanged))
	{
		int slot = MapProfessionToSlot(m_pRole->GetRoleAllInfoPtr()->brief().type());
		rpc->m_oArg.set_account(m_pRole->GetAccount());
		rpc->m_oArg.set_slot(slot);

		KKSG::RoleBriefInfo brief;
		brief.set_roleid(m_pRole->GetID());
		brief.set_name(m_pRole->GetName());
		brief.set_type((KKSG::RoleType)m_pRole->GetProfession());
		brief.set_level(m_pRole->GetLevel());
		FashionBrief fashionbrief = m_pRole->Get<RoleFashion>()->GetFashion();
		for (int i = 0; i < FashionCount; ++i)
		{
			brief.add_fashion(fashionbrief.GetFashionPos(i));
		}
		m_pRole->GetOutLook(*brief.mutable_outlook());
		rpc->m_oArg.set_briefdata(brief.SerializeAsString());

		m_pRole->SetFlag(RSF_ISAccountBriefChanged, false);
	}

	// change pro
	if (m_pRole->IsChangingPro())
	{
		m_pRole->SetChangingPro(false);
		m_pRole->SetBeginChangingPro(false);
		
		KKSG::ChangeProSaveData* changepro = rpc->m_oArg.mutable_changepro();

		RoleChangeProInfo* info = ChangeProfessionMgr::Instance()->GetInfo(m_pRole->GetID());
		if (NULL != info)
		{
			// last slot
			auto i = info->hiddenroles.find(info->lastpro);
			if (i != info->hiddenroles.end())
			{
				// recover
				*changepro->mutable_lastslotrole() = i->second.rolebrief();
				info->hiddenroles.erase(i);
			}
			else
			{
				// empty
				*changepro->mutable_lastslotrole() = "";
			}
			changepro->set_lastslot(info->lastpro);

			// hidden current slot
			if (info->hasslotrole)
			{
				KKSG::HiddenRole hidden;
				hidden.set_slot(info->pro);
				hidden.set_roleid(info->slotrole.roleid());
				*hidden.mutable_rolebrief() = info->slotrole.SerializeAsString();
				info->hiddenroles[info->pro] = hidden;
			}
			// cover hidden roles 
			if (!info->hiddenroles.empty())
			{
				KKSG::HiddenRoles hiddenroles;
				for (auto j = info->hiddenroles.begin(); j != info->hiddenroles.end(); ++j)
				{
					KKSG::HiddenRole* temp = hiddenroles.add_roles();
					*temp = j->second;	
				}
				*changepro->mutable_hiddenroles() = hiddenroles.SerializeAsString();
			}

			ChangeProfessionMgr::Instance()->DelInfo(m_pRole->GetID());
		}
	}

	rpc->m_oArg.set_param(dwParam);

	if (GSConfig::Instance()->IsCrossGS())
	{
		SSDebug << "Cross Gs save role data, roleId: " << m_pRole->GetRoleID() << END;
		m_pRole->SendToRS(*rpc);
	}
	else
	{
		DBLink::Instance()->Send(*rpc);
	}
}
