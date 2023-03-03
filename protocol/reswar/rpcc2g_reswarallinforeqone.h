#ifndef __RPCC2G_RESWARALLINFOREQONE_H__
#define __RPCC2G_RESWARALLINFOREQONE_H__

// generate by ProtoGen at date: 2016/11/2 23:25:13

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_RESWARALLINFOREQONE_TYPE 8828

class RpcC2G_ResWarAllInfoReqOne : public CRpc
{
private:

	RpcC2G_ResWarAllInfoReqOne():CRpc(RPCC2G_RESWARALLINFOREQONE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ResWarAllInfoReqOne *CreateRpc()
	{
		return new RpcC2G_ResWarAllInfoReqOne;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ResWarArg &roArg, const ResWarRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ResWarArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ResWarArg &roArg, ResWarRes &roRes);
	void OnDelayReplyRpc(const ResWarArg &roArg, ResWarRes &roRes, const CUserData &roUserData);

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
	ResWarArg m_oArg;
	ResWarRes m_oRes;
};

#endif
