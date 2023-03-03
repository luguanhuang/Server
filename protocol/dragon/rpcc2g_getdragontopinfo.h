#ifndef __RPCC2G_GETDRAGONTOPINFO_H__
#define __RPCC2G_GETDRAGONTOPINFO_H__

// generate by ProtoGen at date: 2016/7/4 17:55:37

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETDRAGONTOPINFO_TYPE 7973

class RpcC2G_GetDragonTopInfo : public CRpc
{
private:

	RpcC2G_GetDragonTopInfo():CRpc(RPCC2G_GETDRAGONTOPINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetDragonTopInfo *CreateRpc()
	{
		return new RpcC2G_GetDragonTopInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDragonTopInfoArg &roArg, const GetDragonTopInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDragonTopInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDragonTopInfoArg &roArg, GetDragonTopInfoRes &roRes);
	void OnDelayReplyRpc(const GetDragonTopInfoArg &roArg, GetDragonTopInfoRes &roRes, const CUserData &roUserData);

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
	GetDragonTopInfoArg m_oArg;
	GetDragonTopInfoRes m_oRes;
};

#endif
