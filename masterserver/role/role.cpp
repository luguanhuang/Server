#include "pch.h"
#include "role.h"
#include "pb/project.pb.h"
#include "mail/mailmgr.h"
#include "network/gatelink.h"
#include "protocol.h"
#include "network/gslink.h"
#include "network/loginlink.h"
#include "role/rolesummarymgr.h"
#include "worldboss/worldbossmgr.h"
#include "common/ptcm2c_hintnotifyms.h"
#include "common/ptcm2i_updateselfserver.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "friend/friendop.h"
#include "guild/guildmgr.h"
#include "ranklist/ranklistmgr.h"
#include "pk/pkmgr.h"
#include "guild/guildmgr.h"
#include "push/pushmgr.h"
#include "arena/arenamgr.h"
#include "gmf/guildmatchfightmgr.h"
#include "event/eventmgr.h"
#include "skycity/skycitymgr.h"
#include "scene/scenemanager.h"
#include "common/ptcm2c_idippunishinfomsntf.h"
#include "auction/auctionrequest.h"
#include "network/rslink.h"
#include "crpc.h"
#include "platforminfo/platformfriendmgr.h"
#include "guildauct/guildauctmgr.h"
#include "garden/garden_utility.h"
#include "garden/garden_manager.h"
#include "guild/guildarena.h"
#include "reswar/reswarmgr.h"
#include "chat/fmmgr.h"
#include "guild/guildinherit.h"
#include "loghelper/tlogr.h"
#include "garden/garden_manager.h"
#include "nestweek/nestweekmgr.h"
#include "mentorship/mentormgr.h"
#include "guildcastle/guildcastlerequest.h"
#include "leagueteam/leagueteammgr.h"
#include "loghelper/reportdatamgr.h"
#include "other/matchhandler.h"
#include "pay/paymgr.h"
#include "pay/ptcm2c_payparameterinfoinvalidntf.h"
#include "team/ptcm2c_rolematchstatem2cnotify.h"
#include "leaguebattle/leagueeliminationmgr.h"
#include "leaguebattle/leagueseasonmgr.h"
#include "table/OpenSystemMgr.h"
#include "mentorship/mentoroneside.h"
#include "mentorship/mentormgr.h"
#include "define/systemiddef.h"
#include "invfight/invfightmgr.h"
#include "idutil.h"
#include "arenastar/arenastarmgr.h"
#include "skycraft/skycraftelimgr.h"
#include "pay/ibgiftmgr.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlerole.h"
#include "custombattle/custombattlerolemgr.h"
#include "custombattle/custombattlemgr.h"
#include "team/teamtranshandler.h"
#include "team/teamcommon.h"
#include "report/reportmgr.h"
#include "team/tarjateammgr.h"
#include "common/ptcm2c_mserrornotify.h"
#include "pk/pkmgr.h"
#include "social/socialmgr.h"
#include "mobabattle/mobarecordmgr.h"
#include "mayhem/mayhemmgr.h"
#include "marriage/marriagemgr.h"
#include "groupchat/groupchatmgr.h"
#include "battlefield/battlefieldmgr.h"
#include "dragonguild/dragonguildmgr.h"
#include "goalawards/goalawardsmgr.h"
#include "task/roletaskmgr.h"

CRole::CRole()
:m_sessionID(INVALID_SESSION_ID)
,m_serverId(0)
,m_nState(ROLE_STATE_INVALID)
,m_qwID(INVALID_ROLE_ID)
,m_createtime(0)
,m_logintime(0)
,m_dwGsLine(INVALID_LINE_ID)
,m_isCrossGs(false)
,m_dwMapID(0)
,m_dwSceneID(INVALID_SCENE_ID)
,m_teamID(0)
,m_matchType(KKSG::KMT_NONE)
,m_matchUniIndex(0)
,m_loginType(KKSG::LOGIN_PASSWORD)
,m_loginplatid(0)
,m_bIsGetBalance(false)
,m_bPayFirstGetBalance(false)
,m_LastCheckPayTokenTime(0)
,m_TotalCostDiamond(0)
,m_accountBackFlow(false)
,m_chargeBackRoleId(0)
,m_allZonePayCnt(0)
{
	memset(m_CustomBattleReqCD, 0, sizeof(m_CustomBattleReqCD));
}

CRole::~CRole()
{
}

