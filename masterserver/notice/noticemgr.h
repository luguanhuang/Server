#ifndef __NOTICEMGR_H__
#define __NOTICEMGR_H__

#include "table/NoticeTable.h"
#include "time.h"
#include "timer.h"
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

struct stNoticeContent
{
	stNoticeContent()
	{
		NoticeId = 0;
		szContent = "";
		uChannel = 0;
		uPriority = 0;
		uStartTime = 0;
		uEndTime = 0;
	}
	stNoticeContent(UINT32 id, const std::string info, UINT32 channel, UINT32 priority, UINT32 sTime,UINT32 etime)
	{
		NoticeId = id;
		szContent = info;
		uChannel = channel;
		uPriority = priority;
		uStartTime = sTime;
		uEndTime = etime;
	}
	UINT32 NoticeId;
	std::string szContent;
	UINT32 uChannel;
	UINT32 uPriority;
	UINT32 uStartTime;
	UINT32 uEndTime;
};

class CRole;
class NoticeMgr : public ITimer
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

	UINT32 GetChannel(UINT32 index);
	UINT32 GetPriority(UINT32 index);
	UINT32 GetTimeSpan(UINT32 index);
	UINT32 GetLevel(UINT32 index);
	const std::string* GetInfo(UINT32 id);
	NoticeLinkInfo* GetLinkInfo(UINT32 id);
	std::vector<NoticeParamType>* GetParamTypes(UINT32 id);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void AddTimeNotice(UINT32 id, NoticeTime& time);
	void TimeNotice(UINT32 id);
	void LevelSealNotice(UINT32 ID);
	NoticeTime HandlerSpan(UINT32 id, UINT32 count);//播放次数
	void SendNotice(UINT32 uChannel, UINT32 uPro, std::string content);
	bool AddIdipNotice(UINT32 id, stNoticeContent notice, NoticeTime timenode);
	bool DelIdipNotice(UINT32 id);
	void GetIdipNotice(std::vector<std::pair<NoticeTime, stNoticeContent>>& list);

	void TShowTop3Notice(CRoleSummary* srcrole, UINT32 rank);
	typedef void (NoticeMgr::*pFlowerNoticeFunc)(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count);
	void SendFlowerNotice(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count);
	void FlowerNotice(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count);
	void FlowerNotice2(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count);
	void FlowerNotice3(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count);
	void FlowerNotice4(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count);
	void FlowerNotice5(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count);
	pFlowerNoticeFunc GetSendFlowerNoticeFunc(UINT32 itemID, UINT32 count);
	void FlowerValentinesFirstNotice(CRoleSummary* role);
	void FlowerValentinesOtherNotice(CRoleSummary* role, UINT32 rank);
	void Flower520FirstNotice(CRoleSummary* role);
	void Flower520OtherNotice(CRoleSummary* role, UINT32 rank);

	void FirstPassNotice(int serverid, std::vector<CRoleSummary*>& roles, const std::string& name, int rank);
	void SendAddFriendNotice(UINT64 roleid, CRoleSummary* friendrole);
	void NoticeGuildBossLvlup(UINT64 guildId);
	void NoticeGuildBossDps(std::string roleName, UINT64 guildId);
	void GuildArenaOutWar(UINT64 roleId);
	void GuildArenaOnWar(UINT64 roleId);
	void NoticeGuildArenaInit();                  // 擂台战组队通知 
	void NoticeGuildArenaReady(UINT64 guildId);   // 擂台战备战通知 
	void NoticeGuildArenaStart();                 // 擂台战开战通知
	void NoticeGuildArenaYourTurn(UINT64 roleID);   // 擂台战上场通知
	void NoticeGuildArenaKickYou(UINT64 roleID);    // 擂台战部署被移除
	void NoticeGuildArenaBattleOne();
	void NoticeGuildArenaBattleTwo();
	void NoticeGuildArenaBattleFin();
	void NoticeGuildArenaWin(UINT64 guildId, UINT64 losId);
	void NoticeGuildArenaLos(UINT64 guildId, UINT64 winId);
	void NoticeGuildArenaCom(UINT64 guildId, UINT64 losId);
	void NoticeGuildArenaWiner(UINT64 guildId, UINT32 type);
	void GuildArenaBeforeNotice(UINT64 guildId, UINT32 type);  // 公会擂台战  战前通知
	void NoticeGuildSkillUp(std::string skillName, UINT32 skillLvl, UINT64 guildId);
	void NoticeGuildDareFinished(const std::set<UINT64>& roleSet, const std::string sceneName, UINT64 guildId, UINT32 exp);
	void NoticeGuildActivityLeader(const std::string name, const std::string nameTwo, UINT64 guildId);
	void GetGuildBonusNotice(CRoleSummary* source, CRoleSummary* srcrole, UINT64 guildID, UINT32 itemID, UINT32 count);
	void FirstGetGuildBonusNotice(/*CRoleSummary* source*/std::string& bonusName, CRoleSummary* srcrole, UINT64 guildID, UINT32 itemID, UINT32 count);
	void LuckestGetGuildBonusNotice(CRoleSummary* source, const std::string& names, CRoleSummary* scrole, UINT32 itemID, UINT32 count, UINT64 guildID);
	void SendGuildBonusNotice(CRoleSummary* source, UINT32 itemID, UINT32 count, UINT64 guildID);
	void SendGuildBonusNoticeOnTime(UINT64 guildID);
	void PvpNotice(bool beginOrEnd);
	void NoticeGuildLadderReady();
	void NoticeGuildLadderBegin(UINT64 guildId);
	void NoticeGuildLadderWin(UINT64 guildId, UINT64 roleId);
	void NoticeGuildLadderWinTimes(UINT64 guildId, UINT64 roleId, UINT32 winTimes);
	void NoticeGuildLadderTen(UINT64 guildId, UINT32 rank);
	void NoticeGuildLadderFive(UINT64 guildId, UINT32 rank);
	void NoticeGuildLadderEnd(UINT64 guildId, UINT32 rank, std::set<UINT64>& roleSet);
	void NoticeGuildArenaEnterSceneOne(UINT64 guildId);
	void NoticeGuildArenaEnterSceneTwo(UINT64 guildId);
	void NoticeGuildArenaEnterSceneThree(UINT64 guildId);
	void NoticeGuildLeaderOpenQA(UINT64 guildId);
	void NoticeJoinWorldQA(UINT32 min);
	void NoticeJoinGuildQA(UINT64 guildId, UINT32 min);
	void NoticeGuildCardMatch(UINT64 guildid, UINT32 type, UINT32 round = 0);
	void NoticeGuildCardFirst(UINT64 guildId, CRoleSummary* role);
	void NoticeGuildCardBeginSoon();
	void NoticeGuildRankEnd(std::vector<std::string>& roleVec);
	void NoticeGuildRankReward(UINT64 guildId, UINT32 rank);
	void NoticeGuildAuctBuyNow(UINT64 guildId, const std::string& roleName, UINT32 price, const std::string& itemName);
	void NoticeGuildAuctActEnd(UINT64 guildId, const std::string& actName, UINT32 rank);
	void NoticeGuildApply();
	void NoticeArenaIntegralReady();
	void NoticeArenaEnterScene(UINT64 guildId);
	void NoticeArenaIntegralWin(UINT64 guildId, std::string guildName);
	void NoticeArenaIntegralLos(UINT64 guildId, std::string guildName);
	void NoticeIntegralApplyEnd(UINT64 guildId);
	void NoticeGuildBossRank(std::string guildName, UINT32 rank);
	void NoticeGuildAddMember(UINT64 guildId, std::string name);
	void NoticeGuildMiss(UINT64 guildId);
	void NoticeGuildJoinLast(UINT64 guildId);
	void NoticeGuildEnd(UINT64 roleId);
	void NoticeGuildReq(UINT64 roleId);
	void NoticeGuildRereq(UINT64 roleId);
	void NoticeNameChange(UINT64 guildId, const std::string& lastname, const std::string& nowname);
	void NoticeProChange(UINT64 guildId, const std::string& name, const std::string& pro);
	// 扑克之王
	void NoticePokerTournamentBeginSoon();
	void NoticePokerTournamentCardMatch(std::vector<UINT64>& players, UINT32 type, UINT32 round = 0);
	void NoticePokerTournamentFirst(std::vector<UINT64>& roles, CRoleSummary* role);

	void NoticeGuildTerrBegin();
	void NoticeGuildTerrWarning(UINT64 guildId, UINT32 num) ;
	void NoticeGuildTerrDeclareOne(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrDeclareTwo(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrDeclareThree(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrCanDeclareOne(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrCanDeclareTwo(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrCanDeclareThree(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrWarningDeclare(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrWarningDeclareTwo(UINT64 guildId, std::string name, std::string nameTwo) ;
	void NoticeGuildTerrWarningNoDeclare(UINT64 guildId) ;
	void NoticeGuildTerrNotDeclareOne(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrNotDeclareTwo(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrNotDeclareThree(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrEnterSceneOne(UINT64 guildId) ;
	void NoticeGuildTerrEnterSceneTwo(UINT64 guildId) ;
	void NoticeGuildTerrEnterSceneThree(UINT64 guildId) ;
	void NoticeGuildTerrEnterScene() ;
	void NoticeGuildTerrEnd(std::string name) ;
	void NoticeGuildTerrReward(UINT64 guildId) ;
	void NoticeGuildTerrRewardOne(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrRewardTwo(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrRewardThree(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrOtherReward(UINT64 guildId) ;
	void NoticeGuildTerrAllianceOne(UINT64 guildId, std::string name, std::string nameTwo) ;
	void NoticeGuildTerrAllianceTwo(UINT64 guildId, std::string name, std::string nameTwo) ;
	void NoticeGuildTerrAllianceThree(UINT64 guildId, std::string name, std::string nameTwo) ;
	void NoticeGuildTerrDelAlliance(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrDefGuildOne(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrDefGuildTwo(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrDefGuildThree(UINT64 guildId, std::string name) ;
	void NoticeGuildTerrWinOne(UINT64 guildId, std::string nameOne, std::string nameTwo);
	void NoticeGuildTerrWinTwo(UINT64 guildId, std::string nameOne, std::string nameTwo);
	void NoticeTeamKickLeaderBefore(UINT64 roleID, UINT32 t);
	void NoticeTeamKickLeaderAfter(UINT64 roleID);
	void NoticeGuildBossFirstKill(std::string guildName, std::string bossName);
	void NoticeLeagueStartApply();
	void NoticeGuildTerrBwinOne(UINT64 guildId, std::string name);
	void NoticeGuildTerrBwinTwo(UINT64 guildId, std::string name);
	void NoticeGuildTerrBwinThree(UINT64 guildId, std::string name);
	void NoticeGuildTerrSavOne(UINT64 guildId, std::string name);
	void NoticeGuildTerrSavTwo(UINT64 guildId, std::string name);
	void NoticeGuildTerrSavThree(UINT64 guildId, std::string name);



private:
	bool IsBegin(const NoticeTime& timenode, const tm* nowtimenode, time_t nowtime);
	bool IsEnd(const NoticeTime& timenode, const tm* nowtimenode, time_t nowtime);
	UINT32 GetSpan(UINT32 id);
	char GetStringIndex(const std::string& info, UINT32 index);
	std::vector<NoticeParamType> ParseParamTypes(const std::string& info, bool& haslink);

private:
	NoticeTable m_table;
	HTIMER m_timehandle;
	std::map<UINT32, NoticeTime> m_id2time;
	std::map<UINT32, time_t> m_id2timenotice;
	std::map<UINT32, NoticeLinkInfo> m_id2link;
	std::map<UINT32, std::vector<NoticeParamType>> m_id2paramtypes;
	///> 以下用于处理idip公告相关(与系统内公告逻辑分开，方便功能扩展)
	std::map<UINT32, std::pair<NoticeTime, stNoticeContent>> m_Idipid2time;
	std::map<UINT32, time_t> m_Idiptimenotice;
};

#endif