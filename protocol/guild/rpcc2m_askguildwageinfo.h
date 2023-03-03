#ifndef __RPCC2M_ASKGUILDWAGEINFO_H__
#define __RPCC2M_ASKGUILDWAGEINFO_H__

// generate by ProtoGen at date: 2016/10/27 15:24:47

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ASKGUILDWAGEINFO_TYPE 17779

class RpcC2M_AskGuildWageInfo : public CRpc
{
private:

	RpcC2M_AskGuildWageInfo():CRpc(RPCC2M_ASKGUILDWAGEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AskGuildWageInfo *CreateRpc()
	{
		return new RpcC2M_AskGuildWageInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AskGuildWageInfoArg &roArg, const AskGuildWageInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AskGuildWageInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AskGuildWageInfoArg &roArg, AskGuildWageInfoRes &roRes);
	void OnDelayReplyRpc(const AskGuildWageInfoArg &roArg, AskGuildWageInfoRes &roRes, const CUserData &roUserData);

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
	AskGuildWageInfoArg m_oArg;
	AskGuildWageInfoRes m_oRes;
};

#endif
