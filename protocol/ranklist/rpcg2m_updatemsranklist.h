#ifndef __RPCG2M_UPDATEMSRANKLIST_H__
#define __RPCG2M_UPDATEMSRANKLIST_H__

// generate by ProtoGen at date: 2016/8/25 16:21:42

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_UPDATEMSRANKLIST_TYPE 33753

class RpcG2M_UpdateMSRankList : public CRpc
{
private:

	RpcG2M_UpdateMSRankList():CRpc(RPCG2M_UPDATEMSRANKLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_UpdateMSRankList *CreateRpc()
	{
		return new RpcG2M_UpdateMSRankList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UpdateMSRankListArg &roArg, const UpdateMSRankListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UpdateMSRankListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UpdateMSRankListArg &roArg, UpdateMSRankListRes &roRes);
	void OnDelayReplyRpc(const UpdateMSRankListArg &roArg, UpdateMSRankListRes &roRes, const CUserData &roUserData);

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
	UpdateMSRankListArg m_oArg;
	UpdateMSRankListRes m_oRes;
};

#endif
