#ifndef __RPCC2G_GETLEVELSEALINFO_H__
#define __RPCC2G_GETLEVELSEALINFO_H__

// generate by ProtoGen at date: 2016/6/16 14:26:10

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETLEVELSEALINFO_TYPE 10497

class RpcC2G_GetLevelSealInfo : public CRpc
{
private:

	RpcC2G_GetLevelSealInfo():CRpc(RPCC2G_GETLEVELSEALINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetLevelSealInfo *CreateRpc()
	{
		return new RpcC2G_GetLevelSealInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetLevelSealInfoArg &roArg, const GetLevelSealInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetLevelSealInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetLevelSealInfoArg &roArg, GetLevelSealInfoRes &roRes);
	void OnDelayReplyRpc(const GetLevelSealInfoArg &roArg, GetLevelSealInfoRes &roRes, const CUserData &roUserData);

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
	GetLevelSealInfoArg m_oArg;
	GetLevelSealInfoRes m_oRes;
};

#endif
