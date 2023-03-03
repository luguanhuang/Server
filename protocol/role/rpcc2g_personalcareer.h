#ifndef __RPCC2G_PERSONALCAREER_H__
#define __RPCC2G_PERSONALCAREER_H__

// generate by ProtoGen at date: 2017/2/4 16:33:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PERSONALCAREER_TYPE 64048

class RpcC2G_PersonalCareer : public CRpc
{
private:

	RpcC2G_PersonalCareer():CRpc(RPCC2G_PERSONALCAREER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PersonalCareer *CreateRpc()
	{
		return new RpcC2G_PersonalCareer;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PersonalCareerArg &roArg, const PersonalCareerRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PersonalCareerArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PersonalCareerArg &roArg, PersonalCareerRes &roRes);
	void OnDelayReplyRpc(const PersonalCareerArg &roArg, PersonalCareerRes &roRes, const CUserData &roUserData);

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
	PersonalCareerArg m_oArg;
	PersonalCareerRes m_oRes;
};

#endif
