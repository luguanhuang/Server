#ifndef __NOTICEMGR_H__
#define __NOTICEMGR_H__

#include "table/NoticeTable.h"
#include "time.h"
#include "timer.h"
#include "item.h"
#include "notice.h"

struct NoticeTime
{
	bool hasbegin;
	bool hasend;

	tm timebegin;
	tm timeend;
	UINT32 timespan;

	bool isprocess;

	NoticeTime()
	{
		hasbegin = false;
		hasend = false;
		isprocess = false;
		timespan = 0;
	}
	NoticeTime(tm begin, tm end, UINT32 span)
	{
		hasbegin = true;
		hasend = true;
		isprocess = false;
		begin.tm_year += 1900;
		begin.tm_mon += 1;
		end.tm_year += 1900;
		end.tm_mon += 1;
		timebegin = begin;
		timeend = end;
		timespan = span;
	}
};

struct NoticeLinkInfo
{
	int param;
	std::string content;

	NoticeLinkInfo()
	{
		param = 0;
	}
};

class NoticeMgr
{
	NoticeMgr();
	~NoticeMgr();
	DECLARE_SINGLETON(NoticeMgr);
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	typedef void (NoticeMgr::*FlowerNoticeFunc)(Role* srcrole, RoleSummary* destrole, XItem* item);
	UINT32 GetChannel(UINT32 index);
	UINT32 GetPriority(UINT32 index);
	UINT32 GetTimeSpan(UINT32 index);
	UINT32 GetLevel(UINT32 index);
	NoticeLinkInfo* GetLinkInfo(UINT32 id);
	const std::string* GetInfo(UINT32 id);
	std::vector<NoticeParamType>* GetParamTypes(UINT32 id);

	void RawNotice(UINT32 channel, const std::string& info, UINT32 priority);
	//void TimeNotice(UINT32 id);
	void ItemNotice(Role* role, XItem* item, UINT32 level);
	void GuildCardNotice(Role* role, UINT64 guild, UINT32 type, std::vector<ItemDesc>& items);
	//void SendFlowerNotice(Role* srcrole, RoleSummary* destrole, XItem* item);
	//void FlowerNotice(Role* srcrole, RoleSummary* destrole, XItem* item);
	//void FlowerNotice2(Role* srcrole, RoleSummary* destrole, XItem* item);
	//void FlowerNotice3(Role* srcrole, RoleSummary* destrole, XItem* item);
	//void FlowerNotice4(Role* srcrole, RoleSummary* destrole, XItem* item);
	//void FlowerNotice5(Role* srcrole, RoleSummary* destrole, XItem* item);
	//FlowerNoticeFunc GetSendFlowerNoticeFunc(UINT32 itemID, UINT32 count);
	void BossNotice(UINT32 type, UINT32 min, UINT64 guildId);
	void LotteryNotice(Role* role, XItem* item);
	void PandoraNotice(Role* role, const ItemDesc& desc, UINT32 quality);
	//void HellHoundNotice(std::string& names);

	void RegisterSendDelay(Notice* notice, UINT32 timedelay);
	bool SendDelay(Notice* notice);
	void NoticeFlowerDesignation( Role* pRole, UINT32 rank ); //鲜花昨日榜跑马灯
	void NoticeGuildArenaKickYou(UINT64 roleID);    // 擂台战部署被移除
	void NoticeGuildBossLastKill(std::string roleName, XItem* item, UINT64 guildId);
	void GuildQALeftTimeNotice( UINT64 guildID, UINT32 leftMin ); ///>公会答题倒计时提示：10min，5min，2min，1min	
	//void GuildLeaderOpenQANotice( UINT64 guildID );	///>公会官员开启了公会答题通知
	//void JoinWorldQANotice( UINT32 min );							///>参与全民答题倒计时提示
	//void JoinGuildQANotice( UINT64 guildID, UINT32 min);	///>参与公会答题倒计时提示
	void NoticeFetchRoll(Role* pRole, XItem* item, UINT32 result);
	void NoticeGiveUpFetchRoll(Role* pRole, XItem* item);
	void NoticeAllFetchRoll(Role* pRole, XItem* item, UINT32 max);

	void NoticeSendNestLiveWatch(Role* pRole, const std::string& names, int num);
	void NoticeSendDragonLiveWatch(Role* pRole, const std::string& names);
	void NoticeSend1V1LiveWatch(Role* pRole1, Role* pRole2);
	void NoticeSend2V2LiveWatch(Role* pRole1, Role* pRole2);
	void NoticeSendProtectCaptainLiveWatch(Role* pRole1, Role* pRole2);
	void NoticeSendGuildBattleLiveWatch(UINT64 guildID1, UINT64 guildID2);
	void NoticeSendHeroBattleLiveWatch(Role* role1, Role* role2, UINT32 num);
	void NoticeSendLeagueBattleLiveWatch(std::string& name1, std::string& name2, UINT32 num);
	void NoticeSendCustomPkLiveWatch(Role* pRole1, Role* pRole2);

	void NoticeGuildArenaKick(Role* pRole, Role* kicker);
	void NoticeGuildArenaOneFightEnd(Role* winRole, Role* loseRole, UINT64 winGuildID, UINT64 loseGuildID);
	//void NoticeGuildBossFirstKill(std::string guildName, std::string bossName);
	void NoticeGuildEnd(UINT64 roleId);
	void NoticeGuildReq(UINT64 roleId);
	void NoticeGuildRereq(UINT64 roleId);

	void GuildCampPartyLeftTimeNotice( UINT64 guildID, UINT32 leftMin ); ///>公会营地倒计时提示：10min，5min，2min，1min	
	void NoticeBioHellFirstComplete(UINT64 roleId, UINT32 sceneid);
private:
	char GetStringIndex(const std::string& info, UINT32 index);
	std::vector<NoticeParamType> ParseParamTypes(const std::string& info, bool& haslink);

private:
	NoticeTable m_table;
	std::map<UINT32, NoticeLinkInfo> m_id2link;
	std::map<UINT32, std::vector<NoticeParamType>> m_id2paramtypes;
};

#endif