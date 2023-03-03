#ifndef __RPCC2G_ENHANCEITEM_H__
#define __RPCC2G_ENHANCEITEM_H__

// generate by ProtoGen at date: 2016/12/12 17:11:03

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ENHANCEITEM_TYPE 3744

class RpcC2G_EnhanceItem : public CRpc
{
private:

	RpcC2G_EnhanceItem():CRpc(RPCC2G_ENHANCEITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_EnhanceItem *CreateRpc()
	{
		return new RpcC2G_EnhanceItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnhanceItemArg &roArg, const EnhanceItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnhanceItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnhanceItemArg &roArg, EnhanceItemRes &roRes);
	void OnDelayReplyRpc(const EnhanceItemArg &roArg, EnhanceItemRes &roRes, const CUserData &roUserData);

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
	EnhanceItemArg m_oArg;
	EnhanceItemRes m_oRes;
};

#endif
