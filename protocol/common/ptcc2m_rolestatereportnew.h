#ifndef __PTCC2M_ROLESTATEREPORTNEW_H__
#define __PTCC2M_ROLESTATEREPORTNEW_H__

// generate by ProtoGen at date: 2017/3/15 10:03:31

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_ROLESTATEREPORTNEW_TYPE 10217

class PtcC2M_RoleStateReportNew : public CProtocol
{
public:
    explicit PtcC2M_RoleStateReportNew() : CProtocol(PTCC2M_ROLESTATEREPORTNEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_RoleStateReportNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RoleStateReport m_Data;
};

#endif