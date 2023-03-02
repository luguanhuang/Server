#include "pch.h"
#include "guild/guilddonate.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "task/rpcm2g_querydonateinfo.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"

void MemberAskInfo::FromKKSG(const KKSG::GuildMemberAskItem& info)
{
	id = info.id();
	roleId = info.roleid();
	publishTime = info.publishtime();
	itemId = info.itemid();
	needCount = info.needcount();
	getCount = info.getcount();
	level = info.level();
	quality = info.quality();
	taskId = info.taskid();
	tasktype = info.tasktype();
	itemType = info.itemtype();
	itemQuality = info.itemquality();
	index = info.index();
}

void MemberAskInfo::ToKKSG(KKSG::GuildMemberAskItem& info)
{
	info.set_id(id);
	info.set_roleid(roleId);
	info.set_publishtime(publishTime);
	info.set_itemid(itemId);
	info.set_needcount(needCount);
	info.set_getcount(getCount);
	info.set_level(level);
	info.set_quality(quality);
	info.set_taskid(taskId);
	info.set_tasktype(tasktype);
	info.set_itemtype(itemType);
	info.set_itemquality(itemQuality);
	info.set_index(index);
}

void MemberDonateInfo::FromKKSG(const KKSG::GuildMemberDonateInfo& info)
{
	roleId = info.roleid();
	todayDonateCount = info.todaydonatecount();
	totalDonateCount = info.totaldonatecount();
	lastDonateTime = info.lasttime();
}

void MemberDonateInfo::ToKKSG(KKSG::GuildMemberDonateInfo& info)
{
	info.set_roleid(roleId);
	info.set_todaydonatecount(todayDonateCount);
	info.set_totaldonatecount(totalDonateCount);
	info.set_lasttime(lastDonateTime);
}

GuildDonate::GuildDonate(Guild* guild)
:m_guild(guild)
,m_updateTime(0)
,m_newId(0)
{

}

GuildDonate::~GuildDonate()
{

}

bool GuildDonate::IsChanged()
{
	return m_dirty.TestAndReset();
}

bool GuildDonate::Load(const KKSG::GuildAllDBData & guilddata)
{
	if (!guilddata.has_guilddonate())
	{
		return false;
	}
	if (guilddata.guilddonate().empty())
	{
		return false;
	}

	KKSG::GuildDonateSave guilddonate;
	if (!guilddonate.ParseFromString(guilddata.guilddonate()))
	{
		LogWarn("Parse guild donate failed");
		return false;
	}

	m_updateTime = guilddonate.updatetime();

	for (int i = 0; i < guilddonate.askinfo_size(); ++i)
	{
		MemberAskInfo askInfo;
		const KKSG::GuildMemberAskItem& info = guilddonate.askinfo(i);
		askInfo.FromKKSG(info);
		m_askInfos.push_back(askInfo);
		if (askInfo.id > m_newId)
		{
			m_newId = askInfo.id;
		}
	}

	for (int i = 0; i < guilddonate.donateinfo_size(); ++i)
	{
		MemberDonateInfo donateInfo;
		const KKSG::GuildMemberDonateInfo& info = guilddonate.donateinfo(i);
		donateInfo.FromKKSG(info);
		m_donateRecord[donateInfo.roleId] = donateInfo;

		// 排名
		UpdateDonateInfo(donateInfo.roleId, donateInfo.todayDonateCount, donateInfo.totalDonateCount, true);
	}


	UINT32 now = GameTime::GetTime();
	if (!XCommon::IsDaySame(m_updateTime, now, true))
	{
		OnDayPass();
	}
	return true;
}

void GuildDonate::Save(KKSG::GuildAllDBData& guilddata)
{
	guilddata.mutable_guilddonate()->assign(ToBinary());
}

const std::string GuildDonate::ToBinary()
{
	KKSG::GuildDonateSave guilddonate;
	guilddonate.set_updatetime(m_updateTime);

	for (auto it = m_askInfos.begin(); it != m_askInfos.end(); ++it)
	{
		MemberAskInfo& askInfo = *it;
		KKSG::GuildMemberAskItem* info = guilddonate.add_askinfo();
		askInfo.ToKKSG(*info);
	}

	for (auto it = m_donateRecord.begin(); it != m_donateRecord.end(); ++it)
	{
		MemberDonateInfo& donateInfo = it->second;
		KKSG::GuildMemberDonateInfo* info = guilddonate.add_donateinfo();
		donateInfo.ToKKSG(*info);
	}

	return guilddonate.SerializeAsString();
}

