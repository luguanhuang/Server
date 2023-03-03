#ifndef __RPCC2M_REQGUILDLADDERRNAKINFO_H__
#define __RPCC2M_REQGUILDLADDERRNAKINFO_H__

// generate by ProtoGen at date: 2016/9/30 11:27:13

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REQGUILDLADDERRNAKINFO_TYPE 39925

class RpcC2M_ReqGuildLadderRnakInfo : public CRpc
{
private:

	RpcC2M_ReqGuildLadderRnakInfo():CRpc(RPCC2M_REQGUILDLADDERRNAKINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ReqGuildLadderRnakInfo *CreateRpc()
	{
		return new RpcC2M_ReqGuildLadderRnakInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildLadderRnakInfoArg &roArg, const ReqGuildLadderRnakInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildLadderRnakInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildLadderRnakInfoArg &roArg, ReqGuildLadderRnakInfoRes &roRes);
	void OnDelayReplyRpc(const ReqGuildLadderRnakInfoArg &roArg, ReqGuildLadderRnakInfoRes &roRes, const CUserData &roUserData);

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
	ReqGuildLadderRnakInfoArg m_oArg;
	ReqGuildLadderRnakInfoRes m_oRes;
};

#endif
