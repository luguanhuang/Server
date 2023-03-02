#ifndef __GUILD_CAMP_PART_H__
#define __GUILD_CAMP_PART_H__
#include "timer.h"
#include "cvsreader.h"
#include "multiindex_map.h"
#include "unit/role.h"
#include "table/GuildCampPartyReward.h"
#include "define/noticedef.h"

struct GuildCampExchangeChatInfo
{
	UINT64 audio_id;
	UINT32 audio_time;
	std::string chat_text;
	GuildCampExchangeChatInfo()
	{
		audio_id = 0;
		audio_time = 0;
		chat_text.clear();
	}
};

struct TradeData
{
	UINT64 lauch_role_id;
	UINT64 target_role_id;	
	UINT64 lauch_item_uid;
	UINT32 lauch_item_id ;
	UINT64 target_item_uid;
	UINT32 target_item_id ;

	GuildCampExchangeChatInfo lauch_chat;
	GuildCampExchangeChatInfo target_chat;

	bool lauch_status;
	bool target_status;
	TradeData()
	{
		lauch_role_id = 0;
		target_role_id =0;	
		lauch_item_id = 0;
		target_item_id = 0;
		lauch_item_uid = 0;
		target_item_uid = 0;
		lauch_status = false;
		target_status = false;				
	}
};

class CGuildCampPartyMgr : public ITimer
{
	CGuildCampPartyMgr();
	~CGuildCampPartyMgr();
	DECLARE_SINGLETON(CGuildCampPartyMgr);

public:
	enum EGuildCampPartyState  // 状态枚举
	{
		BeginPreNotice		= 0,
		BeginPreWaiting	    = 1,
		SpriteAppear	= 2,
		SpriteEnd  = 3,
		ActiveContinue = 4,
		ActiveEnd = 5,
	};

public:
	bool Init();
	void Uninit();

	bool Reload();
	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
public:

	void ExchangeItemReq(Role *launch_role,Role * target_role);
	void OpenActivities();
	void HalliconsNotify(UINT32 system_id,bool show);
	void FinishActivities();
	void CreateSprite();

	bool DefendPlug(UINT64 role_id);
	void ClearTradeInvitation(UINT64 target_role_id ,UINT64 lauch_role_id,bool clear_all);

	bool CheckSpriteExist(UINT32 scene_id,UINT32 sprite_id,Sequence<float,3>& sequence);
	KKSG::ErrorCode DeleteSprite(Scene *scene,UINT32 sprite_id,UINT64 guild_id);

	void UpdateSprite(Scene *scene);

	void DestroySprite();
	void GetExchangeInfoByTargeId(UINT64 role_id,KKSG::GetGuildCampPartyExchangeInfoRes& res);
	bool CheckRoleTransactionStatus(UINT64 role_id);
	void SetRoleInTransactionStatus(UINT64 role_id);
	void ClearRoleInTransactionStatus(UINT64 role_id);

	UINT32 GetTardeInvitationCount(UINT64 role_id);
	KKSG::ErrorCode CheckPartyConditon(Role *role);

	void NoticeTradeInvitationDisappear(Role *role);

	void MakeTradeRecord(Role *launch_role,Role * target_role);
	void CancleTrade(Role *role,Role *launch_role,Role * target_role);
	void DeleteTrade(UINT64 role_id);
	void SwingUpItem(Role *role,UINT64 item_uid ,UINT32 item_id,Role *launch_role,Role * target_role);
	void MakeSureTrade(Role *role,bool status,Role *launch_role,Role * target_role);
	void UpdateTransaction(Role *launch_role,Role *target_role);

	UINT32 CheckSwingUpItem(Role *role,UINT64 uid);

	KKSG::ErrorCode GetReward(Role *role,UINT32 reward_id);
	KKSG::ErrorCode CheckRoleRewardItems(Role *role,UINT32 reward_id,vector<Sequence<uint, 2>>& items,vector<Sequence<uint, 2>>& reward);
	void SystemOpenNotify(Role *role);
	void UpdatePartyStauts(Role *role,UINT32 scene_id);
	void ClearTradeInfo(Role *role);
	TradeData *GetTradeDataByRole(UINT64 role_id);
	void AudioChat(Role *role,UINT64 audio_id,UINT32 audio_time, std::string translate_text,Role *launch_role,Role * target_role);
	void TextChat(Role *role,std::string text_chat, Role *launch_role,Role * target_role);

	bool CheckSummonSpiritExist(UINT32 scene_id);
	bool CheckHaveSummonSpiritIng(UINT64  guild_id);
	bool CheckSummonSpiritTime();

	void UpdateSceneSpirte(Scene *scene); 
	void SetHaveSummonSpiritIng(UINT64  guild_id);
	void ClearSummonSpiritIng(UINT64  guild_id);
	void SummonSpirit(Role *role);
	bool ActiveIsFinish();
private:
	bool PreNotice(UINT32 now_time);
	void WorldSendNotice(CNoticeType notice_type);
	void GuildSendNotice(CNoticeType notice_type);
	void NoticeTradeStatus(Role *launch_role,Role *target_role);
	void UpdateNpcSprite();
	UINT32 ConsumeItems(Role *role,std::vector<Sequence<uint, 2>>& items);
	void GiveItems(Role *role,std::vector<Sequence<uint, 2>>& items);
	void Reset();

	void SetTimerEvent();

	void InitPartyConf();
	bool Pre5minitNotice();
	bool ClearLottery();
	bool DownNotice();
	bool ActiveIocnNotice();
	bool FinishNotice();
	bool ClearSummonSpirit(time_t  now_time);
private:
	HTIMER time_handler_;
	EGuildCampPartyState state_;	

	std::list<UINT32> timers_;

	std::unordered_map<UINT64,UINT32> summons_spirit_status_;
	// 小妖刷出世间
	std::map<UINT32,UINT32> sprite_wave_appear_;
	UINT32 sprite_time_ ;
	UINT32 sprite_delete_time_;
	UINT32 reward_continued_time_;
	std::set<UINT32> lottery_list_;
	GuildCampPartyReward party_reward_;
	std::unordered_map<UINT64,UINT32> last_time_;
	// 接收到交易请求的玩家
	std::unordered_map<UINT64,std::unordered_map<UINT64,UINT32>> targe_roles_;

	//std::unordered_map<UINT32,std::vector<KKSG::GuildCampSpriteInfo>> scene_sprite_list_;
	std::unordered_map<UINT32,std::map<UINT32,KKSG::GuildCampSpriteInfo>> scene_sprite_list_;
	std::unordered_set<UINT64> transaction_roles_;

	std::unordered_map<UINT32,time_t> scene_summonspirit_;

	UINT32 sprite_index_;

	struct LaunchIndex
	{
		typedef UINT64 key;
		key Key(const TradeData* r) { return r->lauch_role_id; }
	};

	struct TargetIndex
	{
		typedef UINT64 key;
		key Key(const TradeData* r) { return r->target_role_id; }
	};

	mutliindex_map_ptr<TradeData, LaunchIndex, TargetIndex>  trade_data_;
};

#endif