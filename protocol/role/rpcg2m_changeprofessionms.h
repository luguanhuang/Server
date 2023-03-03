#ifndef __RPCG2M_CHANGEPROFESSIONMS_H__
#define __RPCG2M_CHANGEPROFESSIONMS_H__

// generate by ProtoGen at date: 2017/1/12 15:22:37

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_CHANGEPROFESSIONMS_TYPE 47282

class RpcG2M_ChangeProfessionMs : public CRpc
{
private:

	RpcG2M_ChangeProfessionMs():CRpc(RPCG2M_CHANGEPROFESSIONMS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_ChangeProfessionMs *CreateRpc()
	{
		return new RpcG2M_ChangeProfessionMs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeProfessionMsArg &roArg, const ChangeProfessionMsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeProfessionMsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeProfessionMsArg &roArg, ChangeProfessionMsRes &roRes);
	void OnDelayReplyRpc(const ChangeProfessionMsArg &roArg, ChangeProfessionMsRes &roRes, const CUserData &roUserData);

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
	ChangeProfessionMsArg m_oArg;
	ChangeProfessionMsRes m_oRes;
};

#endif
