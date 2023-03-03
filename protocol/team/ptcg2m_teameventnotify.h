#ifndef __PTCG2M_TEAMEVENTNOTIFY_H__
#define __PTCG2M_TEAMEVENTNOTIFY_H__

// generate by ProtoGen at date: 2016/8/24 15:11:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_TEAMEVENTNOTIFY_TYPE 43612

class PtcG2M_TeamEventNotify : public CProtocol
{
public:
    explicit PtcG2M_TeamEventNotify() : CProtocol(PTCG2M_TEAMEVENTNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_TeamEventNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamEvent m_Data;
};

#endif