#ifndef __PTCG2C_NOTICEHUNGRYDOWN_H__
#define __PTCG2C_NOTICEHUNGRYDOWN_H__

// generate by ProtoGen at date: 2016/8/23 17:04:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTICEHUNGRYDOWN_TYPE 36895

class PtcG2C_NoticeHungryDown : public CProtocol
{
public:
    explicit PtcG2C_NoticeHungryDown() : CProtocol(PTCG2C_NOTICEHUNGRYDOWN_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NoticeHungryDown()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeHungryDown m_Data;
};

#endif