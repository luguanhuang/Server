#ifndef __RPCM2D_UPDATEROLEREPORTDATA_H__
#define __RPCM2D_UPDATEROLEREPORTDATA_H__

// generate by ProtoGen at date: 2017/5/15 19:12:30

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_UPDATEROLEREPORTDATA_TYPE 44113

class RpcM2D_UpdateRoleReportData : public CRpc
{
private:

	RpcM2D_UpdateRoleReportData():CRpc(RPCM2D_UPDATEROLEREPORTDATA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_UpdateRoleReportData *CreateRpc()
	{
		return new RpcM2D_UpdateRoleReportData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UpdateRoleReportDataArg &roArg, const UpdateRoleReportDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UpdateRoleReportDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UpdateRoleReportDataArg &roArg, UpdateRoleReportDataRes &roRes);
	void OnDelayReplyRpc(const UpdateRoleReportDataArg &roArg, UpdateRoleReportDataRes &roRes, const CUserData &roUserData);

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
	UpdateRoleReportDataArg m_oArg;
	UpdateRoleReportDataRes m_oRes;
};

#endif
