#include "pch.h"
#include "common/rpcg2m_gmcommandg2m.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "util.h"
#include "team/robotsummarymgr.h"
#include "pvp/pvpmgr.h"
#include "team/teamrequest.h"
#include "ranklist/ranklistmgr.h"
#include "table/DragonExpRankMgr.h"
#include "auction/auctionmgr.h"
#include "table/globalconfig.h"
#include "auction/auctioncart.h"
#include "auction/traderecord.h"
#include "chat/ptcm2c_idipclearchatntf.h"
#include "role/rolesummarymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildgoblin.h"
#include "guild/guildcheckin.h"
#include "gmf/guildmatchfightmgr.h"
#include "event/eventmgr.h"
#include "social/socialmgr.h"
#include "skycity/skycitymgr.h"
#include "guild/guildcard.h"
#include "team/teamforceop.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "ranklist/flowerranklist.h"
#include "qa/qamgr.h"
#include "friend/friendop.h"
#include "table/MultActivityMgr.h"
#include "mail/mailmgr.h"
#include "network/gslink.h"
#include "account/accountkick.h"
#include "account/accountkicklistener.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildbindinfo.h"
#include "auction/auctionrequest.h"
#include "httptask/ordercreate.h"
#include "httptask/httptaskmgr.h"
#include "httptask/paynotify.h"
#include "scene/sceneswitch.h"
#include "garden/garden_manager.h"
#include "crossscene/rpcm2k_getsceneinfo.h"
#include "network/worldlink.h"
#include "guildauct/guildauctmgr.h"
#include "platforminfo/platformfriendmgr.h"
#include "guild/guildbonus.h"
#include "reswar/reswarmgr.h"
#include "reswar/guildbuffmgr.h"
#include "guild/guildcamp.h"
#include "guild/guildarena.h"
#include "util/gametime.h"
#include "common/ptcm2g_syncservertime.h"
#include "network/gslink.h"
#include "common/ptcm2g_worldlevelntf.h"
#include "../share/table/globalconfig.h"
#include "garden/garden_utility.h"
#include "ranklist/nestweekranklist.h"
#include "nestweek/nestweekmgr.h"
#include "invfight/invfightmgr.h"
#include "../../team/teammember.h"
#include "../../herobattle/herobattlemgr.h"
#include "partner/partnermgr.h"
#include "guildcastle/guildcastlemgr.h"
#include "guildcastle/guildcastlerequest.h"
#include "guildcastle/guildcastlebase.h"
#include "guild/guildterritorybattle.h"
#include "pay/paymgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "pay/ptcm2c_payparameterinfoinvalidntf.h"
#include "leagueteam/leagueteammgr.h"
#include "skyteam/skyteammgr.h"
#include "platforminfo/platformfriendmgr.h"
#include "httptask/reportdata.h"
#include "httptask/getlistsubscribe.h"
#include "leaguebattle/leagueeliminationmgr.h"
#include "leaguebattle/leagueseasonmgr.h"
#include "table/OpenSystemMgr.h"
#include "pk/pkmgr.h"
#include "httptask/setsubscribe.h"
#include "httptask/pushsubscribe.h"
#include "idip/platnotice.h"
#include "misc/ptcm2g_clicknoticentf.h"
#include "gm/rpcm2k_gmcommandm2w.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/leagueranklist.h"
#include "ranklist/crossleagueranklist.h"
#include "mentorship/mentormgr.h"
#include "mentorship/mentoroneside.h"
#include "mentorship/mentortask.h"
#include "military/military_manager.h"
#include "guild/guildwage.h"
#include "arenastar/arenastarmgr.h"
#include "ranklist/pkranklist.h"
#include "ranklist/herobattleranklist.h"
#include "pay/ibgiftmgr.h"
#include "mentorship/mentorrelationlist.h"
#include "mentorship/mentormsg.h"
#include "custombattle/custombattlerolemgr.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/custombattlehandle.h"
#include "custombattle/custombattlemgr.h"
#include "network/teamlink.h"
#include "team/teamtranshandler.h"
#include "other/matchhandler.h"
#include "match/matchrequest.h"
#include "report/reportmgr.h"
#include "event/activitymailmgr.h"
#include "marriage/weddingmgr.h"
#include "match/matchmgr.h"
#include "guild/guildactivity.h"
#include "mobabattle/mobabattlemgr.h"
#include "utility/reloadmgr.h"
#include "common/ptcm2g_broadcastglobalvalue.h"
#include "other/servercrossdatamgr.h"
#include "idutil.h"
#include "reswar/reswarpvemgr.h"
#include "wedding/rpcm2g_weddingoperm2g.h"
#include "marriage/marriage.h"
#include "marriage/marriagemgr.h"
#include "ranklist/briefranklist.h"
#include "mayhem/mayhemmgr.h"
#include "dragonguild/dragonguildmgr.h"
#include "tablemgr/dragonguildtablemgr.h"
#include "battlefield/battlefieldmgr.h"
#include "goalawards/goalawardsmgr.h"
#include "npcfeeling/npcfeelingmgr.h"
#include "survive/survivemgr.h"
#include "ranklist/surviveranklist.h"

// generate by ProtoGen at date: 2016/8/22 17:04:04

RPC_SERVER_IMPLEMETION(RpcG2M_GMCommandG2M, GMCmdg2mArg, GMCmdg2mRes)

typedef bool (*GMHandler)(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage);

UINT64 GetGuildIDByName(const std::string& name)
{
	const auto& guilds = CGuildMgr::Instance()->GetGuilds();
	for(auto it = guilds.begin(); it != guilds.end(); ++it)
	{
		if((it->second)->GetName() == name)
		{
			return (it->second)->GetID();
		}
	}
	return 0;
}

static bool DebugGMTest( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	outputMessage = "ms gmtest success ";
	return true;
}

static bool DebugRobot( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(3 == (int)args.size() && "get" == args[1])
	{
		int lookupid = convert<int>(args[2]);	
		RobotSummaryMgr::Instance()->RegisterRobotSummary(pRole->GetID(), lookupid);
	}
	else if(2 == (int)args.size() && "free" == args[1])
	{
		RobotSummaryMgr::Instance()->FreeRobotSummary(pRole->GetID());
	}
	return true;
}

static bool DebugMail(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)args.size();
	if(2 == size && args[1] == "clear")
	{
		CMailMgr::Instance()->DeleteAllMail(pRole->GetID());	
	}
	return true;
}

static bool DebugInvFight(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)args.size();
	if(size == 2 && args[1] == "make")
	{
		const auto& itbegin = CRoleManager::Instance()->Begin();	
		const auto& itend = CRoleManager::Instance()->End();
		for(auto i = itbegin; i != itend; ++i)
		{
			for(auto j = itbegin; j != itend; ++j)
			{
				if(i == j)
				{
					continue;
				}
				InvFightMgr::Instance()->MakeInvFight((*i)->GetID(), (*j)->GetID());
			}
		}
	}
	else if(size == 2 && args[1] == "delfrom")
	{
		InvFightMgr::Instance()->DelFromIDInv(pRole->GetID());
	}
	else if(size == 2 && args[1] == "delto")
	{
		InvFightMgr::Instance()->DelToIDInv(pRole->GetID());
	}
	else if(size == 3 && args[1] == "deluni")
	{
		UINT32 invID = convert<UINT32>(args[2]);
		InvFightMgr::Instance()->Del(UINT64(invID));
	}
	else if(size == 3 && args[1] == "settimeover")
	{
		outputMessage += " old = " + ToString(GetGlobalConfig().InvFightTimeOver);
		GetGlobalConfig().InvFightTimeOver = convert<UINT32>(args[2]);
		outputMessage += " new = " + ToString(GetGlobalConfig().InvFightTimeOver);
	}
	else if(size == 3 && args[1] == "level")
	{
		outputMessage += " old = " + ToString(GetGlobalConfig().InvFightRoleLevel);
		GetGlobalConfig().InvFightRoleLevel = convert<UINT32>(args[2]);
		outputMessage += " new = " + ToString(GetGlobalConfig().InvFightRoleLevel);
	}
	else if(size == 3 && args[1] == "fromcount")
	{
		outputMessage += " old = " + ToString(GetGlobalConfig().InvFightFromCount);
		GetGlobalConfig().InvFightFromCount = convert<UINT32>(args[2]);
		outputMessage += " new = " + ToString(GetGlobalConfig().InvFightFromCount);
	}
	else if(size == 2 && args[1] == "clear")
	{
		InvFightMgr::Instance()->TestClearData();
	}
	else
	{
		InvFightMgr::Instance()->TestPrint();
	}

	return true;
}

static bool DebugNestWeek(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	NestWeekRankList* ranklist = (NestWeekRankList*)CRankListMgr::Instance()->GetRankList(KKSG::NestWeekRank);
	if(NULL == ranklist)
	{
		outputMessage = "ranklist = null";
		return true;
	}
	int size = (int)args.size();
	if(3 == size && args[1] == "cost")
	{
		UINT32 costT = convert<UINT32>(args[2]);
		Team* pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
		if(pTeam)
		{
			NWRKey roles;
			pTeam->GetRoleIDs(roles);
			ranklist->OnChange(roles, costT, (UINT32)time(NULL));
		}
	}
	else if(2 == size && args[1] == "nw")
	{
		NestWeekMgr::Instance()->TestSetNextWeek();
	}
	else
	{
		ranklist->InfoPrint();
		SSDebug << " curexp = " << NestWeekMgr::Instance()->GetCurExp() << END;
	}
	return true;
}

static bool DebugPvp(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	return true;
}

static bool DebugReload( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = int(args.size());
	if(size >= 2)
	{
		std::string para = args[1];
		int type = 0;
		if(size >= 3)
		{
			type = convert<int>(args[2]);
		}
		CReloadMgr::Instance()->ReloadNew(type, para, outputMessage, 0);
	}
	return true;
}

static bool DebugTeam( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)(args.size());
	if(size >= 2)
	{
		const string op = args[1];
		if(3 == (int)args.size() && "type" == args[1])
		{
			int type = convert<int>(args[2]);
			TeamRequest req(pRole);
			req.SwitchMemType((KKSG::TeamMemberType)type);
		}
		else if(op == "getdiamondcost")
		{
			outputMessage = ToString(pRole->GetCostDiamond());
		}
		else if(size >= 3 && op == "setdiammondlimit")
		{
			outputMessage += " old = " + ToString(GetGlobalConfig().TeamDiamondCostLimit);	
			GetGlobalConfig().TeamDiamondCostLimit = convert<UINT32>(args[2]);
			outputMessage += " new = " + ToString(GetGlobalConfig().TeamDiamondCostLimit);	
		}
        else if(size >= 3 && op == "gray")
        {
            if(args[2] != "0")
            {
                GetGlobalConfig().TeamGrayServerIDs.insert(GetServerIDByRoleID(pRole->GetID()));
                outputMessage = "ms gray in";
            }
            else
            {
                GetGlobalConfig().TeamGrayServerIDs.erase(GetServerIDByRoleID(pRole->GetID()));
                outputMessage = "ms gray out";
            }
        }
		else if("myt" == op)
		{
			if(pRole->HasTeamInMs())
			{
				outputMessage = "team in local server";
			}
			else if(pRole->HasTeamInTs())
			{
				outputMessage = "team in cross server";
			}
			else
			{
				outputMessage = " have no team";
			}
		}
		else if("dragonfinish" == op)
		{
			outputMessage = " finish scene : ";
			std::string value = CGlobalConfigDBBuild::Instance()->GetConfigValue(eDragonGlobal);
			KKSG::GlobalDragonInfo info;
			if(info.ParseFromString(value))
			{
				for(int i = 0; i < info.sceneid_size(); ++i)
				{
					outputMessage += ToString(info.sceneid(i)) + " ";
				}
			}

		}
	}
	else
	{
		TeamMgr::Instance()->TestPrint();
	}
	return true;
}

static bool DebugDE( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(2 == (int)args.size() && "tp" == args[1])
	{
		DragonExpRankMgr::Instance()->TestPrint();
	}
	else if(2 == (int)args.size() && "reset" == args[1])
	{
		DragonExpRankMgr::Instance()->GiveRankReward();
		DragonExpRankMgr::Instance()->ResetGroup();
		DragonExpRankMgr::Instance()->TestPrint();
	}
	else if(2 == (int)args.size() && "reqrank" == args[1])
	{
		KKSG::DERankRes roRes;
		DragonExpRankMgr::Instance()->FillRankData(pRole->GetID(), roRes);
	}
	return true;
}

