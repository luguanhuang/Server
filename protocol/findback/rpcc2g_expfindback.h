#ifndef __RPCC2G_EXPFINDBACK_H__
#define __RPCC2G_EXPFINDBACK_H__

// generate by ProtoGen at date: 2016/5/10 10:58:32

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_EXPFINDBACK_TYPE 38008

class RpcC2G_ExpFindBack : public CRpc
{
private:

	RpcC2G_ExpFindBack():CRpc(RPCC2G_EXPFINDBACK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ExpFindBack *CreateRpc()
	{
		return new RpcC2G_ExpFindBack;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ExpFindBackArg &roArg, const ExpFindBackRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ExpFindBackArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ExpFindBackArg &roArg, ExpFindBackRes &roRes);
	void OnDelayReplyRpc(const ExpFindBackArg &roArg, ExpFindBackRes &roRes, const CUserData &roUserData);

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
	ExpFindBackArg m_oArg;
	ExpFindBackRes m_oRes;
};

#endif
