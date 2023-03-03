#ifndef __RPCC2M_GETDAILYTASKREFRESHRECORD_H__
#define __RPCC2M_GETDAILYTASKREFRESHRECORD_H__

// generate by ProtoGen at date: 2017/9/18 1:41:28

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETDAILYTASKREFRESHRECORD_TYPE 7202

class RpcC2M_GetDailyTaskRefreshRecord : public CRpc
{
private:

	RpcC2M_GetDailyTaskRefreshRecord():CRpc(RPCC2M_GETDAILYTASKREFRESHRECORD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetDailyTaskRefreshRecord *CreateRpc()
	{
		return new RpcC2M_GetDailyTaskRefreshRecord;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDailyTaskRefreshRecordArg &roArg, const GetDailyTaskRefreshRecordRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDailyTaskRefreshRecordArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDailyTaskRefreshRecordArg &roArg, GetDailyTaskRefreshRecordRes &roRes);
	void OnDelayReplyRpc(const GetDailyTaskRefreshRecordArg &roArg, GetDailyTaskRefreshRecordRes &roRes, const CUserData &roUserData);

	virtual void OnReplyTimeout() 
	{ 
		OnTimeout(m_oArg, m_oUser); 
	}

	virtual void OnDelayRpc(const CUserData &userData)
	{
		OnDelayReplyRpc( m_oArg, m_oRes, userData);
	}

	virtual CRpc *NewRpc() 
	{
		return CreateRpc();
	}

public:
	GetDailyTaskRefreshRecordArg m_oArg;
	GetDailyTaskRefreshRecordRes m_oRes;
};

#endif