void GuildDonate::OnDayPass()
{
	// 每日重置
	UINT32 now = GameTime::GetTime();
	if (!GameTime::IsInSameWeek(now, m_updateTime, true))
	{
		m_askInfos.clear();
		m_newId = 0;
	}
	else
	{
		for (auto it = m_askInfos.begin(); it != m_askInfos.end(); )
		{
			if (it->tasktype == KKSG::PeriodTaskType_Daily)
			{
				it = m_askInfos.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	for (auto it = m_donateRecord.begin(); it != m_donateRecord.end(); ++it)
	{
		it->second.todayDonateCount = 0;
	}
	m_updateTime = GameTime::GetTime();
	m_dirty.Set();
}

void GuildDonate::OnAddMember(Guild *pGuild, GuildMember *pMember)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(pMember->qwRoleID);
	if (!pRole)
	{
		return;
	}
	RpcM2G_QueryDonateInfo* rpc = RpcM2G_QueryDonateInfo::CreateRpc();
	rpc->m_sessionID = pRole->GetSessionID();
	pRole->SendMsgToGS(*rpc);
}

void GuildDonate::OnRemoveMember(Guild *pGuild, UINT64 qwRoleID)
{
	for (auto it = m_askInfos.begin(); it != m_askInfos.end(); )
	{
		if (it->roleId == qwRoleID)
		{
			it = m_askInfos.erase(it);
		}
		else
		{
			it++;
		}
	}

	m_donateRecord.erase(qwRoleID);
//	m_todayRank.Erase(qwRoleID);
//	m_totalRank.Erase(qwRoleID);
	m_dirty.Set();
}

UINT32 GuildDonate::AddAskItemInfo(MemberAskInfo& info)
{
	info.id = ++m_newId;
	m_askInfos.push_back(info);
	m_dirty.Set();
	return info.id;
}

MemberAskInfo* GuildDonate::GetAskInfo(UINT32 id)
{
	for (auto it = m_askInfos.begin(); it != m_askInfos.end(); ++it)
	{
		MemberAskInfo& info = *it;
		if (id == info.id)
		{
			return &info;
		}
	}
	return NULL;
}

MemberAskInfo* GuildDonate::GetAskInfo(UINT64 roleId, UINT32 index, UINT32 taskId)
{
	for (auto it = m_askInfos.begin(); it != m_askInfos.end(); ++it)
	{
		MemberAskInfo& info = *it;
		if (info.roleId == roleId && info.index == index && (taskId == 0 || taskId == info.taskId))
		{
			return &info;
		}
	}
	return NULL;
}

void GuildDonate::RemoveAskInfo(UINT32 id)
{
	for (auto it = m_askInfos.begin(); it != m_askInfos.end(); ++it)
	{
		MemberAskInfo& info = *it;
		if (info.id == id)
		{
			m_askInfos.erase(it);
			m_dirty.Set();
			break;
		}
	}
}

void GuildDonate::RemoveAskInfo(UINT64 roleId, UINT32 taskId)
{
	for (auto it = m_askInfos.begin(); it != m_askInfos.end(); ++it)
	{
		MemberAskInfo& info = *it;
		if (info.roleId == roleId && info.taskId == taskId)
		{
			m_askInfos.erase(it);
			m_dirty.Set();
			break;
		}
	}
}

void GuildDonate::RemoveWeekAskInfoByRoleId(UINT64 roleId)
{
	for (auto it = m_askInfos.begin(); it != m_askInfos.end();)
	{
		MemberAskInfo& info = *it;
		if (info.roleId == roleId)
		{
			it = m_askInfos.erase(it);
			m_dirty.Set();
		}
		else
		{
			it++;
		}
	}
}

void GuildDonate::UpdateDonateInfo(UINT64 roleId, UINT32 todayValue, UINT32 totalValue, bool init)
{
	if (todayValue == 0 && totalValue == 0)
	{
		return;
	}

	UINT32 now = GameTime::GetTime();

	MemberDonateInfo& info = m_donateRecord[roleId];
	info.roleId = roleId;
	info.todayDonateCount = todayValue;
	info.totalDonateCount = totalValue;
	info.lastDonateTime = now;

	UpdateTodayRank(roleId, todayValue, now);
	UpdateTotalRank(roleId, totalValue, now);
	if (!init)
	{
		m_dirty.Set();
	}
}

void GuildDonate::UpdateTodayRank(UINT64 roleId, UINT32 value, UINT32 time)
{
	/*
	DonateRankInfo info;
	info.roleId = roleId;
	info.value = value;
	info.time = time;
	m_todayRank.Update(info, roleId);
	*/
}

void GuildDonate::UpdateTotalRank(UINT64 roleId, UINT32 value, UINT32 time)
{
	/*
	DonateRankInfo info;
	info.roleId = roleId;
	info.value = value;
	info.time = time;
	m_totalRank.Update(info, roleId);
	*/
}

UINT32 GuildDonate::GetTodayDonateNum(UINT64 roleId)
{
	auto it = m_donateRecord.find(roleId);
	if (it != m_donateRecord.end())
	{
		return it->second.todayDonateCount;
	}
	return 0;
}

void GuildDonate::FillMemberAskInfo(MemberAskInfo& info, KKSG::GuildMemberAskInfo& data)
{
	info.ToKKSG(*data.mutable_item());
	CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(info.roleId);
	if (pRoleSum != NULL)
	{
		data.set_name(pRoleSum->GetName());
		data.set_profession(pRoleSum->GetProfession());
	}
}

void GuildDonate::FillMemberDonateInfo(MemberDonateInfo& info, KKSG::GuildMemberDonateRankItem& data)
{
	data.set_roleid(info.roleId);
	data.set_todaycount(info.todayDonateCount);
	data.set_totalcount(info.totalDonateCount);
	data.set_lasttime(info.lastDonateTime);
	CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(info.roleId);
	if (pRoleSum != NULL)
	{
		data.set_name(pRoleSum->GetName());
		data.set_level(pRoleSum->GetLevel());
		data.set_profession(pRoleSum->GetProfession());
	}
}
