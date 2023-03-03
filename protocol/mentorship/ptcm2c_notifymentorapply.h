#ifndef __PTCM2C_NOTIFYMENTORAPPLY_H__
#define __PTCM2C_NOTIFYMENTORAPPLY_H__

// generate by ProtoGen at date: 2016/12/9 23:10:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTIFYMENTORAPPLY_TYPE 61023

class PtcM2C_NotifyMentorApply : public CProtocol
{
public:
    explicit PtcM2C_NotifyMentorApply() : CProtocol(PTCM2C_NOTIFYMENTORAPPLY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NotifyMentorApply()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyMentorApplyData m_Data;
};

#endif