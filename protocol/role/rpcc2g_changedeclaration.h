#ifndef __RPCC2G_CHANGEDECLARATION_H__
#define __RPCC2G_CHANGEDECLARATION_H__

// generate by ProtoGen at date: 2017/2/7 14:20:22

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_CHANGEDECLARATION_TYPE 1588

class RpcC2G_ChangeDeclaration : public CRpc
{
private:

	RpcC2G_ChangeDeclaration():CRpc(RPCC2G_CHANGEDECLARATION_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ChangeDeclaration *CreateRpc()
	{
		return new RpcC2G_ChangeDeclaration;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeDeclarationArg &roArg, const ChangeDeclarationRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeDeclarationArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeDeclarationArg &roArg, ChangeDeclarationRes &roRes);
	void OnDelayReplyRpc(const ChangeDeclarationArg &roArg, ChangeDeclarationRes &roRes, const CUserData &roUserData);

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
	ChangeDeclarationArg m_oArg;
	ChangeDeclarationRes m_oRes;
};

#endif
