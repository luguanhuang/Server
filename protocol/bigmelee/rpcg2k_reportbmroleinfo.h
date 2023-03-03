#ifndef __RPCG2K_REPORTBMROLEINFO_H__
#define __RPCG2K_REPORTBMROLEINFO_H__

// generate by ProtoGen at date: 2017/6/22 8:19:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2K_REPORTBMROLEINFO_TYPE 5924

class RpcG2K_ReportBMRoleInfo : public CRpc
{
private:

	RpcG2K_ReportBMRoleInfo():CRpc(RPCG2K_REPORTBMROLEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2K_ReportBMRoleInfo *CreateRpc()
	{
		return new RpcG2K_ReportBMRoleInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BMSceneFightInfo &roArg, const BMSceneRankRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BMSceneFightInfo &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BMSceneFightInfo &roArg, BMSceneRankRes &roRes);
	void OnDelayReplyRpc(const BMSceneFightInfo &roArg, BMSceneRankRes &roRes, const CUserData &roUserData);

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
	BMSceneFightInfo m_oArg;
	BMSceneRankRes m_oRes;
};

#endif
