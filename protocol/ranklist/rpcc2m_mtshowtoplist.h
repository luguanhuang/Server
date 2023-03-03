#ifndef __RPCC2M_MTSHOWTOPLIST_H__
#define __RPCC2M_MTSHOWTOPLIST_H__

// generate by ProtoGen at date: 2016/9/22 16:09:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_MTSHOWTOPLIST_TYPE 10166

class RpcC2M_MTShowTopList : public CRpc
{
private:

	RpcC2M_MTShowTopList():CRpc(RPCC2M_MTSHOWTOPLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_MTShowTopList *CreateRpc()
	{
		return new RpcC2M_MTShowTopList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TShowTopListArg &roArg, const TShowTopListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TShowTopListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TShowTopListArg &roArg, TShowTopListRes &roRes);
	void OnDelayReplyRpc(const TShowTopListArg &roArg, TShowTopListRes &roRes, const CUserData &roUserData);

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
	TShowTopListArg m_oArg;
	TShowTopListRes m_oRes;
};

#endif
