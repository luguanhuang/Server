#ifndef __RPCC2M_GETALLWEDDINGINFO_H__
#define __RPCC2M_GETALLWEDDINGINFO_H__

// generate by ProtoGen at date: 2017/7/17 17:58:46

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETALLWEDDINGINFO_TYPE 30155

class RpcC2M_GetAllWeddingInfo : public CRpc
{
private:

	RpcC2M_GetAllWeddingInfo():CRpc(RPCC2M_GETALLWEDDINGINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetAllWeddingInfo *CreateRpc()
	{
		return new RpcC2M_GetAllWeddingInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetAllWeddingInfoArg &roArg, const GetAllWeddingInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetAllWeddingInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetAllWeddingInfoArg &roArg, GetAllWeddingInfoRes &roRes);
	void OnDelayReplyRpc(const GetAllWeddingInfoArg &roArg, GetAllWeddingInfoRes &roRes, const CUserData &roUserData);

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
	GetAllWeddingInfoArg m_oArg;
	GetAllWeddingInfoRes m_oRes;
};

#endif
