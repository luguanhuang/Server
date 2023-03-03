#ifndef __RPCC2G_USEITEM_H__
#define __RPCC2G_USEITEM_H__

// generate by ProtoGen at date: 2015/6/12 10:21:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_USEITEM_TYPE 64132

class RpcC2G_UseItem : public CRpc
{
private:

	RpcC2G_UseItem():CRpc(RPCC2G_USEITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_UseItem *CreateRpc()
	{
		return new RpcC2G_UseItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UseItemArg &roArg, const UseItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UseItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UseItemArg &roArg, UseItemRes &roRes);
	void OnDelayReplyRpc(const UseItemArg &roArg, UseItemRes &roRes, const CUserData &roUserData);

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
	UseItemArg m_oArg;
	UseItemRes m_oRes;
};

#endif
