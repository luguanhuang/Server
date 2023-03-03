#ifndef __RPCM2G_CHECKMARRIAGECOUPLE_H__
#define __RPCM2G_CHECKMARRIAGECOUPLE_H__

// generate by ProtoGen at date: 2017/7/26 19:59:05

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_CHECKMARRIAGECOUPLE_TYPE 56199

class RpcM2G_CheckMarriageCouple : public CRpc
{
private:

	RpcM2G_CheckMarriageCouple():CRpc(RPCM2G_CHECKMARRIAGECOUPLE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_CheckMarriageCouple *CreateRpc()
	{
		return new RpcM2G_CheckMarriageCouple;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CheckMarriageCoupleArg &roArg, const CheckMarriageCoupleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CheckMarriageCoupleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CheckMarriageCoupleArg &roArg, CheckMarriageCoupleRes &roRes);
	void OnDelayReplyRpc(const CheckMarriageCoupleArg &roArg, CheckMarriageCoupleRes &roRes, const CUserData &roUserData);

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
	CheckMarriageCoupleArg m_oArg;
	CheckMarriageCoupleRes m_oRes;
};

#endif
