#ifndef __RPCC2G_GETACTIVITYINFO_H__
#define __RPCC2G_GETACTIVITYINFO_H__

// generate by ProtoGen at date: 2016/8/19 16:29:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETACTIVITYINFO_TYPE 43911

class RpcC2G_GetActivityInfo : public CRpc
{
private:

	RpcC2G_GetActivityInfo():CRpc(RPCC2G_GETACTIVITYINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetActivityInfo *CreateRpc()
	{
		return new RpcC2G_GetActivityInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetActivityInfoArg &roArg, const GetActivityInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetActivityInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetActivityInfoArg &roArg, GetActivityInfoRes &roRes);
	void OnDelayReplyRpc(const GetActivityInfoArg &roArg, GetActivityInfoRes &roRes, const CUserData &roUserData);

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
	GetActivityInfoArg m_oArg;
	GetActivityInfoRes m_oRes;
};

#endif
