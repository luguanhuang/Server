#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "db/mysqlmgr.h"
#include "util.h"
#include "timeutil.h"
#include "network/dblink.h"
#include "role/rolemanager.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/ptcm2d_updatecustombattleondb.h"
#include "custombattle/custombattlerandom.h"
#include "friend/friendop.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "custombattle/ptcm2c_custombattlegmnotify.h"
#include "define/systemiddef.h"
#include "table/globalconfig.h"

#define CUSTOMBATTLE_TIMEUPDATE (GetGlobalConfig().CustomBattleMsUpdate)
#define CUSTOMBATTLE_TIMEUPDATE_LIMIT (GetGlobalConfig().CustomBattleMsUpdateLimit)

INSTANCE_SINGLETON(CustomBattleMgr);

CustomBattleMgr::CustomBattleMgr()
{
	m_isend = false;
	m_generator = new CustomBattleGenerator();
}

CustomBattleMgr::~CustomBattleMgr()
{
	if (NULL != m_generator)
	{
		delete m_generator;
		m_generator = NULL;
	}
}

bool CustomBattleMgr::Init()
{
	// read from db
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select _id, custombattle from custombattle";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table custombattle failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		// uid
		UINT64 uid;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			uid = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		// battle
		std::string custombattle;
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			custombattle.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'custombattle' column failed");
			return false;
		}

		KKSG::CustomBattleData tempcustombattle;
		if(!tempcustombattle.ParseFromString(custombattle))
		{
			LogError("Parse custombattle failed, uid:%llu", uid);
			return false;
		}

		if (KKSG::CustomBattle_PK_Normal == tempcustombattle.config().fighttype()
			|| KKSG::CustomBattle_PKTwo_Normal == tempcustombattle.config().fighttype())
		{
			CustomBattlePk* battle = new CustomBattlePk();
			battle->ToThis(tempcustombattle);
			AddBattle(battle->GetID(), battle);
		}
		else
		{
			SSWarn<<"invalid type:"<<tempcustombattle.config().fighttype()<<END;
		}
	}
	poRes->Release();

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all custombattle data from db use time: %llu ms", qwTimeUsed);

	m_timehandle = CTimerMgr::Instance()->SetTimer(this ,0, CUSTOMBATTLE_TIMEUPDATE, -1, __FILE__, __LINE__);

	m_generator = new CustomBattleGenerator();	

	m_query.Init(this);
	m_create.Init(this);
	m_modify.Init(this);
	m_join.Init(this);
	m_unjoin.Init(this);
	m_match.Init(this);
	m_unmatch.Init(this);
	m_reward.Init(this);
	m_search.Init(this);

	m_createworld.Init(this);
	m_joinworld.Init(this);
	m_unjoinworld.Init(this);
	m_rewardworld.Init(this);

	m_dropsystem.Init(this);
	m_joinsystem.Init(this);
	m_unjoinsystem.Init(this);
	m_rewardsystem.Init(this);

	LoadGM();

	CmdLine::RegistCmd("custombattle", CustomBattleMgr::CmdLine);

	return true;
}

void CustomBattleMgr::Uninit()
{
	if (NULL != m_generator)
	{
		delete m_generator;
		m_generator = NULL;
	}
	if (INVALID_HTIMER != m_timehandle)
	{
		CTimerMgr::Instance()->KillTimer(m_timehandle);
		m_timehandle = INVALID_HTIMER;
	}
}

bool CustomBattleMgr::IsOpen(CRole* role, const CustomBattleReq& req)
{
	if (!role->IsSystemOpened(CustomBattle_All))
	{
		SSWarn<<"custombattle is not open, roleid:"<<role->GetID()<<END;
		return false;
	}
	switch(req.op)
	{
	// only system
	case KKSG::CustomBattle_Query:
	case KKSG::CustomBattle_Drop:
		{
			return role->IsSystemOpened(CustomBattle_System);
		}
	// only custom
	case KKSG::CustomBattle_QuerySelf:
	case KKSG::CustomBattle_QueryOne:
	case KKSG::CustomBattle_QueryRandom:
	case KKSG::CustomBattle_Search:
	case KKSG::CustomBattle_Modify:
	case KKSG::CustomBattle_StartNow:
	case KKSG::CustomBattle_Create:
		{
			return role->IsSystemOpened(CustomBattle_Custom);
		}
	case KKSG::CustomBattle_Join:
	case KKSG::CustomBattle_UnJoin:
	case KKSG::CustomBattle_Match:
	case KKSG::CustomBattle_UnMatch:
	case KKSG::CustomBattle_ClearCD:
	case KKSG::CustomBattle_Reward:
		{
			if (CCustomBattleConfig::Instance()->IsSystem(req.uid))
			{
				return role->IsSystemOpened(CustomBattle_System);
			}
			else
			{
				return role->IsSystemOpened(CustomBattle_Custom);
			}
		}
	default:
		{
			SSWarn<<"invalid req, op:"<<req.op<<" roleid"<<req.roleid<<END;
			return false;
		}
	}
	return false;
}

