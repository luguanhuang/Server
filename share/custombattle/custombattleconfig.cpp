#include "pch.h"
#include <cmath>
#include "define/itemdef.h"
#include "util/gametime.h"
#include "config/itemdropconfig.h"
#include "table/globalconfig.h"
#include "util/XCommon.h"
#include "custombattle/custombattleconfig.h"

INSTANCE_SINGLETON(CCustomBattleConfig);

CCustomBattleConfig::CCustomBattleConfig()
{
}

CCustomBattleConfig::~CCustomBattleConfig()
{
}

bool CCustomBattleConfig::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	return true;
}

void CCustomBattleConfig::Uninit()
{

}

bool CCustomBattleConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	ClearFile();
	m_table.LoadFile("table/CustomBattleSystem.txt");
	m_typetable.LoadFile("table/CustomBattleType.txt");
	m_customtable.LoadFile("table/CustomBattle.txt");
	m_rewardtable.LoadFile("table/CustomReward.txt");
	m_systemrewardtable.LoadFile("table/CustomSystemReward.txt");
	m_pktable.LoadFile("table/CustomPkParam.txt");
	m_pkpreparetable.LoadFile("table/CustomPkPrepare.txt");

	return true;
}

void CCustomBattleConfig::ClearFile()
{
	m_table.Clear();
}

bool CCustomBattleConfig::CheckFile()
{
	CustomBattleSystemTable temptable;
	if (!temptable.LoadFile("table/CustomBattleSystem.txt"))
	{
		SSWarn<<"load file table/CustomBattleSystem.txt failed"<<END;
		return false;
	}
	CustomBattleTable customtemptable;
	if (!customtemptable.LoadFile("table/CustomBattle.txt"))
	{
		SSWarn<<"load file table/CustomBattle.txt failed"<<END;
		return false;
	}
	CustomBattleTypeTable customtypetemptable;
	if (!customtypetemptable.LoadFile("table/CustomBattleType.txt"))
	{
		SSWarn<<"load file table/CustomBattleType.txt failed"<<END;
		return false;
	}

	CustomRewardTable temprewardtable;
	if (!temprewardtable.LoadFile("table/CustomReward.txt"))
	{
		SSWarn<<"load file table/CustomReward.txt failed"<<END;
		return false;
	}
	CustomSystemRewardTable tempsystemrewardtable;
	if (!temprewardtable.LoadFile("table/CustomSystemReward.txt"))
	{
		SSWarn<<"load file table/CustomSystemReward.txt failed"<<END;
		return false;
	}

	PkParamTable temppktable;
	if (!temppktable.LoadFile("table/CustomPkParam.txt"))
	{
		SSWarn<<"load file table/CustomPkParam.txt failed"<<END;
		return false;
	}
	PkPrepareTable temppreparetable;
	if (!temppreparetable.LoadFile("table/CustomPkPrepare.txt"))
	{
		SSWarn<<"load file table/CustomPkPrepare.txt failed"<<END;
		return false;
	}
	return true;
}

