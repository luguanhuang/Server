#ifndef __RPCC2G_SHOWFLOWERPAGE_H__
#define __RPCC2G_SHOWFLOWERPAGE_H__

// generate by ProtoGen at date: 2015/9/23 16:11:28

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SHOWFLOWERPAGE_TYPE 47831

class RpcC2G_ShowFlowerPage : public CRpc
{
private:

	RpcC2G_ShowFlowerPage():CRpc(RPCC2G_SHOWFLOWERPAGE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ShowFlowerPage *CreateRpc()
	{
		return new RpcC2G_ShowFlowerPage;
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