bool CustomBattleMgr::HandleClientReq(const CustomBattleReq& req)
{
	switch(req.op)
	{
	case KKSG::CustomBattle_Query:
	case KKSG::CustomBattle_QuerySelf:
	case KKSG::CustomBattle_QueryOne:
	case KKSG::CustomBattle_QueryRandom:
		{
			m_query.HandleClientReq(req);
			break;
		}
	case KKSG::CustomBattle_Search:
		{
			m_search.HandleClientReq(req);
			break;
		}
	case KKSG::CustomBattle_Modify:
	case KKSG::CustomBattle_StartNow:
		{
			if (!CCustomBattleConfig::Instance()->IsSystem(req.uid))
			{
				m_modify.HandleClientReq(req);
			}
			break;
		}
	case KKSG::CustomBattle_Create:
		{
			if (!GetGlobalConfig().CustomBattleCreate)
			{
				SSInfo<<"custom battle create not open, role:"<<req.roleid<<END;
				return false;
			}
			if (CustomBattle::IsLocalCreate(req.conf))
			{
				m_create.HandleClientReq(req);
			}
			else
			{
				m_createworld.HandleClientReq(req);
			}
			break;
		}
	case KKSG::CustomBattle_Join:
		{
			if (CCustomBattleConfig::Instance()->IsSystem(req.uid))
			{
				///> 系统
				if (!GetGlobalConfig().CustomBattleSystemJoin)
				{
					SSInfo<<"custom battle system join not open, role:"<<req.roleid<<END;
					return false;
				}
				m_joinsystem.HandleClientReq(req);
			}
			else if (IsLocal(req.uid))
			{
				///> 本地自定义
				m_join.HandleClientReq(req);
			}
			else
			{
				///> 跨服自定义
				m_joinworld.HandleClientReq(req);
			}
			break;
		}
	case KKSG::CustomBattle_UnJoin:
		{
			if (CCustomBattleConfig::Instance()->IsSystem(req.uid))
			{
				///> 系统
				m_unjoinsystem.HandleClientReq(req);
			}
			else if (IsLocal(req.uid))
			{
				///> 本地自定义
				m_unjoin.HandleClientReq(req);
			}
			else
			{
				///> 跨服自定义
				m_unjoinworld.HandleClientReq(req);
			}
			break;
		}
	case KKSG::CustomBattle_Match:
		{
			m_match.HandleClientReq(req);
			break;
		}
	case KKSG::CustomBattle_UnMatch:
		{
			m_unmatch.HandleClientReq(req);
			break;
		}
	case KKSG::CustomBattle_ClearCD:
	case KKSG::CustomBattle_Reward:
		{
			if (CCustomBattleConfig::Instance()->IsSystem(req.uid))
			{
				m_rewardsystem.HandleClientReq(req);
			}
			else if (IsLocal(req.uid))
			{
				m_reward.HandleClientReq(req);
			}
			else
			{
				m_rewardworld.HandleClientReq(req);
			}
			break;
		}
	case KKSG::CustomBattle_Drop:
		{
			m_dropsystem.HandleClientReq(req);
			break;
		}
	default:
		{
			SSWarn<<"invalid req, op:"<<req.op<<" roleid"<<req.roleid<<END;
			return false;
		}
		break;
	}
	return true;
}


CustomBattle* CustomBattleMgr::GetBattle(UINT64 uid)
{
	auto i = m_uid2battle.find(uid);
	if (i != m_uid2battle.end())
	{
		return i->second;
	}
	else
	{
		return NULL;
	}
}

