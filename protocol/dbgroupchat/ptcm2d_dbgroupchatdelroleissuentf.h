#ifndef __PTCM2D_DBGROUPCHATDELROLEISSUENTF_H__
#define __PTCM2D_DBGROUPCHATDELROLEISSUENTF_H__

// generate by ProtoGen at date: 2017/7/28 15:39:36

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_DBGROUPCHATDELROLEISSUENTF_TYPE 51604

class PtcM2D_DBGroupChatDelRoleIssueNtf : public CProtocol
{
public:
    explicit PtcM2D_DBGroupChatDelRoleIssueNtf() : CProtocol(PTCM2D_DBGROUPCHATDELROLEISSUENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_DBGroupChatDelRoleIssueNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DBGroupChatDelRoleIssue m_Data;
};

#endif