static bool DebugGuildCastle(CRole* role, std::vector<std::string>& args, std::string& output)
{
	int size = (int)args.size();
	if(size == 3 && args[1] == "make")
	{
		//UINT32 tmp = convert<UINT32>(args[2]);
		//GuildCastleMgr::Instance()->TestMakeFight(tmp);
	}
	else if(size >= 4 && args[1] == "make")
	{
		UINT32 terrID = convert<UINT32>(args[2]);
		UINT64 guildID11 = GetGuildIDByName(args[3]);
		std::set<UINT64> attacks;
		if(size >= 5)
		{
			UINT64 guildID22 = GetGuildIDByName(args[4]);
			attacks.insert(guildID22);
		}
		if(size >= 6)
		{
			UINT64 guildID33 = GetGuildIDByName(args[5]);
			attacks.insert(guildID33);
		}
		GuildCastleMgr::Instance()->AddOneFight(terrID, guildID11, attacks);
	}
	else if(size == 2 && args[1] == "make")
	{
		//UINT64 guildID11 = GetGuildIDByName("aaaa");
		//UINT64 guildID22 = GetGuildIDByName("bbbb");
		//std::set<UINT64> attacks;
		//attacks.insert(guildID22);
		//GuildCastleMgr::Instance()->AddOneFight(2, guildID11, attacks);
		//guildID11 = GetGuildIDByName("bbbb");
		//guildID22 = GetGuildIDByName("cccc");
		//attacks.clear();
		//attacks.insert(guildID22);
		//GuildCastleMgr::Instance()->AddOneFight(3, guildID11, attacks);
	}
	else if(size == 3 && args[1] == "joinready")
	{
		UINT32 tmp = convert<UINT32>(args[2]);
		GcfRequest req(role);
		req.JoinReadyScene(tmp);
	}
	else if(size == 3 && args[1] == "joinfight")
	{
		UINT32 mapID = convert<UINT32>(args[2]);
		GcfRequest req(role);
		req.JoinFightScene(mapID);
	}
	else if(size == 3 && args[1] == "readytime")
	{
		UINT32 tmp = convert<UINT32>(args[2]);
		output += " old = " + ToString(GetGlobalConfig().GuildCastleReadyTime);
		GetGlobalConfig().GuildCastleReadyTime = tmp;
		output += " new = " + ToString(GetGlobalConfig().GuildCastleReadyTime);

	}
	else if(size == 3 && args[1] == "fighttime")
	{
		UINT32 tmp = convert<UINT32>(args[2]);
		output += " old = " + ToString(GetGlobalConfig().GuildCastleFightTime);
		GetGlobalConfig().GuildCastleFightTime = tmp;
		output += " new = " + ToString(GetGlobalConfig().GuildCastleFightTime);
	}
	else if(size == 3 && args[1] == "fightrolecount")
	{
		UINT32 tmp = convert<UINT32>(args[2]);
		output += " old = " + ToString(GetGlobalConfig().GuildCastleBattleRoleMax);
		GetGlobalConfig().GuildCastleBattleRoleMax = tmp;
		output += " new = " + ToString(GetGlobalConfig().GuildCastleBattleRoleMax);
	}
	else if(size == 2 && args[1] == "clear")
	{
		GuildCastleMgr::Instance()->ClearFightData();
	}
	else if(size == 4 && args[1] == "mulpoint")
	{
		UINT32 mapID = convert<UINT32>(args[2]);
		UINT32 mul = convert<UINT32>(args[3]);
		auto pOneFight = GuildCastleMgr::Instance()->GetSceneFight(role->GetSceneID());	
		if(pOneFight)
		{
			pOneFight->TestMakeMultiPoint(mapID, mul, UINT32(time(NULL) + 10));
		}
	}
	else if(size == 5 && args[1] == "addpoint")
	{
		UINT64 guildID = GetGuildIDByName(args[2]);
		UINT32 point = convert<UINT32>(args[3]);
		UINT32 terrid = 2;
		if(size >= 5)
		{
			terrid = convert<UINT32>(args[4]);
		}
		auto pOneFight = GuildCastleMgr::Instance()->GetTerritoryFight(terrid);
		if(pOneFight)
		{
			//pOneFight->TestAddPoint(point);
			pOneFight->AddGuildPoint(guildID, point);
		}
	}
	else
	{
		GuildCastleMgr::Instance()->TestPrint();
	}

	return true;
}

static bool DebugGuildAuct(CRole* role, std::vector<std::string>& args, std::string& output)
{
	int size = (int)args.size();
	if(size >= 4 && "add" == args[1])
	{
		int acttype = convert<int>(args[2]);
		int rank = convert<int>(args[3]);
		UINT64 guildID = CGuildMgr::Instance()->GetGuildIdByRoleId(role->GetID());
		if(0 == guildID)
		{
			output = " no guild ";
			return false;
		}
		std::vector<UINT64> roles;
		auto pGuild = CGuildMgr::Instance()->GetGuild(guildID);
		if(pGuild)
		{
			const auto& members = pGuild->GetGuildMember();
			for (auto iter = members.begin(); iter != members.end(); ++iter)
			{
				if(CRoleManager::Instance()->GetByRoleID(iter->first))
				{
					roles.push_back(iter->first);
				}
			}
			/*
			for(size_t i = 0; i < members.size(); ++i)
			{
			if(CRoleManager::Instance()->GetByRoleID(members[i].qwRoleID))
			{
			roles.push_back(members[i].qwRoleID);
			}
			}
			*/
		}
		GuildAuctMgr::Instance()->AddGuildAuctData(guildID, acttype, rank, roles);
	}
	else if(size >= 3 && "gst" == args[1])
	{
		output += " oldtime = " + ToString(GetGlobalConfig().GuildAuctGuildSaleTime);
		GetGlobalConfig().GuildAuctGuildSaleTime = convert<UINT32>(args[2]);
		output += " newtime = " + ToString(GetGlobalConfig().GuildAuctGuildSaleTime);
	}
	else if(size >= 3 && "wst" == args[1])
	{
		output += " oldtime = " + ToString(GetGlobalConfig().GuildAuctWorldSaleTime);
		GetGlobalConfig().GuildAuctWorldSaleTime = convert<UINT32>(args[2]);
		output += " newtime = " + ToString(GetGlobalConfig().GuildAuctWorldSaleTime);
	}
	else if(size >= 3 && "pmax" == args[1])
	{
		/*output += " oldmax = " + ToString(GetGlobalConfig().GuildAuctProfitMax);
		GetGlobalConfig().GuildAuctProfitMax = convert<UINT32>(args[2]);
		output += " newmax = " + ToString(GetGlobalConfig().GuildAuctProfitMax);*/
	}
	else if(size >= 2 && "clear" == args[1])
	{
		GuildAuctMgr::Instance()->TestClear();
	}
	else
	{
		GuildAuctMgr::Instance()->TestPrint();
	}
	return true;
}

static bool DebugAuction(CRole* role, std::vector<std::string>& args, std::string& output)
{
	if((int)args.size() >= 2)
	{
		if("allsale" == args[1])
		{
			AuctionMgr::Instance()->TestPrintAllSaleItem();
		}
		else if("mysale" == args[1])
		{
			AuctionMgr::Instance()->TestPrintMySaleItem(role->GetID());
		}
		else if((int)args.size() >= 3 && "setonsalet" == args[1])
		{
			output = " old time = " + ToString( GetGlobalConfig().AuctOnSaleTime);
			GetGlobalConfig().AuctOnSaleTime = convert<UINT32>(args[2]);	
			output += " new time = " + ToString( GetGlobalConfig().AuctOnSaleTime);
		}
		else if((int)args.size() >= 4 && "setdelay" == args[1])
		{
			output = " old time min = " + ToString( GetGlobalConfig().AuctSaleDelayMin) + " max = " + ToString( GetGlobalConfig().AuctSaleDelayMax);
			GetGlobalConfig().AuctSaleDelayMin = convert<UINT32>(args[2]);
			GetGlobalConfig().AuctSaleDelayMax = convert<UINT32>(args[3]);
			output += " new time min = " + ToString( GetGlobalConfig().AuctSaleDelayMin) + " max = " + ToString( GetGlobalConfig().AuctSaleDelayMax);
		}
		else if((int)args.size() >= 3 && "setrt" == args[1])
		{
			output = " old time = " + ToString( GetGlobalConfig().AuctRecoverTime);
			GetGlobalConfig().AuctRecoverTime = convert<UINT32>(args[2]);
			output += " new time = " + ToString( GetGlobalConfig().AuctRecoverTime);
		}
		else if((int)args.size() >= 2 && "refresh" == args[1])
		{
			AuctionRequest req(role);
			KKSG::AuctionAllReqRes roRes;
			req.ReqFreeRefresh(roRes);
		}
		else if((int)args.size() >= 4 && "setcount" == args[1])
		{
			output = " old all count = " + ToString( GetGlobalConfig().AuctCartItemAllCount) + " treasure percent = " + ToString( GetGlobalConfig().AuctCartItemTreasurePercent);
			GetGlobalConfig().AuctCartItemAllCount = convert<int>(args[2]);
			GetGlobalConfig().AuctCartItemTreasurePercent = convert<int>(args[3]);
			GetGlobalConfig().AuctCartItemTreasureCount = GetGlobalConfig().AuctCartItemTreasurePercent * GetGlobalConfig().AuctCartItemAllCount / 100;
			GetGlobalConfig().AuctCartItemNormalCount = GetGlobalConfig().AuctCartItemAllCount - GetGlobalConfig().AuctCartItemTreasureCount;
			output += " new all count = " + ToString( GetGlobalConfig().AuctCartItemAllCount) + " treasure percent = " + ToString( GetGlobalConfig().AuctCartItemTreasurePercent); 
		}
		else if((int)args.size() >= 3 && "weeklimit" == args[1])
		{
			output = " old limit = " + ToString( GetGlobalConfig().AuctWeekTradeLimit);
			GetGlobalConfig().AuctWeekTradeLimit = convert<UINT32>(args[2]);
			output += " new limit = " + ToString( GetGlobalConfig().AuctWeekTradeLimit);
		}
		else if((int)args.size() >= 2 && "record" == args[1])
		{
			auto pRecord = AuctionMgr::Instance()->GetATradeRecord();
			if(pRecord)
			{
				pRecord->TestPrint();
			}
		}
		else if((int)args.size() >= 3 && "rt" == args[1])
		{
			output = " old = " + ToString( GetGlobalConfig().AuctTradeRecordTime);
			GetGlobalConfig().AuctTradeRecordTime = convert<UINT32>(args[2]);
			output += " new = " + ToString( GetGlobalConfig().AuctTradeRecordTime);
		}
		else
		{
			GuildAuctMgr::Instance()->TestPrint();
		}
	}
	return true;
}

static bool DebugFirstPassClear( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2 || (args.size() > 2 && "clear" != args[1]))
	{
		outputMessage = "ranklist's name + clear";
		return false;
	}
	CRankListMgr::Instance()->GMClearFirstPass();
	outputMessage = "clear First Pass ranklist OK";
	return true;
}

static bool DebugClearChatNotify( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = "clearchat";
		return false;
	}
	std::string strName = args[1];
	CRoleSummary* pSum =  CRoleSummaryMgr::Instance()->GetRoleSummary(strName);
	if (NULL == pSum)
	{
		outputMessage = "failed,invalid name.";
	}
	else
	{
		PtcM2C_IdipClearChatNtf ptc;
		ptc.m_Data.set_roleid(pSum->GetID());
		pRole->Send(ptc);
		outputMessage = "set clearchat ok";
	}
	return true;
}
static bool DebugCheckIdipAndNotify( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = "checkidip type";
		return false;
	}
	INT32 iType = convert<UINT32>(args[1]);
	if (iType != KKSG::PUNISH_USER_DAILY_PLAY && iType != KKSG::PUNISH_USER_MULTI_ACTIVITY)
	{
		outputMessage = "failed,invalid type.";
	}
	else
	{
		pRole->CheckIdipAndNotify(iType);
		outputMessage = "set checkidip ok";
	}
	return true;
}

static bool DebugAddKillGoblin( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "error, format: addkillgoblin + num, example: addkillgoblin 2";
		return false;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (!pGuild)
	{
		return false;
	}

	pGuild->Get<GuildGoblin>()->addKillNum(atoi(args[1].c_str()));

	outputMessage = "succeed";
	return true;
}

static bool DebugSetGuildCheckInNum(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2)
	{
		outputMessage = "set checkin num error, DebugSetGuildCheckInNum count";
		return false; 
	}
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (!guild)
	{
		outputMessage = "not in guild";
		return false;
	}

	guild->Get<CGuildCheckin>()->DebugSetCount(atoi(args[1].c_str()));
	outputMessage = "set checkin num ok";
	return true;
}

static bool DebugPk(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)args.size();
	if(size >= 3 && args[1] == "tow")
	{
		outputMessage = " old = " + ToString(GetGlobalConfig().PkMatchToWorldTime);
		GetGlobalConfig().PkMatchToWorldTime = convert<UINT32>(args[2]);
		outputMessage += " new = " + ToString(GetGlobalConfig().PkMatchToWorldTime);
	}
	else if(size >= 2 && args[1] == "reset")
	{
		PkMgr::Instance()->TestResetLastWeek();
	}
	else if(size >= 2 && args[1] == "match")
	{
		MatchMgr::Instance()->Update();
	}
	else if(size >= 2 && args[1] == "limitp")
	{
		PkMgr::Instance()->GetRoleOpRecMgr().InfoPrint();
	}
	else if(size >= 2 && args[1] == "limitc")
	{
		PkMgr::Instance()->GetRoleOpRecMgr().Clear();
	}
	else if(size >= 2 && args[1] == "rlog")
	{
		PkMgr::Instance()->PkRankWrite((PkRealTimeRankList*)CRankListMgr::Instance()->GetRankList(KKSG::PkRealTimeRank), GameTime::GetTime());
	}
	else
	{
		PkMgr::Instance()->TestPrint();
	}

	return true;
}


