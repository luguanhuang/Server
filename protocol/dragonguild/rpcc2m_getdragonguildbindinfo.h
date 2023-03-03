#ifndef __RPCC2M_GETDRAGONGUILDBINDINFO_H__
#define __RPCC2M_GETDRAGONGUILDBINDINFO_H__

// generate by ProtoGen at date: 2017/9/13 20:14:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETDRAGONGUILDBINDINFO_TYPE 39788

class RpcC2M_GetDragonGuildBindInfo : public CRpc
{
private:

	RpcC2M_GetDragonGuildBindInfo():CRpc(RPCC2M_GETDRAGONGUILDBINDINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetDragonGuildBindInfo *CreateRpc()
	{
		return new RpcC2M_GetDragonGuildBindInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDragonGuildBindInfoArg &roArg, const GetDragonGuildBindInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDragonGuildBindInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDragonGuildBindInfoArg &roArg, GetDragonGuildBindInfoRes &roRes);
	void OnDelayReplyRpc(const GetDragonGuildBindInfoArg &roArg, GetDragonGuildBindInfoRes &roRes, const CUserData &roUserData);

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
	GetDragonGuildBindInfoArg m_oArg;
	GetDragonGuildBindInfoRes m_oRes;
};

#endif
