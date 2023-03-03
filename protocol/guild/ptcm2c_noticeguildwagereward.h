#ifndef __PTCM2C_NOTICEGUILDWAGEREWARD_H__
#define __PTCM2C_NOTICEGUILDWAGEREWARD_H__

// generate by ProtoGen at date: 2016/10/28 17:19:38

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTICEGUILDWAGEREWARD_TYPE 29986

class PtcM2C_NoticeGuildWageReward : public CProtocol
{
public:
    explicit PtcM2C_NoticeGuildWageReward() : CProtocol(PTCM2C_NOTICEGUILDWAGEREWARD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NoticeGuildWageReward()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildWageReward m_Data;
};

#endif