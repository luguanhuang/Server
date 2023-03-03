#ifndef __RPCM2G_QUERYDONATEINFO_H__
#define __RPCM2G_QUERYDONATEINFO_H__

// generate by ProtoGen at date: 2016/11/19 23:28:55

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_QUERYDONATEINFO_TYPE 56758

class RpcM2G_QueryDonateInfo : public CRpc
{
private:

	RpcM2G_QueryDonateInfo():CRpc(RPCM2G_QUERYDONATEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_QueryDonateInfo *CreateRpc()
	{
		return new RpcM2G_QueryDonateInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryDonateInfoArg &roArg, const QueryDonateInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryDonateInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryDonateInfoArg &roArg, QueryDonateInfoRes &roRes);
	void OnDelayReplyRpc(const QueryDonateInfoArg &roArg, QueryDonateInfoRes &roRes, const CUserData &roUserData);

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
	QueryDonateInfoArg m_oArg;
	QueryDonateInfoRes m_oRes;
};

#endif
