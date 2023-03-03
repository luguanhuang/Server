#ifndef __RPCC2G_RETURNSMELTSTONE_H__
#define __RPCC2G_RETURNSMELTSTONE_H__

// generate by ProtoGen at date: 2017/5/13 19:38:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_RETURNSMELTSTONE_TYPE 16978

class RpcC2G_ReturnSmeltStone : public CRpc
{
private:

	RpcC2G_ReturnSmeltStone():CRpc(RPCC2G_RETURNSMELTSTONE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ReturnSmeltStone *CreateRpc()
	{
		return new RpcC2G_ReturnSmeltStone;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReturnSmeltStoneArg &roArg, const ReturnSmeltStoneRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReturnSmeltStoneArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReturnSmeltStoneArg &roArg, ReturnSmeltStoneRes &roRes);
	void OnDelayReplyRpc(const ReturnSmeltStoneArg &roArg, ReturnSmeltStoneRes &roRes, const CUserData &roUserData);

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
	ReturnSmeltStoneArg m_oArg;
	ReturnSmeltStoneRes m_oRes;
};

#endif
