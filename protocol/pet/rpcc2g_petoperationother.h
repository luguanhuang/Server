#ifndef __RPCC2G_PETOPERATIONOTHER_H__
#define __RPCC2G_PETOPERATIONOTHER_H__

// generate by ProtoGen at date: 2017/6/16 14:16:19

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PETOPERATIONOTHER_TYPE 58525

class RpcC2G_PetOperationOther : public CRpc
{
private:

	RpcC2G_PetOperationOther():CRpc(RPCC2G_PETOPERATIONOTHER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PetOperationOther *CreateRpc()
	{
		return new RpcC2G_PetOperationOther;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PetOperationOtherArg &roArg, const PetOperationOtherRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PetOperationOtherArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PetOperationOtherArg &roArg, PetOperationOtherRes &roRes);
	void OnDelayReplyRpc(const PetOperationOtherArg &roArg, PetOperationOtherRes &roRes, const CUserData &roUserData);

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
	PetOperationOtherArg m_oArg;
	PetOperationOtherRes m_oRes;
};

#endif
