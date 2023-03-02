#ifndef _H_Role_H__
#define _H_Role_H__

#include "baselink.h"
#include "commondef.h"
#include "pb/project.pb.h"

namespace KKSG
{
	class RoleInfoOnMs;
};

enum ERoleState
{
	ROLE_STATE_INVALID,
	ROLE_STATE_IN_GAME,				//游戏中
	ROLE_STATE_LEAVE_SCENE,			//离开场景中
	ROLE_STATE_ENTER_SCENE,			//进入场景中
};

struct SceneSwitchInfo
{
	UINT32		m_dwDestGsLine;
	UINT64		m_qwSceneOwnerID;	//
	UINT32		m_dwDestMapID;		// TemplateID，静态场景，副本使用
	UINT32		m_dwDestSceneID;	// InstanceID, 进入一个指定场景
	bool		m_isCrossGs;		// DestGsLine是否跨服GS
	KKSG::SceneSwitchData m_oData;

	SceneSwitchInfo()
		:m_dwDestGsLine(INVALID_LINE_ID)
		,m_qwSceneOwnerID(0)
		,m_dwDestMapID(INVALID_MAP_ID)
		,m_dwDestSceneID(INVALID_SCENE_ID)
		,m_isCrossGs(false)
	{
	}
	void Clear()
	{
		m_dwDestGsLine = INVALID_LINE_ID;
		m_qwSceneOwnerID = 0;
		m_dwDestMapID = INVALID_MAP_ID;
		m_dwDestSceneID = INVALID_SCENE_ID;
		m_isCrossGs = false;
	}
};

class CProtocol;
class CRole
{
public:
	CRole();
	~CRole();

	bool Init(const KKSG::RoleInfoOnMs& roInfo, UINT64 sessionID);
	void Uninit();


	UINT64 GetID() const				{ return m_qwID; }
	SessionType GetSessionID() const	{ return m_sessionID; }
	const std::string& GetAccount() const{ return m_accountID; }
	const std::string &GetName() const	{ return m_strName; }
	const std::string &GetIP() const	{ return m_strIP; }
	void SetIp(const std::string& ip) { m_strIP = ip; }
	ERoleState GetState() const			{ return m_nState; }
	UINT32 GetGsLine() const			{ return m_dwGsLine; }
	UINT32 GetGateLine() const			{ return (m_sessionID >> SessionGateIDShift) & 0xFF; }
	UINT32 GetMapID() const				{ return m_dwMapID; }
	UINT32 GetSceneID() const			{ return m_dwSceneID; }
	UINT32 GetCreateTime() const		{ return m_createtime; }
	UINT32 GetLoginTime() const	        { return m_logintime; }
	const std::string &GetToken() const { return m_Token; }
	void SetToken(const std::string& token) { m_Token = token; }
	bool IsInCrossGs()					{ return m_isCrossGs; }
	void SetInCrossGs(bool cross)		{ m_isCrossGs = cross; }
	UINT32 GetServerId() const			{ return m_serverId; }

	void SetSessionID(SessionType sessionID) { m_sessionID = sessionID; }
	void SetAccount(const std::string& str)	 { m_accountID = str; }
	void SetRoleID(UINT64 qwID)			{ m_qwID = qwID; }
	void SetName(const std::string& str){ m_strName = str; }
	void SetIP(const std::string& str)	{ m_strIP = str; }
	void SetState(ERoleState nState)	{ m_nState = nState; }
	void SetGsLine(UINT32 dwLine)		{ m_dwGsLine = dwLine; }
	void SetMapID(UINT32 dwMapID);		
	void SetSceneID(UINT32 dwSceneID)	{ m_dwSceneID = dwSceneID; }
	void SetCreateTime(UINT32 time)		{ m_createtime = time; }
	void SetLastVaildMapID(UINT32 dwVaildMapID);
	void DailyClean();

	int GetTeamID(){ return m_teamID; }
	void SetTeamID(int teamid);
	bool HasTeam();
	bool HasTeamInTs();//有跨服队伍
	bool HasTeamInMs();//有本服队伍

	SceneSwitchInfo& GetSceneSwitchInfo(){ return m_oSceneSwitchInfo; }
	void SetPayParameterInfo(const KKSG::PayParameterInfo& oData) { m_oPayParameterInfo = oData; }
	KKSG::PayParameterInfo& GetPayParameterInfo(bool isCheck=false);
	KKSG::LoginType GetLoginType() { return m_loginType;}
	void SetLoginType(KKSG::LoginType type) { m_loginType = type; }
	INT32 GetLoginPlatID() { return m_loginplatid;}
	void SetLoginPlatID(int id) { m_loginplatid = id; }
	bool GetIsBalance(){ return m_bIsGetBalance; }
	void SetIsBalance(bool isStatus) { m_bIsGetBalance = isStatus;}
	bool GetIsPayFirstGetBalance() { return m_bPayFirstGetBalance;}
	void SetIsPayFirstGetBalance(bool isStatus) { m_bPayFirstGetBalance = isStatus;}