static bool DebugBigMelee(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	MayhemMsMgr::Instance()->TestTask();
	return true;
}

static bool DebugBattleFieldSim(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2)
	{
		outputMessage = "set checkin num error, DebugBattleFieldSim count";
		return false; 
	}
	BattleFieldMgr::Instance()->TestTask(atoi(args[1].c_str()));
	return true;
}

static bool DebugGoalAwards(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
    if (args.size() != 3 && args.size() != 4)
    {
        outputMessage = "goalaward + id + num + [gkid]";
        return false;
    }
    UINT32 id = convert<UINT32>(args[1]);
    double num = convert<double>(args[2]);
    UINT32 gkid = 0;
    if(args.size() == 4)
    {
        gkid = convert<UINT32>(args[3]);
    }
    GoalAwardsMgr::Instance()->FreshValue(pRole->GetID(), id, num, gkid);
    return true;
}

static bool DebugGoalAwardsClear(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
    GoalAwardsMgr::Instance()->GMClearDB(pRole->GetID());
    return true;
}

static bool DebugSurvive(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = int(args.size());
	if(size >= 2)
	{
		if(args[1] == "weekzero")
		{
			CSurviveMgr::Instance()->WeekZero();
		}
		else if(args[1] == "reward" && size >= 3)
		{
			UINT32 rank = convert<UINT32>(args[2]);
			CSurviveMgr::Instance()->GiveReward(pRole->GetID(), rank);
		}
		else if(args[1] == "rank" && size >= 3)
		{
			UINT32 point = convert<UINT32>(args[2]);
			SurviveRankList* pList = CSurviveMgr::Instance()->GetRankList();
			if(pList)
			{
				KKSG::SurviveRankData data;
				data.set_roleid(pRole->GetID());
				data.set_rolename(pRole->GetName());
				data.set_point(point);
				data.set_serverid(MSConfig::Instance()->GetServerID());

				pList->OnChange(data);

				pList->InfoPrint();
			}
		}
	}
	else
	{
		CSurviveMgr::Instance()->InfoPrint();	
	}
	return true;
}

static bool DebugNpcFeeling(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	static KKSG::NpcFlGlobal data;
	int size = (int)args.size();
	if(size >= 2)
	{
		if(args[1] == "weekzero")
		{
			CNpcFeelingMgr::Instance()->WeekZero();
		}
		else if(args[1] == "dayzero")
		{
			CNpcFeelingMgr::Instance()->DayZero();
		}
		else if(args[1] == "clearglobal")
		{
			CNpcFeelingMgr::Instance()->Clear();
		}
		else if(args[1] == "debugmerge")
		{
			CNpcFeelingMgr::Instance()->DebugMergeData(data);
		}
		else if(args[1] == "debugamend")
		{
			CNpcFeelingMgr::Instance()->DebugAmendData(data);
		}
		else if(args[1] == "debugprint")
		{
			CNpcFeelingMgr::Instance()->InfoPrint(data);
		}
		else if(size >= 3 && args[1] == "debugup")
		{
			UINT32 exp = convert<UINT32>(args[2]);
			if(size >= 4)
			{
				UINT32 npcID = convert<UINT32>(args[3]);
				KKSG::NpcFlNpc2Role npc2role;
				npc2role.set_npcid(npcID);
				npc2role.mutable_role()->set_roleid(pRole->GetID());
				npc2role.mutable_role()->set_exp(exp);
				npc2role.set_rolename(pRole->GetName());
				CNpcFeelingMgr::Instance()->UpdateNpc2Role(npc2role);
			}
			else
			{
				KKSG::NpcFlRoleExp roleExp;
				roleExp.set_roleid(pRole->GetID());
				roleExp.set_exp(exp);
				CNpcFeelingMgr::Instance()->UpdateTopAvExp(roleExp);
			}
		}
	}
	else
	{
		CNpcFeelingMgr::Instance()->InfoPrint();
	}
	return true;
}

static bool DebugMatch(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)args.size();	
	if(size >= 4)
	{
		KKSG::KMatchCommonArg roArg;
		KKSG::KMatchType type = (KKSG::KMatchType)(convert<int>(args[1]));
		KKSG::KMatchOp op = (KKSG::KMatchOp)(convert<int>(args[2]));
		roArg.set_type(type);
		roArg.set_op(op);
		roArg.set_isteam(args[3] != "0");
		if(pRole->HasTeamInTs())
		{
			if(TeamLink::Instance()->IsConnected())
			{
				TeamTransHandler trans;
				trans.HandleMatchReqFromC(0, pRole, roArg);
			}
			else
			{
			}
			return true;
		}
		
		//KKSG::KMatchRole data;
		//data.mutable_pkrec()->set_point(3333);
		//MatchRequest req(pRole);
		//req.HandleReq(roArg, data);
	}
	return true;
}

static bool DebugDragonGuild(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)args.size();
	DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (pDGuild == NULL)
	{
		outputMessage = "Role is not in dragonguild, enter first.";
		return false;
	}
	if (size >= 3 && args[1] == "addexp")
	{
		int addExp = convert<INT32>(args[2]);
		if (addExp <= 0 )
		{
			outputMessage = "Exp is less than 0, fail";
			return false;
		}
		outputMessage = "level: " + ToString(pDGuild->GetLevel());
		pDGuild->AddExp(addExp);
		outputMessage += " AfterLevel: " + ToString(pDGuild->GetLevel());
		return true;
	}
	else if(size == 3 && args[1] == "liveness")
	{
		int value = convert<INT32>(args[2]);
		if(value< 0)
		{
			outputMessage = "args [value] invalid";
			return false;
		}
		DragonGuildLiveness& liveness = pDGuild->GetDragonGuildLiveness();
		liveness.AddPartnerLiveness(pRole->GetID(), 24, value);
	}
	else if(size == 5 && args[1] == "finishcount" )
	{
		DragonGuildTask& task = pDGuild->GetDragonGuildTask();
		int taskid = convert<INT32>(args[3]);
		int count = convert<INT32>(args[4]); 
		KKSG::DragonGuildTaskType type;
		if(args[2] == "task")
			type = KKSG::TASK_NORMAL;
		else if(args[2] == "achieve")
			type = KKSG::TASK_ACHIVEMENT;
		else 
		{
			outputMessage = "useage:  dragonguild addfinishcount  task/achieve [taskid] [count]";
			return false;
		}
		count = count > 1000?1000:count;
		for(int i=0;i<count;++i)
			task.AddTaskFinishCount(type, taskid);
	}
	else if (size ==  4 && args[1] == "passscenecount")
	{
		int sceneID = convert<INT32>(args[2]);
		if(sceneID < 1100 || sceneID > 1125)
		{
			outputMessage = "support seaDragon, greenDragon, desertDragon only\n";
			return false;
		}
		int count = convert<INT32>(args[3]);
		count = count > 1000?1000:count;
		for(size_t i = 0; i<count; ++i)
			pDGuild->AddPassSceneCount(sceneID);
	}
	else if(size == 3 && args[1] == "modifyguildname")
	{
		DragonGuildMgr::Instance()->ModifyName(pDGuild, pDGuild->GetName(), args[2]);
		pDGuild->SetName(args[2]);
		const MailConf* pConf = MailConfig::Instance()->GetMailConf(MailConf_DragonGuildModifyName);
		if (pConf!=NULL)
		{
			char szBuf[64] = {0};
			string szContent = pConf->m_content;
			string szTitle   = pConf->m_title;
			string szMailContent = XCommon::ReplaceStr(szContent, "%s", args[2]);
			std::vector<ItemDesc> item;
			MailData mail( pConf->m_title, szMailContent, item, MAIL_EXPIRE_TIME);	
			pDGuild->BroadCastMail(mail);

			outputMessage += "modifyname success.";
		}
	}
	else if(size == 2 && args[1] == "beleader")
	{
		DragonGuildMember* m = pDGuild->GetMember(pRole->GetID());
		if (m)
		{
			pDGuild->ChangeMemberPosition(m, DGPOS_LEADER);
			outputMessage += "ChangePosition Success.";
		}
		else
		{
			outputMessage += "GetMember failed.";
		}
	}
	else 
	{
		outputMessage  =  "useage:\n";
		outputMessage +=  "dragonguild addexp          [expValue]\n";
		outputMessage +=  "dragonguild liveness     [Value]\n";
		outputMessage +=  "dragonguild passscenecount           [sceneID] [count]\n";
		outputMessage +=  "dragonguild finishcount  task/achieve [taskid] [count]\n";
		outputMessage +=  "dragonguild modifyguildname [newguildname]\n";
		outputMessage +=  "dragonguild beleader";
		return false;
	}

	return true;
}

static bool DebugArenaStar(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)args.size();
	if(size >= 2)
	{
		const std::string op = args[1];
		if(size >= 3 && op == "end")
		{
			KKSG::ArenaStarType type = (KKSG::ArenaStarType)(convert<int>(args[2]));
			ArenaStarRankP rankP = ArenaStarMgr::Instance()->GetTypeRankP(type);
			UINT64 ID = 0;
			if(type == KKSG::AST_LEAGUE)
			{
				ID = LeagueTeamMgr::Instance()->GetLeagueTeamIdByRoleId(pRole->GetID());
			}
			ArenaStarMgr::Instance()->OnArenaSeasonEnd(type, ID);
			outputMessage = " ok : old rank not clear ";
		}
		else if(size >= 3 && op == "reward")
		{
			KKSG::ArenaStarType type = (KKSG::ArenaStarType)(convert<int>(args[2]));
			ArenaStarMgr::Instance()->GiveDianZanRewardGs(pRole, type);
		}
		else if(op == "ch")
		{
			ArenaStarMgr::Instance()->AddChangeRole(pRole->GetID());
		}
		else if(op == "wdb")
		{
			ArenaStarMgr::Instance()->OnTimer(1,1);
		}
		else if(size >= 3 && op == "wl")
		{
			GetGlobalConfig().ArenaStarSaveDBLimit = convert<UINT32>(args[2]);
			outputMessage = "wl = " + ToString(GetGlobalConfig().ArenaStarSaveDBLimit);
		}
		else if(size >= 3 && op == "p")
		{
			KKSG::ArenaStarType type = (KKSG::ArenaStarType)(convert<int>(args[2]));
			ArenaStarRankP rankP = ArenaStarMgr::Instance()->GetTypeRankP(type);
			rankP.pCurRank->InfoPrint();
			rankP.pLastRank->InfoPrint();
		}
		else if(size >= 3 && "leaguerank" == op)
		{
			int baseValue = convert<int>(args[2]);

			auto pRankList = (LeagueRankList*)CRankListMgr::Instance()->GetRankList(KKSG::LeagueTeamRank);
			auto pLeagueTeam = LeagueTeamMgr::Instance()->GetLeagueTeamByRoleId(pRole->GetID());
			if(pLeagueTeam && pRankList)
			{
				pRankList->OnChange(pLeagueTeam->GetId(), 1 + baseValue, 2.0 + baseValue, time(NULL), 3 + baseValue, 4 + baseValue);
				outputMessage = "success";
			}
		}
		else if(op == "zero")
		{
			ArenaStarMgr::Instance()->Data2Zero();
		}
	}
	else
	{
		ArenaStarMgr::Instance()->InfoPrint();
	}
	return true;
}

static bool DebugGmf(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)args.size();
	if(size >= 2)
	{
		if(size >= 5 && args[1] == "makeroom")
		{
			int type = convert<int>(args[2]);
			if(GMF_RT_GPR != type && GMF_RT_GMF != type)
			{
				outputMessage = "type = 1 or = 2"; 
				return false;
			}
			UINT64 gid11 = GetGuildIDByName(args[3]);
			UINT64 gid22 = GetGuildIDByName(args[4]);
			if(0 == gid11)
			{
				outputMessage = " find guild " + args[3] + " failed ";
				return false;
			}
			if(0 == gid22)
			{
				outputMessage = " find guild " + args[4] + " failed ";
				return false;
			}
			GuildMatchFightMgr::Instance()->TestMakeFight(pRole, type, gid11, gid22);
		}
		else if(args[1] == "sc")
		{
			UINT32 gamenum = 2;
			if(size == 3)
			{
				gamenum = convert<UINT32>(args[2]);
			}
			GuildMatchFightMgr::Instance()->TestBegin(gamenum);
		}
		else if(args[1] == "join")
		{
			GuildMatchFightMgr::Instance()->JoinRequest(pRole);
		}
		else if(args[1] == "up")
		{
			GuildMatchFightMgr::Instance()->TestUpdate();
		}
	}
	else
	{
		GuildMatchFightMgr::Instance()->TestPrint();
	}

	return true;
}

