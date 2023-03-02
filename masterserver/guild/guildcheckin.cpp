#include "pch.h"
#include "guildcheckin.h"
#include "util/XCommon.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/role.h"
//#include "gamelogic/item.h"
#include "role/rolemanager.h"
#include "time.h"
//#include "guild/guildrecord.h"
#include "util/timespecificmgr.h"
//#include "gamelogic/systemiddef.h"
#include "guild/ptcg2c_guildcheckinboxntf.h"
#include "guild/ptcm2g_notifyguildcheckinprocess.h"
#include "guild/ptcm2g_synguildcheckinhint.h"
#include "util/gametime.h"
#include "foreach.h"

CGuildCheckin::CGuildCheckin()
{
	m_updateday = 0;
	m_process = 0;
	m_count = 0;
}

CGuildCheckin::~CGuildCheckin()
{

}

bool CGuildCheckin::IsChanged()
{
	return m_switch.TestAndReset();
}

bool CGuildCheckin::Load(const KKSG::GuildAllDBData & guilddata)
{
	if (guilddata.has_checkin() && !guilddata.checkin().empty())
	{
		KKSG::GuildCheckin checkin;
		checkin.ParseFromString(guilddata.checkin());

		m_updateday = checkin.updateday();
		m_process = checkin.process();
		m_count = checkin.count();

		for (int i = 0; i < checkin.roleid_size(); ++i)
		{
			GuildCheckinRecord record;
			record.roleid = checkin.roleid(i);
			record.type = checkin.type(i);
			record.timestamp = checkin.timestamp(i);
			m_records.push_back(record);
		}
		
		Update();
	}
	return true;
}

void CGuildCheckin::Save(KKSG::GuildAllDBData & guilddata)
{
	guilddata.mutable_checkin()->assign(ToBinary());
}

const std::string CGuildCheckin::ToBinary()
{
	KKSG::GuildCheckin checkin;
	checkin.set_updateday(m_updateday);
	checkin.set_process(m_process);
	checkin.set_count(m_count);
	
	for (auto i = m_records.begin(); i != m_records.end(); ++i)
	{
		checkin.add_roleid(i->roleid);
		checkin.add_type(i->type);
		checkin.add_timestamp(i->timestamp);
	}
	
	return checkin.SerializeAsString();
}

void CGuildCheckin::Update()
{
	UINT32 day = XCommon::GetTodayUnique();
	if (day != m_updateday)
	{
		m_count = 0;
		m_process = 0;
		m_records.clear();
		m_updateday = day;

		m_switch.Set();
	}
}

void CGuildCheckin::Checkin(UINT32 process, UINT32 type, UINT64 roleid)
{
	m_process += process;
	m_count += 1;

	GuildCheckinRecord record;
	record.roleid = roleid;
	record.type = type;
	record.timestamp = (UINT32)time(NULL);

	if (m_records.size() < 100)
	{
		m_records.push_back(record);
	}

	m_switch.Set();
}

const std::vector<GuildCheckinRecord>& CGuildCheckin::GetRecords()
{
	return m_records;
}

/////////////////////////////////////////////////////////////////////
INSTANCE_SINGLETON(GuildCheckinMgr);

GuildCheckinMgr::GuildCheckinMgr()
{

}

GuildCheckinMgr::~GuildCheckinMgr()
{

}

bool GuildCheckinMgr::Init()
{
	if (!m_table.LoadFile("table/GuildCheckin.txt"))
	{
		SSWarn<<"load file table/GuildCheckin.txt failed"<<END;
		return false;
	}
	if (!m_boxtable.LoadFile("table/GuildCheckinBox.txt"))
	{
		SSWarn<<"load file table/GuildCheckinBox.txt failed"<<END;
		return false;
	}

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	m_timer = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&GuildCheckinMgr::OnTimer, this));

	return true;
}

void GuildCheckinMgr::Uninit()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_timer);
}

bool GuildCheckinMgr::OnTimer()
{
	const std::unordered_map<UINT64, Guild*>& guilds = CGuildMgr::Instance()->GetGuilds();
	for (auto i = guilds.begin(); i != guilds.end(); ++i)
	{
		Guild* guild = i->second;
		if (NULL != guild)
		{
			NotifyProcess(NULL, guild);
		}
	}
	return true;
}

