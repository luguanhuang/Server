#ifndef __PTCM2C_NOTIFYMARRIAGEAPPLY_H__
#define __PTCM2C_NOTIFYMARRIAGEAPPLY_H__

// generate by ProtoGen at date: 2017/7/25 16:27:10

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTIFYMARRIAGEAPPLY_TYPE 42923

class PtcM2C_NotifyMarriageApply : public CProtocol
{
public:
    explicit PtcM2C_NotifyMarriageApply() : CProtocol(PTCM2C_NOTIFYMARRIAGEAPPLY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NotifyMarriageApply()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyMarriageApplyData m_Data;
};

#endif