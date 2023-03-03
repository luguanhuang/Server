#ifndef __RPCG2M_UPPAYFRIENDITEM_H__
#define __RPCG2M_UPPAYFRIENDITEM_H__

// generate by ProtoGen at date: 2017/3/20 16:42:55

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_UPPAYFRIENDITEM_TYPE 62866

class RpcG2M_UpPayFriendItem : public CRpc
{
private:

	RpcG2M_UpPayFriendItem():CRpc(RPCG2M_UPPAYFRIENDITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_UpPayFriendItem *CreateRpc()
	{
		return new RpcG2M_UpPayFriendItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UpPayFriendItemArg &roArg, const UpPayFriendItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UpPayFriendItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UpPayFriendItemArg &roArg, UpPayFriendItemRes &roRes);
	void OnDelayReplyRpc(const UpPayFriendItemArg &roArg, UpPayFriendItemRes &roRes, const CUserData &roUserData);

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
	UpPayFriendItemArg m_oArg;
	UpPayFriendItemRes m_oRes;
};

#endif