static bool DebugSetGuildEvent(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (!pGuild)
	{
		outputMessage = "not in guild";
		return false;
	}
	for (UINT32 i = 1;i<=9;i++)
	{
		EventMgr::Instance()->AddGuildEvent(pGuild, DESIGNATION_COM_TYPE_GUILD_ACTIVITY_RANK,i);
		EventMgr::Instance()->TestSave();
	}

	return true;
}


static bool DebugFlowerAch(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	SocialMgr::Instance()->OnFlowerEvent();
	return true;
}

static bool DebugGlobalEvent(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)args.size();
	if (size<3)
	{
		return  false;
	}
	INT32 iEventID = convert<UINT32>(args[1]);
	INT32 iParam1 = convert<UINT32>(args[2]);

	EventMgr::Instance()->AddEvent(pRole->GetID(), iEventID,iParam1); 
	return true;
}


static bool DebugSetEvent(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	EventMgr::Instance()->AddEvent(pRole->GetID(), DESIGNATION_COM_TYPE_FLOWER_RANK_YESTERDAY);
	EventMgr::Instance()->TestSave();
	return true;
}





static bool SetGuildArenaTime( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if (args.size() < 16)
	{
		return false;
	}

	GetGlobalConfig().GuildArenaTimeVec.reserve(1);
	for (UINT32 i = 1; i <= 15; ++i)
	{
		GetGlobalConfig().GuildArenaTimeVec[0][i - 1] = convert<UINT32>(args[i]);
	}

	return true;
}

static bool DissGuild( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if (args.size() < 1)
	{
		return false;
	}
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		return true;
	}
	CGuildMgr::Instance()->AddDismissGuild(pGuild);
	return true;
}


static bool SetGuildDareTime( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if (args.size() < 1)
	{
		return false;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild != NULL)
	{
		pGuild->AddDareTimes(convert<UINT32>(args[1]));
	}

	return true;
}

static bool SetGuildLadderTime( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if (args.size() < 4)
	{
		return false;
	}

	GetGlobalConfig().GuildLadderTime.begin()->seq[0] = convert<UINT32>(args[1]); 
	GetGlobalConfig().GuildLadderTime.begin()->seq[1] = convert<UINT32>(args[2]); 
	GetGlobalConfig().GuildLadderTime.begin()->seq[2] = convert<UINT32>(args[3]); 
	SSWarn<<"the ladder time:"<<GetGlobalConfig().GuildLadderTime.begin()->seq[0]<<GetGlobalConfig().GuildLadderTime.begin()->seq[1]<<GetGlobalConfig().GuildLadderTime.begin()->seq[2]<<END;
	return true;
}

static bool addprestige( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if (args.size() < 2)
	{
		return false;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild != NULL)
	{
		pGuild->AddPrestige(convert<UINT32>(args[1]));
	}

	return true;
}

static bool DebugDelRole(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	CAccountKicker::Instance()->KickAccount(pRole->GetID(), KKSG::KICK_DEL_ROLE, &KickAccountDelRoleListener::Listener);
	return true;
}

static bool DebugClearBind(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (!guild)
	{
		SSWarn << "no guidl" << END;
		return true;
	}

	GuildBindInfo& info = guild->GetBindGroupInfo();
	info.SetQQGroupCode("");
	info.SetQQGroupName("");
	info.SetQQGroupOpenId("");
	SSInfo << "GM unbind" << END;
	return true;
}

static bool DebugAddGuildExp(CRole* role, std::vector<std::string>& args, std::string& outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " exp";
		return false;
	}

	UINT32 exp = convert<UINT32>(args.at(1));
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (NULL != guild)
	{
		guild->AddExp(exp);
	}

	return true;
}

static bool DebugCross(CRole* role, std::vector<std::string>& args, std::string& outputMessage)
{
	//CSceneSwitch::EnterCrossHall(role);
	RpcM2K_GetSceneInfo* rpc = RpcM2K_GetSceneInfo::CreateRpc();
	rpc->m_sessionID = role->GetSessionID();
	WorldLink::Instance()->SendTo(*rpc);
	return true;
}

static bool CreateSkyCityWait( CRole *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if (args.size() < 2)
	{
		return false;
	}
	UINT32	nState = convert<UINT32>(args[1]); 
	SkyCityMgr::Instance()->SetStep(nState);
	SkyCityMgr::Instance()->SetNextTime();
	return true;
}

static bool SetSkyCityGames(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage )
{

	SkyCityMgr::Instance()->SetGames(0);
	SkyCityMgr::Instance()->SetState(1);
	return true;
}

static bool EnterSkyCityRoom(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	SkyCityMgr::Instance()->EnterWaitRoom(pRole);
	return true;
}

static bool DebugSimSkyCicy(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	//拉主城的玩家
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild)
	{
		Notice guildnotice(SkyCityWinGuild);
		guildnotice.Replace("$R",pRole->GetName());
		guildnotice.SendGuild(pGuild->GetID());
	}
	return true;
}


static bool DebugTestRuoJi(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	IBGiftMgr::Instance()->TestGiftIBSucessNtf(pRole);
	return true;
}



static bool DebugGuildCardMatchBegin(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (NULL == guild)
	{
		outputMessage = "not in guild";
		return false;
	}
	if (guild->Get<CGuildCard>()->BeginMatch())
	{
		return true;
	}
	else
	{
		return false;
	}
}

static bool DebugGuildCardMatchEnd(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (NULL == guild)
	{
		outputMessage = "not in guild";
		return false;
	}
	guild->Get<CGuildCard>()->EndMatch();
	return true;
}

static bool DebugGuildCardMatchAdd(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (NULL == guild)
	{
		outputMessage = "not in guild";
		return false;
	}
	if (guild->Get<CGuildCard>()->AddMatch(pRole->GetID()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

static bool DebugFlowerDayPass(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 1)
	{
		outputMessage = "flowerdaypass error";
		return false; 
	}

	SocialMgr::Instance()->DebugDayPass();

	outputMessage = "flowerweekpass ok";
	return true;
}

static bool DebugOpenQA(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size()<2)
		return false; 
	UINT32 qaType = convert<UINT32>(args[1]);
	QAMgr::Instance()->GMOpenQA(qaType);
	return true;
}

static bool DebugFlowerWeekPass(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 1)
	{
		outputMessage = "flowerweekpass error";
		return false; 
	}

	SocialMgr::Instance()->DebugWeekPass();

	outputMessage = "flowerweekpass ok";
	return true;
}

static bool DebugAddDegree(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 3)
	{
		outputMessage = "adddegree name value";
		return false; 
	}
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(args[1]);
	if (NULL != summary)
	{
		FriendOp op(pRole->GetID());
		op.SetOther(summary->GetID());
		int value = convert<int>(args[2]);
		op.AddDegree(0, value);
	}

	outputMessage = "add ok";
	return true;
}

static bool DebugCleanDegree(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2)
	{
		outputMessage = "cleandegree name";
		return false; 
	}
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(args[1]);
	if (NULL != summary)
	{
		FriendOp op(pRole->GetID());
		op.SetOther(summary->GetID());
		op.CleanDegree();
	}

	outputMessage = "clean ok";
	return true;
}

static bool DebugArrangeTeamCard(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 1)
	{
		outputMessage = "arrangeteamcard";
		return false; 
	}

	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (NULL != guild)
	{
		guild->Get<CGuildCard>()->ArrangeRecord();
	}

	outputMessage = "ok";
	return true;
}

static bool DebugTeamOp(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(3 == (int)args.size() && args[1] == "create")
	{
		std::vector<CRole*> roles;
		for(auto it = CRoleManager::Instance()->Begin(); it != CRoleManager::Instance()->End(); ++it)
		{
			roles.push_back((*it));	
		}
		int expid = convert<int>(args[2]);
		TeamForceOp op;
		auto pTeam = op.CreateTeam(roles, expid);
		if(NULL == pTeam)
		{
			outputMessage = "failed";
		}
		else
		{
			outputMessage = "teamid = " + ToString(pTeam->GetID());
		}
	}
	else if(2 == (int)args.size() && args[1] == "join")
	{
		Team* pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
		if(NULL == pTeam)
		{
			outputMessage = " donot find team id = " + ToString(pRole->GetTeamID());
			return true;
		}
		std::vector<CRole*> roles;
		for(auto it = CRoleManager::Instance()->Begin(); it != CRoleManager::Instance()->End(); ++it)
		{
			if((*it) == pRole || (*it)->GetTeamID())
			{
				continue;
			}
			roles.push_back((*it));	
		}
		TeamForceOp op;
		op.JoinTeam(pTeam->GetID(), roles);	
	}
	return true;
}

static bool DebugOpenAct(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 4)
	{
		outputMessage = "usage: setact 1 1130 1140";
		return false;
	}
	UINT32 actId = convert<UINT32>(args[1]);
	UINT32 startTime = convert<UINT32>(args[2]);
	UINT32 endTime = convert<UINT32>(args[3]);
	UINT32 startSec = startTime / 100 * 3600 + startTime % 100 * 60;
	UINT32 endSec = endTime / 100 * 3600 + endTime % 100 * 60;

	MultActivityMgr::Instance()->GMAddOpenDayTime(actId, startSec, endSec);
	return true;
}

static bool DebugResetAct(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "usage: resetact 1";
		return false;
	}
	UINT32 actId = convert<UINT32>(args[1]);
	MultActivityMgr::Instance()->ResetConfig(actId);
	return true;
}

static bool DebugPayCreate(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "usage: paycreate para";
		return false;
	}
	/*std::string req = args[1];
	KKSG::OrderCreateArg oData;
	oData.set_pf("tencent");
	oData.set_area(MSConfig::Instance()->GetServerID());
	oData.set_uin(ToString(pRole->GetID()));
	oData.set_uid(pRole->GetAccount());
	oData.set_req(req);
	OrderCreateTask* pTask = new OrderCreateTask();
	pTask->SetRpcId(1);
	pTask->BuildPostData(oData);
	HttpTaskMgr::Instance()->AddTask(pTask);*/
	return true;
}
static bool DebugPayNotify(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	PayNotifyTask* pTask = new PayNotifyTask();
	pTask->SetParamData(pRole->GetPayParameterInfo());
	pTask->SetRoleId(pRole->GetID());
	CPayMgr::Instance()->BuildHttpParam(pRole, pTask->m_oparams);
	CPayMgr::Instance()->BuildSign(pRole, pTask->m_oparams, GetBalanceUrlPath);
	//CPayMgr::Instance()->PayBegin(roArg.type(), roArg.paramid(), roArg.amount(), role->GetID());
	HttpTaskMgr::Instance()->AddTask(pTask);
	outputMessage = "set ok";
	return true;
}

static bool DebugPayParamNtf(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	PtcM2C_PayParameterInfoInvalidNtf ptc;
	pRole->Send(ptc);
	outputMessage = "set ok";
	return true;
}

static bool DebugSetPayCheck(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "usage: setpaycheck 0/1";
		return false;
	}
	UINT32 Id = convert<UINT32>(args[1]);
	bool isSet = Id == 0?true:false;
	CPayMgr::Instance()->SetGMPayCheckRole(isSet);
	return true;
}

static bool DebugMsReportData(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	CReportDataTask* poTask = new CReportDataTask(pRole);
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
	return true;
}

static bool DebugGetSubscribeList(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	CGetListSubscribeTask* poTask = new CGetListSubscribeTask();
	poTask->SetToken(pRole->GetToken());
	poTask->SetOpenId(pRole->GetAccount());
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
	return true;
}
static bool DebugSetSubscribe(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 3)
	{
		outputMessage = "usage: setsub para";
		return false;
	}
	INT32 op = convert<INT32>(args[1]);
	INT32 id = convert<INT32>(args[2]);
	CSetSubscribeTask* poTask = new CSetSubscribeTask();
	poTask->SetToken(pRole->GetToken());
	poTask->SetOpenId(pRole->GetAccount());
	KKSG::SetSubscirbeArg oData;
	oData.add_msgid(id);
	oData.set_msgtype(op);
	oData.set_token(pRole->GetToken());
	poTask->BuildPostData(oData);
	HttpTaskMgr::Instance()->AddTask(poTask);
	return true;
}

static bool DebugPushSubscribe(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "usage: pushsub para";
		return false;
	}
	INT32 id = convert<INT32>(args[1]);
	CPushSubscribeTask* poTask = new CPushSubscribeTask();
	poTask->BuildPostData(id);
	HttpTaskMgr::Instance()->AddTask(poTask);
	return true;
}

static bool DebugClickNotice(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 5)
	{
		outputMessage = "usage: clicknotice type id areaid platid";
		return false;
	}
	UINT32 type = convert<INT32>(args[1]);
	UINT32 id = convert<INT32>(args[2]);
	UINT32 areaid = convert<INT32>(args[3]);
	UINT32 platid = convert<INT32>(args[4]);
	KKSG::PlatNotice* pData =  CPlatNoticeMgr::Instance()->FindNotice(type, areaid, platid, id);
	if (pData)
	{
		PtcM2G_ClickNoticeNtf ptc;
		ptc.m_Data.mutable_notice()->CopyFrom(*pData);
		ptc.m_Data.set_roleid(pRole->GetID());
		GSLink::Instance()->SendToLine(pRole->GetGsLine(), ptc);
		outputMessage = "set ok";
	}
	return true;
}

