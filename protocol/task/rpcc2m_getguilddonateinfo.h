#ifndef __RPCC2M_GETGUILDDONATEINFO_H__
#define __RPCC2M_GETGUILDDONATEINFO_H__

// generate by ProtoGen at date: 2016/11/16 19:30:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETGUILDDONATEINFO_TYPE 14656

class RpcC2M_GetGuildDonateInfo : public CRpc
{
private:

	RpcC2M_GetGuildDonateInfo():CRpc(RPCC2M_GETGUILDDONATEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetGuildDonateInfo *CreateRpc()
	{
		return new RpcC2M_GetGuildDonateInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildDonateInfoArg &roArg, const GetGuildDonateInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildDonateInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildDonateInfoArg &roArg, GetGuildDonateInfoRes &roRes);
	void OnDelayReplyRpc(const GetGuildDonateInfoArg &roArg, GetGuildDonateInfoRes &roRes, const CUserData &roUserData);

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
	GetGuildDonateInfoArg m_oArg;
	GetGuildDonateInfoRes m_oRes;
};

#endif
