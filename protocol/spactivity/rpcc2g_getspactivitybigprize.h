#ifndef __RPCC2G_GETSPACTIVITYBIGPRIZE_H__
#define __RPCC2G_GETSPACTIVITYBIGPRIZE_H__

// generate by ProtoGen at date: 2016/10/24 10:23:05

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETSPACTIVITYBIGPRIZE_TYPE 17229

class RpcC2G_GetSpActivityBigPrize : public CRpc
{
private:

	RpcC2G_GetSpActivityBigPrize():CRpc(RPCC2G_GETSPACTIVITYBIGPRIZE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetSpActivityBigPrize *CreateRpc()
	{
		return new RpcC2G_GetSpActivityBigPrize;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetSpActivityBigPrizeArg &roArg, const GetSpActivityBigPrizeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetSpActivityBigPrizeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetSpActivityBigPrizeArg &roArg, GetSpActivityBigPrizeRes &roRes);
	void OnDelayReplyRpc(const GetSpActivityBigPrizeArg &roArg, GetSpActivityBigPrizeRes &roRes, const CUserData &roUserData);

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
	GetSpActivityBigPrizeArg m_oArg;
	GetSpActivityBigPrizeRes m_oRes;
};

#endif
