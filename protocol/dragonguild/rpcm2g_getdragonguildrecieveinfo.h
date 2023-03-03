#ifndef __RPCM2G_GETDRAGONGUILDRECIEVEINFO_H__
#define __RPCM2G_GETDRAGONGUILDRECIEVEINFO_H__

// generate by ProtoGen at date: 2017/9/12 21:36:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GETDRAGONGUILDRECIEVEINFO_TYPE 19993

class RpcM2G_GetDragonGuildRecieveInfo : public CRpc
{
private:

	RpcM2G_GetDragonGuildRecieveInfo():CRpc(RPCM2G_GETDRAGONGUILDRECIEVEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GetDragonGuildRecieveInfo *CreateRpc()
	{
		return new RpcM2G_GetDragonGuildRecieveInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDragonGuildRecieveInfoArg &roArg, const GetDragonGuildRecieveInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDragonGuildRecieveInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDragonGuildRecieveInfoArg &roArg, GetDragonGuildRecieveInfoRes &roRes);
	void OnDelayReplyRpc(const GetDragonGuildRecieveInfoArg &roArg, GetDragonGuildRecieveInfoRes &roRes, const CUserData &roUserData);

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
	GetDragonGuildRecieveInfoArg m_oArg;
	GetDragonGuildRecieveInfoRes m_oRes;
};

#endif
