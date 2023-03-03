#ifndef __RPCC2G_GETRISKMAPINFOS_H__
#define __RPCC2G_GETRISKMAPINFOS_H__

// generate by ProtoGen at date: 2016/9/5 16:04:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETRISKMAPINFOS_TYPE 11628

class RpcC2G_GetRiskMapInfos : public CRpc
{
private:

	RpcC2G_GetRiskMapInfos():CRpc(RPCC2G_GETRISKMAPINFOS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetRiskMapInfos *CreateRpc()
	{
		return new RpcC2G_GetRiskMapInfos;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetRiskMapInfosArg &roArg, const GetRiskMapInfosRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetRiskMapInfosArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetRiskMapInfosArg &roArg, GetRiskMapInfosRes &roRes);
	void OnDelayReplyRpc(const GetRiskMapInfosArg &roArg, GetRiskMapInfosRes &roRes, const CUserData &roUserData);

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
	GetRiskMapInfosArg m_oArg;
	GetRiskMapInfosRes m_oRes;
};

#endif
