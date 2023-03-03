#ifndef __RPCC2M_GETSKYCRAFTELIINFO_H__
#define __RPCC2M_GETSKYCRAFTELIINFO_H__

// generate by ProtoGen at date: 2017/4/11 15:17:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETSKYCRAFTELIINFO_TYPE 41103

class RpcC2M_GetSkyCraftEliInfo : public CRpc
{
private:

	RpcC2M_GetSkyCraftEliInfo():CRpc(RPCC2M_GETSKYCRAFTELIINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetSkyCraftEliInfo *CreateRpc()
	{
		return new RpcC2M_GetSkyCraftEliInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetSkyCraftEliInfoArg &roArg, const GetSkyCraftEliInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetSkyCraftEliInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetSkyCraftEliInfoArg &roArg, GetSkyCraftEliInfoRes &roRes);
	void OnDelayReplyRpc(const GetSkyCraftEliInfoArg &roArg, GetSkyCraftEliInfoRes &roRes, const CUserData &roUserData);

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
	GetSkyCraftEliInfoArg m_oArg;
	GetSkyCraftEliInfoRes m_oRes;
};

#endif
