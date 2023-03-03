#ifndef __RPCC2G_ENCHANTTRANSFER_H__
#define __RPCC2G_ENCHANTTRANSFER_H__

// generate by ProtoGen at date: 2017/1/14 17:47:09

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ENCHANTTRANSFER_TYPE 54906

class RpcC2G_EnchantTransfer : public CRpc
{
private:

	RpcC2G_EnchantTransfer():CRpc(RPCC2G_ENCHANTTRANSFER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_EnchantTransfer *CreateRpc()
	{
		return new RpcC2G_EnchantTransfer;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnchantTransferArg &roArg, const EnchantTransferRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnchantTransferArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnchantTransferArg &roArg, EnchantTransferRes &roRes);
	void OnDelayReplyRpc(const EnchantTransferArg &roArg, EnchantTransferRes &roRes, const CUserData &roUserData);

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
	EnchantTransferArg m_oArg;
	EnchantTransferRes m_oRes;
};

#endif
