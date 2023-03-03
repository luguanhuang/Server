#ifndef __RPCC2G_ARTIFACTDEITYSTOVEOP_H__
#define __RPCC2G_ARTIFACTDEITYSTOVEOP_H__

// generate by ProtoGen at date: 2017/7/31 21:24:02

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ARTIFACTDEITYSTOVEOP_TYPE 35155

class RpcC2G_ArtifactDeityStoveOp : public CRpc
{
private:

	RpcC2G_ArtifactDeityStoveOp():CRpc(RPCC2G_ARTIFACTDEITYSTOVEOP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ArtifactDeityStoveOp *CreateRpc()
	{
		return new RpcC2G_ArtifactDeityStoveOp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ArtifactDeityStoveOpArg &roArg, const ArtifactDeityStoveOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ArtifactDeityStoveOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ArtifactDeityStoveOpArg &roArg, ArtifactDeityStoveOpRes &roRes);
	void OnDelayReplyRpc(const ArtifactDeityStoveOpArg &roArg, ArtifactDeityStoveOpRes &roRes, const CUserData &roUserData);

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
	ArtifactDeityStoveOpArg m_oArg;
	ArtifactDeityStoveOpRes m_oRes;
};

#endif
