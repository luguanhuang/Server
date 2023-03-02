#ifndef __GUILD_BONUS_H__
#define __GUILD_BONUS_H__

#include "guild/guilddef.h"
#include "guildlistener.h"
#include "guild.h"
#include <time.h>
#include "table/GuildBonusTable.h"
#include "pb/project.pb.h"
#include "guild/guildmgr.h"
#include "role/switch.h"
#include "timer.h"

class CRole;

enum
{
	BONUS_MONEY = 7,
	BONUS_GOLD = 1,
};

enum
{
	BONUS_CAN_GET,
	BONUS_CANNOT_GET,
	BONUS_ALREADY_GET,
	BONUS_ALL_GET,
};

#define BONUS_GET_TIME 43200
#define BONUS_BRINGBACK_TIME 43200
#define BONUS_MAXNUM 40

struct GetBonusInfo
{
	bool canThank;
	UINT32 getNum;
	UINT32 getTime;
	UINT64 roleID;
	std::string roleName;
	GetBonusInfo()
	{
		getNum = 0;
		getTime = 0;
		roleID = 0;
		canThank = true;
	}

	void load(const KKSG::GetGuildBonusInfo& a)
	{
		getNum = a.getnum();
		getTime = a.gettime();
		roleID = a.roleid();
		roleName = a.rolename();
		if(a.has_canthank())
			canThank = a.canthank();
	}

	void save(KKSG::GetGuildBonusInfo* a)
	{
		a->set_getnum(getNum);
		a->set_gettime(getTime);
		a->set_roleid(roleID);
		a->set_rolename(roleName);
		a->set_canthank(canThank);
	}
};

struct GuildBonusInfo
{
	UINT32 bonusID;
	UINT8 bonusType;//≈‰÷√Ω±¿¯
	UINT16 maxPeopleNum;
	UINT32 bonusNum;
	UINT32 sendTime;
	UINT32 templateID;//≈‰÷√id
	UINT32 getTime;
	UINT64 sourceCRoleID;
	UINT64 leaderID;
	UINT64 luckestID;
	std::string source;
	std::string content;
	std::vector<GetBonusInfo> getBonusList;
	GuildBonusInfo()
	{
		bonusID = 0;
		bonusType = 0;
		maxPeopleNum = 0;
		bonusNum = 0;
		sendTime = 0;
		getTime = 0;
		templateID = 0;
		getBonusList.clear();
		sourceCRoleID = 0;
		leaderID = 0;
		luckestID = 0;
	}
	void load(const KKSG::GuildBonus& a)
	{
		bonusID = a.bonusid();
		bonusType = a.bonustype();
		maxPeopleNum = a.maxpeoplenum();
		bonusNum = a.bonusnum();

		UINT32 cur = (UINT32)time(NULL);
		sendTime = a.sendtime();
		getTime = a.gettime();

		source = a.sourcename();
		content = a.content();
		templateID = a.contenttype();
		sourceCRoleID = a.sourceid();
		luckestID = a.luckestid();
		leaderID = a.leaderid();

		GetBonusInfo bonusInfo;
		getBonusList.clear();
		for (size_t i = 0; i < (size_t)a.getbonuslist_size(); ++i)
		{
			bonusInfo.load(a.getbonuslist(i));
			getBonusList.push_back(bonusInfo);
		}

		return;
	}

	void save(KKSG::GuildBonus& a)
	{
		a.set_bonusid(bonusID);
		a.set_bonustype(bonusType);
		a.set_maxpeoplenum(maxPeopleNum);
		a.set_bonusnum(bonusNum);
		a.set_sendtime(sendTime);
		a.set_gettime(getTime);

		a.set_sourcename(source);
		a.set_sourceid(sourceCRoleID);
		a.set_content(content);
		a.set_contenttype(templateID);

		a.set_leaderid(leaderID);
		a.set_luckestid(luckestID);

		for (auto iter = getBonusList.begin(); iter != getBonusList.end(); ++iter)
		{
			KKSG::GetGuildBonusInfo* pInfo = a.add_getbonuslist();
			if (pInfo)
			{
				iter->save(pInfo);
			}
		}

		return;
	}

};

struct GuildCheckInBonus
{
	UINT32 bonusID;
	UINT32 templateID;
	UINT32 needCheckInNum;
	GuildCheckInBonus()
	{
		memset(this, 0, sizeof(*this));
	}

	void load(const KKSG::GuildCheckInBonusData& a)
	{
		bonusID = a.bonusid();
		templateID = a.contenttype();
		needCheckInNum = a.needcheckinnum();
		return;
	}
	void save(KKSG::GuildCheckInBonusData& a)
	{
		a.set_bonusid(bonusID);
		a.set_contenttype(templateID);
		a.set_needcheckinnum(needCheckInNum);

		return;
	}
};

class GuildBonusMgr : public IGuildSerializerInterface, public IGuildListener, public ITimer
{ 
public:
	GuildBonusMgr(Guild* pGuild);
	~GuildBonusMgr();

	virtual void OnAddMember(Guild *pGuild, GuildMember *pMember);
	virtual void OnAddMemberFromDB(Guild *pGuild, GuildMember *pMember){}
	virtual void OnRemoveMember(Guild *pGuild, UINT64 qwCRoleID) {}
	virtual void OnMemberPosChanged(Guild *pGuild, GuildMember *pMember) {}
	virtual void OnMembergGuildBossDps(Guild *pGuild, GuildMember *pMember) {}
	virtual void OnGuildLevelUp(Guild *pGuild);