	void OnEnterScene();
	void OnEnterOwnScene(const UINT64 garden_id);
	void OnLogin(UINT32 level);
	void OnLoginReconnect();
	void OnFirstEnterScene();
	void OnLogout(UINT32 level);
	// role first enter scene after login 
	void OnEnterHall();

	//关于战斗状态 (待优化)
	void OnMapChange(UINT32 oldMapID, UINT32 newMapID);
	bool IsInBattle();
	bool IsInBattle(UINT32 dwMapID);
	void OnEnterBattle();
	void OnLeaveBattle();
	bool IsInFamily();

	bool IsFamiliar(UINT64 roleID);

	void FillTeamInviteData(KKSG::TeamInvite& data);

	void Send(const CProtocol &roPtc);
	void SendMsgToGS(const CProtocol& msg);
	void SendMsgToGS(CRpc& msg);

	void HintNotify(UINT32 systemid, bool isremove);
	void UpdateSelfServer(UINT32 level);

	void SetIdipPunishData(KKSG::IdipPunishData& oData);
	void UnDoIdipPunishData(INT32 iType);
	bool CheckIdipAndNotify(INT32 iType);

	int GetMatchType() { return m_matchType; }
	UINT64 GetMatchUniIndex() { return m_matchUniIndex; }
	void SetMatchType(int type);
	void MatchTypeNtf();

	void DoOpenSystem(UINT32 systemID);
	bool IsSystemOpened(UINT32 systemID);

	UINT32 GetLastPayTokenTime() { return m_LastCheckPayTokenTime; }
	void SetLastPayTokenTime(UINT32 time) { m_LastCheckPayTokenTime = time;}

	void ErrorCodeNtf(UINT32 errorno, bool istip);

	void SetCustomBattleReq(KKSG::CustomBattleOp op);
	void ClearCustomBattleReq(KKSG::CustomBattleOp op);
	bool IsCustomBattleTimeout(KKSG::CustomBattleOp op);

	UINT32 GetCostDiamond(){return m_TotalCostDiamond;}
	void SetCostDiamond(UINT32 num){m_TotalCostDiamond = num;}

	void FillCrossData(KKSG::RoleCrossData& data);
	bool IsAccountBackFlow() { return m_accountBackFlow; }

	google::protobuf::RepeatedPtrField<KKSG::ZoneRoleInfo>& GetCacheZoneRole() { return m_ZoneRoleCache; }
	UINT64 GetChargeBackRoleId() { return m_chargeBackRoleId; }

	void SetChargeBackRoleId(UINT64 roleId) { m_chargeBackRoleId = roleId; }
	void SetCacheZoneRole(const google::protobuf::RepeatedPtrField<KKSG::ZoneRoleInfo>& roles);
	UINT32 GetAllZonePayCnt() { return m_allZonePayCnt; }


	
private:
	SessionType m_sessionID;
	std::string m_accountID;		//tencent openid
	UINT32		m_serverId;
	ERoleState	m_nState;
	UINT64		m_qwID;
	std::string	m_strName;
	std::string	m_strIP;
	UINT32		m_createtime;
	UINT32      m_logintime;
	std::string m_Token;

	UINT32		m_dwGsLine;
	bool		m_isCrossGs;		//是否跨服GS
	UINT32		m_dwMapID;			//所在场景模板ID
	UINT32		m_dwSceneID;		//所在场景实例ID (动态场景有效)

	int m_teamID;//玩家队伍ID 为0表示没有队伍

	int m_matchType;
	UINT64 m_matchUniIndex;

	SceneSwitchInfo	m_oSceneSwitchInfo;

	KKSG::PayParameterInfo m_oPayParameterInfo;

	KKSG::LoginType m_loginType;

	INT32 m_loginplatid;//玩家登录的是ios，安卓，主要是混服时用config中的难以区分;
	std::unordered_map<INT32, KKSG::IdipPunishData> m_vPunishInfo;//type=>data 少部分玩法在ms上控制的,用来判断封禁玩法的
	bool m_bIsGetBalance;
	bool m_bPayFirstGetBalance;
	UINT32 m_LastCheckPayTokenTime;
	UINT32 m_TotalCostDiamond;

	///> custom battle req cd
	time_t m_CustomBattleReqCD[KKSG::CustomBattleOp_ARRAYSIZE];
	bool m_accountBackFlow;

	google::protobuf::RepeatedPtrField<KKSG::ZoneRoleInfo> m_ZoneRoleCache;
	UINT64 m_chargeBackRoleId;
	UINT32 m_allZonePayCnt;
};

#endif