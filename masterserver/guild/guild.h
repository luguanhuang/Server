#ifndef __GUILD_H__
#define __GUILD_H__

#include "guild/guilddef.h"
#include "pb/project.pb.h"
#include "table/guildConfig.h"
#include "guild/rpcg2m_opguildreqdata.h"
#include "role/switch.h"
#include "guild/guildapps.h"
#include "table/globalconfig.h"
#include "guild/guildbindinfo.h"
#include "mail/maildata.h"


class CRole;
class Scene;
class IGuildListener;
class CProtocol;
class TGuildFlowLog;

typedef void (*MemberCB)(GuildMember *pMember, void *arg);

struct GuildBrief : public IGuildSerializerInterface
{
	UINT64 m_ID;
	UINT64 m_LeaderID;
	int    m_Level;
	int	   m_exp;
	UINT32 m_lastExp;
	int    m_Icon;
	int    m_RecruitPPT;
	int    m_NeedApproval;
	int    m_capcity;
	int    m_rank;
	UINT32 m_dare;
	UINT32 m_dareLvl;
	UINT32 m_bossIndex;
	UINT32 m_openTime;
	UINT32 m_prestige; // 威望
	std::string m_Annoucement;
	std::string m_LeaderName;
	std::string m_Name;
	std::map<UINT64, GuildMember> m_Members;
	std::map<UINT32, UINT32> m_studySkill;
	GuildBindInfo m_bindInfo;
	Switch chengeRec;

	UINT32 max_territory_level_;

	GuildBrief();
	inline void SetChange() { chengeRec.Set(); }
	virtual bool IsChanged();
	bool Load(const KKSG::GuildAllDBData & guilddata);
	void Save(KKSG::GuildAllDBData & guilddata) { guilddata.mutable_brief()->assign(ToBinary()); }
	const std::string ToBinary();
};

////////////////////////////////////////////////////////////////////
class Guild 
{

public:
	enum SYN_TYPE
	{
		ADD_MEMBER      = 1,
		DEL_MEMBER      = 2,
		DEL_GUILD       = 3,
		SYN_COIN        = 4,
		SYN_LEVEL       = 5,
		SYN_POSTION     = 6,
		SYN_NAME		= 7,
	};

	enum GUILD_REWARD
	{
		GUILD_EXP       = 1, // 公会经验
		GUILD_PRESTIGE  = 2, // 公会威望
	};

	struct GuildLevelCMP
	{
		bool operator()(Guild *pG1, Guild *pG2) const;
	};

	struct GuildMemberCountCMP
	{
		bool operator()(const Guild *pG1, const Guild *pG2) const;
	};

	struct GuildNameCMP
	{
		bool operator()(const Guild *pG1, const Guild *pG2) const;
	};

	struct GuildExpCMP
	{
		bool operator()(const Guild *pG1, const Guild *pG2) const;
	};

	struct GuildPrestigeCMP
	{
		bool operator()(Guild *pG1, Guild *pG2) const;
	};
	
public:
	Guild();
	~Guild();

	void InitSerializer();
	void InsertTODB();
	void SaveToDB(UINT32& num);
	void LoadFromDB(const KKSG::GuildAllDBData & guilddata);
	void Dismiss();                 // 解散公会
	void DoRoleLogin(CRole *pRole); // 玩家上线处理
	void DoDailyWork();             // 0点处理
	void DoWeekWork();              // 周事件处理（周一0点） 
	void OnCreate(CRole *pRole, UINT64 gid, const std::string &gName);

