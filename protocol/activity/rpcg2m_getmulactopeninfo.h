#ifndef __RPCG2M_GETMULACTOPENINFO_H__
#define __RPCG2M_GETMULACTOPENINFO_H__

// generate by ProtoGen at date: 2017/2/10 21:44:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GETMULACTOPENINFO_TYPE 46515

class RpcG2M_GetMulActOpenInfo : public CRpc
{
private:

	RpcG2M_GetMulActOpenInfo():CRpc(RPCG2M_GETMULACTOPENINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GetMulActOpenInfo *CreateRpc()
	{
		return new RpcG2M_GetMulActOpenInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetMulActOpenInfoArg &roArg, const GetMulActOpenInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetMulActOpenInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetMulActOpenInfoArg &roArg, GetMulActOpenInfoRes &roRes);
	void OnDelayReplyRpc(const GetMulActOpenInfoArg &roArg, GetMulActOpenInfoRes &roRes, const CUserData &roUserData);

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
	GetMulActOpenInfoArg m_oArg;
	GetMulActOpenInfoRes m_oRes;
};

#endif
