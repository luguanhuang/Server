#ifndef __PTCC2G_SHAREREPORT_H__
#define __PTCC2G_SHAREREPORT_H__

// generate by ProtoGen at date: 2017/7/31 20:31:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_SHAREREPORT_TYPE 31884

class PtcC2G_ShareReport : public CProtocol
{
public:
    explicit PtcC2G_ShareReport() : CProtocol(PTCC2G_SHAREREPORT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_ShareReport()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ShareReportData m_Data;
};

#endif