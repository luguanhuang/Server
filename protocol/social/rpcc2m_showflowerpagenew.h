#ifndef __RPCC2M_SHOWFLOWERPAGENEW_H__
#define __RPCC2M_SHOWFLOWERPAGENEW_H__

// generate by ProtoGen at date: 2016/8/18 11:11:26

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_SHOWFLOWERPAGENEW_TYPE 49446

class RpcC2M_ShowFlowerPageNew : public CRpc
{
private:

	RpcC2M_ShowFlowerPageNew():CRpc(RPCC2M_SHOWFLOWERPAGENEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ShowFlowerPageNew *CreateRpc()
	{
		return new RpcC2M_ShowFlowerPageNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ShowFlowerPageArg &roArg, const ShowFlowerPageRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ShowFlowerPageArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ShowFlowerPageArg &roArg, ShowFlowerPageRes &roRes);
	void OnDelayReplyRpc(const ShowFlowerPageArg &roArg, ShowFlowerPageRes &roRes, const CUserData &roUserData);

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
	ShowFlowerPageArg m_oArg;
	ShowFlowerPageRes m_oRes;
};

#endif
