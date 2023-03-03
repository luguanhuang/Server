#ifndef __RPCI2S_REQPLATUSERSINFOI2S_H__
#define __RPCI2S_REQPLATUSERSINFOI2S_H__

// generate by ProtoGen at date: 2017/6/2 21:05:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCI2S_REQPLATUSERSINFOI2S_TYPE 19123

class RpcI2S_ReqPlatUsersInfoI2S : public CRpc
{
private:

	RpcI2S_ReqPlatUsersInfoI2S():CRpc(RPCI2S_REQPLATUSERSINFOI2S_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcI2S_ReqPlatUsersInfoI2S *CreateRpc()
	{
		return new RpcI2S_ReqPlatUsersInfoI2S;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const M2SReqPlatUserInfoArg &roArg, const M2SReqPlatUserInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const M2SReqPlatUserInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const M2SReqPlatUserInfoArg &roArg, M2SReqPlatUserInfoRes &roRes);
	void OnDelayReplyRpc(const M2SReqPlatUserInfoArg &roArg, M2SReqPlatUserInfoRes &roRes, const CUserData &roUserData);

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
	M2SReqPlatUserInfoArg m_oArg;
	M2SReqPlatUserInfoRes m_oRes;
};

#endif