static bool DebugPayItemBill(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "usage: payitembill billno";
		return false;
	}
	std::string strBillNo = args[1];
	KKSG::PayGiftIbItem* pIbItem = IBGiftMgr::Instance()->GetIbItem(strBillNo);
	if (NULL == pIbItem || pIbItem->status() == PayTokenStatus_AddItem || pIbItem->status() == PayGiftIbStatus_SuccessNotAddItem)
	{
		outputMessage = "invalid billno";
		return false;
	}
	pIbItem->set_status(PayGiftIbStatus_SuccessNotAddItem);//
	IBGiftMgr::Instance()->Save(MYSQL_OP_UPDATE,*pIbItem);
	IBGiftMgr::Instance()->GiftItem(*pIbItem);
	outputMessage = "set ok";
	return true;
}

static bool DebugSetPay(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "usage: setpay num";
		return false;
	}
	UINT32 num = convert<UINT32>(args[1]);
	stPayAccessInfo* pPayInfo = CPayMgr::Instance()->GetPayAccessInfo(pRole->GetID());
	if (pPayInfo)
	{
		pPayInfo->m_iSaveamt = num;
		CPayMgr::Instance()->SaveData2DB(pRole->GetID(), *pPayInfo);
		outputMessage = "set ok";
	}
	return true;
}


static bool DebugSetItemBill(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "usage: setitembill billno";
		return false;
	}
	std::string strBillNo = args[1];
	KKSG::PayGiftIbItem* pIbItem = IBGiftMgr::Instance()->GetIbItem(strBillNo);
	if (NULL == pIbItem)
	{
		outputMessage = "invalid billno";
		return false;
	}
	pIbItem->set_status(PayGiftIbStatus_SuccessNotAddItem);//
	IBGiftMgr::Instance()->Save(MYSQL_OP_UPDATE,*pIbItem);
	IBGiftMgr::Instance()->GiftItem(*pIbItem);
	outputMessage = "set ok";
	return true;
}


static bool DebugAddItemBill(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "usage: additembill roleid";
		return false;
	}
	UINT64 nRole  = convert<UINT64>(args[1]);
	CRole *pToRole = CRoleManager::Instance()->GetByRoleID(nRole);
	if (pToRole == NULL)
	{
		outputMessage = "usage: additembill role offline";
		return false;
	}
	IBGiftMgr::Instance()->AddSimOrder(pRole,pToRole);
	return true;
}



static bool DebugGarden(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	Garden* garden = GardenManager::Instance()->GetGarden(pRole->GetID(),pRole->GetName());
	if (NULL == garden)
	{
		//roRes.set_result(KKSG::ERR_GARDEN_TOLOADING);
		return false;
	}
	if( "cooklevel" == args[1])
	{
		if (args.size()  !=  3)
		{
			return false;
		}
		UINT32 level  = convert<UINT32>(args[2]);
		garden->SetCookLevel(level);
        GoalAwardsMgr::Instance()->FreshValue(pRole->GetID(), GOAL_AWARD_ID_801, level, 0);
	}
	else if( "cookexp" == args[1])
	{
		if (args.size()  !=  3)
		{
			return false;
		}
		UINT32 experiences  = convert<UINT32>(args[2]);
		garden->SetCookExperiences(experiences);
	} 
	else if( "fishlevel" == args[1])
	{
		if (args.size()  !=  3)
		{
			return false;
		}
		UINT32 level  = convert<UINT32>(args[2]);
		garden->SetFishLevel(level);
        GoalAwardsMgr::Instance()->FreshValue(pRole->GetID(), GOAL_AWARD_ID_803, level, 0);
	}
	else if( "fishexp" == args[1])
	{
		if (args.size()  !=  3)
		{
			return false;
		}

		UINT32 experiences  = convert<UINT32>(args[2]);
		garden->SetFishExperiences(experiences);
	}
	else if ("farmland"== args[1] )
	{
		if (args.size()  !=  4)
		{
			return false;
		}

		UINT32 farmland_id  = convert<UINT32>(args[2]);
		UINT32 amount  = convert<UINT32>(args[3]);
		garden->AddGrownAmount(farmland_id,amount);
	}
	else if ("guildfarmland"== args[1] )
	{
		if (args.size()  !=  4)
		{
			return false;
		}

		Guild *pGuild = NULL;
		UINT64 gid = CGuildMgr::Instance()->GetRoleGuild(pRole->GetID());
		if (gid == INVALID_GUILDID)
		{
			return false;
		}
		pGuild  = CGuildMgr::Instance()->GetGuild(gid);
		if (pGuild == NULL)
		{
			LogWarn("not find the guild garden,role id is : %llu",pRole->GetID());						
			return false;
		}
		
		GuildGarden* guild_garden = GardenUtility::Instance()->GetGuildGarden(pGuild->GetID());
		if (NULL == guild_garden)
		{
			LogWarn("not find the guild garden,guild id is : %llu",pGuild->GetID());						
			return false;
		}

		UINT32 farmland_id  = convert<UINT32>(args[2]);
		UINT32 amount  = convert<UINT32>(args[3]);
		guild_garden->AddGrownAmount(farmland_id,amount);
	}
	else if ("cancelplant" == args[1])
	{
		if (args.size()  !=  3)
		{
			return false;
		}

		UINT32 farmland_id  = convert<UINT32>(args[2]);		
		garden->CanclePlant(farmland_id);
	}
	else if ("cancleguildplant" == args[1])
	{
		if (args.size()  !=  3)
		{
			return false;
		}

		Guild *pGuild = NULL;
		UINT64 gid = CGuildMgr::Instance()->GetRoleGuild(pRole->GetID());
		if (gid != INVALID_GUILDID)
		{
			pGuild  = CGuildMgr::Instance()->GetGuild(gid);
			if (pGuild == NULL)
			{
				LogWarn("not find the guild garden,role id is : %llu",pRole->GetID());						
				return false;
			}
		}

		GuildGarden* guild_garden = GardenUtility::Instance()->GetGuildGarden(pGuild->GetID());
		if (NULL == guild_garden)
		{
			LogWarn("not find the guild garden,guild id is : %llu",pGuild->GetID());						
			return false;
		}
		auto guild_plant =  guild_garden->GetGuildPlant();
		UINT32 farmland_id  = convert<UINT32>(args[2]);	
		guild_plant->CanclePlant(farmland_id);
	}

	return true;
}
static bool AddGuildDareTime(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() < 1)
	{
		return false;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild != NULL)
	{
		pGuild->AddDareTimes(convert<UINT32>(args[1]));
	}

	return true;
}

static bool ResetPlatFriendTime(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	PlatFriendList* list = CPlatformFriendMgr::Instance()->GetPlatFriendList(pRole);
	if (!list)
	{
		return false;
	}

	list->DebugSetClientReqTime(time(0) - 3600 * 2);

	return true;
}

static bool DebugAddGuildBonus(CRole* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = args[0] + "addguildbonus + id in bonustable";
		return false;
	}

	UINT32 bonusContentType = convert<UINT32>(args.at(1));

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (!pGuild)
	{
		return false;
	}
	pGuild->Get<GuildBonusMgr>()->addGuildBonus(bonusContentType, role->GetID());
	return true;
}

static bool DebugReswarMatch(CRole* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2){	return false;}
	UINT32 nType = convert<UINT32>(args.at(1));
	if (nType == 1)
	{
		ResWarMgr::Instance()->Destory();
		ResWarMgr::Instance()->SetStep(eGuildResFlowIdle);
		ResWarMgr::Instance()->SetState(true);
	}else if (nType == 2)
	{
		Team *pTeam = TeamMgr::Instance()->FindTeam(role->GetTeamID());
		if (!pTeam) return true;
		UINT64 nGuildID = ResWarMgr::Instance()->GetGuildID(pTeam);
		UINT32 nTeamID  = pTeam->GetID();
		GuildTeam*pGuildTeam = ResWarMgr::Instance()->GetGuildTeam(nGuildID);
		if (!pGuildTeam || !pGuildTeam->pGroupGuildInfo) 
			return true;	
		ResWarMgr::Instance()->SortGuildWarRankLess(pGuildTeam->pGroupGuildInfo->nGroupID);
		ResWarMgr::Instance()->SortFightRoleLess(pGuildTeam->pGroupGuildInfo->nGroupID);

	}else if(nType == 3)
	{
		UINT32 nTeamID = role->GetTeamID();
		Team *pTeam = TeamMgr::Instance()->FindTeam(nTeamID);
		if (!pTeam) return false;
		ResWarMgr::Instance()->Explore(pTeam);
		Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
		if (!pGuild) return false;
		UINT64 nGuildID = pGuild->GetID();
		GuildTeam*pGuildTeam = ResWarMgr::Instance()->GetGuildTeam(nGuildID);
		if (!pGuildTeam)  return false;
		GuildFightTeam *pFightTeam = ResWarMgr::Instance()->GetGuildFightTeam(nGuildID, nTeamID);
		if(!pFightTeam) return false;
		pFightTeam->nTime = 0;
		pFightTeam->nState = eGuildResStateExploreIng;
		ResWarMgr::Instance()->ExploreRes();
		ResWarMgr::Instance()->BuildMineralBattle(pFightTeam);
		UINT32 nSize = pFightTeam->vecMineral.size();
		if (nSize > 0)
		{
			UINT32 nRand = XRandom::randInt(0,nSize-1);
			ResWarMgr::Instance()->StartBattlePVE(pGuildTeam,pFightTeam,pFightTeam->vecMineral[nRand],pFightTeam->vecBuff[nRand]);
		}
	}else if (nType == 4)
	{
		ResWarMgr::Instance()->GiveFinalAward();
	}else if (nType == 5)
	{
		UINT32 nTeamID = role->GetTeamID();
		Team *pTeam = TeamMgr::Instance()->FindTeam(nTeamID);
		if (!pTeam) return false;
		Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
		if (!pGuild) return false;
		UINT64 nGuildID = pGuild->GetID();
		GuildTeam*pGuildTeam = ResWarMgr::Instance()->GetGuildTeam(nGuildID);
		if (!pGuildTeam)  return false;
		ResWarMgr::Instance()->AddTeam(pTeam);
		ResWarMgr::Instance()->Explore(pTeam);
	}else if(nType == 7)
	{
		ResWarMgr::Instance()->SetFightTime();
	}else if (nType == 8)
	{
		UINT32 nTeamID = role->GetTeamID();
		Team *pTeam = TeamMgr::Instance()->FindTeam(nTeamID);
		if (!pTeam) 
			return false;
		Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
		if (!pGuild)
			return false;
		UINT64 nGuildID = pGuild->GetID();
		GuildTeam *pGuildTeam = ResWarMgr::Instance()->GetGuildTeam(nGuildID);
		if (!pGuildTeam) 
			return false;
		ResWarMgr::Instance()->AddNotice(pGuildTeam, 0, eSubSelfTime, 0);
		ResWarMgr::Instance()->AddNotice(pGuildTeam, 0, eAddSelfRes, 0);
		ResWarMgr::Instance()->AddNotice(pGuildTeam, 0, eClearDeBuff, 0);
		ResWarMgr::Instance()->AddRoleNotice(pGuildTeam,role->GetID(),1000);

	}else if (nType == 9)
	{
		ResWarMgr::Instance()->SetStep(	eGuildResFlowEnd);
	}
	return true;
}




static bool DebugAddGuildBuffItem(CRole* role, std::vector<std::string>& args, std::string& output)
{
	KKSG::ResWarBase data;
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (!pGuild) return false;
	data.set_guildid(pGuild->GetID());
	data.set_teamid(role->GetTeamID());
	for (UINT32 i = eStealGuildRes; i <= eClearDeBuff; i++)
	{
		KKSG::GuildBuffItem* pBuffItem = data.add_itemlist();
		GuildBuffTable::RowData *pRowData =  ResWarAwardMgr::Instance()->GetGuildBuffID(i);
		pBuffItem->set_itemid(pRowData->itemid);
		pBuffItem->set_count(2);
	}
	ResWarMgr::Instance()->SetGuildResData(data);
	return true;
}

static bool DebugResetGuildCamp(CRole* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() < 2)
	{
		return false;
	}
	UINT32 day = convert<UINT32>(args.at(1));
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (pGuild)
	{
		pGuild->Get<CGuildCamp>()->DebugReset(day);
	}
	return true;
}

static bool DebugUseGuildBuffItem(CRole* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() < 2)
	{
		return false;
	}
	UINT32 nGuildBuffID = convert<UINT32>(args.at(1));
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (!pGuild) return false;
	GuildBuffTable::RowData *pRowData = ResWarAwardMgr::Instance()->GetGuildBuffID(nGuildBuffID);
	if (!pRowData) return false;
	if (nGuildBuffID < 6)
	{
		Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
		if (!pGuild) return false;
		ResWarMgr::Instance()->UseBuffItem(pGuild->GetID(),pRowData->itemid);

	}
	return true;
}

