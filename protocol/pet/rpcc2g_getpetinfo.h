#ifndef __RPCC2G_GETPETINFO_H__
#define __RPCC2G_GETPETINFO_H__

// generate by ProtoGen at date: 2016/9/14 17:17:55

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETPETINFO_TYPE 63259

class RpcC2G_GetPetInfo : public CRpc
{
private:

	RpcC2G_GetPetInfo():CRpc(RPCC2G_GETPETINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetPetInfo *CreateRpc()
	{
		return new RpcC2G_GetPetInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPetInfoArg &roArg, const GetPetInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPetInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPetInfoArg &roArg, GetPetInfoRes &roRes);
	void OnDelayReplyRpc(const GetPetInfoArg &roArg, GetPetInfoRes &roRes, const CUserData &roUserData);

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
	GetPetInfoArg m_oArg;
	GetPetInfoRes m_oRes;
};

#endif