void GuildCheckinMgr::NotifyProcess(CRole* pRole, Guild* guild)
{
	PtcM2G_NotifyGuildCheckinProcess msg;
	if (pRole != NULL)
	{
		Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
		if (guild == NULL)
		{
			return;
		}

		CGuildCheckin* checkin = guild->Get<CGuildCheckin>();
		checkin->Update();
		msg.m_Data.set_process(checkin->GetPorcess());
		msg.m_Data.add_roles(pRole->GetID());
		pRole->SendMsgToGS(msg);
	}
	else
	{
		CGuildCheckin* checkin = guild->Get<CGuildCheckin>();
		checkin->Update();
		msg.m_Data.set_process(checkin->GetPorcess());

		const std::map<UINT64, GuildMember>& members = guild->GetGuildMember();
		for (auto i = members.begin(); i != members.end(); ++i)
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(i->first);
			if (pRole == NULL)
			{
				continue;
			}

			msg.m_Data.add_roles(i->first);
			pRole->SendMsgToGS(msg);
		}
	}


	/*
	if (NULL != role)
	{
		Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
		if (guild == NULL)
		{
			return;
		}
		CGuildCheckin* checkin = guild->Get<CGuildCheckin>();
		checkin->Update();
		UINT32 process = checkin->GetPorcess();

		PtcG2C_GuildCheckinBoxNtf ntf;
		ntf.m_Data.set_processbar(process);
		CGuildRecord* record = role->Get<CGuildRecord>();
		record->Update();
		ntf.m_Data.set_boxmask(record->GetBoxMask());
		role->Send(ntf);
	}
	else
	{
		CGuildCheckin* checkin = guild->Get<CGuildCheckin>();
		checkin->Update();
		UINT32 process = checkin->GetPorcess();

		PtcG2C_GuildCheckinBoxNtf ntf;
		ntf.m_Data.set_processbar(process);
		const std::vector<GuildMember>& members = guild->GetGuildMember();
		for (auto i = members.begin(); i != members.end(); ++i)
		{
			Role* role = RoleManager::Instance()->FindByRoleID(i->qwRoleID);
			if (NULL != role)
			{
				CGuildRecord* record = role->Get<CGuildRecord>();
				record->Update();
				ntf.m_Data.set_boxmask(record->GetBoxMask());
				role->Send(ntf);
			}
		}
	}
	*/
}

ItemDesc GuildCheckinMgr::GetConsume(UINT32 type)
{
	ItemDesc item;
	GuildCheckinTable::RowData* rowdata = m_table.GetBytype(type);
	if (NULL == rowdata)
	{
		return item;
	}
	item.itemID = rowdata->consume[0];
	item.itemCount = rowdata->consume[1];
	return item;
}

ItemDesc GuildCheckinMgr::GetReward(UINT32 type)
{
	ItemDesc item;
	GuildCheckinTable::RowData* rowdata = m_table.GetBytype(type);
	if (NULL == rowdata)
	{
		return item;
	}
	item.itemID = rowdata->reward[0];
	item.itemCount = rowdata->reward[1];
	return item;
}

UINT32 GuildCheckinMgr::GetPorcess(UINT32 type)
{
	GuildCheckinTable::RowData* rowdata = m_table.GetBytype(type);
	if (NULL == rowdata)
	{
		return 0;
	}
	return rowdata->process;
}

std::vector<ItemDesc> GuildCheckinMgr::GetBoxReward(UINT32 index, UINT32 process)
{
	std::vector<ItemDesc> items;
	if (m_boxtable.Table.size() <= index)
	{
		return items;
	}
	GuildCheckinBoxTable::RowData* rowdata = m_boxtable.Table[index];
	if (rowdata->process > process)
	{
		return items;
	}
	for (auto i = rowdata->reward.begin(); i != rowdata->reward.end(); ++i)
	{
		ItemDesc item;
		item.itemID = i->seq[0];
		item.itemCount = i->seq[1];
		items.push_back(item);
	}
	return items;
}

void GuildCheckinMgr::GetGuildReward(UINT32 type, std::map<UINT32, UINT32>& guildReward)
{
	GuildCheckinTable::RowData* rowdata = m_table.GetBytype(type);
	if (NULL == rowdata)
	{
		return ;
	}
	foreach (i in rowdata->guildreward)
	{
		guildReward[i->seq[0]] = i->seq[1];
	}
}
/*
UINT32 GuildCheckinMgr::GetExp(UINT32 type)
{
	GuildCheckinTable::RowData* rowdata = m_table.GetBytype(type);
	if (NULL == rowdata)
	{
		return 0;
	}
	return rowdata->exp;
}
*/

void CGuildCheckin::HintNotify(CRole* role)
{
	/*
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (NULL == guild)
	{
		return;
	}
	*/
	PtcM2G_SynGuildCheckinHint msg;
	msg.m_Data.set_roleid(role->GetID());
	role->SendMsgToGS(msg);
	
	/*
	if (!role->Get<CGuildRecord>()->IsCheckin())
	{
		role->HintNotify(GuildCheckinId, false);
	}
	*/
}
