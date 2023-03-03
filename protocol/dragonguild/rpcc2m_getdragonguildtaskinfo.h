#ifndef __RPCC2M_GETDRAGONGUILDTASKINFO_H__
#define __RPCC2M_GETDRAGONGUILDTASKINFO_H__

// generate by ProtoGen at date: 2017/9/11 11:00:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETDRAGONGUILDTASKINFO_TYPE 36879

class RpcC2M_GetDragonGuildTaskInfo : public CRpc
{
private:

	RpcC2M_GetDragonGuildTaskInfo():CRpc(RPCC2M_GETDRAGONGUILDTASKINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetDragonGuildTaskInfo *CreateRpc()
	{
		return new RpcC2M_GetDragonGuildTaskInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDragonGuildTaskInfoArg &roArg, const GetDragonGuildTaskInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDragonGuildTaskInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDragonGuildTaskInfoArg &roArg, GetDragonGuildTaskInfoRes &roRes);
	void OnDelayReplyRpc(const GetDragonGuildTaskInfoArg &roArg, GetDragonGuildTaskInfoRes &roRes, const CUserData &roUserData);

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
	GetDragonGuildTaskInfoArg m_oArg;
	GetDragonGuildTaskInfoRes m_oRes;
};

#endif
