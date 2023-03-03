#ifndef __RPCC2G_CHANGEPROFESSION_H__
#define __RPCC2G_CHANGEPROFESSION_H__

// generate by ProtoGen at date: 2017/1/11 16:33:03

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_CHANGEPROFESSION_TYPE 48822

class RpcC2G_ChangeProfession : public CRpc
{
private:

	RpcC2G_ChangeProfession():CRpc(RPCC2G_CHANGEPROFESSION_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ChangeProfession *CreateRpc()
	{
		return new RpcC2G_ChangeProfession;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeProfessionArg &roArg, const ChangeProfessionRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeProfessionArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeProfessionArg &roArg, ChangeProfessionRes &roRes);
	void OnDelayReplyRpc(const ChangeProfessionArg &roArg, ChangeProfessionRes &roRes, const CUserData &roUserData);

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
	ChangeProfessionArg m_oArg;
	ChangeProfessionRes m_oRes;
};

#endif
