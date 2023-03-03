#ifndef __RPCC2G_QUICKPK_H__
#define __RPCC2G_QUICKPK_H__

// generate by ProtoGen at date: 2016/3/29 17:19:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_QUICKPK_TYPE 40778

class RpcC2G_QuickPK : public CRpc
{
private:

	RpcC2G_QuickPK():CRpc(RPCC2G_QUICKPK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_QuickPK *CreateRpc()
	{
		return new RpcC2G_QuickPK;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QuickPKArg &roArg, const QuickPKRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QuickPKArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QuickPKArg &roArg, QuickPKRes &roRes);
	void OnDelayReplyRpc(const QuickPKArg &roArg, QuickPKRes &roRes, const CUserData &roUserData);

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
	QuickPKArg m_oArg;
	QuickPKRes m_oRes;
};

#endif
