#ifndef __RPCC2G_SYNPETINFO_H__
#define __RPCC2G_SYNPETINFO_H__

// generate by ProtoGen at date: 2016/8/16 18:11:22

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SYNPETINFO_TYPE 6548

class RpcC2G_SynPetInfo : public CRpc
{
private:

	RpcC2G_SynPetInfo():CRpc(RPCC2G_SYNPETINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SynPetInfo *CreateRpc()
	{
		return new RpcC2G_SynPetInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SynPetInfoArg &roArg, const SynPetInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SynPetInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SynPetInfoArg &roArg, SynPetInfoRes &roRes);
	void OnDelayReplyRpc(const SynPetInfoArg &roArg, SynPetInfoRes &roRes, const CUserData &roUserData);

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
	SynPetInfoArg m_oArg;
	SynPetInfoRes m_oRes;
};

#endif
