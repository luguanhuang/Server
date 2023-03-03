#ifndef __RPCC2M_STARTWEDDINGCAR_H__
#define __RPCC2M_STARTWEDDINGCAR_H__

// generate by ProtoGen at date: 2017/8/1 0:23:38

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_STARTWEDDINGCAR_TYPE 26388

class RpcC2M_StartWeddingCar : public CRpc
{
private:

	RpcC2M_StartWeddingCar():CRpc(RPCC2M_STARTWEDDINGCAR_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_StartWeddingCar *CreateRpc()
	{
		return new RpcC2M_StartWeddingCar;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const StartWeddingCarArg &roArg, const StartWeddingCarRes &roRes, const CUserData &roUserData);
	void OnTimeout(const StartWeddingCarArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const StartWeddingCarArg &roArg, StartWeddingCarRes &roRes);
	void OnDelayReplyRpc(const StartWeddingCarArg &roArg, StartWeddingCarRes &roRes, const CUserData &roUserData);

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
	StartWeddingCarArg m_oArg;
	StartWeddingCarRes m_oRes;
};

#endif
