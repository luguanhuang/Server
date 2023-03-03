#ifndef __RPCC2G_SETPRESHOW_H__
#define __RPCC2G_SETPRESHOW_H__

// generate by ProtoGen at date: 2017/9/7 15:32:38

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SETPRESHOW_TYPE 346

class RpcC2G_SetPreShow : public CRpc
{
private:

	RpcC2G_SetPreShow():CRpc(RPCC2G_SETPRESHOW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SetPreShow *CreateRpc()
	{
		return new RpcC2G_SetPreShow;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SetPreShowArg &roArg, const SetPreShowRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SetPreShowArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SetPreShowArg &roArg, SetPreShowRes &roRes);
	void OnDelayReplyRpc(const SetPreShowArg &roArg, SetPreShowRes &roRes, const CUserData &roUserData);

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
	SetPreShowArg m_oArg;
	SetPreShowRes m_oRes;
};

#endif
