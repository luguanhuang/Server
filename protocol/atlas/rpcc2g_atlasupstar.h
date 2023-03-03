#ifndef __RPCC2G_ATLASUPSTAR_H__
#define __RPCC2G_ATLASUPSTAR_H__

// generate by ProtoGen at date: 2017/1/7 19:45:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ATLASUPSTAR_TYPE 41051

class RpcC2G_AtlasUpStar : public CRpc
{
private:

	RpcC2G_AtlasUpStar():CRpc(RPCC2G_ATLASUPSTAR_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_AtlasUpStar *CreateRpc()
	{
		return new RpcC2G_AtlasUpStar;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AtlasUpStarArg &roArg, const AtlasUpStarRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AtlasUpStarArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AtlasUpStarArg &roArg, AtlasUpStarRes &roRes);
	void OnDelayReplyRpc(const AtlasUpStarArg &roArg, AtlasUpStarRes &roRes, const CUserData &roUserData);

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
	AtlasUpStarArg m_oArg;
	AtlasUpStarRes m_oRes;
};

#endif