void CustomBattleMgr::AddBattle(UINT64 uid, CustomBattle* battle)
{
	auto i = m_uid2battle.find(uid);
	if (i != m_uid2battle.end())
	{
		delete battle;
		SSWarn<<"uid:"<<uid<<" is exist"<<END;
	}
	else
	{
		m_uid2battle[uid] = battle;
		m_name2uid[battle->GetData().config().name()].push_back(uid);
		m_name2uid[battle->GetData().config().token()].push_back(uid);
	}
}

void CustomBattleMgr::DelBattleByName(const std::string& name, UINT64 uid)
{
	auto k = m_name2uid.find(name);
	if (k != m_name2uid.end())
	{
		for (auto l = k->second.begin(); l != k->second.end(); ++l)
		{
			if (*l == uid)
			{
				k->second.erase(l);
				break;
			}
		}
		if (k->second.empty())
		{
			m_name2uid.erase(k);
		}
	}
}

void CustomBattleMgr::DelBattle(UINT64 uid)
{
	auto j = m_uid2battle.find(uid);
	if (j != m_uid2battle.end())
	{
		CustomBattle* battle = j->second;

		DelBattleByName(battle->GetData().config().name(), uid);
		DelBattleByName(battle->GetData().config().token(), uid);

		delete j->second;
		j->second = NULL;
		m_uid2battle.erase(j);
	}
}

bool CustomBattleMgr::IsLocal(UINT64 uid)
{
	return NULL != CustomBattleMgr::Instance()->GetBattle(uid);
}

void CustomBattleMgr::FillRandomBattle(KKSG::CustomBattleQueryInfo* queryinfo, UINT64 uid, UINT64 roleid)
{
	CustomBattleRandom randomhelper;
	for (auto i = m_uid2battle.begin(); i != m_uid2battle.end(); ++i)
	{
		if (i->first == uid)
		{
			continue;
		}
		CustomBattle* battle = i->second;
		UINT64 creator = battle->GetCreator();
		// friend 
		FriendOp op(roleid);
		UINT32 frienddegree = op.GetDegree(creator);
		bool isfriend = op.IsFriend(creator);
		// guild
		bool isguild = false;
		Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(battle->GetCreator());
		if (NULL != guild)
		{
			if (guild->IsPlayerInGuild(roleid))
			{
				isguild = true;
			}
		}

		randomhelper.Add(battle, frienddegree, isfriend, isguild);
	}

	std::vector<CustomBattle*> randomresult = randomhelper.Get();
	for (auto i = randomresult.begin();  i != randomresult.end(); ++i)
	{
		CustomBattle* battle = *i;
		KKSG::CustomBattleDataRole* datarole = queryinfo->add_battlerandom();
		battle->ToKKSGBrief(datarole->mutable_data());
		FillTag(battle, datarole->mutable_data(), battle->GetCreator(), roleid);
	}
}

void CustomBattleMgr::Dump(std::string& output)
{
	std::stringstream ss;
	ss << "CustomBattle All:\n";
	for (auto i = m_uid2battle.begin(); i != m_uid2battle.end(); ++i)
	{
		ss<<"battle uid:"<<i->first<<" join count:"<<i->second->Size()<<"\n";
	}

	output = ss.str();
	LogDebug("%s", output.c_str());
}

bool CustomBattleMgr::CmdLine(const CmdLineArgs &args, std::string &outputMessage)
{
	CustomBattleMgr::Instance()->Dump(outputMessage);
	return true;
}

void CustomBattleMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 countlimit = m_isend ? (UINT32)(-1) : CUSTOMBATTLE_TIMEUPDATE_LIMIT;
	UINT32 updatelimit = 0;
	std::vector<UINT64> todelete;
	for (auto i = m_uid2battle.begin(); i != m_uid2battle.end(); ++i)
	{
		i->second->Update();

		if (i->second->IsDestory())
		{
			todelete.push_back(i->first);
			continue;
		}
		if (updatelimit > countlimit)
		{
			break;;
		}
		if (i->second->IsChanged())
		{
			///// save to db...
			PtcM2D_UpdateCustomBattleOnDB ntf;
			ntf.m_Data.set_isdelete(false);
			i->second->ToKKSGDB(ntf.m_Data.mutable_data());
			DBLink::Instance()->SendTo(ntf);

			i->second->ResetChanged();

			++updatelimit;
		}
	}
	UINT32 deletelimit = 0;
	for (auto i = todelete.begin(); i != todelete.end(); ++i)
	{
		if (deletelimit >= countlimit)
		{
			break;
		}

		DelBattle(*i);

		// delete from db
		PtcM2D_UpdateCustomBattleOnDB ntf;
		ntf.m_Data.set_isdelete(true);
		ntf.m_Data.set_uid(*i);
		DBLink::Instance()->SendTo(ntf);
		
		++deletelimit;

		SSInfo<<"try delete uid:"<<*i<<END;
	}
}
//
//std::vector<UINT64>* CustomBattleMgr::GetBattleByToken(const std::string& token)
//{
//	auto i = m_token2uid.find(token);
//	if (i == m_token2uid.end())
//	{
//		return NULL;
//	}
//	else
//	{
//		return &i->second;
//	}	
//}

