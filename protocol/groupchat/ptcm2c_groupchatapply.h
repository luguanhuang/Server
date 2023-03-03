#ifndef __PTCM2C_GROUPCHATAPPLY_H__
#define __PTCM2C_GROUPCHATAPPLY_H__

// generate by ProtoGen at date: 2017/7/28 10:37:37

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GROUPCHATAPPLY_TYPE 34424

class PtcM2C_GroupChatApply : public CProtocol
{
public:
    explicit PtcM2C_GroupChatApply() : CProtocol(PTCM2C_GROUPCHATAPPLY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GroupChatApply()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GroupChatApplyNtf m_Data;
};

#endif