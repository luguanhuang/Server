#ifndef __RPCC2M_STARTRESWARPVE_H__
#define __RPCC2M_STARTRESWARPVE_H__

// generate by ProtoGen at date: 2016/11/3 15:56:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_STARTRESWARPVE_TYPE 35398

class RpcC2M_StartResWarPVE : public CRpc
{
private:

	RpcC2M_StartResWarPVE():CRpc(RPCC2M_STARTRESWARPVE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_StartResWarPVE *CreateRpc()
	{
		return new RpcC2M_StartResWarPVE;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ResWarPVEArg &roArg, const ResWarPVERes &roRes, const CUserData &roUserData);
	void OnTimeout(const ResWarPVEArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ResWarPVEArg &roArg, ResWarPVERes &roRes);
	void OnDelayReplyRpc(const ResWarPVEArg &roArg, ResWarPVERes &roRes, const CUserData &roUserData);

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
	ResWarPVEArg m_oArg;
	ResWarPVERes m_oRes;
};

#endif
