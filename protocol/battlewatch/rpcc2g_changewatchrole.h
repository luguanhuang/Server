#ifndef __RPCC2G_CHANGEWATCHROLE_H__
#define __RPCC2G_CHANGEWATCHROLE_H__

// generate by ProtoGen at date: 2016/7/11 16:33:38

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_CHANGEWATCHROLE_TYPE 35369

class RpcC2G_ChangeWatchRole : public CRpc
{
private:

	RpcC2G_ChangeWatchRole():CRpc(RPCC2G_CHANGEWATCHROLE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ChangeWatchRole *CreateRpc()
	{
		return new RpcC2G_ChangeWatchRole;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeWatchRoleArg &roArg, const ChangeWatchRoleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeWatchRoleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeWatchRoleArg &roArg, ChangeWatchRoleRes &roRes);
	void OnDelayReplyRpc(const ChangeWatchRoleArg &roArg, ChangeWatchRoleRes &roRes, const CUserData &roUserData);

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
	ChangeWatchRoleArg m_oArg;
	ChangeWatchRoleRes m_oRes;
};

#endif
