#ifndef __PTCM2D_DBGROUPCHATSAVEROLEISSUENTF_H__
#define __PTCM2D_DBGROUPCHATSAVEROLEISSUENTF_H__

// generate by ProtoGen at date: 2017/7/28 15:39:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_DBGROUPCHATSAVEROLEISSUENTF_TYPE 866

class PtcM2D_DBGroupChatSaveRoleIssueNtf : public CProtocol
{
public:
    explicit PtcM2D_DBGroupChatSaveRoleIssueNtf() : CProtocol(PTCM2D_DBGROUPCHATSAVEROLEISSUENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_DBGroupChatSaveRoleIssueNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DBGroupChatSaveRoleIssue m_Data;
};

#endif