#ifndef __RPCC2G_STAGECOUNTRESET_H__
#define __RPCC2G_STAGECOUNTRESET_H__

// generate by ProtoGen at date: 2015/10/8 14:21:24

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_STAGECOUNTRESET_TYPE 8496

class RpcC2G_StageCountReset : public CRpc
{
private:

	RpcC2G_StageCountReset():CRpc(RPCC2G_STAGECOUNTRESET_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_StageCountReset *CreateRpc()
	{
		return new RpcC2G_StageCountReset;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const StageCountResetArg &roArg, const StageCountResetRes &roRes, const CUserData &roUserData);
	void OnTimeout(const StageCountResetArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const StageCountResetArg &roArg, StageCountResetRes &roRes);
	void OnDelayReplyRpc(const StageCountResetArg &roArg, StageCountResetRes &roRes, const CUserData &roUserData);

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
	StageCountResetArg m_oArg;
	StageCountResetRes m_oRes;
};

#endif
