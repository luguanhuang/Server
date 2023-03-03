#ifndef __RPCC2M_GETGUILDINTEGRALINFO_H__
#define __RPCC2M_GETGUILDINTEGRALINFO_H__

// generate by ProtoGen at date: 2016/11/4 14:28:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETGUILDINTEGRALINFO_TYPE 56762

class RpcC2M_GetGuildIntegralInfo : public CRpc
{
private:

	RpcC2M_GetGuildIntegralInfo():CRpc(RPCC2M_GETGUILDINTEGRALINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetGuildIntegralInfo *CreateRpc()
	{
		return new RpcC2M_GetGuildIntegralInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildIntegralInfoArg &roArg, const GetGuildIntegralInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildIntegralInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildIntegralInfoArg &roArg, GetGuildIntegralInfoRes &roRes);
	void OnDelayReplyRpc(const GetGuildIntegralInfoArg &roArg, GetGuildIntegralInfoRes &roRes, const CUserData &roUserData);

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
	GetGuildIntegralInfoArg m_oArg;
	GetGuildIntegralInfoRes m_oRes;
};

#endif
