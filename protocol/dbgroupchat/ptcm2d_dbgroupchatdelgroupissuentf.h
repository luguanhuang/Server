#ifndef __PTCM2D_DBGROUPCHATDELGROUPISSUENTF_H__
#define __PTCM2D_DBGROUPCHATDELGROUPISSUENTF_H__

// generate by ProtoGen at date: 2017/7/28 15:38:21

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_DBGROUPCHATDELGROUPISSUENTF_TYPE 34614

class PtcM2D_DBGroupChatDelGroupIssueNtf : public CProtocol
{
public:
    explicit PtcM2D_DBGroupChatDelGroupIssueNtf() : CProtocol(PTCM2D_DBGROUPCHATDELGROUPISSUENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_DBGroupChatDelGroupIssueNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DBGroupChatDelGroupIssue m_Data;
};

#endif