#ifndef __RPCC2G_QUERYSCENETIME_H__
#define __RPCC2G_QUERYSCENETIME_H__

// generate by ProtoGen at date: 2016/9/16 10:07:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_QUERYSCENETIME_TYPE 39595

class RpcC2G_QuerySceneTime : public CRpc
{
private:

	RpcC2G_QuerySceneTime():CRpc(RPCC2G_QUERYSCENETIME_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_QuerySceneTime *CreateRpc()
	{
		return new RpcC2G_QuerySceneTime;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QuerySceneTimeArg &roArg, const QuerySceneTimeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QuerySceneTimeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QuerySceneTimeArg &roArg, QuerySceneTimeRes &roRes);
	void OnDelayReplyRpc(const QuerySceneTimeArg &roArg, QuerySceneTimeRes &roRes, const CUserData &roUserData);

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
	QuerySceneTimeArg m_oArg;
	QuerySceneTimeRes m_oRes;
};

#endif
