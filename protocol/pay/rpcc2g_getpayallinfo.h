#ifndef __RPCC2G_GETPAYALLINFO_H__
#define __RPCC2G_GETPAYALLINFO_H__

// generate by ProtoGen at date: 2016/8/16 20:18:45

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETPAYALLINFO_TYPE 41260

class RpcC2G_GetPayAllInfo : public CRpc
{
private:

	RpcC2G_GetPayAllInfo():CRpc(RPCC2G_GETPAYALLINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetPayAllInfo *CreateRpc()
	{
		return new RpcC2G_GetPayAllInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPayAllInfoArg &roArg, const GetPayAllInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPayAllInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPayAllInfoArg &roArg, GetPayAllInfoRes &roRes);
	void OnDelayReplyRpc(const GetPayAllInfoArg &roArg, GetPayAllInfoRes &roRes, const CUserData &roUserData);

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
	GetPayAllInfoArg m_oArg;
	GetPayAllInfoRes m_oRes;
};

#endif