bool CCustomBattleConfig::IsSystem(UINT64 uid)
{
	///> 大于UINT32的最大值，一定是自定义
	if (uid > (UINT64)(UINT32)(-1))
	{
		return false;
	}
	CustomBattleSystemTable::RowData* data = m_table.GetByid((UINT32)uid);
	if (NULL == data)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CCustomBattleConfig::IsLevelLimit(UINT32 configid, UINT32 level)
{
	CustomBattleSystemTable::RowData* systemdata = GetSystemData(configid);
	if (NULL != systemdata)
	{
		return level < systemdata->levellimit;
	}
	CustomBattleTable::RowData* data = GetData(configid);
	if (NULL != data)
	{
		return level < data->levellimit;
	}
	return true;
}

std::vector<ItemDesc> CCustomBattleConfig::GetJoinCost(UINT32 configid)
{
	std::vector<ItemDesc> items;
	CustomBattleSystemTable::RowData* data = GetSystemData(configid);
	if (NULL != data)
	{
		ItemDesc item;
		item.itemID = data->ticket[0];
		item.itemCount = data->ticket[1];
		items.push_back(item);
	}
	return items;
}

std::vector<ItemDesc> CCustomBattleConfig::GetGenerateCost(UINT32 configid)
{
	std::vector<ItemDesc> items;
	CustomBattleTable::RowData* data = GetData(configid);
	if (NULL != data)
	{
		for (auto i = data->create.begin(); i != data->create.end(); ++i)
		{
			ItemDesc item;
			item.itemID = i->seq[0];
			item.itemCount = i->seq[1];
			items.push_back(item);
		}
	}
	return items;
}

CustomBattleSystemTable::RowData* CCustomBattleConfig::GetSystemData(UINT32 configid)
{
	return m_table.GetByid(configid);
}

CustomBattleTable::RowData* CCustomBattleConfig::GetData(UINT32 configid)
{
	return m_customtable.GetByid(configid);
}
/*
message CustomBattleConfig{
	optional CustomBattleType type = 1;
	optional uint32 configid = 2;
	optional uint32 scalemask = 3;
	optional string name = 4;
	optional string desc = 5;
	optional bool haspassword = 6;
	optional string password = 7;
	optional bool isfair = 8;
	optional uint32 battletime = 9;
	optional uint32 canjoincount = 10;
	optional uint64 creator = 11;
	optional string creatorname = 12;
	optional CustomBattleState state = 13;
	optional uint32 readytime = 14;
	optional bool issystem = 15;
	optional uint32 hasjoincount = 16;
}
*/
KKSG::ErrorCode CCustomBattleConfig::IsValidCreate(const KKSG::CustomBattleConfig& config)
{
	///> 配置是否存在
	CustomBattleTable::RowData* data = GetData(config.configid());
	if (NULL == data)
	{
		return KKSG::ERR_CUSTOMBATTLE_CREATECONFINVALID;
	}
	///> 比赛时间非法
	bool isbattletimevalid = false;
	for (auto i = data->timespan.begin(); i != data->timespan.end(); ++i)
	{
		if (config.battletime() == *i)
		{
			isbattletimevalid = true;
			break;
		}
	}
	if (!isbattletimevalid)
	{
		return KKSG::ERR_CUSTOMBATTLE_CREATECONFINVALID;
	}
	KKSG::ErrorCode errorcode = KKSG::ERR_CUSTOMBATTLE_CREATECONFINVALID;
	///> 密码验证
	errorcode = ValidPassword(config.password());
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		return errorcode;
	}
	///> 标题，描述
	if (config.name().length() > (UINT32)GetGlobalConfig().CustomBattleNameLimit1)
	{
		return KKSG::ERR_CUSTOMBATTLE_NAMEMAX;
	}
	if (config.name().length() <= (UINT32)GetGlobalConfig().CustomBattleNameMin)
	{
		return KKSG::ERR_CUSTOMBATTLE_NAMEMIN;
	}
	//if (config.desc().length() > 500)
	//{
	//	return KKSG::ERR_CUSTOMBATTLE_CREATECONFINVALID;
	//}
	//errorcode = CTssHandler::Instance()->ValidNameCheck(config.name());
	//if (KKSG::ERR_SUCCESS != errorcode)
	//{
	//	return errorcode;
	//}
	//errorcode = CTssHandler::Instance()->ValidNameCheck(config.desc());
	//if (KKSG::ERR_SUCCESS != errorcode)
	//{
	//	return errorcode;
	//}
	///> 战场范围(scalemask)

	///> 公平模式(isfair)

	return KKSG::ERR_SUCCESS;	
}

bool CCustomBattleConfig::IsGMCreate(UINT32 configid)
{
	///> 配置是否存在
	CustomBattleTable::RowData* data = GetData(configid);
	if (NULL == data)
	{
		return false;
	}
	CustomBattleTypeTable::RowData* typedata = m_typetable.GetBytype(data->type);
	if (NULL == typedata)
	{
		return false;
	}
	return typedata->gmcreate;
}

bool CCustomBattleConfig::IsGMJoin(UINT32 configid)
{
	///> 配置是否存在
	CustomBattleTable::RowData* data = GetData(configid);
	if (NULL == data)
	{
		return false;
	}
	return data->gm > 0;
}

