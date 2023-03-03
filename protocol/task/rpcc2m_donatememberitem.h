#ifndef __RPCC2M_DONATEMEMBERITEM_H__
#define __RPCC2M_DONATEMEMBERITEM_H__

// generate by ProtoGen at date: 2016/11/19 23:28:13

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_DONATEMEMBERITEM_TYPE 4241

class RpcC2M_DonateMemberItem : public CRpc
{
private:

	RpcC2M_DonateMemberItem():CRpc(RPCC2M_DONATEMEMBERITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_DonateMemberItem *CreateRpc()
	{
		return new RpcC2M_DonateMemberItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DonateMemberItemArg &roArg, const DonateMemberItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DonateMemberItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DonateMemberItemArg &roArg, DonateMemberItemRes &roRes);
	void OnDelayReplyRpc(const DonateMemberItemArg &roArg, DonateMemberItemRes &roRes, const CUserData &roUserData);

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
	DonateMemberItemArg m_oArg;
	DonateMemberItemRes m_oRes;
};

#endif
