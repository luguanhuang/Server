#ifndef __RPCS2M_INVFIGHTCROSS2MSREQ_H__
#define __RPCS2M_INVFIGHTCROSS2MSREQ_H__

// generate by ProtoGen at date: 2017/4/13 16:01:33

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCS2M_INVFIGHTCROSS2MSREQ_TYPE 31830

class RpcS2M_InvFightCross2MSReq : public CRpc
{
private:

	RpcS2M_InvFightCross2MSReq():CRpc(RPCS2M_INVFIGHTCROSS2MSREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcS2M_InvFightCross2MSReq *CreateRpc()
	{
		return new RpcS2M_InvFightCross2MSReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const InvFightCross2MSReqArg &roArg, const InvFightCross2MSReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const InvFightCross2MSReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const InvFightCross2MSReqArg &roArg, InvFightCross2MSReqRes &roRes);
	void OnDelayReplyRpc(const InvFightCross2MSReqArg &roArg, InvFightCross2MSReqRes &roRes, const CUserData &roUserData);

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
	InvFightCross2MSReqArg m_oArg;
	InvFightCross2MSReqRes m_oRes;
};

#endif
