#ifndef __RPCK2H_GMCOMMANDW2T_H__
#define __RPCK2H_GMCOMMANDW2T_H__

// generate by ProtoGen at date: 2017/6/23 20:45:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCK2H_GMCOMMANDW2T_TYPE 24217

class RpcK2H_GMCommandW2T : public CRpc
{
private:

	RpcK2H_GMCommandW2T():CRpc(RPCK2H_GMCOMMANDW2T_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcK2H_GMCommandW2T *CreateRpc()
	{
		return new RpcK2H_GMCommandW2T;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GMCmdg2mArg &roArg, const GMCmdg2mRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GMCmdg2mArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GMCmdg2mArg &roArg, GMCmdg2mRes &roRes);
	void OnDelayReplyRpc(const GMCmdg2mArg &roArg, GMCmdg2mRes &roRes, const CUserData &roUserData);

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
	GMCmdg2mArg m_oArg;
	GMCmdg2mRes m_oRes;
};

#endif
