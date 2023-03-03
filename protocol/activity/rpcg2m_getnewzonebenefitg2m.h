#ifndef __RPCG2M_GETNEWZONEBENEFITG2M_H__
#define __RPCG2M_GETNEWZONEBENEFITG2M_H__

// generate by ProtoGen at date: 2017/10/24 14:33:28

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GETNEWZONEBENEFITG2M_TYPE 24760

class RpcG2M_GetNewZoneBenefitG2M : public CRpc
{
private:

	RpcG2M_GetNewZoneBenefitG2M():CRpc(RPCG2M_GETNEWZONEBENEFITG2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GetNewZoneBenefitG2M *CreateRpc()
	{
		return new RpcG2M_GetNewZoneBenefitG2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetNewZoneBenefitG2MArg &roArg, const GetNewZoneBenefitG2MRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetNewZoneBenefitG2MArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetNewZoneBenefitG2MArg &roArg, GetNewZoneBenefitG2MRes &roRes);
	void OnDelayReplyRpc(const GetNewZoneBenefitG2MArg &roArg, GetNewZoneBenefitG2MRes &roRes, const CUserData &roUserData);

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
	GetNewZoneBenefitG2MArg m_oArg;
	GetNewZoneBenefitG2MRes m_oRes;
};

#endif