bool CRole::Init(const KKSG::RoleInfoOnMs& roInfo, UINT64 sessionID)
{
	m_sessionID = sessionID;
	m_accountID = roInfo.accountid();
	m_qwID      = roInfo.roleid();
	m_strName   = roInfo.name();
	m_dwMapID	= roInfo.mapid();
	m_dwSceneID = roInfo.sceneid();
	m_strIP		= roInfo.clientip();
	m_createtime = roInfo.rolecreatedtime();
	m_logintime = roInfo.lastlogintime();
	m_Token = roInfo.token();
	m_loginType = roInfo.logintype();
	m_loginplatid = roInfo.loginplatid();
	m_serverId = GetServerIDByRoleID(roInfo.roleid());

	if (roInfo.has_idipdata())
	{
		const KKSG::IdipData& data = roInfo.idipdata();
		for (INT32 i = 0; i < data.punishinfo_size(); i ++)
		{
			m_vPunishInfo.insert(std::make_pair(data.punishinfo(i).type(), data.punishinfo(i)));
		}
	}
	m_accountBackFlow = roInfo.account_backflow();
	return true;
}

void CRole::Uninit()
{

}

void CRole::OnEnterOwnScene(const UINT64 garden_id)
{
	const UINT64 role_id = GetID();
	if (garden_id != role_id && garden_id > 0)
	{
		// 进入了别人的家园
		 std::string garden_name;
		GardenUtility::Instance()->GetGardenName(GetID(),
			garden_id,
			garden_name);
		Garden* garden = GardenManager::Instance()->GetGarden(garden_id,garden_name);
		if (NULL == garden)
		{
			//roRes.set_result(KKSG::ERR_GARDEN_TOLOADING);
			return ;
		}
		garden->VisitLog(GetID(),GetName());

		TGardenFlow oLog(TLogMgr::Instance()->GetTagString());
		oLog.m_uRoleID = GetID();
		oLog.m_uGardenID = garden_id;
		oLog.m_iOp = TX_Garden_Visit;
		oLog.m_iQuestType = KKSG::FRIEND;
		oLog.Do();
	}
}

void CRole::OnEnterScene()
{
	FriendOp op(GetID());
	op.NotifyGS(this);
	EventMgr::Instance()->OnChangedGS(this);

	TeamMgr::Instance()->OnRoleEnterScene(this);

	//CustomBattleMsMatcher::Instance()->Cancel(m_qwID);
	IBGiftMgr::Instance()->OnLogin(this);
	TeamTransHandler trans;
	trans.SynTeamExtraInfoToTs(GetID());
}

void CRole::OnLoginReconnect()
{
	OnFirstEnterScene();
}

void CRole::OnLogin(UINT32 level)
{
	if (m_dwMapID != GetGlobalConfig().InitialSceneID)
	{
		SetMapID(HALL_MAP_ID);
	}
	UpdateSelfServer(level);
	MobaRecordMgr::Instance()->OnLogin(m_qwID);
	// 尽量放到OnFirstEnterScene
}

void CRole::OnLogout(UINT32 level)
{
	TeamMgr::Instance()->OnRoleLogout(this);
	HMatchHandler handler;
	handler.OnLogout(this);
	PushMgr::Instance()->OnRoleLogout(m_qwID);
	SkyCityMgr::Instance()->OnRoleLogout(this);
	AuctionRequest auctreq(this);
	auctreq.ReqLogoutHandle();
	UpdateSelfServer(level);
	FMMgr::Instance()->OnLoginOut(this);
	ArenaMgr::Instance()->PopChallengedRoleId(GetID());
	GcfRequest req(this);
	req.OnLogout();

	CPlatformFriendMgr::Instance()->OnLoginOrOut(this, false);
	LeagueEliminationMgr::Instance()->RemoveWatcher(m_qwID);
	SkyCraftEliMgr::Instance()->RemoveWatcher(m_qwID);

	//CustomBattleMsMatcher::Instance()->Cancel(m_qwID);

	///> 统计离开世界boss的人数
	if (GetMapID() == WORLDBOSS_MAP_ID)
	{
		WorldBossMgr::Instance()->DesRole(GetID());
		SSInfo<<"role leave world boss scene, logout roleid:"<<GetID()<<END;
	}

	TLogMgr::Instance()->Logout(this);
	MayhemMsMgr::Instance()->OnMapChange(this,GetMapID(),0);
	MarriageMgr::Instance()->OnLogout(this);
}

