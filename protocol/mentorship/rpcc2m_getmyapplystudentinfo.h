#ifndef __RPCC2M_GETMYAPPLYSTUDENTINFO_H__
#define __RPCC2M_GETMYAPPLYSTUDENTINFO_H__

// generate by ProtoGen at date: 2016/12/9 21:08:40

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETMYAPPLYSTUDENTINFO_TYPE 28961

class RpcC2M_GetMyApplyStudentInfo : public CRpc
{
private:

	RpcC2M_GetMyApplyStudentInfo():CRpc(RPCC2M_GETMYAPPLYSTUDENTINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetMyApplyStudentInfo *CreateRpc()
	{
		return new RpcC2M_GetMyApplyStudentInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetMyApplyStudentInfoArg &roArg, const GetMyApplyStudentInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetMyApplyStudentInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetMyApplyStudentInfoArg &roArg, GetMyApplyStudentInfoRes &roRes);
	void OnDelayReplyRpc(const GetMyApplyStudentInfoArg &roArg, GetMyApplyStudentInfoRes &roRes, const CUserData &roUserData);

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
	GetMyApplyStudentInfoArg m_oArg;
	GetMyApplyStudentInfoRes m_oRes;
};

#endif
