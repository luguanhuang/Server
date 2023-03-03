#ifndef __RPCC2M_GETAPPLYGUILDLIST_H__
#define __RPCC2M_GETAPPLYGUILDLIST_H__

// generate by ProtoGen at date: 2016/11/3 19:39:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETAPPLYGUILDLIST_TYPE 31771

class RpcC2M_getapplyguildlist : public CRpc
{
private:

	RpcC2M_getapplyguildlist():CRpc(RPCC2M_GETAPPLYGUILDLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_getapplyguildlist *CreateRpc()
	{
		return new RpcC2M_getapplyguildlist;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const getapplyguildlistarg &roArg, const getapplyguildlistres &roRes, const CUserData &roUserData);
	void OnTimeout(const getapplyguildlistarg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const getapplyguildlistarg &roArg, getapplyguildlistres &roRes);
	void OnDelayReplyRpc(const getapplyguildlistarg &roArg, getapplyguildlistres &roRes, const CUserData &roUserData);

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
	getapplyguildlistarg m_oArg;
	getapplyguildlistres m_oRes;
};

#endif
