#ifndef __PTCG2M_REPORTWATCHNTF_H__
#define __PTCG2M_REPORTWATCHNTF_H__

// generate by ProtoGen at date: 2016/8/4 6:43:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_REPORTWATCHNTF_TYPE 4097

class PtcG2M_ReportWatchNtf : public CProtocol
{
public:
    explicit PtcG2M_ReportWatchNtf() : CProtocol(PTCG2M_REPORTWATCHNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_ReportWatchNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReportWatcherNtf m_Data;
};

#endif