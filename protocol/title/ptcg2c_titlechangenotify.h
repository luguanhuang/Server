#ifndef __PTCG2C_TITLECHANGENOTIFY_H__
#define __PTCG2C_TITLECHANGENOTIFY_H__

// generate by ProtoGen at date: 2016/8/25 9:51:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TITLECHANGENOTIFY_TYPE 1040

class PtcG2C_TitleChangeNotify : public CProtocol
{
public:
    explicit PtcG2C_TitleChangeNotify() : CProtocol(PTCG2C_TITLECHANGENOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TitleChangeNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::titleChangeData m_Data;
};

#endif