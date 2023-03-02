#include "pch.h"
#include "guildhistory.h"
#include "role/rolesummarymgr.h"
#include "pb/project.pb.h"
#include "foreach.h"

const static int HistorySize = 20;

GuildHistory::GuildHistory(Guild *pGuild)
{
	m_pGuild = pGuild;
	m_CleanCount = 0;
}

GuildHistory::~GuildHistory()
{

}

bool GuildHistory::IsChanged()
{
	return m_isModified.TestAndReset();
}

bool GuildHistory::Load(const KKSG::GuildAllDBData & guilddata)
{
	KKSG::GHisRecordList records;
	if (!records.ParseFromString(guilddata.history()))
	{
		return false;
	}

	for (int i = 0; i < records.records_size(); ++i)
	{
		GuildMemberLog record;
		const KKSG::GHisRecord &Record = records.records(i);
		record.roleid = Record.roleid();
		record.rolename = Record.rolename();
		record.type = Record.type();
		record.position = Record.position();
		record.time = Record.time();
		m_MemberLog.push_back(record);
	}

	return true;
}

void GuildHistory::Save(KKSG::GuildAllDBData & guilddata)
{
	guilddata.mutable_history()->assign(ToBinary());
}

const std::string GuildHistory::ToBinary()
{
	KKSG::GHisRecordList records;
	int j = 0;
	for (auto i = m_MemberLog.begin(); i != m_MemberLog.end(); ++i, ++j)
	{
		if (j >= HistorySize)
		{
			break;
		}

		KKSG::GHisRecord *pRecord = records.add_records();
		pRecord->set_roleid(i->roleid);
		pRecord->set_rolename(i->rolename);
		pRecord->set_position(i->position);
		pRecord->set_type(i->type);
		pRecord->set_time((UINT32)i->time);
	}

	return records.SerializeAsString();
}

void GuildHistory::OnAddMember(Guild *pGuild, GuildMember *pMember)
{
	GuildMemberLog record;
	record.roleid = pMember->qwRoleID;
	record.rolename = CRoleSummaryMgr::Instance()->GetRoleName(pMember->qwRoleID);
	record.type = KKSG::GuildLog_Join;
	record.position = pMember->position;
	record.time = time(NULL);
	m_MemberLog.push_front(record);
	++m_CleanCount;
	Shrink();
	m_isModified.Set();
}

void GuildHistory::OnRemoveMember(Guild *pGuild, UINT64 qwRoleID)
{
	GuildMemberLog record;
	record.roleid = qwRoleID;
	record.rolename = CRoleSummaryMgr::Instance()->GetRoleName(qwRoleID);
	record.type = KKSG::GuildLog_Leave;
	record.position = GPOS_MAX;
	record.time = time(NULL);
	m_MemberLog.push_front(record);
	++m_CleanCount;
	Shrink();
	m_isModified.Set();
}

void GuildHistory::OnMemberPosChanged(Guild *pGuild, GuildMember *pMember)
{
	GuildMemberLog record;
	record.roleid = pMember->qwRoleID;
	record.rolename = CRoleSummaryMgr::Instance()->GetRoleName(pMember->qwRoleID);
	record.type = KKSG::GuildLog_ChangePosition;
	record.position = pMember->position;
	record.time = time(NULL);
	m_MemberLog.push_front(record);
	++m_CleanCount;
	Shrink();
	m_isModified.Set();
}

void GuildHistory::OnMembergGuildBossDps(Guild *pGuild, GuildMember *pMember)
{
	GuildMemberLog record;
	record.roleid = pMember->qwRoleID;
	record.rolename = CRoleSummaryMgr::Instance()->GetRoleName(pMember->qwRoleID);
	record.type = KKSG::GuildLog_BossDps;
	record.position = pMember->position;
	record.time = time(NULL);
	m_MemberLog.push_front(record);
	++m_CleanCount;
	Shrink();
	m_isModified.Set();
}

void GuildHistory::Shrink()
{
	if (m_CleanCount > HistorySize)
	{
		auto it = m_MemberLog.begin();
		std::advance(it, HistorySize);
		m_MemberLog.erase(it, m_MemberLog.end());
		m_CleanCount = 0;
	}
}

void GuildHistory::OnAddMemberFromDB(Guild *pGuild, GuildMember *pMember)
{
}

void GuildHistory::Foreach(int type, GuildLogCB cb, void *arg)
{
	if (type == KKSG::GLog_Member)
	{
		int j = 0;
		foreach(i in m_MemberLog)
		{
			cb(&*i, arg);

			if (++j >= HistorySize)
			{
				break;
			}
		}
	}
}
