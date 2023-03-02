#ifndef __CUSTOMBATTLEMGR_H__
#define __CUSTOMBATTLEMGR_H__

#include "cmdline.h"
#include "timer.h"
#include "custombattle/custombattledef.h"
#include "custombattle/custombattlehandle.h"

class CRole;
class CRpc;
struct ItemDesc;
class CustomBattle;
class CustomBattleGenerator;
class CustomBattleMgr : public ITimer
{
public:
	CustomBattleMgr();
	~CustomBattleMgr();
	DECLARE_SINGLETON(CustomBattleMgr);
public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool IsOpen(CRole* role, const CustomBattleReq& req);

	bool HandleClientReq(const CustomBattleReq& req);
	bool HandleGsReply(const CustomBattleReq& req);
	bool HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& clientinfo);

	KKSG::ErrorCode TransReqToWorld(const CustomBattleReq& req);

	bool IsLocal(UINT64 uid);
	CustomBattle* GetBattle(UINT64 uid);
	void AddBattle(UINT64 uid, CustomBattle* battle);
	void DelBattle(UINT64 uid);
	void DelBattleByName(const std::string& name, UINT64 uid);
	CustomBattleGenerator* GetGenerator() { return m_generator; }

	KKSG::ErrorCode DelayTakeItem(const CustomBattleReq& req, const std::vector<ItemDesc>& items);
	void DelayGiveItem(const CustomBattleReq& req, const std::vector<ItemDesc>& items);

	void FillRandomBattle(KKSG::CustomBattleQueryInfo* queryinfo, UINT64 uid, UINT64 roleid);

	void Dump(std::string& output);
	static bool CmdLine(const CmdLineArgs &args, std::string &outputMessage);

	//std::vector<UINT64>* GetBattleByToken(const std::string& token);
	std::vector<UINT64>* GetBattleByName(const std::string& name);

	void SetEnd() { m_isend = true; }

	void FillTag(CustomBattle* battle, KKSG::CustomBattleData* data, UINT64 creator, UINT64 roleid);

	void OnRoleLogin(CRole* role);

	bool LoadGM();
	bool IsGMCreate(const std::string& openid);
	bool IsGMJoin(const std::string& openid);

private:
	bool m_isend;
	HTIMER m_timehandle;
	// handle
	CustomBattleHandleQuery m_query;
	CustomBattleHandleModify m_modify;
	CustomBattleHandleCreate m_create;
	CustomBattleHandleJoin m_join;
	CustomBattleHandleUnJoin m_unjoin;
	CustomBattleHandleMatch m_match;
	CustomBattleHandleUnMatch m_unmatch;
	CustomBattleHandleReward m_reward;
	CustomBattleHandleSearch m_search;

	CustomBattleHandleCreateWorld m_createworld;
	CustomBattleHandleJoinWorld m_joinworld;
	CustomBattleHandleUnJoinWorld m_unjoinworld;
	CustomBattleHandleRewardWorld m_rewardworld;

	CustomBattleHandleDrop m_dropsystem;
	CustomBattleHandleJoinSystem m_joinsystem;
	CustomBattleHandleUnJoinSystem m_unjoinsystem;
	CustomBattleHandleRewardSystem m_rewardsystem;

	CustomBattleGenerator* m_generator;
	std::unordered_map<UINT64, CustomBattle*> m_uid2battle;

	// search
	std::unordered_map<std::string, std::vector<UINT64>> m_name2uid;
	//std::unordered_map<std::string, std::vector<UINT64>> m_token2uid;

	std::set<std::string> m_gmcreate;
	std::set<std::string> m_gmjoin;
};

#endif