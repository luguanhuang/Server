#ifndef __PTCG2M_REPORTCUSTOMBATTLERESULT_H__
#define __PTCG2M_REPORTCUSTOMBATTLERESULT_H__

// generate by ProtoGen at date: 2017/5/6 16:57:12

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_REPORTCUSTOMBATTLERESULT_TYPE 47975

class PtcG2M_ReportCustomBattleResult : public CProtocol
{
public:
    explicit PtcG2M_ReportCustomBattleResult() : CProtocol(PTCG2M_REPORTCUSTOMBATTLERESULT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_ReportCustomBattleResult()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReportCustomBattleResult m_Data;
};

#endif