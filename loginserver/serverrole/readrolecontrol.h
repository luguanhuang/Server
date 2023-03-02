#ifndef _READ_ROLE_CONTROL_H_
#define _READ_ROLE_CONTROL_H_

#include "util/utilsingleton.h"
#include "pb/project.pb.h"
#include "timer.h"
#include "timeutil.h"


enum ReadState
{
	ReadState_Reading = 0,
	ReadState_Finish = 1,
	ReadState_Failed = 2,
};

struct OneZoneRoleInfo
{
	OneZoneRoleInfo(): m_state(ReadState_Reading) {};

	UINT32 m_state;
	std::vector<KKSG::ZoneRoleInfo> m_roles;
};

struct ReadRoleInfo
{
	ReadRoleInfo(): m_bindRoleId(0), m_bindPay(0), m_rpcId(0)
	{
		m_startTime = TimeUtil::GetTime();
	}

	std::map<UINT32, OneZoneRoleInfo> m_zoneRoles;
	UINT64 m_bindRoleId;
	UINT32 m_bindPay;
	UINT32 m_rpcId;
	UINT32 m_startTime;
};

class ReadRoleControl : public ITimer, public Singleton<ReadRoleControl>
{
public:
	ReadRoleControl();
	~ReadRoleControl();

	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	ReadRoleInfo* GetReadRole(const std::string& account);
	ReadRoleInfo* NewReadRole(const std::string& account);
	void RemoveReadRole(const std::string& account);

	void OnReadCompleate(const std::string& account, UINT32 serverId, const KKSG::GetZoneRoleInfoI2MRes &roRes);
	void OnReadTimeOut(const std::string& account, UINT32 serverId);

	bool CheckFinish(ReadRoleInfo* pInfo);

private:
	std::unordered_map<std::string, ReadRoleInfo*> m_readingInfo;
};

#endif