#ifndef __RPCC2G_PETOPERATION_H__
#define __RPCC2G_PETOPERATION_H__

// generate by ProtoGen at date: 2016/4/15 11:20:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PETOPERATION_TYPE 28857

class RpcC2G_PetOperation : public CRpc
{
private:

	RpcC2G_PetOperation():CRpc(RPCC2G_PETOPERATION_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PetOperation *CreateRpc()
	{
		return new RpcC2G_PetOperation;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PetOperationArg &roArg, const PetOperationRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PetOperationArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PetOperationArg &roArg, PetOperationRes &roRes);
	void OnDelayReplyRpc(const PetOperationArg &roArg, PetOperationRes &roRes, const CUserData &roUserData);

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
	PetOperationArg m_oArg;
	PetOperationRes m_oRes;
};

#endif
