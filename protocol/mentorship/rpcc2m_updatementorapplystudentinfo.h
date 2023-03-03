#ifndef __RPCC2M_UPDATEMENTORAPPLYSTUDENTINFO_H__
#define __RPCC2M_UPDATEMENTORAPPLYSTUDENTINFO_H__

// generate by ProtoGen at date: 2016/12/9 22:54:06

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_UPDATEMENTORAPPLYSTUDENTINFO_TYPE 55126

class RpcC2M_UpdateMentorApplyStudentInfo : public CRpc
{
private:

	RpcC2M_UpdateMentorApplyStudentInfo():CRpc(RPCC2M_UPDATEMENTORAPPLYSTUDENTINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_UpdateMentorApplyStudentInfo *CreateRpc()
	{
		return new RpcC2M_UpdateMentorApplyStudentInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UpdateMentorApplyStudentInfoArg &roArg, const UpdateMentorApplyStudentInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UpdateMentorApplyStudentInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UpdateMentorApplyStudentInfoArg &roArg, UpdateMentorApplyStudentInfoRes &roRes);
	void OnDelayReplyRpc(const UpdateMentorApplyStudentInfoArg &roArg, UpdateMentorApplyStudentInfoRes &roRes, const CUserData &roUserData);

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
	UpdateMentorApplyStudentInfoArg m_oArg;
	UpdateMentorApplyStudentInfoRes m_oRes;
};

#endif
