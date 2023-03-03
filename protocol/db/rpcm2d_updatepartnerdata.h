#ifndef __RPCM2D_UPDATEPARTNERDATA_H__
#define __RPCM2D_UPDATEPARTNERDATA_H__

// generate by ProtoGen at date: 2016/12/9 23:42:36

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_UPDATEPARTNERDATA_TYPE 6189

class RpcM2D_UpdatePartnerData : public CRpc
{
private:

	RpcM2D_UpdatePartnerData():CRpc(RPCM2D_UPDATEPARTNERDATA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_UpdatePartnerData *CreateRpc()
	{
		return new RpcM2D_UpdatePartnerData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UpdatePartnerDataArg &roArg, const UpdatePartnerDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UpdatePartnerDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UpdatePartnerDataArg &roArg, UpdatePartnerDataRes &roRes);
	void OnDelayReplyRpc(const UpdatePartnerDataArg &roArg, UpdatePartnerDataRes &roRes, const CUserData &roUserData);

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
	UpdatePartnerDataArg m_oArg;
	UpdatePartnerDataRes m_oRes;
};

#endif
