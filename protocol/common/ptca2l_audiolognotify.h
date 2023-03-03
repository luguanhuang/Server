#ifndef __PTCA2L_AUDIOLOGNOTIFY_H__
#define __PTCA2L_AUDIOLOGNOTIFY_H__

// generate by ProtoGen at date: 2016/6/22 14:24:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCA2L_AUDIOLOGNOTIFY_TYPE 13942

class PtcA2L_AudioLogNotify : public CProtocol
{
public:
    explicit PtcA2L_AudioLogNotify() : CProtocol(PTCA2L_AUDIOLOGNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcA2L_AudioLogNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LogMsg m_Data;
};

#endif