#ifndef __PTCC2G_ONLINEREWARDREPORT_H__
#define __PTCC2G_ONLINEREWARDREPORT_H__

// generate by ProtoGen at date: 2015/10/21 23:17:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_ONLINEREWARDREPORT_TYPE 36178

class PtcC2G_OnlineRewardReport : public CProtocol
{
public:
    explicit PtcC2G_OnlineRewardReport() : CProtocol(PTCC2G_ONLINEREWARDREPORT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_OnlineRewardReport()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::OnlineRewardReport m_Data;
};

#endif