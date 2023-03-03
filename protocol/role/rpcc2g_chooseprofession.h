#ifndef __RPCC2G_CHOOSEPROFESSION_H__
#define __RPCC2G_CHOOSEPROFESSION_H__

// generate by ProtoGen at date: 2015/4/14 13:48:01

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_CHOOSEPROFESSION_TYPE 24314

class RpcC2G_ChooseProfession : public CRpc
{
private:

	RpcC2G_ChooseProfession():CRpc(RPCC2G_CHOOSEPROFESSION_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ChooseProfession *CreateRpc()
	{
		return new RpcC2G_ChooseProfession;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChooseProfArg &roArg, const ChooseProfRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChooseProfArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChooseProfArg &roArg, ChooseProfRes &roRes);
	void OnDelayReplyRpc(const ChooseProfArg &roArg, ChooseProfRes &roRes, const CUserData &roUserData);

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
	ChooseProfArg m_oArg;
	ChooseProfRes m_oRes;
};

#endif
