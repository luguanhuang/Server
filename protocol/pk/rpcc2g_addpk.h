#ifndef __RPCC2G_ADDPK_H__
#define __RPCC2G_ADDPK_H__

// generate by ProtoGen at date: 2015/12/10 11:10:30

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ADDPK_TYPE 61829

class RpcC2G_AddPk : public CRpc
{
private:

	RpcC2G_AddPk():CRpc(RPCC2G_ADDPK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_AddPk *CreateRpc()
	{
		return new RpcC2G_AddPk;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AddPkReq &roArg, const AddPkRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AddPkReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AddPkReq &roArg, AddPkRes &roRes);
	void OnDelayReplyRpc(const AddPkReq &roArg, AddPkRes &roRes, const CUserData &roUserData);

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
	AddPkReq m_oArg;
	AddPkRes m_oRes;
};

#endif
