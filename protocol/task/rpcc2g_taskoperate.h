#ifndef __RPCC2G_TASKOPERATE_H__
#define __RPCC2G_TASKOPERATE_H__

// generate by ProtoGen at date: 2016/12/27 22:19:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_TASKOPERATE_TYPE 20029

class RpcC2G_TaskOperate : public CRpc
{
private:

	RpcC2G_TaskOperate():CRpc(RPCC2G_TASKOPERATE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_TaskOperate *CreateRpc()
	{
		return new RpcC2G_TaskOperate;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TaskOPArg &roArg, const TaskOPRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TaskOPArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TaskOPArg &roArg, TaskOPRes &roRes);
	void OnDelayReplyRpc(const TaskOPArg &roArg, TaskOPRes &roRes, const CUserData &roUserData);

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
	TaskOPArg m_oArg;
	TaskOPRes m_oRes;
};

#endif
