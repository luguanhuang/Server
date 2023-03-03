#ifndef __PTCC2G_NEXTDAYREWARDREPORT_H__
#define __PTCC2G_NEXTDAYREWARDREPORT_H__

// generate by ProtoGen at date: 2015/10/21 23:18:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_NEXTDAYREWARDREPORT_TYPE 1059

class PtcC2G_NextDayRewardReport : public CProtocol
{
public:
    explicit PtcC2G_NextDayRewardReport() : CProtocol(PTCC2G_NEXTDAYREWARDREPORT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_NextDayRewardReport()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NextDayRewardReport m_Data;
};

#endif