#ifndef __RPCC2G_GETENHANCEATTR_H__
#define __RPCC2G_GETENHANCEATTR_H__

// generate by ProtoGen at date: 2017/7/6 16:17:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETENHANCEATTR_TYPE 23396

class RpcC2G_GetEnhanceAttr : public CRpc
{
private:

	RpcC2G_GetEnhanceAttr():CRpc(RPCC2G_GETENHANCEATTR_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetEnhanceAttr *CreateRpc()
	{
		return new RpcC2G_GetEnhanceAttr;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetEnhanceAttrArg &roArg, const GetEnhanceAttrRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetEnhanceAttrArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetEnhanceAttrArg &roArg, GetEnhanceAttrRes &roRes);
	void OnDelayReplyRpc(const GetEnhanceAttrArg &roArg, GetEnhanceAttrRes &roRes, const CUserData &roUserData);

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
	GetEnhanceAttrArg m_oArg;
	GetEnhanceAttrRes m_oRes;
};

#endif
