#ifndef __RPCC2M_ADDBLACKLISTNEW_H__
#define __RPCC2M_ADDBLACKLISTNEW_H__

// generate by ProtoGen at date: 2016/7/28 23:49:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ADDBLACKLISTNEW_TYPE 265

class RpcC2M_AddBlackListNew : public CRpc
{
private:

	RpcC2M_AddBlackListNew():CRpc(RPCC2M_ADDBLACKLISTNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AddBlackListNew *CreateRpc()
	{
		return new RpcC2M_AddBlackListNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AddBlackListArg &roArg, const AddBlackListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AddBlackListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AddBlackListArg &roArg, AddBlackListRes &roRes);
	void OnDelayReplyRpc(const AddBlackListArg &roArg, AddBlackListRes &roRes, const CUserData &roUserData);

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
	AddBlackListArg m_oArg;
	AddBlackListRes m_oRes;
};

#endif
