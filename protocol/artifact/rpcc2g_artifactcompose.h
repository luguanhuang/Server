#ifndef __RPCC2G_ARTIFACTCOMPOSE_H__
#define __RPCC2G_ARTIFACTCOMPOSE_H__

// generate by ProtoGen at date: 2017/5/6 11:06:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ARTIFACTCOMPOSE_TYPE 599

class RpcC2G_ArtifactCompose : public CRpc
{
private:

	RpcC2G_ArtifactCompose():CRpc(RPCC2G_ARTIFACTCOMPOSE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ArtifactCompose *CreateRpc()
	{
		return new RpcC2G_ArtifactCompose;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ArtifactComposeArg &roArg, const ArtifactComposeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ArtifactComposeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ArtifactComposeArg &roArg, ArtifactComposeRes &roRes);
	void OnDelayReplyRpc(const ArtifactComposeArg &roArg, ArtifactComposeRes &roRes, const CUserData &roUserData);

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
	ArtifactComposeArg m_oArg;
	ArtifactComposeRes m_oRes;
};

#endif
