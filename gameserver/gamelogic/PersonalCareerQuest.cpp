#include "pch.h"
#include "pb/project.pb.h"
#include "pb/enum.pb.h"
#include "gamelogic/PersonalCareerQuest.h"
#include "gamelogic/qqviprecord.h"
#include "gamelogic/pkrecord.h"
#include "gamelogic/payv2Record.h"
#include "util/gametime.h"
#include "gamelogic/rolemisc.h"
#include <time.h>
#include "util/gametime.h"
#include "util/XCommon.h"

INSTANCE_SINGLETON(PersonalCareerQuest);

PersonalCareerQuest::PersonalCareerQuest()
{
	ClearFile();
}

PersonalCareerQuest::~PersonalCareerQuest()
{

}

bool PersonalCareerQuest::Init()
{
	return  LoadFile();
}
bool PersonalCareerQuest::Reload()
{
	Uninit();	
	return Init();
}
void PersonalCareerQuest::Uninit()
{
	ClearFile();
}
bool PersonalCareerQuest::CheckFile()
{
	TrophyInfo tmpinfo;
	if(!tmpinfo.LoadFile("table/TrophyInfo.txt"))
	{
		SSWarn << " load TrophyInfo.txt failed " << END;
		return false;
	}
	TrophyReward  tmpreward;
	if(!tmpreward.LoadFile("table/TrophyReward.txt"))
	{
		SSWarn << " load TrophyReward.txt failed " << END;
		return false;
	}

	return true;
}
bool PersonalCareerQuest::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	trophy_info_.LoadFile("table/TrophyInfo.txt");
	trophy_reward_.LoadFile("table/TrophyReward.txt");

	return true;
}
void PersonalCareerQuest::ClearFile()
{
	trophy_info_.Clear();	
	trophy_reward_.Clear();
	return ;
}

void PersonalCareerQuest::GetHomePage(
	KKSG::PersonalCareerTotalInfoRes& total_info ,
	Role& role,
	KKSG::PersonalHomePage& home_page)
{
	// 0.普通，1，vip，2.svip
	if (role.Get<QQVipRecord>()->IsVip())
	{
		home_page.set_qq_vip(1);
	}
	else if (role.Get<QQVipRecord>()->IsSVip())
	{
		home_page.set_qq_vip(2);
	}

	UINT32 paymember_id = 0;
	CPayV2Record* record = role.Get<CPayV2Record>();
	for (UINT32 i = 0; i < record->memberInfo.size(); i ++)
	{
		if (record->memberInfo[i].expiretime() > INT32(GameTime::GetTime()))
		{
			paymember_id |= (1 << record->memberInfo[i].id());
		}
	}
	home_page.set_paymember_id(paymember_id);
	home_page.set_play_name(role.GetName());
	home_page.set_uid(role.GetNickId());	
	RoleMisc* misc = role.Get<RoleMisc>();
	if (NULL != record)
	{
		home_page.set_declaration(misc->GetDeclaration());
	}
	home_page.set_server_name(total_info.server_name());
	home_page.set_level(role.GetLevel());
	home_page.set_profession_id(role.GetProfession());
	UINT64 guild_id = GuildSimpleMgr::Instance()->GetGuildId(role.GetID());
	string guild_name = GuildSimpleMgr::Instance()->GetGuildName(guild_id);
	home_page.set_guild_name(guild_name);
	home_page.set_power(role.GetAttr(TOTAL_POWERPOINT));
	home_page.set_continue_login_time(total_info.continue_login_days());
	home_page.set_online_time(role.GetOnlineTime());
	for (int i = 0; i < total_info.career_data_size(); ++i)
	{
		if (total_info.career_data(i).type() == KKSG::CARRER_DATA_LEVEL)
		{
			auto level_filter = GetGlobalConfig().CareerLevelFilter.find(total_info.career_data(i).para1());
			if (level_filter == GetGlobalConfig().CareerLevelFilter.end())
			{
				continue;
			}			
		}

		KKSG::CareerData* career_data = home_page.add_carrer_data();
		career_data->set_type(total_info.career_data(i).type());
		career_data->set_para1(total_info.career_data(i).para1());
		career_data->set_time(total_info.career_data(i).time());
	}

	KKSG::CareerData* create_time = home_page.add_carrer_data();
	create_time->set_type(KKSG::CARRER_DATA_CREATEROLE);
	create_time->set_time(GetInitRoleTime((UINT32)role.GetCreateTime()));
}


void PersonalCareerQuest::GetPVPInformation(
	KKSG::PersonalCareerTotalInfoRes& total_info ,
	Role& role,
	KKSG::PVPInformation& pvp_info)
{
	KKSG::PKInformation* pk_info = pvp_info.mutable_pk_info();
	pk_info->set_pk_rank(total_info.pk_rank());
	pk_info->set_pk_profession_rank(total_info.profession_rank());

	CPkRecord* record = role.Get<CPkRecord>();
	record->Update();
	KKSG::PkRecord* recinfo = pk_info->mutable_pk_record();
	record->ToKKSG(recinfo);

	pk_info->set_pk_all_roles_rate(total_info.all_roles());
	pk_info->set_pk_warrior_rate(total_info.warrior());
	pk_info->set_pk_archer_rate(total_info.archer());
	pk_info->set_pk_master_rate(total_info.master());
	pk_info->set_pk_minister_rate(total_info.minister());
	pk_info->set_pk_max_score(total_info.max_score_history());

}


