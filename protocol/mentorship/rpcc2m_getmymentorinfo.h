#ifndef __RPCC2M_GETMYMENTORINFO_H__
#define __RPCC2M_GETMYMENTORINFO_H__

// generate by ProtoGen at date: 2016/12/9 20:55:38

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETMYMENTORINFO_TYPE 8287

class RpcC2M_GetMyMentorInfo : public CRpc
{
private:

	RpcC2M_GetMyMentorInfo():CRpc(RPCC2M_GETMYMENTORINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetMyMentorInfo *CreateRpc()
	{
		return new RpcC2M_GetMyMentorInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetMyMentorInfoArg &roArg, const GetMyMentorInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetMyMentorInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetMyMentorInfoArg &roArg, GetMyMentorInfoRes &roRes);
	void OnDelayReplyRpc(const GetMyMentorInfoArg &roArg, GetMyMentorInfoRes &roRes, const CUserData &roUserData);

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
	GetMyMentorInfoArg m_oArg;
	GetMyMentorInfoRes m_oRes;
};

#endif
