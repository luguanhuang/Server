#ifndef __RPCC2M_GETMARRIAGERELATION_H__
#define __RPCC2M_GETMARRIAGERELATION_H__

// generate by ProtoGen at date: 2017/7/24 20:59:09

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETMARRIAGERELATION_TYPE 13460

class RpcC2M_GetMarriageRelation : public CRpc
{
private:

	RpcC2M_GetMarriageRelation():CRpc(RPCC2M_GETMARRIAGERELATION_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetMarriageRelation *CreateRpc()
	{
		return new RpcC2M_GetMarriageRelation;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetMarriageRelationArg &roArg, const GetMarriageRelationRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetMarriageRelationArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetMarriageRelationArg &roArg, GetMarriageRelationRes &roRes);
	void OnDelayReplyRpc(const GetMarriageRelationArg &roArg, GetMarriageRelationRes &roRes, const CUserData &roUserData);

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
	GetMarriageRelationArg m_oArg;
	GetMarriageRelationRes m_oRes;
};

#endif
