#ifndef __RPCC2M_CLEARPRIVATECHATLIST_H__
#define __RPCC2M_CLEARPRIVATECHATLIST_H__

// generate by ProtoGen at date: 2016/8/26 10:05:34

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CLEARPRIVATECHATLIST_TYPE 27304

class RpcC2M_ClearPrivateChatList : public CRpc
{
private:

	RpcC2M_ClearPrivateChatList():CRpc(RPCC2M_CLEARPRIVATECHATLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ClearPrivateChatList *CreateRpc()
	{
		return new RpcC2M_ClearPrivateChatList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ClearPrivateChatListArg &roArg, const ClearPrivateChatListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ClearPrivateChatListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ClearPrivateChatListArg &roArg, ClearPrivateChatListRes &roRes);
	void OnDelayReplyRpc(const ClearPrivateChatListArg &roArg, ClearPrivateChatListRes &roRes, const CUserData &roUserData);

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
	ClearPrivateChatListArg m_oArg;
	ClearPrivateChatListRes m_oRes;
};

#endif