KKSG::ErrorCode CCustomBattleConfig::FillCreate(const KKSG::CustomBattleConfig& config, UINT64 roleid, const std::string& name, KKSG::CustomBattleConfig& serverconfig, bool isgm)
{
	///> 配置是否存在
	CustomBattleTable::RowData* data = GetData(config.configid());
	if (NULL == data)
	{
		return KKSG::ERR_CUSTOMBATTLE_CREATECONFINVALID;
	}

	///> 服务器根据客户端选项构造数据
	serverconfig = config;
	serverconfig.clear_tagmask();
	if (isgm)
	{
		serverconfig.clear_scalemask();
		serverconfig.set_tagmask(1 << KKSG::CustomBattle_Tag_GM);
	}
	serverconfig.mutable_desc()->clear();
	serverconfig.set_issystem(false);
	serverconfig.set_creator(roleid);
	serverconfig.set_creatorname(name);

	serverconfig.set_tagtype(data->type);
	serverconfig.set_fighttype((KKSG::CustomBattleType)data->fighttype);
	serverconfig.set_state(KKSG::CustomBattle_Ready);
	serverconfig.set_canjoincount(data->joincount);
	serverconfig.set_hasjoincount(0);
	serverconfig.set_battletimeconf(serverconfig.battletime());
	serverconfig.set_readytime((UINT32)GameTime::GetTime() + data->readytimepan);
	serverconfig.set_battletime(serverconfig.readytime() + serverconfig.battletime());

	return KKSG::ERR_SUCCESS;
}

UINT32 CCustomBattleConfig::WinLimit(UINT32 configid)
{
	CustomBattleSystemTable::RowData* data = GetSystemData(configid);
	if (NULL == data)
	{
		SSWarn<<"invalid configid:"<<configid<<END;
		return (UINT32)(-1);
	}
	else
	{
		return data->end[0];
	}
}

