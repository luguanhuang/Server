#ifndef __RPCC2G_GETMYWATCHRECORD_H__
#define __RPCC2G_GETMYWATCHRECORD_H__

// generate by ProtoGen at date: 2016/7/11 16:12:16

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETMYWATCHRECORD_TYPE 22907

class RpcC2G_GetMyWatchRecord : public CRpc
{
private:

	RpcC2G_GetMyWatchRecord():CRpc(RPCC2G_GETMYWATCHRECORD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetMyWatchRecord *CreateRpc()
	{
		return new RpcC2G_GetMyWatchRecord;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetMyWatchRecordArg &roArg, const GetMyWatchRecordRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetMyWatchRecordArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetMyWatchRecordArg &roArg, GetMyWatchRecordRes &roRes);
	void OnDelayReplyRpc(const GetMyWatchRecordArg &roArg, GetMyWatchRecordRes &roRes, const CUserData &roUserData);

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
	GetMyWatchRecordArg m_oArg;
	GetMyWatchRecordRes m_oRes;
};

#endif
