#include "pch.h"
#include "gamelogic/military_rank_manager.h"
#include "unit/rolemanager.h"
#include "gamelogic/military_rank_record.h"
#include "militaryrank/rpcg2m_getmilitaryrank.h"
#include "unit/roleoutlook.h"
#include "rolesummary/rolesummarybase.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"
#include "define/tlogenum.h"


INSTANCE_SINGLETON(MilitaryRankManager)

	MilitaryRankManager::MilitaryRankManager()
{
	ClearFile();
}

MilitaryRankManager::~MilitaryRankManager()
{

}

bool MilitaryRankManager::Init()
{
	return  LoadFile();
}
bool MilitaryRankManager::Reload()
{
	Uninit();	
	return Init();
}
void MilitaryRankManager::Uninit()
{
	ClearFile();
}

bool MilitaryRankManager::CheckFile()
{
	MilitaryRankByExploit tmprank;
	if(!tmprank.LoadFile("table/MilitaryRankByExploit.txt"))
	{
		SSWarn << " load MilitaryRankByExploit.txt failed " << END;
		return false;
	}
	GetMilitaryExploitConfig  tmpExploit;
	if(!tmpExploit.LoadFile("table/GetMilitaryExploitConfig.txt"))
	{
		SSWarn << " load GetMilitaryExploitConfig.txt failed " << END;
		return false;
	}
	return true;
}
bool MilitaryRankManager::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	if(!military_rank_.LoadFile("table/MilitaryRankByExploit.txt"))
	{
		SSWarn << " load MilitaryRankByExploit.txt failed " << END;
		return false;
	}
	GetMilitaryExploitConfig  military_expoit;
	if(!military_expoit.LoadFile("table/GetMilitaryExploitConfig.txt"))
	{
		SSWarn << " load GetMilitaryExploitConfig.txt failed " << END;
		return false;
	}

	for (auto it = military_expoit.Table.begin(); it != military_expoit.Table.end(); ++it)
	{
		ExploitValue exploi_value((*it)->MilitaryExploit,(*it)->ReduceMilitaryExploit); 
		auto exist_it = battle_type_military_.find((*it)->BattleType);
		if (exist_it != battle_type_military_.end())
		{
			exist_it->second.insert(std::make_pair((*it)->MilitaryRank,exploi_value));
		}
		else
		{
			MilitaryValue military_value;
			military_value.insert(std::make_pair((*it)->MilitaryRank,exploi_value));
			battle_type_military_.insert(std::make_pair((*it)->BattleType,military_value));
		}
	}

	return true;
}


void MilitaryRankManager::ClearFile()
{
	military_rank_.Clear();
	battle_type_military_.clear();	
}

// 当前战斗类型下，此军阶赢一场的军功
UINT32 MilitaryRankManager::GetMilitaryExploit(KKSG::SceneType scene_type,UINT32 military_rank)
{
	auto military_it = battle_type_military_.find(scene_type);
	if (military_it == battle_type_military_.end())
	{
		return 0;
	}

	auto exploit_it = military_it->second.find(military_rank);
	if (exploit_it == military_it->second.end())
	{
		return 0;
	}

	return  exploit_it->second.first;
}

UINT32 MilitaryRankManager::GetReduceMilitaryExploit(KKSG::SceneType scene_type,UINT32 military_rank)
{
	auto military_it = battle_type_military_.find(scene_type);
	if (military_it == battle_type_military_.end())
	{
		return 0;
	}

	auto exploit_it = military_it->second.find(military_rank);
	if (exploit_it == military_it->second.end())
	{
		return 0;
	}

	return  exploit_it->second.second;
}

UINT32 MilitaryRankManager::GetMilitaryRankByExploit(const UINT32 military_exploit)
{
	// 防止军功太多，超越能评特殊头衔的军功
	UINT32 temp_rank = 0;
	if (military_exploit == 0)
	{
		return temp_rank;
	}
	for (auto it = military_rank_.Table.begin(); it != military_rank_.Table.end(); ++it)
	{
		if (military_exploit >=(*it)->MilitaryExploit.seq[0]  && military_exploit <= (*it)->MilitaryExploit.seq[1] )
		{
			return (*it)->MilitaryRank;
		}
		if (military_exploit >=(*it)->MilitaryExploit.seq[0] && (*it)->MilitaryExploit.seq[0] > 1)
		{
			temp_rank = (*it)->MilitaryRank;
		}
	}

	return temp_rank;
}

void  MilitaryRankManager::GetMilitaryRankNameByRank(const UINT32 military_rank,std::string& name)
{
	for (auto it = military_rank_.Table.begin(); it != military_rank_.Table.end(); ++it)
	{
		if (military_rank == (*it)->MilitaryRank)
		{
			name = (*it)->Name;
		}
	}	
}
void MilitaryRankManager::GetMilitaryRankByRank(const UINT32 military_rank, std::vector<ItemDesc>& items)
{
	for (auto it = military_rank_.Table.begin(); it != military_rank_.Table.end(); ++it)
	{
		if (military_rank ==(*it)->MilitaryRank)
		{
			auto reward_it = (*it)->Reward.begin();
			auto reward_it_end = (*it)->Reward.end();
			for ( ; reward_it != reward_it_end; ++reward_it)
			{
				ItemDesc itemdesc;
				itemdesc.itemID = reward_it->seq[0];
				itemdesc.itemCount = reward_it->seq[1];
				itemdesc.isbind = false;
				items.push_back(itemdesc);
			}
		}
	}
}

// 更新军阶
void MilitaryRankManager::UpDateMilitaryRankMS(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	UINT32 rank = role->Get<MilitaryRankRecord>()->GetMilitaryRank();	
	bool need_update = needUpdateRank(rank);
	if (need_update)
	{
		RpcG2M_GetMilitaryRank* msg = RpcG2M_GetMilitaryRank::CreateRpc();
		msg->m_oArg.set_role_id(role->GetRoleID());
		role->SendToMS(*msg);
	}
}

bool MilitaryRankManager::needUpdateRank(UINT32 rank)
{
	bool need_update = false;
	auto it = GetGlobalConfig().MilitaryRank.begin();
	auto it_end = GetGlobalConfig().MilitaryRank.end();
	for (; it != it_end; ++it)
	{
		if (it->seq[0] == rank)
		{
			need_update = true;
			break;
		}
	}

	return need_update;
}

void MilitaryRankManager::GiveRewardForFirstAchieveRank(UINT64 role_id,UINT32 military_rank)
{
	std::vector<ItemDesc> items;
	GetMilitaryRankByRank(military_rank,items);
	std::string name;
	GetMilitaryRankNameByRank(military_rank,name);
	const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_Military_UpRank);
	if (NULL != conf && items.size() > 0 && !name.empty())
	{
		MailSender sender;
		std::string mailcontent = MailConfig::Instance()->ReplaceString("%s", name, conf->m_content);
		sender.SetContent(Mail_System, conf->m_title,mailcontent);
		sender.SetReason(ItemFlow_FirstAchieveRank);
		sender.SetItems(items);
		sender.Send(role_id);
	}
}