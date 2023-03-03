﻿#ifndef __RPCC2G_ACTIVATATLAS_H__
#define __RPCC2G_ACTIVATATLAS_H__

// generate by ProtoGen at date: 2016/9/1 16:42:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ACTIVATATLAS_TYPE 15919

class RpcC2G_ActivatAtlas : public CRpc
{
private:

	RpcC2G_ActivatAtlas():CRpc(RPCC2G_ACTIVATATLAS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ActivatAtlas *CreateRpc()
	{
		return new RpcC2G_ActivatAtlas;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ActivatAtlasArg &roArg, const ActivatAtlasRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ActivatAtlasArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ActivatAtlasArg &roArg, ActivatAtlasRes &roRes);
	void OnDelayReplyRpc(const ActivatAtlasArg &roArg, ActivatAtlasRes &roRes, const CUserData &roUserData);

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
	ActivatAtlasArg m_oArg;
	ActivatAtlasRes m_oRes;
};

#endif