void CRole::OnFirstEnterScene() // 登录第一次进主城
{
	CMailMgr::Instance()->OnRoleLogin(this);
	WorldBossMgr::Instance()->HintNotify(this);
	CGuildMgr::Instance()->DoRoleLogin(this);
	AuctionRequest auctreq(this);
	auctreq.ReqLoginHandle();
	GuildAuctMgr::Instance()->OnRoleLogin(this);
	CGuildArenaIntegralMgr::Instance()->DoOnlineCheck(GetID());
	CGuildWarMgr::Instance()->CheckPlayerOnline(this);
	ResWarMgr::Instance()->OnLogin(this);
	FMMgr::Instance()->OnLogin(this);
	CGuildInherit::Instance()->DoRoleOnline(this->GetID());
	GardenManager::Instance()->CheckGardenNtf(GetID()); 
	CGuildMgr::Instance()->DoOnlineCheck(m_qwID);
	GardenManager::Instance()->CheckGardenNtf(m_qwID);
	NestWeekMgr::Instance()->CheckHint(this);
	MentorMgr::Instance()->OnLogin(GetID());
	CReportDataMgr::Instance()->OnLogin(this);
	IBGiftMgr::Instance()->OnLogin(this);
	SocialMgr::Instance()->OnLogin(this);
	PkMgr::Instance()->OnRoleLogin(this);
	MayhemMsMgr::Instance()->OnLogin(this);
	LeagueTeamMgr::Instance()->OnRoleLogin(this);
	LeagueSeasonMgr::Instance()->OnRoleLogin(this);
	ReportMgr::Instance()->OnRoleLogin(GetID());
	PushMgr::Instance()->OnRoleLogin(m_qwID);
	CPlatformFriendMgr::Instance()->OnLoginOrOut(this, true);
	InvFightMgr::Instance()->OnLogin(this);
	ArenaStarMgr::Instance()->OnLogin(this);
	TarjaTeamMgr::Instance()->OnLogin(this);
	MarriageMgr::Instance()->OnLogin(this);
    GoalAwardsMgr::Instance()->PtcGoalAwardsRedPoint(m_qwID);
    GroupChatMgr::Instance()->LoginRedPoint(m_qwID);
	DragonGuildMgr::Instance()->DoRoleLogin(this);
	BattleFieldMgr::Instance()->OnLogin(this);
	RoleTaskMgr::Instance()->OnRoleLogin(this);
	SkyCityMgr::Instance()->OnLogin(this);

	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->TryGetInfo(m_qwID);
	if (NULL != roleinfo)
	{
		roleinfo->OnLogin(this);
	}
	CustomBattleMgr::Instance()->OnRoleLogin(this);


}

void CRole::OnEnterHall() // 每次进主城
{
	MarriageMgr::Instance()->OnEnterHall(this);
}

void CRole::Send(const CProtocol &roPtc)
{
	if (m_sessionID == INVALID_SESSION_ID)
	{
		return;
	}
	roPtc.m_sessionID = m_sessionID;
	GateLink::Instance()->SendToSession(roPtc);
}

void CRole::HintNotify(UINT32 systemid, bool isremove)
{
	if (!isremove && !IsSystemOpened(systemid))
	{
		return;
	}
	PtcM2C_HintNotifyMS ntf;
	ntf.m_Data.add_systemid(systemid);
	ntf.m_Data.set_isremove(isremove);
	Send(ntf);
}

void CRole::SetMapID(UINT32 dwMapID)
{
	UINT32 oldMapID = m_dwMapID;

	m_dwMapID = dwMapID; 

	OnMapChange(oldMapID, dwMapID);
}

bool CRole::IsInBattle()
{
	return IsInBattle(m_dwMapID);
}

bool CRole::IsInBattle(UINT32 dwMapID)
{
	if(HALL_MAP_ID == dwMapID || GUILD_MAP_ID == dwMapID)
	{
		return false;
	}
	return true;
}

bool CRole::IsInFamily()
{
	return (FAMILYGARDEN_MAP_ID == m_dwMapID); 
}

void CRole::OnEnterBattle()
{
	TeamMgr::Instance()->OnRoleEnterBattle(this);
}

void CRole::OnLeaveBattle()
{
	TeamMgr::Instance()->OnRoleLeaveBattle(this);
}