static bool DebugOpenResWar(CRole* role, std::vector<std::string>& args, std::string& output)
{
	return true;
}

static bool DebugSetGuildLeader(CRole* role, std::vector<std::string>& args, std::string& output)
{
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (!pGuild)
		return false;
	pGuild->SetLeader(role->GetID());
	return true;
}

static bool ClearIntegralData(CRole* role, std::vector<std::string>& args, std::string& output)
{
	CGuildArenaIntegralMgr::Instance()->ClearALLData();
	return true;
}

static bool DebugGetTime(CRole* role, std::vector<std::string>& args, std::string& output)
{
	time_t t = GameTime::GetTime();
	const std::string timeStr = TimeUtil::GetDateTimeStr(t);
	output.append(timeStr);
	return true;
}

static bool DebugSetTime(CRole* role, std::vector<std::string>& args, std::string& output)
{
	string timeStr;
	for (size_t i = 1; i < args.size(); ++i)
	{
		timeStr.append(args[i]);
		timeStr.append(" ");
	}
	time_t t = XCommon::ParseTime(timeStr);
	if (t == 0)
	{
		output = "invalid time";
		return true;
	}
	if (t < GameTime::GetTime())
	{
		output = "can't back roll time";
		return true;
	}
	SSWarn << "Account: " << role->GetAccount() << ", Role: " << role->GetName()
		   << ", Ip: " << role->GetIP() << " SetTime: " << timeStr << END;
	// 系统时间戳
	time_t sysTime = TimeUtil::GetTime();
	INT32 offset = t - sysTime; 
	GameTime::SetTimeOffset(offset);

	PtcM2G_syncservertime ptc;
	ptc.m_Data.set_now(t);
	GSLink::Instance()->SendToAllLine(ptc);
	return true;
}

static bool DebugWorldLevel(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2)
	{
		outputMessage = "setworldlevel + level";
		return false;
	}
	UINT32 level = convert<UINT32>(args[1]);
	SSWarn << "Role:" << pRole->GetName() << ", SetWorldLevel: " << level << END;

	CGlobalConfigDBBuild::Instance()->SetConfigValue(eWorldLevel, ToString(level));
    ServerCrossDataSyn::SynDataToWorld();

	PtcM2G_WorldLevelNtf oPtc;
	oPtc.m_Data.set_worldlevel(level);
	GSLink::Instance()->SendToAllLine(oPtc);

	CBriefRankList* ranklist = (CBriefRankList*)CRankListMgr::Instance()->GetRankList(KKSG::LevelRank);
	ranklist->GMSetUpdateNextWorldLevelFalse();

	outputMessage = "set worldlevel succeed";
	return true;
}

static bool DebugGetWorldLevel(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	outputMessage = CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldLevel);
	return true;
}

static bool AddGuildInherit(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2)
	{
		return false;
	}

	GetGlobalConfig().GuildInheritTeaLimit = convert<UINT32>(args[1]); 
	GetGlobalConfig().GuildInheritStuLimit = convert<UINT32>(args[1]); 
	return true;
}

static bool DebugServerInfo(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	outputMessage = MSConfig::Instance()->GetServerInfo();
	return true;
}

static bool SetInheritCDtime(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2)
	{
		return false;
	}

	GetGlobalConfig().GuildInheritCDTime = convert<UINT32>(args[1]);  
	return true;
}

static bool DebugCreateLeagueTeam(CRole* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if (args.size() < 2)
	{
		outputMessage = "createleagueteam name";
		return false;
	}
	if (LeagueTeamMgr::Instance()->HasLeagueTeam(pRole->GetID()))
	{
		outputMessage = "Already Has leagueteam";
		return false;
	}

	const std::string& name = args[1];
	std::vector<UINT64> roleIds;
	roleIds.push_back(pRole->GetID());

	auto it = CRoleManager::Instance()->Begin();
	for ( ; it != CRoleManager::Instance()->End(); ++it)
	{
		CRole* other = *it;
		if (other == pRole)
		{
			continue;
		}
		if (LeagueTeamMgr::Instance()->HasLeagueTeam(other->GetID()))
		{
			continue;
		}
		roleIds.push_back(other->GetID());
		if (roleIds.size() >= LeagueTeamMgr::Instance()->GetTeamRoleNum())
		{
			break;
		}
	}

	LeagueTeamMgr::Instance()->GMCreateLeagueTeam(roleIds, name, outputMessage);
	return true;
}

static bool DebugLeaveLeagueTeam(CRole* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if (LeagueTeamMgr::Instance()->GMLeaveLeagueTeam(pRole->GetID()) == KKSG::ERR_SUCCESS)
	{
		outputMessage = "success";
	}
	else
	{
		outputMessage = "failed";
	}
	return true;
}

static bool DebugCreateSkyTeam(CRole* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if (args.size() < 2)
	{
		outputMessage = "createskyteam name";
		return false;
	}
	if (SkyTeamMgr::Instance()->HasSkyTeam(pRole->GetID()))
	{
		outputMessage = "Already has skyteam";
		return false;
	}

	const std::string& name = args[1];
	std::vector<UINT64> roleIds;
	roleIds.push_back(pRole->GetID());

	auto it = CRoleManager::Instance()->Begin();
	for ( ; it != CRoleManager::Instance()->End(); ++it)
	{
		CRole* other = *it;
		if (other == pRole)
		{
			continue;
		}
		if (SkyTeamMgr::Instance()->HasSkyTeam(other->GetID()))
		{
			continue;
		}
		roleIds.push_back(other->GetID());
		if (roleIds.size() >= SkyTeamMgr::Instance()->GetTeamRoleNum())
		{
			break;
		}
	}

	SkyTeamMgr::Instance()->GMCreateTeam(roleIds, name, outputMessage);
	return true;
}

static bool DebugLeaveSkyTeam(CRole* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if (SkyTeamMgr::Instance()->LeaveMember(pRole->GetID(), true) == KKSG::ERR_SUCCESS)
	{
		outputMessage = "success";
	}
	else
	{
		outputMessage = "failed";
	}
		
	return true;
}

static bool DebugSyncPlatSelf(CRole* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	CPlatformFriendMgr::Instance()->DebugSyncPlatSelf(pRole);
	outputMessage = "success";
	return true;
}

static bool DebugLeagueEle(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() >= 2 && args[1] == "init")
	{
		LeagueEliminationMgr::Instance()->InitRound();
	}
	return true;
}

static bool DebugLeagueBattle(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() >= 2 && args[1] == "open")
	{
		LeagueSeasonMgr::Instance()->SetSeasonOpen(true);
	}
	if (args.size() >= 2 && args[1] == "close")
	{
		LeagueSeasonMgr::Instance()->SetSeasonOpen(false);
	}
	if (args.size() >= 2 && args[1] == "nolimit")
	{
		LeagueSeasonMgr::Instance()->SetRaceMatchNoLimit(true);
	}
	if (args.size() >= 2 && args[1] == "limit")
	{
		LeagueSeasonMgr::Instance()->SetRaceMatchNoLimit(false);
	}
	if (args.size() >= 3 && args[1] == "num")
	{
		UINT32 num = convert<UINT32>(args[2]);
		LeagueTeamMgr::Instance()->GMSetRoleNum(num);
	}
	if (args.size() >= 2 && args[1] == "season")
	{
		LeagueSeasonMgr::Instance()->DumpSeason(output);
	}
	if (args.size() >= 3 && args[1] == "score")
	{
		UINT32 score = convert<UINT32>(args[2]);
		if (!LeagueSeasonMgr::Instance()->IsInCrossState())
		{
			LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamByRoleId(pRole->GetID());
			if (pLTeam == NULL)
			{
				output = "no lteam";
				return false;
			}
			pLTeam->GMSetScore(score);
		}
	}
	if (args.size() >= 4 && args[1] == "reward")
	{
		UINT32 round = convert<UINT32>(args[2]);
		int isWin = convert<UINT32>(args[3]);
		UINT64 leagueId = LeagueTeamMgr::Instance()->GetLeagueTeamIdByRoleId(pRole->GetID());
		LeagueTeamMgr::Instance()->TestGiveRoundReward(round, leagueId, bool(isWin));
	}
	if (args.size() >= 2 && args[1] == "clearrank")
	{
		CRankListMgr::Instance()->GetLeagueRankList()->RemoveAll();
		CRankListMgr::Instance()->GetCrossLeagueRankList()->RemoveAll();
	}
	return true;
}

static bool DebugMakePartner(CRole* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	std::vector<UINT64> roleIds;
	roleIds.push_back(pRole->GetID());

	auto it = CRoleManager::Instance()->Begin();
	for ( ; it != CRoleManager::Instance()->End(); ++it)
	{
		CRole* other = *it;
		if (other == pRole)
		{
			continue;
		}
		if (PartnerMgr::Instance()->HasPartner(other->GetID()))
		{
			continue;
		}
		roleIds.push_back(other->GetID());
		if (roleIds.size() >= GetGlobalConfig().PartnerNum)
		{
			break;
		}
	}

	//if (roleIds.size() == GetGlobalConfig().PartnerNum)
	{
		PartnerMgr::Instance()->MakePartner(roleIds);
	}
	//else
	{
	//	outputMessage = "role not enough";
	}
	return true;
}

static bool DebugLeavePartner(CRole* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	Partner* p = PartnerMgr::Instance()->GetPartnerByRole(pRole->GetID());
	if (p == NULL)
	{
		outputMessage = "no partner";
		return false;
	}
	p->DoLeaveMember(pRole->GetID(), true);

	return true;
}

static bool DebugRefreshPartnerShop(CRole* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	Partner* p = PartnerMgr::Instance()->GetPartnerByRole(pRole->GetID());
	if (p == NULL)
	{
		outputMessage = "no partner";
		return false;
	}
	p->GetPartnerShop().RefreshItem();
	return true;
}

static bool DebugHeroNum(CRole* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if (args.size() < 2)
	{
		outputMessage = "heronum + num";
		return false;
	}
	UINT32 num = convert<UINT32>(args[1]);
	if (HeroBattleMgr::Instance()->GMChangeNum(num))
	{
		outputMessage = "change succeed";
	}
	else
	{
		outputMessage = "num is too small or too large";
	}
	return true;
}

static bool DebugMobaNum(CRole* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if (args.size() < 2)
	{
		outputMessage = "mobanum + num";
		return false;
	}
	UINT32 num = convert<UINT32>(args[1]);
	if (MobaBattleMgr::Instance()->GMChangeNum(num))
	{
		outputMessage = "change succeed";
	}
	else
	{
		outputMessage = "num is too small or too large";
	}
	return true;
}

static bool ClearTerritoryData(CRole* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if (args.size() == 2)
	{
		CGuildTerritoryBattle::Instance()->ClearData(true);
	}
	else
	{
		CGuildTerritoryBattle::Instance()->ClearData();
	}
	return true;
}

static bool DebugPokerTournament(CRole *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2)
	{
		outputMessage = "cardsign + num";
		return false;
	}
	UINT32 num = convert<UINT32>(args[1]);
	GetGlobalConfig().PokerTournamentPlayNum = num;
	outputMessage = "set cardsign succeed";
	return true;
}

static bool DebugIdipOpenSystem(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 3)
	{
		output = "args error, format:idipopensystem id [open|close]";
		return false;
	}
	UINT32 id = convert<UINT32>(args[1].c_str());
	bool isOpen = strcmp(args[2].c_str(), "open") == 0 ? true : false;
	OpenSystemMgr::Instance()->IdipOpSystem(id, isOpen);

	return true;
}

static bool SetGuildWage(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		return false;
	}

	UINT32 wageLvl = convert<UINT32>(args[1].c_str());
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild != NULL)
	{
		CGuildWage* pGuildWage = pGuild->Get<CGuildWage>();
		if (pGuildWage != NULL)
		{
			pGuildWage->GMSetWageLvl(wageLvl);
		}
	}

	return true;
}

static bool DebugAddMentorTask(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 4)
	{
		output = "args error, format:addmentortask type var num";
		return false;
	}
	UINT32 type = convert<UINT32>(args[1].c_str());
	UINT64 var = convert<UINT32>(args[2].c_str());
	UINT32 add = convert<UINT32>(args[3].c_str());

	MentorShipOneSide* oneside = MentorMgr::Instance()->GetMentorShip(pRole->GetID());
	if (!oneside)
	{
		output = "fail, not exist";
		return false;
	}
	oneside->GetMentorTask()->AddTaskValue(type, var, add);

	output = "success";

	return true;
}

static bool DebugMilitaryRank(CRole* pRole, std::vector<std::string>& args, std::string& output)
{	
	if (args.size() != 2)
	{
		output = "args error, format:setflowerranknum num";
		return false;
	}
	//UINT32 num = convert<UINT32>(args[1].c_str());
	if (args[1] == "clear")
	{
		MilitaryManager::Instance()->TestClear();
	}
	if (args[1] == "test")
	{
		MilitaryManager::Instance()->TestPrint();
	}
	return true;
}

