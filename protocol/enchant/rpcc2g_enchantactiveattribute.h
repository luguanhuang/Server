#ifndef __RPCC2G_ENCHANTACTIVEATTRIBUTE_H__
#define __RPCC2G_ENCHANTACTIVEATTRIBUTE_H__

// generate by ProtoGen at date: 2017/5/8 20:49:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ENCHANTACTIVEATTRIBUTE_TYPE 19086

class RpcC2G_EnchantActiveAttribute : public CRpc
{
private:

	RpcC2G_EnchantActiveAttribute():CRpc(RPCC2G_ENCHANTACTIVEATTRIBUTE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_EnchantActiveAttribute *CreateRpc()
	{
		return new RpcC2G_EnchantActiveAttribute;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnchantActiveAttributeArg &roArg, const EnchantActiveAttributeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnchantActiveAttributeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnchantActiveAttributeArg &roArg, EnchantActiveAttributeRes &roRes);
	void OnDelayReplyRpc(const EnchantActiveAttributeArg &roArg, EnchantActiveAttributeRes &roRes, const CUserData &roUserData);

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
	EnchantActiveAttributeArg m_oArg;
	EnchantActiveAttributeRes m_oRes;
};

#endif
