#ifndef __RPCC2M_DRAGONGUILDBINDGROUPREQ_H__
#define __RPCC2M_DRAGONGUILDBINDGROUPREQ_H__

// generate by ProtoGen at date: 2017/9/13 20:17:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_DRAGONGUILDBINDGROUPREQ_TYPE 34774

class RpcC2M_DragonGuildBindGroupReq : public CRpc
{
private:

	RpcC2M_DragonGuildBindGroupReq():CRpc(RPCC2M_DRAGONGUILDBINDGROUPREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_DragonGuildBindGroupReq *CreateRpc()
	{
		return new RpcC2M_DragonGuildBindGroupReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DragonGuildBindReq &roArg, const DragonGuildBindRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DragonGuildBindReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DragonGuildBindReq &roArg, DragonGuildBindRes &roRes);
	void OnDelayReplyRpc(const DragonGuildBindReq &roArg, DragonGuildBindRes &roRes, const CUserData &roUserData);

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
	DragonGuildBindReq m_oArg;
	DragonGuildBindRes m_oRes;
};

#endif
