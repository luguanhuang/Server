#ifndef __SOCIALMGR_H__
#define __SOCIALMGR_H__

#include "timer.h"

#define  SYSTEM_GAOJIGU 91
namespace KKSG
{
	class SocialInfo2Db;
	class ChatInfo;
	class ChangeModelInfoArg;
	class GetGaoJiGuModelRewardArg;
	class ItemBrief;
	class TShowVoteInfoRes;
}
struct SocialInfo;
struct FriendInfo;
struct Flower; 
class Role;
class GaoJiGu;
class CRole;
class SocialMgr : ITimer
{
	SocialMgr();
	~SocialMgr();
	DECLARE_SINGLETON(SocialMgr);
public:
	bool Init();
	void Uninit();

	void OnLogin(CRole* role);

	void OnTimer(UINT32 dwID, UINT32 dwCount);
	void StartUpFlowerEvent();
	void OnFlowerEvent(UINT32 nTime=0);
	bool OnDayTimer();

	void SetLoaded() { m_isloaded = true; };

	bool QueryDb();
	void LoadFromDb(const KKSG::SocialInfo2Db* socialinfo2db);
	void Save2Db();

	void DebugDayPass();
	void DebugWeekPass();
	void DoFlowerRank(UINT64 roleid, KKSG::RankeType type, UINT32 rank);

	void UpdateFlowerActivityInfo();
	void ClearFlowerActivityInfo();

public:	
	SocialInfo* GetSocialInfo(UINT64 roleid);
	inline void SetChanged(UINT64 roleid) { m_changeds.insert(roleid); }

private:
	bool m_isloaded;
	HTIMER m_timehandler;
	UINT32 m_timer;
	std::unordered_set<UINT64> m_changeds;
	std::unordered_map<UINT64, SocialInfo*> m_roleid2socialinfo;
};

#endif