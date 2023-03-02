#ifndef __PLAT_SHARE_RECORD_H__
#define __PLAT_SHARE_RECORD_H__
#include "unit/roleserilizeinterface.h"

class Role;
class PlatformShareRecord : public RoleSerializeInterface
{
public:
	PlatformShareRecord(Role* role);
	~PlatformShareRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void Update();
public:
	void AddFirstShare(UINT32 scene_id);
	void AddShareNumWeekly();
	void NotifyFirstShare(UINT32 scene_id);

	void GetFirstShareReward(const UINT32 scene_id);
	void GetWeeklyShareReward();
	// debug ½Ó¿Ú
	void DebugTest();

	void NotifyPlatShareAward();

	void SetDisappearRedPoint();

	bool ReSetWeekly();
	void AddDragonCoinsConsume(UINT32 dragon_coins);
	UINT32 GetAverageConsume7Day();
private:
	Role* role_;
	Switch m_isModify;

	std::unordered_map<UINT32,bool> frist_pass_share_list_;
	std::unordered_set<UINT32> have_notify_scene_;
	UINT32 m_lastUpdateTime;
	bool weekly_award_;
	bool disappear_redpoint_;
	UINT32 weekly_share_number_;

	UINT32 time_handle_;
private:

	UINT32 last_update_consume_;

	UINT64 consume_dragoncoins_now_;
	UINT64 consume_dragoncoins_before_1_;
	UINT64 consume_dragoncoins_before_2_;
	UINT64 consume_dragoncoins_before_3_;
	UINT64 consume_dragoncoins_before_4_;
	UINT64 consume_dragoncoins_before_5_;
	UINT64 consume_dragoncoins_before_6_;
	UINT64 consume_dragoncoins_before_7_;
};


#endif
