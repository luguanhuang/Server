#ifndef __RPCC2M_RECALLIANCE_H__
#define __RPCC2M_RECALLIANCE_H__

// generate by ProtoGen at date: 2016/12/20 11:11:05

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_RECALLIANCE_TYPE 31937

class RpcC2M_RecAlliance : public CRpc
{
private:

	RpcC2M_RecAlliance():CRpc(RPCC2M_RECALLIANCE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_RecAlliance *CreateRpc()
	{
		return new RpcC2M_RecAlliance;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RecAllianceArg &roArg, const RecAllianceRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RecAllianceArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RecAllianceArg &roArg, RecAllianceRes &roRes);
	void OnDelayReplyRpc(const RecAllianceArg &roArg, RecAllianceRes &roRes, const CUserData &roUserData);

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
	RecAllianceArg m_oArg;
	RecAllianceRes m_oRes;
};

#endif
