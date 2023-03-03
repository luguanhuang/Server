#ifndef __PTCN2R_REPORTSESSIONINFO_H__
#define __PTCN2R_REPORTSESSIONINFO_H__

// generate by ProtoGen at date: 2017/3/15 10:03:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2R_REPORTSESSIONINFO_TYPE 7787

class PtcN2R_ReportSessionInfo : public CProtocol
{
public:
    explicit PtcN2R_ReportSessionInfo() : CProtocol(PTCN2R_REPORTSESSIONINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2R_ReportSessionInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReportSessionInfo m_Data;
};

#endif