#ifndef __RPCC2G_WEEKEND4V4GETINFO_H__
#define __RPCC2G_WEEKEND4V4GETINFO_H__

// generate by ProtoGen at date: 2017/6/6 19:41:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_WEEKEND4V4GETINFO_TYPE 59573

class RpcC2G_WeekEnd4v4GetInfo : public CRpc
{
private:

	RpcC2G_WeekEnd4v4GetInfo():CRpc(RPCC2G_WEEKEND4V4GETINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_WeekEnd4v4GetInfo *CreateRpc()
	{
		return new RpcC2G_WeekEnd4v4GetInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const WeekEnd4v4GetInfoArg &roArg, const WeekEnd4v4GetInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const WeekEnd4v4GetInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const WeekEnd4v4GetInfoArg &roArg, WeekEnd4v4GetInfoRes &roRes);
	void OnDelayReplyRpc(const WeekEnd4v4GetInfoArg &roArg, WeekEnd4v4GetInfoRes &roRes, const CUserData &roUserData);

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
	WeekEnd4v4GetInfoArg m_oArg;
	WeekEnd4v4GetInfoRes m_oRes;
};

#endif