	inline UINT64 GetID() { return m_pBrief->m_ID; }
	inline UINT32 GetLevel() { return m_pBrief->m_Level; }
	inline int GetExp() { return m_pBrief->m_exp; }
	inline int GetLastExp() { return m_pBrief->m_lastExp; }
	inline int GetCapacity() { return m_pBrief->m_capcity; }
	inline int GetIcon() { return m_pBrief->m_Icon; }
	inline int GetRecuritPPT() { return m_pBrief->m_RecruitPPT; }
	inline int GetNeedApproval() { return m_pBrief->m_NeedApproval; }
	inline int GetRank() { return m_pBrief->m_rank; }
	inline UINT32 GetPrestige() { return m_pBrief->m_prestige; }
	inline UINT64 GetLeaderID() { return m_pBrief->m_LeaderID; }
	inline const std::string &GetName() { return m_pBrief->m_Name; }
	inline const std::string &GetLeaderName() { return m_pBrief->m_LeaderName; }
	inline const std::string &GetAnnoucement() { return m_pBrief->m_Annoucement; }
	inline bool ContainStr(const std::string &s) { return (m_pBrief->m_Name.find(s) != std::string::npos); }
	inline int GetMemberCount() { return m_pBrief->m_Members.size(); }
	inline int GetCapcityCount() { return m_pBrief->m_capcity; }
	inline int GetDareTimes() { return m_pBrief->m_dare; }
	inline UINT32 GetBossIndex() { return m_pBrief->m_bossIndex; }
	inline UINT32 GetDareLvl() { return m_pBrief->m_dareLvl; }
	inline UINT32 GetOpenTime() { return m_pBrief->m_openTime; }
	inline const std::map<UINT64, GuildMember>& GetGuildMember() { return m_pBrief->m_Members; }
	inline bool isLeader(UINT64 roleId) { return GetPosition(roleId) == GPOS_LEADER ? true : false; }
	inline bool isViceLeader(UINT64 roleId){ return GetPosition(roleId) == GPOS_VICELEADER ? true : false; }
	inline bool isOfficer(UINT64 roleId){ return GetPosition(roleId) == GPOS_OFFICER ? true : false; }
	inline bool isRuler(UINT64 roleId){return isLeader(roleId) || isViceLeader(roleId) || isOfficer(roleId); }
	inline bool IsFull() { return (int)m_pBrief->m_Members.size() >= m_pBrief->m_capcity; }
	inline bool IsApplyFull() { return Get<GuildApplication>()->Count() >= GetGlobalConfig().GuildApplyMax; }
	inline UINT32 CanDareRewardTimes() { return CGuildConfig::Instance()->GetCanDareRewardTimes(GetLevel(), GetDareTimes()); }
	inline std::map<UINT32, UINT32>&  GetStudySkill() { return m_pBrief->m_studySkill; }
	inline void SetRank(int rank) { m_pBrief->m_rank = rank; m_pBrief->SetChange(); }
	inline void ClearDareData() { m_pBrief->m_dare = 0; m_pBrief->SetChange(); }
	inline bool CheckDareReward(UINT32 dareNum) { return CGuildConfig::Instance()->GetChallegeConf(GetLevel(), dareNum) == NULL ? false : true; }
	inline void SetDareLvl(UINT32 dareLvl) { m_pBrief->m_dareLvl = dareLvl; m_pBrief->SetChange(); }
	inline void DecLastExp(UINT32 decNum) { m_pBrief->m_lastExp = m_pBrief->m_lastExp > decNum ? (m_pBrief->m_lastExp - decNum) : 0; m_pBrief->SetChange();}
	inline void SetGuildHall(Scene *pScene) { m_pGuildHall = pScene; }
	inline void SetBossScene(Scene *pScene) { m_pBossScene = pScene; }
	inline void AddListener(IGuildListener *l) { m_listener.push_back(l); }
	inline Scene *GetGuildHall() { return m_pGuildHall; }
	inline Scene *GetBossScene() { return m_pBossScene; }
	inline UINT32 GetRecruitCDTime() { return m_recruitCDTime; }
	inline void SetRecruitCDTime(UINT32 nowTime) { m_recruitCDTime = nowTime; }
	template<typename T> T *Get() { return (T*)m_serializers[IndexOf<GuildHandlerType, T>::value]; }
	template<typename T> void Set(T *p) { m_serializers[IndexOf<GuildHandlerType, T>::value] = p; }
	GuildBindInfo& GetBindGroupInfo() { return m_pBrief->m_bindInfo; }
	UINT32 GetServerId();

