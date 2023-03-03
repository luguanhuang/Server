#ifndef __RPCC2G_GETLUCKYACTIVITYINFO_H__
#define __RPCC2G_GETLUCKYACTIVITYINFO_H__

// generate by ProtoGen at date: 2017/9/12 11:13:18

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETLUCKYACTIVITYINFO_TYPE 384

class RpcC2G_GetLuckyActivityInfo : public CRpc
{
private:

	RpcC2G_GetLuckyActivityInfo():CRpc(RPCC2G_GETLUCKYACTIVITYINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetLuckyActivityInfo *CreateRpc()
	{
		return new RpcC2G_GetLuckyActivityInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetLuckyActivityInfoArg &roArg, const GetLuckyActivityInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetLuckyActivityInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetLuckyActivityInfoArg &roArg, GetLuckyActivityInfoRes &roRes);
	void OnDelayReplyRpc(const GetLuckyActivityInfoArg &roArg, GetLuckyActivityInfoRes &roRes, const CUserData &roUserData);

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
	GetLuckyActivityInfoArg m_oArg;
	GetLuckyActivityInfoRes m_oRes;
};

#endif