std::vector<UINT64>* CustomBattleMgr::GetBattleByName(const std::string& name)
{
	auto i = m_name2uid.find(name);
	if (i == m_name2uid.end())
	{
		return NULL;
	}
	else
	{
		return &i->second;
	}	
}


void CustomBattleMgr::FillTag(CustomBattle* battle, KKSG::CustomBattleData* data, UINT64 creator, UINT64 roleid)
{
	if (creator == roleid)
	{
		if(battle->IsFriendOnly())
		{
			UINT32 mask = data->mutable_config()->tagmask();
			data->mutable_config()->set_tagmask((1 << KKSG::CustomBattle_Tag_Friend) | mask);
		}
		if (battle->IsGuildOnly())
		{
			UINT32 mask = data->mutable_config()->tagmask();
			data->mutable_config()->set_tagmask((1 << KKSG::CustomBattle_Tag_Guild) | mask);
		}
		return;
	}
   	// friend 
	if (battle->IsFriendOnly())
	{
		FriendOp op(roleid);
		bool isfriend = op.IsFriend(creator);
		if (isfriend)
		{
			UINT32 mask = data->mutable_config()->tagmask();
			data->mutable_config()->set_tagmask((1 << KKSG::CustomBattle_Tag_Friend) | mask);
		}
	}
	if (battle->IsGuildOnly())
	{
		// guild
		Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(creator);
		if (NULL != guild)
		{
			bool isguild = guild->IsPlayerInGuild(roleid);
			if (isguild)
			{
				UINT32 mask = data->mutable_config()->tagmask();
				data->mutable_config()->set_tagmask((1 << KKSG::CustomBattle_Tag_Guild) | mask);
			}
		}
	}
}

bool CustomBattleMgr::IsGMCreate(const std::string& openid)
{
	auto i = m_gmcreate.find(openid);
	if (i == m_gmcreate.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CustomBattleMgr::IsGMJoin(const std::string& openid)
{
	auto i = m_gmjoin.find(openid);
	if (i == m_gmjoin.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CustomBattleMgr::LoadGM()
{
	m_gmcreate.clear();
	std::string data = MSConfig::Instance()->ReadFileContent("serveronly/custombattlegmcreate.txt");
	if (!data.empty())
	{
		std::vector<std::string> Lines = Split(data, "\r\n");
		for (auto i = Lines.begin(); i != Lines.end(); ++i)
		{
			m_gmcreate.insert(*i);
			SSInfo<<"custombattle gmcreate:"<<*i<<END;
		}
	}
	m_gmjoin.clear();
	std::string joindata = MSConfig::Instance()->ReadFileContent("serveronly/custombattlegmjoin.txt");
	if (!joindata.empty())
	{
		std::vector<std::string> Lines = Split(joindata, "\r\n");
		for (auto i = Lines.begin(); i != Lines.end(); ++i)
		{
			m_gmjoin.insert(*i);
			SSInfo<<"custombattle gmjoin:"<<*i<<END;
		}
	}
	return true;
}

void CustomBattleMgr::OnRoleLogin(CRole* role)
{
	bool isgm = false;
	PtcM2C_CustomBattleGMNotify ntf;
	if (IsGMCreate(role->GetAccount()))
	{
		isgm = true;
		ntf.m_Data.set_isgmcreate(true);
		SSInfo<<"custombattle gmcreate:"<<role->GetAccount()<<" login"<<END;
	}
	if (IsGMJoin(role->GetAccount()))
	{
		isgm = true;
		ntf.m_Data.set_isgmjoin(true);
		SSInfo<<"custombattle gmjoin:"<<role->GetAccount()<<" login"<<END;
	}
	if (isgm)
	{
		role->Send(ntf);
	}
}


