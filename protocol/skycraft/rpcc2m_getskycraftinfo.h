#ifndef __RPCC2M_GETSKYCRAFTINFO_H__
#define __RPCC2M_GETSKYCRAFTINFO_H__

// generate by ProtoGen at date: 2017/4/11 14:28:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETSKYCRAFTINFO_TYPE 26199

class RpcC2M_GetSkyCraftInfo : public CRpc
{
private:

	RpcC2M_GetSkyCraftInfo():CRpc(RPCC2M_GETSKYCRAFTINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetSkyCraftInfo *CreateRpc()
	{
		return new RpcC2M_GetSkyCraftInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetSkyCraftInfoArg &roArg, const GetSkyCraftInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetSkyCraftInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetSkyCraftInfoArg &roArg, GetSkyCraftInfoRes &roRes);
	void OnDelayReplyRpc(const GetSkyCraftInfoArg &roArg, GetSkyCraftInfoRes &roRes, const CUserData &roUserData);

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
	GetSkyCraftInfoArg m_oArg;
	GetSkyCraftInfoRes m_oRes;
};

#endif
