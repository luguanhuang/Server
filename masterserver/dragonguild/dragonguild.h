#ifndef __DRAGON_GUILD_H__
#define __DRAGON_GUILD_H__

#include "dragonguildshop.h"
#include "dragonguildliveness.h"
#include "dragonguildapps.h"
#include "dragonguildbindinfo.h"
#include "dragonguildtask.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "dragonguilddef.h"

struct DragonGuildBriefStruct
{
	UINT64 m_id;
	UINT64 m_leaderId;
	int m_level;
	int m_exp;
	int m_rank;
	UINT32 m_createTime;
	std::string m_leaderName;
	std::string m_name;
	
	UINT32 m_icon;
	UINT32 m_recuirtPPT;
	UINT32 m_needApproval;
	UINT32 m_mapId;
	UINT32 m_mapPassCnt;
	UINT32 m_firstPassTime;
	
	std::string m_announcement;
	UINT64 m_totalPPT;

	DragonGuildBriefStruct()
	{
		Init();
	}

	void Init();
	bool Load(const KKSG::DragonGuildBaseData& baseData);
	void Save(KKSG::DragonGuildBaseData& baseData) { baseData.mutable_brief()->assign(ToBinaty()); }
	const std::string ToBinaty();
};

struct DragonGuildMember 
{
	DragonGuildMember()
	{	
		roleId = 0;
		position = DGPOS_MEMBER;
		activity = 0;
		joinTime = 0;
		refreshShopLevel = 0;
		ppt = 0;
		groupPos =  KKSG::GBS_NotMember;
		updateBindTime = 0;
	}
	UINT64 roleId;
	DragonGuildPosition position;
	UINT32 activity;				// 活跃度
	UINT32 joinTime;				// 加入时间
	UINT32 refreshShopLevel;	// 商店刷新时等级
	UINT32 ppt;
	UINT32 openShopTime;
	KKSG::GuildBindStatus groupPos;	
	UINT32 updateBindTime;					/// 更新公会群信息的时间

	void FromKKSG(const KKSG::DragonGuildMemberData& member)
	{
		roleId = member.roleid();
		position = (DragonGuildPosition)member.position();
		activity = member.activity();
		joinTime = member.jointime();
		refreshShopLevel = member.refreshshoplevel();
		ppt = member.ppt();
		openShopTime = member.openshoptime();
		groupPos = member.bindstatus();
		updateBindTime = member.updatebindtime();
	}

	void ToKKSG(KKSG::DragonGuildMemberData& data)
	{
		data.set_roleid(roleId);
		data.set_position((UINT32)position);
		data.set_activity(activity);
		data.set_jointime(joinTime);
		data.set_refreshshoplevel(refreshShopLevel);
		data.set_ppt(ppt);
		data.set_openshoptime(openShopTime);
		data.set_bindstatus(groupPos);
		data.set_updatebindtime(updateBindTime);
	}
};

class CProtocol;
class CRole;
class TDragonGuildFlowLog;
class DragonGuild
{
public:
	struct DragonGuildMembersCMP
	{
		bool operator()(DragonGuild *pG1, DragonGuild *pG2) const;
	};

	struct DragonGuildLevelCMP
	{
		bool operator()(DragonGuild *pG1, DragonGuild *pG2) const;
	};

	struct DragonGuildPPTCMP
	{
		bool operator()(DragonGuild *pG1, DragonGuild *pG2) const;
	};

	struct DragonGuildSceneID
	{
		bool operator()(DragonGuild *pG1, DragonGuild *pG2) const;
	};

	struct DragonGuildName
	{
		bool operator()(DragonGuild *pG1, DragonGuild *pG2) const;
	};

	struct DragonGuildLeaderName
	{
		bool operator()(DragonGuild *pG1, DragonGuild *pG2) const;
	};

public:
	DragonGuild();
	~DragonGuild();

	void LoadFromDB(const KKSG::DragonGuildSaveData& data);

	void FromKKSG(const KKSG::DragonGuildSaveData& data);
	void ToKKSG(KKSG::DragonGuildSaveData& data);
	void OnLoaded();

	void AddToDB();
	void UpdateToDB();
	void DelToDB();
	void ReplaceIdToDB();
	