void CRole::OnMapChange(UINT32 oldMapID, UINT32 newMapID)
{
	if(!oldMapID || !newMapID)
	{
		return;
	}
	bool oldInBattle = IsInBattle(oldMapID);
	bool newInBattle = IsInBattle(newMapID);

	if(!oldInBattle && newInBattle)
	{
		OnEnterBattle();
	}
	else if(oldInBattle && !newInBattle)
	{
		OnLeaveBattle();
	}
	SkyCityMgr::Instance()->OnLeaveFightScene(this,oldMapID,newMapID);
	MayhemMsMgr::Instance()->OnMapChange(this,oldMapID,newMapID);
	BattleFieldMgr::Instance()->OnMapChange(this,oldMapID,newMapID);

}

void CRole::DailyClean()
{
	//日清变量
}

bool CRole::IsFamiliar(UINT64 roleID)
{
	FriendOp op(GetID());	
	if(op.IsFriend(roleID))
	{
		return true;
	}

	if(CGuildMgr::Instance()->IsSameGuild(GetID(), roleID))
	{
		return true;
	}

	return false;
}

void CRole::UpdateSelfServer(UINT32 level)
{
	if (0 != level)
	{
		PtcM2I_UpdateSelfServer ntf;
		ntf.m_Data.set_accountid(m_accountID);
		ntf.m_Data.set_serverid(m_serverId);
		ntf.m_Data.set_level(level);
		LoginLink::Instance()->SendTo(ntf);
	}
}

void CRole::SendMsgToGS(const CProtocol& msg)
{
	msg.m_sessionID = m_sessionID;
	// 本服GS直接发送
	if (!m_isCrossGs)
	{
		GSLink::Instance()->SendToLine(GetGsLine(), msg);
	}
	// 跨服GS发给Router
	else
	{
		RSLink::Instance()->SendTo(msg);
	}
}

void CRole::SendMsgToGS(CRpc& msg)
{
	msg.m_sessionID = m_sessionID;
	// 本服GS直接发送
	if (!m_isCrossGs)
	{
		GSLink::Instance()->SendToLine(GetGsLine(), msg);
	}
	// 跨服GS发给Router
	else
	{
		RSLink::Instance()->SendTo(msg);
	}
}

void CRole::SetIdipPunishData(KKSG::IdipPunishData& oData)
{
	auto it = m_vPunishInfo.find(oData.type());
	if (it == m_vPunishInfo.end())
	{
		m_vPunishInfo.insert(std::make_pair(oData.type(), oData));
	}
	else
	{
		it->second.CopyFrom(oData);
	}
}

void CRole::UnDoIdipPunishData(INT32 iType)
{
	auto it = m_vPunishInfo.find(iType);
	if (it != m_vPunishInfo.end())
	{
		m_vPunishInfo.erase(it);
	}
}

bool CRole::CheckIdipAndNotify(INT32 iType)
{
	auto it = m_vPunishInfo.find(iType);
	if (it != m_vPunishInfo.end())
	{
		if ((it->second.punishtime() + it->second.bantime()) > (INT32)time(NULL))
		{
			PtcM2C_IdipPunishInfoMsNtf ptc;
			ptc.m_Data.set_type(iType);
			ptc.m_Data.set_bantime(it->second.bantime());
			ptc.m_Data.set_endtime(it->second.punishtime() + it->second.bantime());
			ptc.m_Data.set_lefttime(INT32(ptc.m_Data.endtime()-time(NULL)));
			Send(ptc);
			return true;
		}
		m_vPunishInfo.erase(it);
	}
	return false;
}

void CRole::SetTeamID(int teamid)
{
	SSInfo << __FUNCTION__ << " role = " << GetID() << " team = " << teamid << END;
	if(!m_teamID && teamid)
	{
		HMatchHandler handler;
		handler.OnJoinTeam(this);
	}

	int oldTeamID = m_teamID;	

	m_teamID = teamid;
}

bool CRole::HasTeam()
{
	return (0 != GetTeamID());
}

bool CRole::HasTeamInTs()
{
	return HTeam::IsTsTeam(GetTeamID());
}

bool CRole::HasTeamInMs()
{
	return HTeam::IsMsTeam(GetTeamID());
}

void CRole::SetMatchType(int type)
{
	SSInfo << " match role = " << GetID() << " old = " << m_matchType << " new = " << type << END;
	m_matchType  = type;
	m_matchUniIndex = HMatchHandler::GetMatchUniqueIndex(true);
	MatchTypeNtf();
}

