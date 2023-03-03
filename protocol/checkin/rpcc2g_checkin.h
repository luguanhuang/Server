#ifndef __RPCC2G_CHECKIN_H__
#define __RPCC2G_CHECKIN_H__

// generate by ProtoGen at date: 2015/6/22 21:19:55

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_CHECKIN_TYPE 56127

class RpcC2G_Checkin : public CRpc
{
private:

	RpcC2G_Checkin():CRpc(RPCC2G_CHECKIN_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_Checkin *CreateRpc()
	{
		return new RpcC2G_Checkin;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CheckinArg &roArg, const CheckinRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CheckinArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CheckinArg &roArg, CheckinRes &roRes);
	void OnDelayReplyRpc(const CheckinArg &roArg, CheckinRes &roRes, const CUserData &roUserData);

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
	CheckinArg m_oArg;
	CheckinRes m_oRes;
};

#endif
