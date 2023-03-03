#ifndef __RPCC2M_DELGUILDINHERIT_H__
#define __RPCC2M_DELGUILDINHERIT_H__

// generate by ProtoGen at date: 2016/11/18 16:31:32

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_DELGUILDINHERIT_TYPE 3671

class RpcC2M_DelGuildInherit : public CRpc
{
private:

	RpcC2M_DelGuildInherit():CRpc(RPCC2M_DELGUILDINHERIT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_DelGuildInherit *CreateRpc()
	{
		return new RpcC2M_DelGuildInherit;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DelGuildInheritArg &roArg, const DelGuildInheritRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DelGuildInheritArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DelGuildInheritArg &roArg, DelGuildInheritRes &roRes);
	void OnDelayReplyRpc(const DelGuildInheritArg &roArg, DelGuildInheritRes &roRes, const CUserData &roUserData);

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
	DelGuildInheritArg m_oArg;
	DelGuildInheritRes m_oRes;
};

#endif