	UINT64 GetID() { return m_brief.m_id; }
	std::string GetName() { return m_brief.m_name; }
	std::string GetLeaderName() { return m_brief.m_leaderName; }
	UINT64 GetLeaderId() { return m_brief.m_leaderId; }
	UINT32 GetRecuitPPT() { return m_brief.m_recuirtPPT; }
	UINT32 GetNeedApproval() { return m_brief.m_needApproval; }
	std::string GetAnnoucement() { return m_brief.m_announcement; }
	UINT32 GetCapacity();
	UINT64 GetTotalPPT() { return m_brief.m_totalPPT; }
	UINT32 GetSceneId() { return m_brief.m_mapId; }
	UINT32 GetSceneCnt() { return m_brief.m_mapPassCnt; }
	void SetID(UINT64 ID) { m_brief.m_id = ID; }
	void SetRecuitPPT(UINT32 recuitPPt ) { m_brief.m_recuirtPPT = recuitPPt; }
	void SetNeedApproval(UINT32 approval) { m_brief.m_needApproval = approval; }
	void SetAnnoucement(std::string annoucement) { m_brief.m_announcement = annoucement; }
	void SetLeaderName(std::string leadername) { m_brief.m_leaderName = leadername; }
	void SetLeaderID(UINT64 leaderId) { m_brief.m_leaderId = leaderId; }
	void SetName(std::string name);
	void SetMember(std::vector<UINT64>& memberIDs);
	inline bool ContainStr(const std::string &s) { return (m_brief.m_name.find(s) != std::string::npos); }
	void AddTotalPPT(UINT32 ppt) { m_brief.m_totalPPT += ppt; }
	void ReduceTotalPPT(UINT32 ppt);
	UINT32 GetViceLeaderCnt();
	UINT32 GetServerID(){ return (UINT32)((GetID() >> 48) & 0xFFFF); }
	DragonGuildPosition GetPosition(UINT64 roleID);

	UINT32 GetLiveness() { return GetDragonGuildLiveness().GetLiveness(); }
	UINT32 GetLevel() { return m_brief.m_level; }
	UINT32 GetExp() { return m_brief.m_exp; }
	void AddExp(UINT32 exp);
	void AddPassSceneCount(UINT32 sceneID);

	void GetDragonGuildMemberIds(std::vector<UINT64>& roleIds);
	DragonGuildMember* GetMember(UINT64 roleId);
	void RemoveMember(UINT64 roleId);
	void AddMember(UINT64 roleId, DragonGuildPosition pos);
	KKSG::ErrorCode ChangeMemberPosition(DragonGuildMember* pMember, DragonGuildPosition pos);
	void NotifyPosition(UINT64 roleId, DragonGuildPosition pos);
	void FillMemberDetail(DragonGuildMember& member, KKSG::DragonGuildMemberDetail& data);
	void FillAllMemberDetail(KKSG::DragonGuildDetailInfoRes& res);

	void BroadCastMail(MailData& mail);
	void BroadCast(const CProtocol& ptc);
	void BroadCastToGS(const CProtocol& ptc);
	void SendDragonGuildBaseInfo(CRole* pRole);
	void SendDragonGuildSettingInfo(CRole* pRole);
	void SendDragonGuildBaseInfoToAll();
	void SendDragonGuildSettinfInfoToAll();
	void GetSessions(std::vector<SessionType>& sessions);

	KKSG::ErrorCode DoLeaveMember(UINT64 roleId, bool force = false);
	void OnChangePPT(UINT64 roleId, UINT32 ppt);

	void DoRoleLogin(CRole* pRole);
	KKSG::ErrorCode DoMiss();

	void OnDayPass();
	void OnWeekPass();
	void OnCreate(CRole* pRole, UINT64 ID, std::string name);

	void SetDirty();
	bool IsDirty() { return m_dirty; }
	inline void SetRank(UINT32 rank) { m_brief.m_rank = rank;}
	void DoTxLog(TDragonGuildFlowLog& oLog);

	bool IsFull();
	UINT32 AddApplication(UINT64 roleId, UINT32 attr);

	std::map<UINT64, DragonGuildMember>& GetAllMember() { return m_members; }
	UINT32 GetMemberCount() { return m_members.size(); }

	inline UINT32 GetRecruitCDTime() { return m_recruitCDTime; }
	inline void SetRecruitCDTime(UINT32 nowTime) { m_recruitCDTime = nowTime; }

public:
	enum SYN_TYPE
	{
		ADD_MEMBER      = 1,
		DEL_MEMBER      = 2,
		DEL_GUILD       = 3,
		SYN_LEVEL       = 4,
		SYN_POSTION     = 5,
		SYN_NAME		= 6,
	};

	void SynSimpleDataToGs(SYN_TYPE type, UINT64 roleId = 0);

private:
	UINT32 m_recruitCDTime;
	bool m_dirty;
	DragonGuildBriefStruct m_brief;
	std::map<UINT64, DragonGuildMember> m_members;

#define DRAGONGUILD_MODULE(module) \
private: \
	module m_##module; \
public: \
	module& Get##module() { return m_##module; }
#include "dragonguildmodule.h"
#undef DRAGONGUILD_MODULE

};

#endif
