#ifndef __RPCC2M_GETUNITAPPEARANCENEW_H__
#define __RPCC2M_GETUNITAPPEARANCENEW_H__

// generate by ProtoGen at date: 2016/12/25 18:49:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETUNITAPPEARANCENEW_TYPE 40593

class RpcC2M_GetUnitAppearanceNew : public CRpc
{
private:

	RpcC2M_GetUnitAppearanceNew():CRpc(RPCC2M_GETUNITAPPEARANCENEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetUnitAppearanceNew *CreateRpc()
	{
		return new RpcC2M_GetUnitAppearanceNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetUnitAppearanceArg &roArg, const GetUnitAppearanceRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetUnitAppearanceArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetUnitAppearanceArg &roArg, GetUnitAppearanceRes &roRes);
	void OnDelayReplyRpc(const GetUnitAppearanceArg &roArg, GetUnitAppearanceRes &roRes, const CUserData &roUserData);

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
	GetUnitAppearanceArg m_oArg;
	GetUnitAppearanceRes m_oRes;
};

#endif
