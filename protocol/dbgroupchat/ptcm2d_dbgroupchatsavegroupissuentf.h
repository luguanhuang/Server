#ifndef __PTCM2D_DBGROUPCHATSAVEGROUPISSUENTF_H__
#define __PTCM2D_DBGROUPCHATSAVEGROUPISSUENTF_H__

// generate by ProtoGen at date: 2017/7/28 15:36:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_DBGROUPCHATSAVEGROUPISSUENTF_TYPE 4171

class PtcM2D_DBGroupChatSaveGroupIssueNtf : public CProtocol
{
public:
    explicit PtcM2D_DBGroupChatSaveGroupIssueNtf() : CProtocol(PTCM2D_DBGROUPCHATSAVEGROUPISSUENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_DBGroupChatSaveGroupIssueNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DBGroupChatSaveGroupIssue m_Data;
};

#endif