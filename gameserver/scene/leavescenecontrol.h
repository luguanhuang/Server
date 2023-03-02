#ifndef _Leave_Scene_Control_H__
#define _Leave_Scene_Control_H__

#include "timer.h"
#include "pb/enum.pb.h"

class Role;

class CLeaveSceneReq
{
public:
	UINT32		m_dwSerialID;
	UINT32		m_dwRpcDelayed;
	UINT64		m_qwBeginTime;
	UINT64		m_qwEndTime;
	std::vector<UINT64> m_oRoleList;
	std::set<UINT64>	m_oWaitSet;
	std::map<UINT64, std::string> m_oRoleDataMap;

	CLeaveSceneReq()
		:m_dwSerialID(0)
		,m_dwRpcDelayed(0)
		,m_qwBeginTime(0)
		,m_qwEndTime(0)
	{
	}
	~CLeaveSceneReq()
	{
	}
	const std::string& GetRoleData(UINT64 qwRoleID) const
	{
		std::map<UINT64, std::string>::const_iterator it = m_oRoleDataMap.find(qwRoleID);
		if(it == m_oRoleDataMap.end())
		{
			static std::string soEmpty;
			return soEmpty;
		}
		return it->second;
	}
	void AddRoleData(UINT64 qwRoleID, const std::string& strData)
	{
		m_oRoleDataMap[qwRoleID] = strData;
	}
};


class CLeaveSceneControl
{
	CLeaveSceneControl();
	virtual ~CLeaveSceneControl();
	DECLARE_SINGLETON(CLeaveSceneControl)

public:
	bool Init();
	void Uninit();

	//return 0 if failed
	UINT32 BeginLeave(UINT32 dwRpcDealyed, const std::vector<Role*>& roRoleList);
	void CancelLeave(UINT32 dwSerialID);

	void OnSaveRoleDataReply(UINT32 dwSerialID, UINT64 qwRoleID, KKSG::ErrorCode nResult);

	CLeaveSceneReq* GetReq(UINT32 dwSerialID);
private:
	typedef std::set<UINT64>	CRoleSet;
	typedef std::map<UINT32, CLeaveSceneReq>	CRequestMap;

	UINT32		m_dwSerialID;
	CRoleSet	m_oRoleSet;
	CRequestMap m_oReqMap;
};

#endif