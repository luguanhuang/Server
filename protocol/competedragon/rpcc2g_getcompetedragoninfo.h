#ifndef __RPCC2G_GETCOMPETEDRAGONINFO_H__
#define __RPCC2G_GETCOMPETEDRAGONINFO_H__

// generate by ProtoGen at date: 2017/9/7 11:20:31

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETCOMPETEDRAGONINFO_TYPE 65362

class RpcC2G_GetCompeteDragonInfo : public CRpc
{
private:

	RpcC2G_GetCompeteDragonInfo():CRpc(RPCC2G_GETCOMPETEDRAGONINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetCompeteDragonInfo *CreateRpc()
	{
		return new RpcC2G_GetCompeteDragonInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetCompeteDragonInfoArg &roArg, const GetCompeteDragonInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetCompeteDragonInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetCompeteDragonInfoArg &roArg, GetCompeteDragonInfoRes &roRes);
	void OnDelayReplyRpc(const GetCompeteDragonInfoArg &roArg, GetCompeteDragonInfoRes &roRes, const CUserData &roUserData);

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
	GetCompeteDragonInfoArg m_oArg;
	GetCompeteDragonInfoRes m_oRes;
};

#endif