void CRole::MatchTypeNtf()
{
	PtcM2C_RoleMatchStateM2CNotify oPtc;
	if(KKSG::KMatchType_IsValid(m_matchType))
	{
		oPtc.m_Data.set_matchtype(KKSG::KMatchType(m_matchType));
	}
	Send(oPtc);
}
KKSG::PayParameterInfo&  CRole::GetPayParameterInfo(bool isCheck)
{
	if (isCheck && (m_oPayParameterInfo.pf().empty() || m_oPayParameterInfo.pfkey().empty() || m_oPayParameterInfo.appid().empty()))
	{
		PtcM2C_PayParameterInfoInvalidNtf ptc;
		Send(ptc);
	}
	return m_oPayParameterInfo;
}

void CRole::DoOpenSystem(UINT32 systemID)
{
	if (systemID == SYS_MENTOR)
	{
		MentorShipOneSide* oneside = MentorMgr::Instance()->GetMentorShip(GetID());
		if (!oneside)
		{
			return;
		}

		oneside->DoOpen();
		LogInfo("%llu, %s, open mentor system", GetID(), GetName().c_str());
	}
}

bool CRole::IsSystemOpened(UINT32 systemID)
{
	return !OpenSystemMgr::Instance()->IsClosedSystem(systemID);
}

void CRole::FillTeamInviteData(KKSG::TeamInvite& data)
{
	data.set_invfromroleid(GetID());
	data.set_invfromrolename(GetName());
	data.set_invguildid(CGuildMgr::Instance()->GetGuildIdByRoleId(GetID()));
	data.set_invdragonguildid(DragonGuildMgr::Instance()->GetDragonGuildIdByRole(GetID()));
}

void CRole::ErrorCodeNtf(UINT32 errorno, bool istip)
{
	PtcM2C_MSErrorNotify ptc;
	ptc.m_Data.set_errorno(errorno);
	ptc.m_Data.set_istip(istip);
	Send(ptc);
}

void CRole::SetCustomBattleReq(KKSG::CustomBattleOp op)
{
	if ((int)op >= KKSG::CustomBattleOp_ARRAYSIZE || (int)op <= 0)
	{
		SSWarn<<"invalid op:"<<op<<END;
		return;
	}
	if (KKSG::CustomBattle_Create == op)
	{
		m_CustomBattleReqCD[op] = GameTime::GetTime() + GetGlobalConfig().CustomBattleCreateCD;
	}
	else
	{
		m_CustomBattleReqCD[op] = GameTime::GetTime() + GetGlobalConfig().CustomBattleReqCD;
	}
}

void CRole::ClearCustomBattleReq(KKSG::CustomBattleOp op)
{
	if ((int)op >= KKSG::CustomBattleOp_ARRAYSIZE || (int)op <= 0)
	{
		SSWarn<<"invalid op:"<<op<<END;
		return;
	}
	m_CustomBattleReqCD[op] = 0;
}

bool CRole::IsCustomBattleTimeout(KKSG::CustomBattleOp op)
{
	if ((int)op >= KKSG::CustomBattleOp_ARRAYSIZE || (int)op <= 0)
	{
		SSWarn<<"invalid op:"<<op<<END;
		return false;
	}
	if (GameTime::GetTime() >= m_CustomBattleReqCD[op])
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CRole::FillCrossData(KKSG::RoleCrossData& data)
{
	if (data.roleid() && data.roleid() != GetID())
	{
		SSError << " roleid not match data_roleid = " << data.roleid() << " Role_roleid = " << GetID() << END;
		return;
	}

	data.set_roleid(GetID());
	auto pGuild =  CGuildMgr::Instance()->GetGuildByRoleId(GetID());
	if (pGuild)
	{
		pGuild->FillGuildBrief(data.mutable_guild());
	}
}

void CRole::SetCacheZoneRole(const google::protobuf::RepeatedPtrField<KKSG::ZoneRoleInfo>& roles)
{ 
	m_ZoneRoleCache.CopyFrom(roles); 
	m_allZonePayCnt = 0;
	for (int i = 0; i < m_ZoneRoleCache.size(); ++i)
	{
		m_allZonePayCnt += m_ZoneRoleCache.Get(i).paycnt();
	}
}
