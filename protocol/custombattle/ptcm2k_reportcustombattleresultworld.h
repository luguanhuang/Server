#ifndef __PTCM2K_REPORTCUSTOMBATTLERESULTWORLD_H__
#define __PTCM2K_REPORTCUSTOMBATTLERESULTWORLD_H__

// generate by ProtoGen at date: 2017/5/6 20:11:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2K_REPORTCUSTOMBATTLERESULTWORLD_TYPE 2704

class PtcM2K_ReportCustomBattleResultWorld : public CProtocol
{
public:
    explicit PtcM2K_ReportCustomBattleResultWorld() : CProtocol(PTCM2K_REPORTCUSTOMBATTLERESULTWORLD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2K_ReportCustomBattleResultWorld()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReportCustomBattleResult m_Data;
};

#endif