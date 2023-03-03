#ifndef __RPCC2M_GETWEDDINGINVITEINFO_H__
#define __RPCC2M_GETWEDDINGINVITEINFO_H__

// generate by ProtoGen at date: 2017/7/17 16:36:23

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETWEDDINGINVITEINFO_TYPE 2804

class RpcC2M_GetWeddingInviteInfo : public CRpc
{
private:

	RpcC2M_GetWeddingInviteInfo():CRpc(RPCC2M_GETWEDDINGINVITEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetWeddingInviteInfo *CreateRpc()
	{
		return new RpcC2M_GetWeddingInviteInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetWeddingInviteInfoArg &roArg, const GetWeddingInviteInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetWeddingInviteInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetWeddingInviteInfoArg &roArg, GetWeddingInviteInfoRes &roRes);
	void OnDelayReplyRpc(const GetWeddingInviteInfoArg &roArg, GetWeddingInviteInfoRes &roRes, const CUserData &roUserData);

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
	GetWeddingInviteInfoArg m_oArg;
	GetWeddingInviteInfoRes m_oRes;
};

#endif
