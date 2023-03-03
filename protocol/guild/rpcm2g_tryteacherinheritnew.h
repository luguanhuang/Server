#ifndef __RPCM2G_TRYTEACHERINHERITNEW_H__
#define __RPCM2G_TRYTEACHERINHERITNEW_H__

// generate by ProtoGen at date: 2017/6/14 18:38:10

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_TRYTEACHERINHERITNEW_TYPE 58897

class RpcM2G_TryTeacherInheritNew : public CRpc
{
private:

	RpcM2G_TryTeacherInheritNew():CRpc(RPCM2G_TRYTEACHERINHERITNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_TryTeacherInheritNew *CreateRpc()
	{
		return new RpcM2G_TryTeacherInheritNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TryTeacherInheritNewArg &roArg, const TryTeacherInheritNewRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TryTeacherInheritNewArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TryTeacherInheritNewArg &roArg, TryTeacherInheritNewRes &roRes);
	void OnDelayReplyRpc(const TryTeacherInheritNewArg &roArg, TryTeacherInheritNewRes &roRes, const CUserData &roUserData);

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
	TryTeacherInheritNewArg m_oArg;
	TryTeacherInheritNewRes m_oRes;
};

#endif
