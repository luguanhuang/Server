#ifndef _PARTNER_H_
#define _PARTNER_H_

#include "partnershop.h"
#include "partnerliveness.h"

struct PartnerMember
{
	PartnerMember() :roleId(0) ,isApplyLeave(false) ,applyLeaveTime(0), openShopTime(0), refreshShopLevel(0) {}
	UINT64 roleId;
	bool   isApplyLeave; 
	UINT32 applyLeaveTime;
	UINT32 openShopTime;
	UINT32 refreshShopLevel;	// 商店刷新时等级

	void FromKKSG(const KKSG::PartnerMemberData& data)
	{
		roleId = data.roleid();
		isApplyLeave = data.is_apply_leave();
		applyLeaveTime = data.apply_leave_time();
		openShopTime = data.open_shop_time();
		refreshShopLevel = data.refresh_shop_level();
	}

	void ToKKSG(KKSG::PartnerMemberData& data)
	{
		data.set_roleid(roleId);
		data.set_is_apply_leave(isApplyLeave);
		data.set_apply_leave_time(applyLeaveTime);
		data.set_open_shop_time(openShopTime);
		data.set_refresh_shop_level(refreshShopLevel);
	}
	UINT32 GetLeftLeaveTime();
};

class CProtocol;
class CRole;
class Partner
{
public:
	Partner();
	~Partner();

	void LoadFromDB(const KKSG::PartnerSaveData& data);

	void FromKKSG(const KKSG::PartnerSaveData& data);
	void ToKKSG(KKSG::PartnerSaveData& data);
	void OnLoaded();

	void AddToDB();
	void UpdateToDB();
	void DelToDB();
	void ReplaceIdToDB();
	void SetOldId(UINT64 id) { m_oldId = id; }

	UINT64 GetID() { return m_id; }
	void SetID(UINT64 ID) { m_id = ID; }
	void SetMember(std::vector<UINT64>& memberIDs);

	UINT32 GetLevel() { return m_level; }
	UINT32 GetLiveness() { return GetPartnerLiveness().GetLiveness(); }
	UINT32 GetDegree() { return m_degree; }
	void GetPartnerMemberIds(std::vector<UINT64>& roleIds);
	PartnerMember* GetMember(UINT64 roleId);
	void RemoveMember(UINT64 roleId);

	void OnDayPass();
	void OnCreate();


	void FillMemberDetail(PartnerMember& member, KKSG::PartnerMemberDetail& data);
	void FillAllMemberDetail(KKSG::GetPartnerDetailInfoRes& res);

	void AddFriendDegree(UINT32 value);
	void BroadCast(const CProtocol& ptc);
	void BroadCastToGS(const CProtocol& ptc);
	void GetSessions(std::vector<SessionType>& sessions);

	KKSG::ErrorCode LeaveMember(CRole* pRole);
	KKSG::ErrorCode CancelLeaveMember(CRole* pRole); 

	void DoLeaveMember(UINT64 roleId, bool force = false);
	void CheckDissolve();

	void UpdateDegree();

	void SetDirty();

	std::map<UINT64, PartnerMember>& GetAllMember() { return m_members; }

private:
	bool m_dirty;
	UINT64 m_id;
	UINT32 m_degree;
	UINT32 m_level;		// 等级，不存db，启动时从表格计算
	std::map<UINT64, PartnerMember> m_members;
	UINT64 m_oldId;		// UINT32 转 UINT64时临时用

#define PARTNER_MODULE(module)	\
private:\
	module m_##module;\
public:\
	module& Get##module() { return m_##module; }
#include "partnermodule.h"
#undef PARTNER_MODULE
};

#endif