	virtual bool IsChanged();
	virtual bool Load(const KKSG::GuildAllDBData & guilddata);
	virtual void Save(KKSG::GuildAllDBData & guilddata);
	virtual const std::string ToBinary();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	//void addGuildBonus(UINT32 type, UINT32 var, CRole* pCRole=NULL);
	//const GuildBonusInfo* addGuildBonus(UINT32 templateID, CRole* pCRole);
	void addGuildBonus(UINT32 type, UINT32 var, UINT64 roleID);
	const GuildBonusInfo* addGuildBonus(UINT32 templateID, UINT64 roleID);

	void Update();
	bool onTimer();
	void onLogin(CRole* pCRole);

	void getBonusList(CRole* pCRole, KKSG::GetGuildBonusListResult &roRes, KKSG::GuildRecord* record);
	void getBonusDetailInfo(CRole* pCRole, UINT32 bonusID, KKSG::GetGuildBonusDetailInfoResult &roRes, KKSG::GuildRecord* record);
	GuildBonusInfo* GetBonusInfoByID(UINT32 bonusID);

	void getBonusReward(CRole* pCRole, UINT32 bonusID, /*KKSG::GetGuildBonusRewardResult*/ KKSG::MGuildBonusOperateReqRes &roRes, KKSG::GuildRecord* record);
	void GetBonusLeftList(CRole* pCRole,  KKSG::MGuildBonusOperateReqRes &roRes, KKSG::GuildRecord* record);

	UINT32 ThankForBonus(CRole* role, UINT32 bonusID);

	UINT32 ManualSendBonusInSendList(CRole* role, UINT32 bonusID);
	UINT32 GetGuildBonusInSendList(CRole* role, KKSG::GetGuildBonusSendListRes& roRes);

	UINT32 GetLastCheckinBonusTime();
	UINT32 SendCheckInBonus(CRole* role);
	bool IsInCheckInBonusTime();
	UINT32 GetCheckInBonusInfo(CRole* role, KKSG::GuildCheckInBonusInfoRes &roRe, KKSG::GuildRecord* record);
	bool IsCheckInBonusAllGot();

private:
	UINT32 _getNextBonusID(){return ++m_curBonusID;}
	void _generateBonusRandomNum(GuildBonusInfo* pInfo);
	void _generateBonusAverageNum(GuildBonusInfo* pInfo);
	UINT64 _CalcLuckestID(const GuildBonusInfo* info);
	bool _IsCheckInBonus(UINT32 templateID);

	void _GenerateCheckInBonus();
	UINT32 _GetBonusSendPeopleNum(const GuildBonusTable::RowData* data);

	UINT32 _getLeftOpenCount(GuildBonusInfo* info);
	UINT32 _getLeftOpenTime(UINT32 sendTime, UINT32 templateID);
	UINT32 _getLeftBringBackTime(UINT32 sendTime, UINT32 templateID);
	UINT32 _getAlreadyGetSizeOfBonus(GuildBonusInfo* pInfo);
	UINT32 _GetBonusStatus(CRole* role, GuildBonusInfo* info, KKSG::GuildRecord* record);
	UINT32 _GetMemberGotNum(const GuildBonusInfo* info, UINT64 roleID);

	void _fillBonusAppear(CRole* pCRole, KKSG::GuildBonusAppear* dest, GuildBonusInfo* src, KKSG::GuildRecord* record);

	const GuildBonusInfo* _addGuildBonus(UINT64 roleID, const std::string& sourceName, UINT8 bonusType, UINT16 maxPeopleNum, UINT32 bonusNum, UINT32 templateID, bool manualSend, const std::string& content, bool isRandomDispatch=true, UINT32 guildBonusType = 0);
	void _addGuildBonus(const GuildBonusInfo& bonusInfo);
	void _NotifyAddNewBonus(const GuildBonusInfo* info);
	void _InsertIntoBonusList(const GuildBonusInfo& bonusInfo);
	void _OnAddNewBonus(const GuildBonusInfo& bonusInfo);

	bool _checkGetBonusReward(CRole* pCRole, GuildBonusInfo& info);
	UINT32 _checkNotGetMaxNum(UINT32 bonusTemplateID, KKSG::GuildRecord* record);
	bool _getRandReward(GuildBonusInfo* info, UINT32& result);
	void _OnGetBonusReward(CRole* pRole, const GuildBonusInfo& info);
	void _OnBonusGetAll(CRole* pRole, const GuildBonusInfo& info);

	bool _CheckHasSendBonus(CRole* role);
	void _NotifySendBonus(bool hasLeftSend, CRole* role);
	void _CheckAutoSendBonusInSendList();
	void _SendBonusInSendList(const GuildBonusInfo* info);
	void _RemoveExceedBonus();

private:
	Switch m_isModified;
	Guild *m_pGuild;
	std::map<UINT32,GuildBonusInfo> m_bonusList;
	UINT32 m_curBonusID;

	UINT32 m_updateDay;
	std::vector<GuildCheckInBonus> m_checkinBonusData;
	UINT32 m_timehandler;

	HTIMER m_timer;
	std::map<UINT32,GuildBonusInfo> m_manualSendBonusList;
	std::map<UINT32,GuildBonusInfo> m_timeBonusList;
};
#endif // __GUILD_BONUS_H__
