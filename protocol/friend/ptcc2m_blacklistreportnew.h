#ifndef __PTCC2M_BLACKLISTREPORTNEW_H__
#define __PTCC2M_BLACKLISTREPORTNEW_H__

// generate by ProtoGen at date: 2016/7/29 14:26:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_BLACKLISTREPORTNEW_TYPE 57057

class PtcC2M_BlackListReportNew : public CProtocol
{
public:
    explicit PtcC2M_BlackListReportNew() : CProtocol(PTCC2M_BLACKLISTREPORTNEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_BlackListReportNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BlackListReport m_Data;
};

#endif