#ifndef __RPCC2M_ADDGUILDINHERIT_H__
#define __RPCC2M_ADDGUILDINHERIT_H__

// generate by ProtoGen at date: 2016/11/17 15:39:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ADDGUILDINHERIT_TYPE 15845

class RpcC2M_AddGuildInherit : public CRpc
{
private:

	RpcC2M_AddGuildInherit():CRpc(RPCC2M_ADDGUILDINHERIT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AddGuildInherit *CreateRpc()
	{
		return new RpcC2M_AddGuildInherit;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AddGuildInheritArg &roArg, const AddGuildInheritRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AddGuildInheritArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AddGuildInheritArg &roArg, AddGuildInheritRes &roRes);
	void OnDelayReplyRpc(const AddGuildInheritArg &roArg, AddGuildInheritRes &roRes, const CUserData &roUserData);

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
	AddGuildInheritArg m_oArg;
	AddGuildInheritRes m_oRes;
};

#endif
