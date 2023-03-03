#ifndef __RPCC2G_GETGUILDCHECKINBOX_H__
#define __RPCC2G_GETGUILDCHECKINBOX_H__

// generate by ProtoGen at date: 2015/10/8 14:12:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGUILDCHECKINBOX_TYPE 19269

class RpcC2G_GetGuildCheckinBox : public CRpc
{
private:

	RpcC2G_GetGuildCheckinBox():CRpc(RPCC2G_GETGUILDCHECKINBOX_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGuildCheckinBox *CreateRpc()
	{
		return new RpcC2G_GetGuildCheckinBox;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildCheckinBoxArg &roArg, const GetGuildCheckinBoxRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildCheckinBoxArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildCheckinBoxArg &roArg, GetGuildCheckinBoxRes &roRes);
	void OnDelayReplyRpc(const GetGuildCheckinBoxArg &roArg, GetGuildCheckinBoxRes &roRes, const CUserData &roUserData);

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
	GetGuildCheckinBoxArg m_oArg;
	GetGuildCheckinBoxRes m_oRes;
};

#endif
