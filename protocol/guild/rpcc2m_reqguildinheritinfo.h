#ifndef __RPCC2M_REQGUILDINHERITINFO_H__
#define __RPCC2M_REQGUILDINHERITINFO_H__

// generate by ProtoGen at date: 2016/11/18 16:23:02

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REQGUILDINHERITINFO_TYPE 7131

class RpcC2M_ReqGuildInheritInfo : public CRpc
{
private:

	RpcC2M_ReqGuildInheritInfo():CRpc(RPCC2M_REQGUILDINHERITINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ReqGuildInheritInfo *CreateRpc()
	{
		return new RpcC2M_ReqGuildInheritInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildInheritInfoArg &roArg, const ReqGuildInheritInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildInheritInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildInheritInfoArg &roArg, ReqGuildInheritInfoRes &roRes);
	void OnDelayReplyRpc(const ReqGuildInheritInfoArg &roArg, ReqGuildInheritInfoRes &roRes, const CUserData &roUserData);

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
	ReqGuildInheritInfoArg m_oArg;
	ReqGuildInheritInfoRes m_oRes;
};

#endif