static bool DebugSetFlowerRankNum(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "args error, format:setflowerranknum num";
		return false;
	}
	UINT32 num = convert<UINT32>(args[1].c_str());
	
	CRankListMgr::Instance()->DebugSetFlowerRankNum(num);

	output = "flowerranknum success";
	return true;
}
	

static bool DebugGetMaxLevel(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	output += ToString(CRoleSummaryMgr::Instance()->GetMaxLevel());
	return true;
}

static bool DebugSetMaxLevel(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "args error, format: setmaxlevel level";
		return false;
	}
	UINT32 level = convert<UINT32>(args[1].c_str());
	CRoleSummaryMgr::Instance()->SetMaxLevel(level);
	LogInfo("SetMaxLevel [%u]", level);
	return true;
}

static bool DebugTestCreateGmfScene(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "args error, format: creategmfscene [count]";
		return false;
	}

	UINT32 dwCount = convert<UINT32>(args[1].c_str());

	static UINT64 sqwGuildID = 1;
	for(UINT32 i = 0; i < dwCount; ++i)
	{
		GuildMatchFightMgr::Instance()->TestMakeFight(pRole, GMF_RT_GPR, sqwGuildID, sqwGuildID + 1);
		sqwGuildID += 2;
	}

	return true;
}

static bool DebugHeroSceneID(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "herosceneid + sceneid";
		return false;
	}
	UINT32 sceneid = convert<UINT32>(args[1]);
	if (!HeroBattleMgr::Instance()->GMSetNextSceneID(sceneid))
	{
		output = "sceneid is invalid";
		return false;
	}
	output = "set sceneid succeed";
	return true;
}

static bool DebugGetGuildByName(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "getguildbyname + guildname";
		return false;
	}
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByName(args[1].c_str());
	if (pGuild)
		output = "guild exist";
	else
		output = "guild unexist";
	return  true;
}

static bool DebugReadSummary(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "readsummary + roleid";
		return false;
	}
	UINT64 roleid = convert<UINT64>(args[1]);	
	CRoleSummaryMgr::Instance()->QuerySummaryFromDb(roleid);	
	output = "succeed";

	return  true;
}

static bool  DebugStressTest(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "reswarstress + maxteamcount";
		return false;
	}
	UINT32 dwCount = convert<UINT32>(args[1].c_str());
	CGuildMgr::Instance()->StressTest(dwCount);
	return true;
}

static bool DebugGetMaxPPT(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	CRoleSummary* pSum	 = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
	if (pSum)
	{
		output = "maxppt=" +  ToString(pSum->GetMaxPPT());
		output += "powerpoint" + ToString(pSum->GetPowerPoint()); 
	}
	return true;
}

static bool DebugClearMentorTask(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 1)
	{
		output = "args error, format:clearmentortask";
		return false;
	}

	MentorShipOneSide* oneside = MentorMgr::Instance()->GetMentorShip(pRole->GetID());
	if (!oneside)
	{
		output = "not exist";
		return false;
	}

	oneside->GetMentorTask()->DebugClear();

	const std::map<UINT64, OneMentorRelation>&list = oneside->GetRelationList()->GetRelationList();
	for (auto iter = list.begin(); iter != list.end(); ++iter)
	{
		MentorShipOneSide* other = MentorMgr::Instance()->GetMentorShip(iter->second.m_roleID);
		if (other)
		{
			other->GetMentorMsg()->DebugClear(oneside->GetOwnerID(), KKSG::MentorMsgApplyReportTask);
			MentorMgr::Instance()->QuickSave(other);
		}
	}

	MentorMgr::Instance()->QuickSave(oneside);

	output = "success";

	return true;
}

static bool DebugClearMentorInherit(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 1)
	{
		output = "args error, format:clearmentorinherit";
		return false;
	}

	MentorShipOneSide* oneside = MentorMgr::Instance()->GetMentorShip(pRole->GetID());
	if (!oneside)
	{
		output = "not exist";
		return false;
	}

	oneside->GetRelationList()->DebugClearInherit();
	oneside->GetMentorMsg()->DebugClear(0, KKSG::MentorMsgApplyInherit);

	const std::map<UINT64, OneMentorRelation>&list = oneside->GetRelationList()->GetRelationList();
	for (auto iter = list.begin(); iter != list.end(); ++iter)
	{
		MentorShipOneSide* other = MentorMgr::Instance()->GetMentorShip(iter->second.m_roleID);
		if (other)
		{
			other->GetRelationList()->DebugClearInherit(oneside->GetOwnerID());
			other->GetMentorMsg()->DebugClear(oneside->GetOwnerID(), KKSG::MentorMsgApplyInherit);
			MentorMgr::Instance()->QuickSave(other);
		}
	}

	MentorMgr::Instance()->QuickSave(oneside);

	output = "success";

	return true;
}

static bool DebugCustombattleOp(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 3)
	{
		output = "args error, format:custom type op";
		return false;
	}
	int op = convert<int>(args[2]); 
	UINT64 uid = 0;
	SCustomBattleRole* roledata = CustomBattleRoleMgr::Instance()->GetInfo(pRole->GetID());
	if ("0" == args[1])
	{
		uid = roledata->GetJoinId();
	}
	else
	{
		uid = convert<UINT64>(args[1]);
	}
	if (0 != uid)
	{
		CustomBattleReq req;
		req.op = (KKSG::CustomBattleOp)op;
		req.uid = uid;
		req.roleid = pRole->GetID();
		CustomBattleMgr::Instance()->HandleClientReq(req);

		output = "success";
		return true;
	}

	output = "error";

	return true;
}

static bool DebugAbyssMail(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "args error, format:abyssmail max";
		return false;
	}
	ActivityMailMgr::Instance()->SetMaxMail(convert<UINT32>(args[1]));
	return true;
}

static bool DebugSetGuildBossIdx(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "args error, format:setguildbossidx idx";
		return false;
	}
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		output = "guild is null";
		return false;
	}
	pGuild->SetBossIndex(convert<int>(args[1]));
	return true;
}


static bool DebugSetGuildActivity(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "args error, format:setguildactivity act";
		return false;
	}
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		output = "guild is null";
		return false;
	}
	pGuild->Get<CGuildActivity>()->AddActivity(pRole->GetID(), convert<int>(args[1]));
	return true;
}

static bool DebugResetDragonGlobal(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 1)
	{
		output = "args error, format:resetdragonglobal";
		return false;
	}

	CGlobalConfigDBBuild::Instance()->SetConfigValue(eDragonGlobal, "");
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eDragonGlobal);

	SSInfo<<"reset dragon global"<<END;

	// send to all line
	PtcM2G_BroadCastGlobalValue globalvalue;
	globalvalue.m_Data.set_id(eDragonGlobal);
	globalvalue.m_Data.set_value(CGlobalConfigDBBuild::Instance()->GetConfigValue(eDragonGlobal));
	GSLink::Instance()->SendToAllLine(globalvalue);

	return true;
}

static bool DebugResetCompeteDragonGlobal(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 1)
	{
		output = "args error, format:resetcompetedragonglobal";
		return false;
	}

	CGlobalConfigDBBuild::Instance()->SetConfigValue(eCompeteDragonInfo, "");
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eCompeteDragonInfo);

	SSInfo<<"reset competedragon global"<<END;

	// send to all line
	PtcM2G_BroadCastGlobalValue globalvalue;
	globalvalue.m_Data.set_id(eCompeteDragonInfo);
	globalvalue.m_Data.set_value(CGlobalConfigDBBuild::Instance()->GetConfigValue(eCompeteDragonInfo));
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, globalvalue);

	return true;
}

static bool DebugWedding(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() < 2)
	{
		output = "args error, format:wedding create|destory";
		return false;
	}

	if (args[1] == "create")
	{
		Team* pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
		if (pTeam == NULL)
		{
			output = "no team";
			return false;
		}
		if (pTeam->GetMemberCount() != 2)
		{
			output= "team member not 2";
			return false;
		}
		KKSG::WeddingType type = KKSG::WeddingType_Normal;
		if (args.size() > 3)
		{
			type = (KKSG::WeddingType)convert<UINT32>(args[2]);
		}
		const std::list<CTeamMember *>& members = pTeam->GetMember();
		UINT64 roleId1 = (*members.begin())->m_qwRoleID;
		UINT64 roleId2 = (*members.rbegin())->m_qwRoleID;
		WeddingMgr::Instance()->CreateWedding(type, roleId1, roleId2);
	}
	else if (args[1] == "destory")
	{
		
	}
	else if (args[1] == "car")
	{
		Team* pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
		if (pTeam == NULL)
		{
			output = "no team";
			return false;
		}
		if (pTeam->GetMemberCount() != 2)
		{
			output= "team member not 2";
			return false;
		}
		const std::list<CTeamMember *>& members = pTeam->GetMember();
		UINT64 roleId1 = (*members.begin())->m_qwRoleID;
		UINT64 roleId2 = (*members.rbegin())->m_qwRoleID;
		RpcM2G_WeddingOperM2G* rpc = RpcM2G_WeddingOperM2G::CreateRpc();
		rpc->m_oArg.set_roleid1(roleId1);
		rpc->m_oArg.set_roleid2(roleId2);
		rpc->m_oArg.set_type(KKSG::WeddingType_Normal);
		rpc->m_oArg.set_opertype(1);
		GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
	}
	else if (args[1] == "reset")
	{
		Marriage* pMarr1 = MarriageMgr::Instance()->GetMarriage(pRole->GetID());
		if (pMarr1)
		{
			pMarr1->SetTakeWeddingCar(false);
			pMarr1->SetHoldedWedding(false);
			Marriage* pMarr2 = MarriageMgr::Instance()->GetMarriage(pMarr1->GetCoupleID());
			if (pMarr2)
			{
				pMarr2->SetTakeWeddingCar(false);
				pMarr2->SetHoldedWedding(false);
			}
		}
	}
	else if (args[1] == "break")
	{
		Marriage* pMarr1 = MarriageMgr::Instance()->GetMarriage(pRole->GetID());
		if (pMarr1)
		{
			Marriage* pMarr2 = MarriageMgr::Instance()->GetMarriage(pMarr1->GetCoupleID());
			if (pMarr2)
			{
				MarriageMgr::Instance()->AutoBreak(pMarr1, pMarr2);
			}
		}
	}

	return true;
}

static bool DebugAddReport(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 3)
	{
		output = "args error, format:addreport scenetype num";
		return false;
	}
	UINT32 sceneType = convert<UINT32>(args[1]);
	int num = convert<int>(args[2]);
	RoleReportInfo* pInfo = ReportMgr::Instance()->GetRoleReportInfo(pRole->GetID(), true);
	if (pInfo == NULL)
	{
		output = "failed";
		return false;
	}
	pInfo->AddReportNum(sceneType, num);
	output = "success, reportnum: " + ToString(pInfo->GetReportNum(sceneType));

	return true;
}

static bool DebugGetReport(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "args error, format:getreport scenetype";
		return false;
	}
	UINT32 sceneType = convert<UINT32>(args[1]);
	RoleReportInfo* pInfo = ReportMgr::Instance()->GetRoleReportInfo(pRole->GetID(), true);
	if (pInfo == NULL)
	{
		output = "failed";
		return false;
	}
	output = "reportnum: " + ToString(pInfo->GetReportNum(sceneType));

	return true;
}

static bool DebugResetBan(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "args error, format:addreport scenetype";
		return false;
	}
	UINT32 sceneType = convert<UINT32>(args[1]);
	RoleReportInfo* pInfo = ReportMgr::Instance()->GetRoleReportInfo(pRole->GetID());
	if (pInfo == NULL)
	{
		output = "success";
		return false;
	}
	pInfo->ResetBanTime(sceneType);
	output = "success";
	return true;
}

static bool DebugCustombattleCreate(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 3)
	{
		output = "args error, format:customcreate configid count";
		return false;
	}
	int configid = convert<int>(args[1]); 
	int count = convert<int>(args[2]); 

	KKSG::CustomBattleConfig serverconfig;
	serverconfig.set_name(pRole->GetName());
	serverconfig.set_creator(pRole->GetID());
	serverconfig.set_creatorname(pRole->GetName());
	serverconfig.set_configid(configid);

	serverconfig.set_tagtype(1);
	serverconfig.set_fighttype(KKSG::CustomBattleType(1));
	serverconfig.set_state(KKSG::CustomBattle_Ready);
	serverconfig.set_canjoincount(100);
	serverconfig.set_hasjoincount(0);
	serverconfig.set_battletimeconf(3600);
	serverconfig.set_readytime((UINT32)GameTime::GetTime() + 3600);
	serverconfig.set_battletime(serverconfig.readytime() + serverconfig.battletime());

	for (int i = 0; i < count; ++i)
	{
		CustomBattleReq req;
		req.op = KKSG::CustomBattle_DoCreate;
		req.roleid = pRole->GetID();
		req.conf = serverconfig;
		CustomBattleMgr::Instance()->TransReqToWorld(req);
	}

	output = "scuccess";
	return true;
}