UINT32 PersonalCareerQuest::GetInitRoleTime(UINT32 create_time)
{
	UINT32 year = 0;
	UINT32 month = 0;
	UINT32 day = 0;

	XCommon::GetDate(create_time,year,month,day);
	string stryear = ToString(year);

	string strmon ;
	if ( month < 10 )
	{
		strmon = "0" +ToString(month);
	}
	else
	{
		strmon = ToString(month);
	}

	string strday;
	if (day < 10)
	{
		strday = "0" + ToString(day);
	}
	else
	{
		strday =  ToString(day);
	}

	string iniymd = stryear + strmon + strday;

	return SafeConvert<UINT32>(iniymd);
}


void PersonalCareerQuest::GetTrophyRank(const UINT32 scene_id,StageTrophyInfo& trophy,std::vector<ItemDesc>& items)
{
	std::map<UINT64,std::vector<ItemDesc>> score_items;
	for (auto it = trophy_info_.Table.begin(); it != trophy_info_.Table.end(); ++it)
	{
		if ( scene_id == (*it)->SceneID)
		{
			for (auto trophy_it = trophy.trophy_data.begin(); trophy_it != trophy.trophy_data.end(); ++trophy_it)			
			{
				auto&  data = (*trophy_it);
				if (data.scene_id == scene_id)
				{	
					UINT32 detail_size = 0;
					auto exist_detail_it = data.trophy_detail.find((*it)->ID);
					if (exist_detail_it != data.trophy_detail.end())
					{
						detail_size = exist_detail_it->second.size();
					}
					// 满了，不需要处理了
					if (detail_size >= 3)
					{
						return;
					}
					// 检查是否可以得到铜
					if (detail_size == 0)
					{
						if (CheckTrophyType(data,(*it)->Third,(*it)->ThirdPara))
						{
							TrophyTypeDetail  trophy_detail;							
							trophy_detail.trophy_order = 1;
							trophy_detail.trophy_time = (UINT32)GameTime::GetTime();
							std::vector<TrophyTypeDetail> detail_list;
							detail_list.push_back(trophy_detail);
							data.trophy_detail.insert(std::make_pair((*it)->ID,detail_list));

							trophy.total_score += (*it)->TrophyScore.seq[0];	
							detail_size = 1;
						}
					}
					// 检查是否可以得到银
					if (detail_size == 1)
					{
						if (CheckTrophyType(data,(*it)->Second,(*it)->SecondPara))
						{
							TrophyTypeDetail  trophy_detail;							
							trophy_detail.trophy_order = 2;
							trophy_detail.trophy_time = (UINT32)GameTime::GetTime();

							auto exist_it = data.trophy_detail.find((*it)->ID);
							auto exist_it_end = data.trophy_detail.end();
							if (exist_it != exist_it_end)
							{
								exist_it->second.push_back(trophy_detail);
							}
							else
							{
								std::vector<TrophyTypeDetail> detail_list;
								detail_list.push_back(trophy_detail);
								data.trophy_detail.insert(std::make_pair((*it)->ID,detail_list));
							}
							trophy.total_score += (*it)->TrophyScore.seq[1];	
							detail_size = 2;
						}
					}

					// 检查是否可以得到金
					if (detail_size == 2)
					{
						if (CheckTrophyType(data,(*it)->First,(*it)->FirstPara))
						{
							TrophyTypeDetail  trophy_detail;							
							trophy_detail.trophy_order = 3;
							trophy_detail.trophy_time = (UINT32)GameTime::GetTime();

							auto exist_it = data.trophy_detail.find((*it)->ID);
							auto exist_it_end = data.trophy_detail.end();
							if (exist_it != exist_it_end)
							{
								exist_it->second.push_back(trophy_detail);
							}
							else
							{
								std::vector<TrophyTypeDetail> detail_list;
								detail_list.push_back(trophy_detail);
								data.trophy_detail.insert(std::make_pair((*it)->ID,detail_list));
							}
							
							trophy.total_score += (*it)->TrophyScore.seq[2];		
							detail_size  = 3;
						}
					}					

					break;
				}
			}
			break;
		}		
	}

	for (auto award_it = trophy_reward_.Table.begin(); award_it != trophy_reward_.Table.end(); )
	{
		if (trophy.honour_rank <= (*award_it)->HonourRank && trophy.total_score >= (*award_it)->TrophyScore)
		{
			++award_it;
			if (award_it != trophy_reward_.Table.end())
			{
				trophy.honour_rank = (*award_it)->HonourRank ;
				for (auto it = (*award_it)->Rewards.begin(); it != (*award_it)->Rewards.end(); ++it)
				{
					ItemDesc item;
					item.itemID =it->seq[0];
					item.itemCount = it->seq[1];
					items.push_back(item);
				}
			}		
		}
		else
		{
			++award_it;
		}
	}
}


// 类型1铜（1通关次数2最快通关时间3最高伤害4最高治愈5助战次数6不死通关次数）
bool  PersonalCareerQuest::CheckTrophyType(
	const StageTrophyDataDetail& data,
	const UINT32 trophy_type,
	const UINT32 trophy_type_value )
{
	switch (trophy_type)
	{
	case 1: // 通关次数
		if (data.pass_count >= trophy_type_value)
		{
			return true;
		}
		break;
	case 2: // 最快通关次数
		if (data.quickly_pass_time <= trophy_type_value)
		{
			return true;
		}
		break;
	case 3: // 最高伤害
		if (data.hight_damage >= trophy_type_value*10000)
		{
			return true;
		}
			break;
	case 4: // 最高治愈
		if(data.hight_treat >= trophy_type_value*10000)
		{
			return true;
		}
		break;
	case 5:
		if (data.help_count >= trophy_type_value)
		{
			return true;
		}
		break;
	case 6:
		if (data.no_deathpass_count >= trophy_type_value)
		{
			return true;
		}
		break;
	case 7:
		if (data.quickly_pass_time <= trophy_type_value)
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;

}