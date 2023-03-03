#ifndef __RPCC2G_FETCHCHAPTERCHEST_H__
#define __RPCC2G_FETCHCHAPTERCHEST_H__

// generate by ProtoGen at date: 2016/10/28 22:23:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_FETCHCHAPTERCHEST_TYPE 21099

class RpcC2G_FetchChapterChest : public CRpc
{
private:

	RpcC2G_FetchChapterChest():CRpc(RPCC2G_FETCHCHAPTERCHEST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_FetchChapterChest *CreateRpc()
	{
		return new RpcC2G_FetchChapterChest;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FetchChapterChestArg &roArg, const FetchChapterChestRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FetchChapterChestArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FetchChapterChestArg &roArg, FetchChapterChestRes &roRes);
	void OnDelayReplyRpc(const FetchChapterChestArg &roArg, FetchChapterChestRes &roRes, const CUserData &roUserData);

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
	FetchChapterChestArg m_oArg;
	FetchChapterChestRes m_oRes;
};

#endif
