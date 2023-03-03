#ifndef __PTCC2G_BATTLESTATISTICSREPORT_H__
#define __PTCC2G_BATTLESTATISTICSREPORT_H__

// generate by ProtoGen at date: 2017/7/25 20:35:42

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_BATTLESTATISTICSREPORT_TYPE 3612

class PtcC2G_BattleStatisticsReport : public CProtocol
{
public:
    explicit PtcC2G_BattleStatisticsReport() : CProtocol(PTCC2G_BATTLESTATISTICSREPORT_TYPE)
    {
		
    }

    virtual ~PtcC2G_BattleStatisticsReport()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif