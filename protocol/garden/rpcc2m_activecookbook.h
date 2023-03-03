#ifndef __RPCC2M_ACTIVECOOKBOOK_H__
#define __RPCC2M_ACTIVECOOKBOOK_H__

// generate by ProtoGen at date: 2016/10/21 11:36:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ACTIVECOOKBOOK_TYPE 31076

class RpcC2M_ActiveCookbook : public CRpc
{
private:

	RpcC2M_ActiveCookbook():CRpc(RPCC2M_ACTIVECOOKBOOK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ActiveCookbook *CreateRpc()
	{
		return new RpcC2M_ActiveCookbook;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ActiveCookbookArg &roArg, const ActiveCookbookRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ActiveCookbookArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ActiveCookbookArg &roArg, ActiveCookbookRes &roRes);
	void OnDelayReplyRpc(const ActiveCookbookArg &roArg, ActiveCookbookRes &roRes, const CUserData &roUserData);

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
	ActiveCookbookArg m_oArg;
	ActiveCookbookRes m_oRes;
};

#endif
