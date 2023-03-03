#ifndef __RPCC2G_FIRSTPASSGETTOPROLEINFO_H__
#define __RPCC2G_FIRSTPASSGETTOPROLEINFO_H__

// generate by ProtoGen at date: 2016/8/24 20:58:24

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_FIRSTPASSGETTOPROLEINFO_TYPE 37076

class RpcC2G_FirstPassGetTopRoleInfo : public CRpc
{
private:

	RpcC2G_FirstPassGetTopRoleInfo():CRpc(RPCC2G_FIRSTPASSGETTOPROLEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_FirstPassGetTopRoleInfo *CreateRpc()
	{
		return new RpcC2G_FirstPassGetTopRoleInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FirstPassGetTopRoleInfoArg &roArg, const FirstPassGetTopRoleInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FirstPassGetTopRoleInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FirstPassGetTopRoleInfoArg &roArg, FirstPassGetTopRoleInfoRes &roRes);
	void OnDelayReplyRpc(const FirstPassGetTopRoleInfoArg &roArg, FirstPassGetTopRoleInfoRes &roRes, const CUserData &roUserData);

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
	FirstPassGetTopRoleInfoArg m_oArg;
	FirstPassGetTopRoleInfoRes m_oRes;
};

#endif
