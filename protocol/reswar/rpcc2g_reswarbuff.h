#ifndef __RPCC2G_RESWARBUFF_H__
#define __RPCC2G_RESWARBUFF_H__

// generate by ProtoGen at date: 2016/11/26 17:31:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_RESWARBUFF_TYPE 17670

class RpcC2G_ResWarBuff : public CRpc
{
private:

	RpcC2G_ResWarBuff():CRpc(RPCC2G_RESWARBUFF_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ResWarBuff *CreateRpc()
	{
		return new RpcC2G_ResWarBuff;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ResWarBuffArg &roArg, const ResWarBuffRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ResWarBuffArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ResWarBuffArg &roArg, ResWarBuffRes &roRes);
	void OnDelayReplyRpc(const ResWarBuffArg &roArg, ResWarBuffRes &roRes, const CUserData &roUserData);

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
	ResWarBuffArg m_oArg;
	ResWarBuffRes m_oRes;
};

#endif
