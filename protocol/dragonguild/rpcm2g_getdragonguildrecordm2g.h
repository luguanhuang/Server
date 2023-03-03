﻿#ifndef __RPCM2G_GETDRAGONGUILDRECORDM2G_H__
#define __RPCM2G_GETDRAGONGUILDRECORDM2G_H__

// generate by ProtoGen at date: 2017/9/6 10:33:09

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GETDRAGONGUILDRECORDM2G_TYPE 56765

class RpcM2G_GetDragonGuildRecordM2G : public CRpc
{
private:

	RpcM2G_GetDragonGuildRecordM2G():CRpc(RPCM2G_GETDRAGONGUILDRECORDM2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GetDragonGuildRecordM2G *CreateRpc()
	{
		return new RpcM2G_GetDragonGuildRecordM2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPartnerRecordM2GArg &roArg, const GetPartnerRecordM2GRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPartnerRecordM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPartnerRecordM2GArg &roArg, GetPartnerRecordM2GRes &roRes);
	void OnDelayReplyRpc(const GetPartnerRecordM2GArg &roArg, GetPartnerRecordM2GRes &roRes, const CUserData &roUserData);

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
	GetPartnerRecordM2GArg m_oArg;
	GetPartnerRecordM2GRes m_oRes;
};

#endif
