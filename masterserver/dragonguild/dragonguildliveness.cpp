#include "pch.h"
#include "dragonguildliveness.h"
#include "dragonguild.h"
#include "util/gametime.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "tablemgr/dragonguildtablemgr.h"
#include "partner/ptcm2c_updatepartnertoclient.h"
#include "dragonguild/ptcm2g_notifydragonguildredpoint.h"
#include "define/systemiddef.h"
#include "table/globalconfig.h"
#include "mail/mailconfig.h"
#include "mail/mailmgr.h"
#include "foreach.h"

DragonGuildLiveness::DragonGuildLiveness(DragonGuild* dragonguild):DragonGuildBase(dragonguild)
	,m_liveness(0)
{
	m_roleActivityMap.clear();
	m_livenessItem.clear();
}

bool DragonGuildLiveness::LoadFromDB(const KKSG::DragonGuildSaveData& data)
{
	KKSG::DragonGuildLivenessData ln;

	if (!ln.ParseFromString(data.liveness()))
	{
		LogError("parse livenes failed");
		return false;
	}

	m_liveness = ln.liveness();

	m_livenessItem.clear();
	m_livenessItem.resize(ln.record_size());
	for (int i = 0; i < ln.record_size(); ++i)
	{
		m_livenessItem[i].FromKKSG(ln.record(i));
	}
	
	m_roleActivityMap.clear();
	for (int i=0; i < ln.roleactivity_size(); ++ i)
	{
		UINT64 roleId = ln.roleactivity(i).roleid();
		UINT32 value = ln.roleactivity(i).activity();
		m_roleActivityMap[roleId] = value;
	}
	return true;
}

void DragonGuildLiveness::SaveToDB(KKSG::DragonGuildSaveData& data)
{
	KKSG::DragonGuildLivenessData ln;

	ln.set_liveness(m_liveness);

	ln.clear_record();
	for (auto it = m_livenessItem.begin(); it != m_livenessItem.end(); ++it)
	{
		KKSG::DragonGuildLivenessRecordData* record = ln.add_record();
		it->ToKKSG(*record);
	}

	ln.clear_roleactivity();
	for (auto it = m_roleActivityMap.begin(); it != m_roleActivityMap.end(); ++ it)
	{
		KKSG::DragonGuildRoleActicityData* data = ln.add_roleactivity();
		data->set_roleid(it->first);
		data->set_activity(it->second);
	}

	ln.SerializeToString(data.mutable_liveness());
}

void DragonGuildLiveness::OnDayPass()
{
	_CheckLeaderActivity();

	m_liveness = 0;	
	m_livenessItem.clear();
	m_roleActivityMap.clear();
	
	SetDirty();
}

void DragonGuildLiveness::AddPartnerLiveness(UINT64 roleId, UINT32 actId, UINT32 value)
{
	DragonGuildLivenessRecord item;
	item.roleId = roleId;
	item.actId = actId;
	item.value = value;
	item.time = GameTime::GetTime();
	
	UINT32 oldValue = m_liveness;
	m_liveness += value;
	m_livenessItem.push_front(item);

	if (m_livenessItem.size() > GetGlobalConfig().DragonGuildLivenessRecordNum)
	{
		m_livenessItem.pop_back();
	}

	m_roleActivityMap[roleId] += value;

	// 有箱子可以领取的时候, 推送给GS活跃度，用于推送给客户端红点
	bool hasChest = DragonGuildTableMgr::Instance()->HasNewCanTakChest(m_owner->GetLevel(), oldValue, m_liveness);
	if (hasChest)
	{
		PtcM2G_NotifyDragonGuildRedPoint ntf;
		ntf.m_Data.set_systemid(SYS_DRAGONGUILD_LIVENESS);
		ntf.m_Data.set_liveness(m_liveness);
		m_owner->BroadCastToGS(ntf);
	}

	SetDirty();
}

void DragonGuildLiveness::FillLivenessRecord(KKSG::GetPartnerLivenessRes& res)
{
	UINT32 now = GameTime::GetTime();
	for (auto it = m_livenessItem.begin(); it != m_livenessItem.end(); ++it)
	{
		DragonGuildLivenessRecord& item = *it;
		CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(item.roleId);
		if (su == NULL)
		{
			continue;
		}
		KKSG::PartnerLivenessItem* record = res.add_record();
		record->set_roleid(item.roleId);
		record->set_name(su->GetName());
		record->set_actid(item.actId);
		record->set_value(item.value);
		UINT32 passTime = now > item.time ? (now - item.time) : 0;
		record->set_time(passTime);
	}
}

UINT64 DragonGuildLiveness::_FindNextLeader()
{
	std::vector<UINT64> vecRoles;
	std::map<UINT32, std::multimap<UINT32, UINT64>> memMap;
	m_owner->GetDragonGuildMemberIds(vecRoles);
	for (int i = 0; i < vecRoles.size(); ++ i)
	{
		DragonGuildMember* m = m_owner->GetMember(vecRoles[i]);
		if (m && m->position != DGPOS_LEADER)
		{
			std::multimap<UINT32, UINT64>& data = memMap[m->position];
			data.insert(std::make_pair(m->activity, vecRoles[i]));
		}
	}

	foreach (j in memMap)
	{
		for (std::multimap<UINT32, UINT64>::reverse_iterator iter = j->second.rbegin(); iter != j->second.rend(); ++iter)
		{
			CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(iter->second); 
			if (pRoleSum != NULL && pRoleSum->GetLastLogout() < GetGlobalConfig().DragonGuildActLeaDay * DAY_SECONDS)
			{
				return iter->second;
			}
		}
	}
	
	return 0;
}

void DragonGuildLiveness::_CheckLeaderActivity()
{
	CRoleSummary* pOldSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_owner->GetLeaderId());
	if (pOldSum != NULL && pOldSum->GetLastLogout() >= GetGlobalConfig().DragonGuildActLeaDay * DAY_SECONDS)
	{
		UINT64 nextLeaderId = _FindNextLeader();
		CRoleSummary* pNewSum = CRoleSummaryMgr::Instance()->GetRoleSummary(nextLeaderId);
		if (pNewSum != NULL)
		{
			std::vector<ItemDesc> itemVec;
			const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_DragonGuildLeaderDown);
			MailData mail(conf->m_title, conf->m_content, itemVec, MAIL_EXPIRE_TIME);
			CMailMgr::Instance()->SendMail(pOldSum->GetID(), mail);

			const MailConf* confnew = MailConfig::Instance()->GetMailConf(MailConf_DragonGuildLeaderUp);
			MailData mailnew(confnew->m_title, confnew->m_content, itemVec, MAIL_EXPIRE_TIME);
			CMailMgr::Instance()->SendMail(nextLeaderId, mailnew);

			DragonGuildMember* m = m_owner->GetMember(nextLeaderId);
			m_owner->ChangeMemberPosition(m, DGPOS_LEADER);
		}
	}
}

void DragonGuildLiveness::DoRoleLeave(UINT64 roleId)
{
	auto it = m_roleActivityMap.find(roleId);
	if (it == m_roleActivityMap.end())
	{
		return ;
	}

	m_roleActivityMap.erase(roleId);
	// m_liveness -= it->second; 

	SetDirty();
}

UINT32 DragonGuildLiveness::GetRoleActivity(UINT64 roleId)
{
	auto it = m_roleActivityMap.find(roleId);
	if (it == m_roleActivityMap.end())
	{
		return 0;
	}
	return m_roleActivityMap[roleId];
}