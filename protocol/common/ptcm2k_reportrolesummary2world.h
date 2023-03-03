#ifndef __PTCM2K_REPORTROLESUMMARY2WORLD_H__
#define __PTCM2K_REPORTROLESUMMARY2WORLD_H__

// generate by ProtoGen at date: 2017/2/11 21:10:37

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2K_REPORTROLESUMMARY2WORLD_TYPE 49809

class PtcM2K_ReportRoleSummary2World : public CProtocol
{
public:
    explicit PtcM2K_ReportRoleSummary2World() : CProtocol(PTCM2K_REPORTROLESUMMARY2WORLD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2K_ReportRoleSummary2World()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SyncRoleSummaryToMS m_Data;
};

#endif