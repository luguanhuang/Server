#ifndef __RPCC2G_ARGENTAACTIVITY_H__
#define __RPCC2G_ARGENTAACTIVITY_H__

// generate by ProtoGen at date: 2017/4/11 14:48:30

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ARGENTAACTIVITY_TYPE 838

class RpcC2G_ArgentaActivity : public CRpc
{
private:

	RpcC2G_ArgentaActivity():CRpc(RPCC2G_ARGENTAACTIVITY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ArgentaActivity *CreateRpc()
	{
		return new RpcC2G_ArgentaActivity;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ArgentaActivityArg &roArg, const ArgentaActivityRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ArgentaActivityArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ArgentaActivityArg &roArg, ArgentaActivityRes &roRes);
	void OnDelayReplyRpc(const ArgentaActivityArg &roArg, ArgentaActivityRes &roRes, const CUserData &roUserData);

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
	ArgentaActivityArg m_oArg;
	ArgentaActivityRes m_oRes;
};

#endif
