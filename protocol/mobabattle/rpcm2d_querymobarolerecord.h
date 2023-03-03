#ifndef __RPCM2D_QUERYMOBAROLERECORD_H__
#define __RPCM2D_QUERYMOBAROLERECORD_H__

// generate by ProtoGen at date: 2017/6/17 19:25:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_QUERYMOBAROLERECORD_TYPE 30481

class RpcM2D_QueryMobaRoleRecord : public CRpc
{
private:

	RpcM2D_QueryMobaRoleRecord():CRpc(RPCM2D_QUERYMOBAROLERECORD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_QueryMobaRoleRecord *CreateRpc()
	{
		return new RpcM2D_QueryMobaRoleRecord;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryMobaRoleRecordArg &roArg, const QueryMobaRoleRecordRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryMobaRoleRecordArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryMobaRoleRecordArg &roArg, QueryMobaRoleRecordRes &roRes);
	void OnDelayReplyRpc(const QueryMobaRoleRecordArg &roArg, QueryMobaRoleRecordRes &roRes, const CUserData &roUserData);

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
	QueryMobaRoleRecordArg m_oArg;
	QueryMobaRoleRecordRes m_oRes;
};

#endif
