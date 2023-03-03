#ifndef __PTCA2T_AUDIOOPENGATE_H__
#define __PTCA2T_AUDIOOPENGATE_H__

// generate by ProtoGen at date: 2016/6/22 14:24:53

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCA2T_AUDIOOPENGATE_TYPE 55883

class PtcA2T_AudioOpenGate : public CProtocol
{
public:
    explicit PtcA2T_AudioOpenGate() : CProtocol(PTCA2T_AUDIOOPENGATE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcA2T_AudioOpenGate()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::gateflag m_Data;
};

#endif