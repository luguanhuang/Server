#ifndef __LOGINRECORD_H__
#define __LOGINRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "item.h"
#include "pb/enum.pb.h"

namespace KKSG
{
	class RoleAllInfo;
	class LoginRewardRet;
};

class Role;

struct MyLoginReward
{
	MyLoginReward()
	{
		day = 0;
		state = KKSG::LOGINRS_CANNOT;
	}
	int day;
	KKSG::LoginRewardState state;
	std::vector<ItemDesc> items;
};

enum LoginRewardOpenState
{
	LROS_UNOPEN = 1,
	LROS_OPEN = 2,
	LROP_SHUTDOWN = 3,
};

class MyLoginRecord : public RoleSerializeInterface
{
public:
	MyLoginRecord(Role* pRole);
	~MyLoginRecord();
	virtual bool Load( const KKSG::RoleAllInfo *poRoleAllInfo );
	virtual void Save( KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged );
	virtual void FirstInit(UINT32 roleNum);
	void Update();

	void SetLoginRewardState(int day, KKSG::LoginRewardState state);
	KKSG::LoginRewardState GetLoginRewardState(int day);
	void GetLoginRewardItem(int day, std::vector<ItemDesc>& items);

	void UpdateLoginRewardOpenState();

	void OnDayZero();

	void OnEnterScene();
	void SetLoginRewardConf();

	UINT32 GetRealLoginDayCount();

	void TestAddLoginDay();
	void TestClear();
	void TestPrint();

private:
	void LoginRewardNtf();
	void FillMyLoginRewards(KKSG::LoginRewardRet& ret);

	void OnLoginRewardOpened();
	void OnLoginRewardShutDown();

	UINT32 m_lastUpdateDay;//满足唯一性 不满足连续性
	UINT32 m_loginDayCount;
	int m_loginDayForLoginReward;//登录奖励开启后 登录天数
	LoginRewardOpenState m_lroState;
	std::vector<MyLoginReward> m_loginRewards;

	Role* m_role;

	Switch m_IsModified;
};

#endif