#ifndef __RPCC2G_OPENSCENECHEST_H__
#define __RPCC2G_OPENSCENECHEST_H__

// generate by ProtoGen at date: 2015/10/9 15:23:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_OPENSCENECHEST_TYPE 27401

class RpcC2G_OpenSceneChest : public CRpc
{
private:

	RpcC2G_OpenSceneChest():CRpc(RPCC2G_OPENSCENECHEST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_OpenSceneChest *CreateRpc()
	{
		return new RpcC2G_OpenSceneChest;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const OpenSceneChestArg &roArg, const OpenSceneChestRes &roRes, const CUserData &roUserData);
	void OnTimeout(const OpenSceneChestArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const OpenSceneChestArg &roArg, OpenSceneChestRes &roRes);
	void OnDelayReplyRpc(const OpenSceneChestArg &roArg, OpenSceneChestRes &roRes, const CUserData &roUserData);

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
	OpenSceneChestArg m_oArg;
	OpenSceneChestRes m_oRes;
};

#endif
