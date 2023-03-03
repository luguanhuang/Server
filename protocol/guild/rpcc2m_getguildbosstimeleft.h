#ifndef __RPCC2M_GETGUILDBOSSTIMELEFT_H__
#define __RPCC2M_GETGUILDBOSSTIMELEFT_H__

// generate by ProtoGen at date: 2016/9/15 16:26:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETGUILDBOSSTIMELEFT_TYPE 25923

class RpcC2M_getguildbosstimeleft : public CRpc
{
private:

	RpcC2M_getguildbosstimeleft():CRpc(RPCC2M_GETGUILDBOSSTIMELEFT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_getguildbosstimeleft *CreateRpc()
	{
		return new RpcC2M_getguildbosstimeleft;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const getguildbosstimeleftArg &roArg, const getguildbosstimeleftRes &roRes, const CUserData &roUserData);
	void OnTimeout(const getguildbosstimeleftArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const getguildbosstimeleftArg &roArg, getguildbosstimeleftRes &roRes);
	void OnDelayReplyRpc(const getguildbosstimeleftArg &roArg, getguildbosstimeleftRes &roRes, const CUserData &roUserData);

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
	getguildbosstimeleftArg m_oArg;
	getguildbosstimeleftRes m_oRes;
};

#endif
