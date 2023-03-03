#ifndef __RPCC2M_GETMYAPPLYMASTERINFO_H__
#define __RPCC2M_GETMYAPPLYMASTERINFO_H__

// generate by ProtoGen at date: 2016/12/9 21:21:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETMYAPPLYMASTERINFO_TYPE 61902

class RpcC2M_GetMyApplyMasterInfo : public CRpc
{
private:

	RpcC2M_GetMyApplyMasterInfo():CRpc(RPCC2M_GETMYAPPLYMASTERINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetMyApplyMasterInfo *CreateRpc()
	{
		return new RpcC2M_GetMyApplyMasterInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetMyApplyMasterInfoArg &roArg, const GetMyApplyMasterInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetMyApplyMasterInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetMyApplyMasterInfoArg &roArg, GetMyApplyMasterInfoRes &roRes);
	void OnDelayReplyRpc(const GetMyApplyMasterInfoArg &roArg, GetMyApplyMasterInfoRes &roRes, const CUserData &roUserData);

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
	GetMyApplyMasterInfoArg m_oArg;
	GetMyApplyMasterInfoRes m_oRes;
};

#endif
