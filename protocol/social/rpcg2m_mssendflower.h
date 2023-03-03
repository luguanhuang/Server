#ifndef __RPCG2M_MSSENDFLOWER_H__
#define __RPCG2M_MSSENDFLOWER_H__

// generate by ProtoGen at date: 2016/8/18 16:41:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_MSSENDFLOWER_TYPE 5914

class RpcG2M_MSSendFlower : public CRpc
{
private:

	RpcG2M_MSSendFlower():CRpc(RPCG2M_MSSENDFLOWER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_MSSendFlower *CreateRpc()
	{
		return new RpcG2M_MSSendFlower;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const MSSendFlowerArg &roArg, const MSSendFlowerRes &roRes, const CUserData &roUserData);
	void OnTimeout(const MSSendFlowerArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const MSSendFlowerArg &roArg, MSSendFlowerRes &roRes);
	void OnDelayReplyRpc(const MSSendFlowerArg &roArg, MSSendFlowerRes &roRes, const CUserData &roUserData);

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
	MSSendFlowerArg m_oArg;
	MSSendFlowerRes m_oRes;
};

#endif
