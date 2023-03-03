#ifndef __PTCM2L_MSLOGNOTIFY_H__
#define __PTCM2L_MSLOGNOTIFY_H__

// generate by ProtoGen at date: 2016/8/10 2:59:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2L_MSLOGNOTIFY_TYPE 13934

class PtcM2L_MSLogNotify : public CProtocol
{
public:
    explicit PtcM2L_MSLogNotify() : CProtocol(PTCM2L_MSLOGNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2L_MSLogNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LogMsg m_Data;
};

#endif