static bool DebugCustombattle(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 3)
	{
		output = "args error, format:custom type point";
		return false;
	}
	int point = convert<int>(args[2]); 
	UINT64 uid = 0;
	SCustomBattleRole* roledata = CustomBattleRoleMgr::Instance()->GetInfo(pRole->GetID());
	if ("0" == args[1])
	{
		uid = roledata->GetJoinId();
		if (0 != uid)
		{
			roledata->UpdatePoint(uid, point);
		}
	}
	else
	{
		uid = convert<UINT64>(args[1]);
		if (point > 0)
		{
			while(point > 0)
			{
				roledata->UpdateWinOrLose(uid, true);
				--point;
			}
		}
		else
		{
			while(point < 0)
			{
				roledata->UpdateWinOrLose(uid, false);
				++point;
			}
		}

		output = "success";
		return true;
	}

	output = "error";
	return true;
}





static bool DebugMarryDesi(CRole* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 3)
	{
		output = "args error, format:marrydesi 1 destroleid";

		return false;
	}
	UINT64 destroleid = convert<UINT64>(args[2]); 
	UINT32 oper		  = convert<UINT32>(args[1]); 
	if (oper>2)
	{
		return false;
	}

	CRoleSummary* destSumm = CRoleSummaryMgr::Instance()->GetRoleSummary(destroleid);
	if(destSumm)
	{
		EventMgr::Instance()->AddEvent(pRole->GetID(),DESIGNATION_COM_TYPE_MARRY,KKSG::MarriagePos_Husband,oper,destSumm->GetName());
		EventMgr::Instance()->AddEvent(destroleid,DESIGNATION_COM_TYPE_MARRY,KKSG::MarriagePos_Wife,oper,pRole->GetName());
	}
	output = "sucess";
	return true;
}


static std::map<std::string, GMHandler> GMFunctions;

static void InitGMFunctions()
{
	GMFunctions["gmtest"] = DebugGMTest;
	GMFunctions["robot"] = DebugRobot;
	GMFunctions["mail"] = DebugMail;
	GMFunctions["pvp"] = DebugPvp;
	GMFunctions["steam"] = DebugTeam;
	GMFunctions["firstpass"] = DebugFirstPassClear;
	GMFunctions["de"] = DebugDE;
	GMFunctions["auction"] = DebugAuction;
	GMFunctions["clearchat"] = DebugClearChatNotify;
	GMFunctions["checkidip"] = DebugCheckIdipAndNotify;
	GMFunctions["addkillgoblin"] = DebugAddKillGoblin;
	GMFunctions["setguildcheckinnum"] = DebugSetGuildCheckInNum;
	GMFunctions["setguildevent"] = DebugSetGuildEvent;
	GMFunctions["setevent"] = DebugSetEvent;
	GMFunctions["gmf"] = DebugGmf;
	GMFunctions["setguildarenatime"] = SetGuildArenaTime;
	GMFunctions["setguilddaretime"] = SetGuildDareTime;
	GMFunctions["setflowerach"] = DebugFlowerAch;
	GMFunctions["updateevent"] = DebugGlobalEvent;
	GMFunctions["setguildladdertime"] = SetGuildLadderTime;
	GMFunctions["skycitywait"]	= CreateSkyCityWait;
	GMFunctions["setskycitygames"] =  SetSkyCityGames;
	GMFunctions["enterskycityroom"]	= EnterSkyCityRoom;
	GMFunctions["cardbegin"]	= DebugGuildCardMatchBegin;
	GMFunctions["cardend"]	= DebugGuildCardMatchEnd;
	GMFunctions["teamop"]	= DebugTeamOp;
	GMFunctions["flowerweekpass"] = DebugFlowerWeekPass;
	GMFunctions["flowerdaypass"] = DebugFlowerDayPass;
	GMFunctions["dissguild"] = DissGuild;
	GMFunctions["openqa"]	= DebugOpenQA;
	GMFunctions["adddegree"] = DebugAddDegree;
	GMFunctions["cleandegree"] = DebugCleanDegree;
	GMFunctions["arrangeteamcard"] = DebugArrangeTeamCard;
	GMFunctions["setact"] = DebugOpenAct;
	GMFunctions["resetact"] = DebugResetAct;
	GMFunctions["addprestige"] = addprestige;
	GMFunctions["delrole"] = DebugDelRole;
	GMFunctions["simsskycity"] = DebugSimSkyCicy;
	GMFunctions["clearbind"] = DebugClearBind;
	GMFunctions["paycreate"] = DebugPayCreate;
	GMFunctions["paynotify"] = DebugPayNotify;
	GMFunctions["payntf"] = DebugPayParamNtf;
	GMFunctions["setpaycheck"] = DebugSetPayCheck;
	GMFunctions["msreportdata"] = DebugMsReportData;
	GMFunctions["getsublist"] = DebugGetSubscribeList;
	GMFunctions["setsub"] = DebugSetSubscribe;
	GMFunctions["pushsub"] = DebugPushSubscribe;
	GMFunctions["clicknotice"] = DebugClickNotice;
	GMFunctions["payitembill"] = DebugPayItemBill;
	GMFunctions["setpay"] = DebugSetPay;
	GMFunctions["setitembill"] = DebugSetItemBill;
	GMFunctions["guildexp"] = DebugAddGuildExp;
	GMFunctions["cross"] = DebugCross;
	GMFunctions["garden"] = DebugGarden;
	GMFunctions["guildauct"] = DebugGuildAuct;
	GMFunctions["addguilddaretime"] = AddGuildDareTime;
	GMFunctions["resetplatfriendtime"] = ResetPlatFriendTime;
	GMFunctions["addguildbonus"] = DebugAddGuildBonus;
	GMFunctions["reswarmatch"]	 = DebugReswarMatch;
	GMFunctions["addguildbuff"]		 = DebugAddGuildBuffItem;
	GMFunctions["useguildbuff"]		 = DebugUseGuildBuffItem;
	GMFunctions["resetguildcamp"]	 = DebugResetGuildCamp;
	GMFunctions["openreswar"]		 = DebugOpenResWar;
	GMFunctions["setguildleader"]		 = DebugSetGuildLeader;
	GMFunctions["clearintegraldata"] = ClearIntegralData;
	GMFunctions["gettime"] = DebugGetTime;
	GMFunctions["settime"] = DebugSetTime;
	GMFunctions["setworldlevel"] = DebugWorldLevel;
	GMFunctions["getworldlevel"] = DebugGetWorldLevel;
	GMFunctions["addguildinherit"] = AddGuildInherit;
	GMFunctions["version"] = DebugServerInfo;
	GMFunctions["setinheritcdtime"] = SetInheritCDtime;
	GMFunctions["nestweek"] = DebugNestWeek;
	GMFunctions["invfight"] = DebugInvFight;
	GMFunctions["makepartner"] = DebugMakePartner;
	GMFunctions["leavepartner"] = DebugLeavePartner;
	GMFunctions["partnershop"] = DebugRefreshPartnerShop;
	GMFunctions["castle"] = DebugGuildCastle;
	GMFunctions["heronum"] = DebugHeroNum;
	GMFunctions["mobanum"] = DebugMobaNum;
	GMFunctions["clearterritory"] = ClearTerritoryData;
	GMFunctions["cardsign"]	= DebugPokerTournament;
	GMFunctions["createleagueteam"] = DebugCreateLeagueTeam;
	GMFunctions["leaveleagueteam"] = DebugLeaveLeagueTeam;
	GMFunctions["createskyteam"] = DebugCreateSkyTeam;
	GMFunctions["leaveskyteam"] = DebugLeaveSkyTeam;
	GMFunctions["syncplatself"] = DebugSyncPlatSelf;
	GMFunctions["lbe"] = DebugLeagueEle;
	GMFunctions["lb"] = DebugLeagueBattle;
	GMFunctions["idipopensystem"] = DebugIdipOpenSystem;
	GMFunctions["pk"] = DebugPk;
	GMFunctions["addmentortask"] = DebugAddMentorTask;
	GMFunctions["setflowerranknum"] = DebugSetFlowerRankNum;
	GMFunctions["creategmfscene"] = DebugTestCreateGmfScene;
	GMFunctions["arenastar"] = DebugArenaStar;
	GMFunctions["match"] = DebugMatch;
	GMFunctions["npcfl"] = DebugNpcFeeling;
	GMFunctions["survive"] = DebugSurvive;

	GMFunctions["militaryrank"] = DebugMilitaryRank;
	GMFunctions["getmaxlevel"] = DebugGetMaxLevel;
	GMFunctions["setmaxlevel"] = DebugSetMaxLevel;
	GMFunctions["setguildwage"] = SetGuildWage;
	GMFunctions["herosceneid"] = DebugHeroSceneID;
	GMFunctions["reswarstress"] = DebugStressTest;
	GMFunctions["getguildbyname"] = DebugGetGuildByName;

	GMFunctions["getmaxppt"] = DebugGetMaxPPT;
	GMFunctions["readsummary"] = DebugReadSummary;
	GMFunctions["clearmentortask"] = DebugClearMentorTask;
	GMFunctions["clearmentorinherit"] = DebugClearMentorInherit;
	GMFunctions["ibgiftntf"] = DebugTestRuoJi;
	GMFunctions["additembill"] = DebugAddItemBill;

	GMFunctions["custom"] = DebugCustombattle;
	GMFunctions["customop"] = DebugCustombattleOp;
	GMFunctions["customcreate"] = DebugCustombattleCreate;
	GMFunctions["addreport"] = DebugAddReport;
	GMFunctions["getreport"] = DebugGetReport;
	GMFunctions["resetban"] = DebugResetBan;
	GMFunctions["abyssmail"] = DebugAbyssMail;	
	GMFunctions["setguildbossidx"] = DebugSetGuildBossIdx;
	GMFunctions["setguildactivity"] = DebugSetGuildActivity;

	GMFunctions["setguildbossidx"] = DebugSetGuildBossIdx;
	GMFunctions["setguildactivity"] = DebugSetGuildActivity;

	GMFunctions["resetdragonglobal"] = DebugResetDragonGlobal;

	GMFunctions["resetcompetedragonglobal"] = DebugResetCompeteDragonGlobal;

	GMFunctions["wedding"] = DebugWedding;
	GMFunctions["marrydesi"] = DebugMarryDesi;
	GMFunctions["bigmelee"] = DebugBigMelee;
	GMFunctions["battlefieldsim"] = DebugBattleFieldSim;

    GMFunctions["goalaward"] = DebugGoalAwards;
    GMFunctions["goalawardclear"] = DebugGoalAwardsClear;

	GMFunctions["dragonguild"] = DebugDragonGuild;

	GMFunctions["reload"] = DebugReload;
}

void RpcG2M_GMCommandG2M::OnCall(const GMCmdg2mArg &roArg, GMCmdg2mRes &roRes)
{
	const GMCmdArg& c2gArg = roArg.c2garg();
	GMCmdRes& c2gRes = *roRes.mutable_c2gres();
	roRes.set_handled(false);

	if(GMFunctions.empty())
	{
		InitGMFunctions();
	}

	if(WorldLink::Instance()->IsConnected())
	{
		RpcM2K_GMCommandM2W *rpc = RpcM2K_GMCommandM2W::CreateRpc();
		rpc->m_oArg.CopyFrom(roArg);
		rpc->m_oArg.set_delayid(DelayRpc());
		WorldLink::Instance()->SendTo(*rpc);
	}

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
	if(NULL == pRole)
	{
		c2gRes.set_result(false);
		c2gRes.set_outputmessage("find not role");
		return ;
	}

	std::vector<std::string> args = Split(c2gArg.cmd(), ' ');
	if(args.empty())//不处理help
	{
		c2gRes.set_result(false);
		c2gRes.set_outputmessage("");
		return ;
	}

	if(GMFunctions.find(args[0]) != GMFunctions.end())
	{
		bool result;
		std::string outputMessage;
		result = GMFunctions[args[0]](pRole, args, outputMessage);
		c2gRes.set_result(result);
		c2gRes.set_outputmessage(outputMessage);
		roRes.set_handled(true);
		return ;
	}

	c2gRes.set_result(false);
	c2gRes.set_outputmessage("");
}

void RpcG2M_GMCommandG2M::OnDelayReplyRpc(const GMCmdg2mArg &roArg, GMCmdg2mRes &roRes, const CUserData &roUserData)
{
	if(NULL == roUserData.m_pUserPtr)
	{
		return ;
	}
	GMCmdg2mRes* tmp = (GMCmdg2mRes*)(roUserData.m_pUserPtr); 
	std::string output = roRes.c2gres().outputmessage();
	std::string outputWorld = tmp->c2gres().outputmessage();
	if(tmp->handled())//没有考虑共同处理的情况
	{
		roRes.CopyFrom(*tmp);
		roRes.mutable_c2gres()->set_outputmessage(output + " " + outputWorld);
	}
}


