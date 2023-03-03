#ifndef __RPCG2M_GETWATCHINFOBYTYPE_H__
#define __RPCG2M_GETWATCHINFOBYTYPE_H__

// generate by ProtoGen at date: 2016/7/29 17:09:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GETWATCHINFOBYTYPE_TYPE 28313

class RpcG2M_GetWatchInfoByType : public CRpc
{
private:

	RpcG2M_GetWatchInfoByType():CRpc(RPCG2M_GETWATCHINFOBYTYPE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GetWatchInfoByType *CreateRpc()
	{
		return new RpcG2M_GetWatchInfoByType;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetWatchInfoByTypeArg &roArg, const GetWatchInfoByTypeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetWatchInfoByTypeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetWatchInfoByTypeArg &roArg, GetWatchInfoByTypeRes &roRes);
	void OnDelayReplyRpc(const GetWatchInfoByTypeArg &roArg, GetWatchInfoByTypeRes &roRes, const CUserData &roUserData);

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
	GetWatchInfoByTypeArg m_oArg;
	GetWatchInfoByTypeRes m_oRes;
};

#endif