	void AddExp(UINT32 exp);
	void DelExp(UINT32 delExp);
	void LevelUp();
	void AddMember(UINT64 roleId, GuildPosition pos);
	void RemoveMember(UINT64 roleId);
	void FindRuler(std::vector<UINT64>& ruleVec);
	void GetVicLeader(std::set<UINT64>& vicLeaderSet);
	void SetLeader(UINT64 roleId);
	void SetAnnoucement(const std::string &s);
	void SetIcon(int icon);
	void SetRecruitPowerPoint(int ppt);
	void SetNeedApproval(int n);
	void SetBossIndex(UINT32 bossIndex);
	void GMSetBossIndex(UINT32 bossIndex);
	void AddPrestige(UINT32 addNum);
	void DelPrestige(UINT32 delNum);
	int GetOnLineNum();
	GuildPosition GetPosition(UINT64 roleId);
	int CountPosition(GuildPosition pos);
	void GetSessions(std::vector<SessionType>& sessions);
	bool IsPlayerInGuild(UINT64 roleId);
	int  AddApplication(UINT64 roleId, UINT32 attr);
	int  SetMemberPos(UINT64 qwRoleID, GuildPosition pos);
	void ForeachMember(MemberCB cb, void *arg);
	void NotifyPosition(UINT64 roleId, GuildPosition pos);
	void AddRoleContribute(UINT64 qwRoleID, int contribute);
	void SetRoleContribute(UINT64 qwRoleID, int contribute);
	GuildMember* GetMember(UINT64 qwRoleID);
	GuildMember* FindViceLeader();
	UINT32 GetSkillLvl(UINT32 skillId);
	void AddSkillLvl(UINT32 skillId);
	UINT32 canStudySkill(UINT64 roleId, UINT32 skillId);
	void doStudySkill(UINT32 skillId);
	void AddDareTimes(UINT32 addNum);
	void BroadCast(const CProtocol& ptc);
	void UpdateGuildSkillDataToEveryone();
	void sendCmdToAllMember(const CProtocol &roPtc);
	void RecordBossDps(UINT64 roleId);
	void FillGuildBrief(KKSG::MyGuild* ginfo);
	void GuildDataToGS(const OpGuildReqDataArg &roArg, KKSG::OpGuildReqDataRes& res);
	void SynSimpleDataToGs(SYN_TYPE type, UINT64 roleId = 0);  //  同步简要信息到gs
	UINT32 GetJoinTime(UINT64 roleId);

	void SendGuildRankReward(UINT32 rank);
	void AddGuildReward(std::map<UINT32, UINT32>& guildRewardMap);
	void DoTxLog(TGuildFlowLog& oLog);
	UINT32 GetAllExp();

	void SetMaxTerritoryLevel(UINT32 max_territory_level);
	UINT32 GetMaxTerritoryLevel();
	void SetName(const std::string &name);

	void OnLeaderChange(UINT64 oldLeaderId, UINT64 newLeaderId);
	void ChangeLeaderName(UINT64 roleid,const std::string &name);
	void SendModifyNameMail(MailData &mail);
private:
	void SetMemberPosition(GuildMember *pMember, GuildPosition pos);

	void _SendGuildInfo(CRole* pRole);
	void _SendGuildSkillInfo(CRole* pRole);
	void _SynRoleInGuild(CRole* pRole);
	void _DelPrestigeWeekly();  // 公会威望周衰减处理
	void _AddDesignation(UINT64 nRole,UINT32 nRank);

private:
	UINT32 m_recruitCDTime;
	Scene *m_pGuildHall;
	Scene *m_pBossScene;
	GuildBrief *m_pBrief;
	std::vector<IGuildListener *> m_listener;
	IGuildSerializerInterface *m_serializers[Length<GuildHandlerType>::value];

};

#endif // __GUILD_H__