#ifndef __PTCC2M_FRIENDQUERYREPORTNEW_H__
#define __PTCC2M_FRIENDQUERYREPORTNEW_H__

// generate by ProtoGen at date: 2017/3/15 10:55:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_FRIENDQUERYREPORTNEW_TYPE 15079

class PtcC2M_FriendQueryReportNew : public CProtocol
{
public:
    explicit PtcC2M_FriendQueryReportNew() : CProtocol(PTCC2M_FRIENDQUERYREPORTNEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_FriendQueryReportNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FriendQueryReportNew m_Data;
};

#endif