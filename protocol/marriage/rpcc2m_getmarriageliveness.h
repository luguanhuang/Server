#ifndef __RPCC2M_GETMARRIAGELIVENESS_H__
#define __RPCC2M_GETMARRIAGELIVENESS_H__

// generate by ProtoGen at date: 2017/7/28 15:59:07

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETMARRIAGELIVENESS_TYPE 30055

class RpcC2M_GetMarriageLiveness : public CRpc
{
private:

	RpcC2M_GetMarriageLiveness():CRpc(RPCC2M_GETMARRIAGELIVENESS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetMarriageLiveness *CreateRpc()
	{
		return new RpcC2M_GetMarriageLiveness;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetMarriageLivenessArg &roArg, const GetMarriageLivenessRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetMarriageLivenessArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetMarriageLivenessArg &roArg, GetMarriageLivenessRes &roRes);
	void OnDelayReplyRpc(const GetMarriageLivenessArg &roArg, GetMarriageLivenessRes &roRes, const CUserData &roUserData);

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
	GetMarriageLivenessArg m_oArg;
	GetMarriageLivenessRes m_oRes;
};

#endif
