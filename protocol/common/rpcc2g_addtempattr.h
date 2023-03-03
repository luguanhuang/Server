#ifndef __RPCC2G_ADDTEMPATTR_H__
#define __RPCC2G_ADDTEMPATTR_H__

// generate by ProtoGen at date: 2015/7/21 16:18:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ADDTEMPATTR_TYPE 22021

class RpcC2G_AddTempAttr : public CRpc
{
private:

	RpcC2G_AddTempAttr():CRpc(RPCC2G_ADDTEMPATTR_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_AddTempAttr *CreateRpc()
	{
		return new RpcC2G_AddTempAttr;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AddTempAttrArg &roArg, const AddTempAttrRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AddTempAttrArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AddTempAttrArg &roArg, AddTempAttrRes &roRes);
	void OnDelayReplyRpc(const AddTempAttrArg &roArg, AddTempAttrRes &roRes, const CUserData &roUserData);

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
	AddTempAttrArg m_oArg;
	AddTempAttrRes m_oRes;
};

#endif
