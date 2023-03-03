#ifndef __PTCM2C_GROUPCHATISSUECOUNT_H__
#define __PTCM2C_GROUPCHATISSUECOUNT_H__

// generate by ProtoGen at date: 2017/7/27 20:20:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GROUPCHATISSUECOUNT_TYPE 61968

class PtcM2C_GroupChatIssueCount : public CProtocol
{
public:
    explicit PtcM2C_GroupChatIssueCount() : CProtocol(PTCM2C_GROUPCHATISSUECOUNT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GroupChatIssueCount()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GroupChatIssueCountNtf m_Data;
};

#endif