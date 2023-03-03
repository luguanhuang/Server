#ifndef __RPCM2D_UPDATEROLETASKDATA_H__
#define __RPCM2D_UPDATEROLETASKDATA_H__

// generate by ProtoGen at date: 2017/9/20 7:41:32

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_UPDATEROLETASKDATA_TYPE 1572

class RpcM2D_UpdateRoleTaskData : public CRpc
{
private:

	RpcM2D_UpdateRoleTaskData():CRpc(RPCM2D_UPDATEROLETASKDATA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_UpdateRoleTaskData *CreateRpc()
	{
		return new RpcM2D_UpdateRoleTaskData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UpdateRoleTaskDataArg &roArg, const UpdateRoleTaskDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UpdateRoleTaskDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UpdateRoleTaskDataArg &roArg, UpdateRoleTaskDataRes &roRes);
	void OnDelayReplyRpc(const UpdateRoleTaskDataArg &roArg, UpdateRoleTaskDataRes &roRes, const CUserData &roUserData);

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
	UpdateRoleTaskDataArg m_oArg;
	UpdateRoleTaskDataRes m_oRes;
};

#endif
