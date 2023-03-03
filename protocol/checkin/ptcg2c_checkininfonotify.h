#ifndef __PTCG2C_CHECKININFONOTIFY_H__
#define __PTCG2C_CHECKININFONOTIFY_H__

// generate by ProtoGen at date: 2015/6/24 15:51:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_CHECKININFONOTIFY_TYPE 29332

class PtcG2C_CheckinInfoNotify : public CProtocol
{
public:
    explicit PtcG2C_CheckinInfoNotify() : CProtocol(PTCG2C_CHECKININFONOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_CheckinInfoNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CheckinInfoNotify m_Data;
};

#endif