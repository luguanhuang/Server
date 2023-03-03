#ifndef __RPCC2G_GETGUILDCHECKINRECORDS_H__
#define __RPCC2G_GETGUILDCHECKINRECORDS_H__

// generate by ProtoGen at date: 2015/10/8 19:46:33

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGUILDCHECKINRECORDS_TYPE 51906

class RpcC2G_GetGuildCheckinRecords : public CRpc
{
private:

	RpcC2G_GetGuildCheckinRecords():CRpc(RPCC2G_GETGUILDCHECKINRECORDS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGuildCheckinRecords *CreateRpc()
	{
		return new RpcC2G_GetGuildCheckinRecords;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildCheckinRecordsArg &roArg, const GetGuildCheckinRecordsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildCheckinRecordsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildCheckinRecordsArg &roArg, GetGuildCheckinRecordsRes &roRes);
	void OnDelayReplyRpc(const GetGuildCheckinRecordsArg &roArg, GetGuildCheckinRecordsRes &roRes, const CUserData &roUserData);

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
	GetGuildCheckinRecordsArg m_oArg;
	GetGuildCheckinRecordsRes m_oRes;
};

#endif
