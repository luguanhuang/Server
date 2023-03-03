#ifndef __RPCC2M_GETSKYCRAFTRECORD_H__
#define __RPCC2M_GETSKYCRAFTRECORD_H__

// generate by ProtoGen at date: 2017/4/11 14:47:47

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETSKYCRAFTRECORD_TYPE 39327

class RpcC2M_GetSkyCraftRecord : public CRpc
{
private:

	RpcC2M_GetSkyCraftRecord():CRpc(RPCC2M_GETSKYCRAFTRECORD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetSkyCraftRecord *CreateRpc()
	{
		return new RpcC2M_GetSkyCraftRecord;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetSkyCraftRecordArg &roArg, const GetSkyCraftRecordRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetSkyCraftRecordArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetSkyCraftRecordArg &roArg, GetSkyCraftRecordRes &roRes);
	void OnDelayReplyRpc(const GetSkyCraftRecordArg &roArg, GetSkyCraftRecordRes &roRes, const CUserData &roUserData);

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
	GetSkyCraftRecordArg m_oArg;
	GetSkyCraftRecordRes m_oRes;
};

#endif
