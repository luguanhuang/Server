#ifndef __RPCC2G_GETTOWERACTIVITYTOP_H__
#define __RPCC2G_GETTOWERACTIVITYTOP_H__

// generate by ProtoGen at date: 2016/5/25 16:40:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETTOWERACTIVITYTOP_TYPE 5168

class RpcC2G_GetTowerActivityTop : public CRpc
{
private:

	RpcC2G_GetTowerActivityTop():CRpc(RPCC2G_GETTOWERACTIVITYTOP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetTowerActivityTop *CreateRpc()
	{
		return new RpcC2G_GetTowerActivityTop;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetTowerActivityTopArg &roArg, const GetTowerActivityTopRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetTowerActivityTopArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetTowerActivityTopArg &roArg, GetTowerActivityTopRes &roRes);
	void OnDelayReplyRpc(const GetTowerActivityTopArg &roArg, GetTowerActivityTopRes &roRes, const CUserData &roUserData);

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
	GetTowerActivityTopArg m_oArg;
	GetTowerActivityTopRes m_oRes;
};

#endif