UINT32 CCustomBattleConfig::LoseLimit(UINT32 configid)
{
	CustomBattleSystemTable::RowData* data = GetSystemData(configid);
	if (NULL == data)
	{
		SSWarn<<"invalid configid:"<<configid<<END;
		return (UINT32)(-1);
	}
	else
	{
		return data->end[1];
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<ItemDesc> CCustomBattleConfig::GetClearSystemCD(UINT32 configid, UINT32 win)
{
	std::vector<ItemDesc> items;
	for (auto i = m_systemrewardtable.Table.begin(); i != m_systemrewardtable.Table.end(); ++i)
	{
		CustomSystemRewardTable::RowData* data = *i;
		if (data->id == configid && data->wincounts == win)
		{
			ItemDesc item;
			item.itemID = data->boxquickopen[0];
			item.itemCount = data->boxquickopen[1];
			items.push_back(item);
		}
	}
	return items;
}

UINT32 CCustomBattleConfig::GetSystemRewardCD(UINT32 configid, UINT32 win)
{
	UINT32 cd = 0;
	for (auto i = m_systemrewardtable.Table.begin(); i != m_systemrewardtable.Table.end(); ++i)
	{
		CustomSystemRewardTable::RowData* data = *i;
		cd = data->boxcooldown;
		if (data->id == configid && data->wincounts == win)
		{
			break;
		}
	}
	return cd;
}

std::vector<ItemDesc> CCustomBattleConfig::GetSystemReward(UINT32 configid, UINT32 win)
{
	std::vector<ItemDesc> items;
	for (auto i = m_systemrewardtable.Table.begin(); i != m_systemrewardtable.Table.end(); ++i)
	{
		CustomSystemRewardTable::RowData* data = *i;
		if (data->id == configid && data->wincounts == win)
		{
			const ItemDesc* item = ItemDropConfig::Instance()->GetRandomDrop(data->rewarddrop);
			if (NULL != item)
			{
				items.push_back(*item);
			}
		}
	}
	return items;
}

std::vector<ItemDesc> CCustomBattleConfig::GetClearCustomCD(UINT32 configid, UINT32 rank)
{
	std::vector<ItemDesc> items;
	for (auto i = m_rewardtable.Table.begin(); i != m_rewardtable.Table.end(); ++i)
	{
		CustomRewardTable::RowData* data = *i;
		if (data->id == configid && (rank >= data->rank[0] && rank <= data->rank[1]))
		{
			ItemDesc item;
			item.itemID = data->boxquickopen[0];
			item.itemCount = data->boxquickopen[1];
			items.push_back(item);
		}
	}
	return items;
}

UINT32 CCustomBattleConfig::GetCustomRewardCD(UINT32 configid, UINT32 rank)
{
	UINT32 cd = 0;
	for (auto i = m_rewardtable.Table.begin(); i != m_rewardtable.Table.end(); ++i)
	{
		CustomRewardTable::RowData* data = *i;
		cd = data->boxcooldown;
		if (data->id == configid && (rank >= data->rank[0] && rank <= data->rank[1]))
		{
			break;
		}
	}
	return cd;

}

std::vector<ItemDesc> CCustomBattleConfig::GetCustomReward(UINT32 configid, UINT32 rank)
{
	std::vector<ItemDesc> items;
	for (auto i = m_rewardtable.Table.begin(); i != m_rewardtable.Table.end(); ++i)
	{
		CustomRewardTable::RowData* data = *i;
		if (data->id == configid && (rank >= data->rank[0] && rank <= data->rank[1]))
		{
			const ItemDesc* item = ItemDropConfig::Instance()->GetRandomDrop(data->rewarddrop);
			if (NULL != item)
			{
				items.push_back(*item);
			}
		}
	}
	return items;
}

UINT32 CCustomBattleConfig::GetParam(UINT32 point, bool iswin)
{
	for (auto i = m_pktable.Table.begin(); i != m_pktable.Table.end(); ++i)
	{
		PkParamTable::RowData* rowdata = *i;
		if (point >= rowdata->min && rowdata->max >= point)
		{
			return iswin ? rowdata->winK : rowdata->loseK;
		}
	}
	return 40;
}

int CCustomBattleConfig::GetPkPoint(int basepoint, int pointspan, int result)
{
	if (0 == result)
	{
		return 0;
	}
	bool iswin = result > 0;
	float expect = 1/(1 + pow(10, (-(float)pointspan/(float)400)));
	float W = iswin ? 1.0f : 0.0f;
	float K = (float)GetParam(basepoint, iswin);
	int point = (int)(K*(W - expect));
	return point;
}

KKSG::ErrorCode CCustomBattleConfig::ValidPassword(const std::string& password)
{
	if (password.empty())
	{
		return KKSG::ERR_SUCCESS;
	}
	if (password.size() > 4)
	{
		return KKSG::ERR_CUSTOMBATTLE_PASSWORDINVALID;
	}
	if (!XCommon::IsNumber(password))
	{
		return KKSG::ERR_CUSTOMBATTLE_PASSWORDINVALID;
	}
	return KKSG::ERR_SUCCESS;
}

std::string CCustomBattleConfig::GenerateToken()
{
	std::string token;
	for (int i = 0; i < 6; ++i)
	{
		char random = RandomChar();
		token += random;
	}
	return token;	
}

char CCustomBattleConfig::RandomChar()
{
	int a = XRandom::randInt(0, 2);
	if (0 == a)
	{
		int b = XRandom::randInt(0, 10);
		return '0' + b;
	}
	else
	{
		int c = XRandom::randInt(0, 24);
		return 'a' + c;
	}
}

void CCustomBattleConfig::FillPkStage(UINT32 point, KKSG::CustomBattleMatchRoleInfo& data, KKSG::PkNVNType type)
{
	int last = int(m_pkpreparetable.Table.size()) - 1;
	if(last < 0)
	{
		return;
	}
	for(int i = last; i >= 0; --i)
	{
		PkPrepareTable::RowData* conf = m_pkpreparetable.Table[i];
		if(conf->point[0] <= point && point <= conf->point[1] && conf->type == type)
		{
			auto& tmp = *data.add_stages();

			tmp.set_ctime(conf->time);
			tmp.set_span(conf->span);
		}
	}

	last = data.stages_size() - 1;
	if(last < 0)
	{
		return;
	}
	UINT32 now = UINT32(time(NULL));
	for(int i = last; i >= 0; --i)
	{
		auto& tmp = *data.mutable_stages(i);
		UINT32 tt = tmp.ctime();
		if(i == last)
		{
			tmp.set_ctime(tt + now);
		}
		else
		{
			tmp.set_ctime(tt + data.stages(i+1).ctime());
		}
	}
}

bool CCustomBattleConfig::IsPvpTwo(UINT32 configid)
{
	CustomBattleTable::RowData* data = GetData(configid);
	if (NULL == data)
	{
		return false;
	}
	return data->fighttype == KKSG::CustomBattle_PKTwo_Normal;
}
