#ifndef __RPCC2G_LEVELSEALBUTTONSTATUS_H__
#define __RPCC2G_LEVELSEALBUTTONSTATUS_H__

// generate by ProtoGen at date: 2016/6/16 14:26:03

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_LEVELSEALBUTTONSTATUS_TYPE 10396

class RpcC2G_LevelSealButtonStatus : public CRpc
{
private:

	RpcC2G_LevelSealButtonStatus():CRpc(RPCC2G_LEVELSEALBUTTONSTATUS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_LevelSealButtonStatus *CreateRpc()
	{
		return new RpcC2G_LevelSealButtonStatus;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LevelSealOverExpArg &roArg, const LevelSealOverExpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LevelSealOverExpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LevelSealOverExpArg &roArg, LevelSealOverExpRes &roRes);
	void OnDelayReplyRpc(const LevelSealOverExpArg &roArg, LevelSealOverExpRes &roRes, const CUserData &roUserData);

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
	LevelSealOverExpArg m_oArg;
	LevelSealOverExpRes m_oRes;
};

#endif
