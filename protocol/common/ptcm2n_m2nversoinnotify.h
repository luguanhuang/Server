#ifndef __PTCM2N_M2NVERSOINNOTIFY_H__
#define __PTCM2N_M2NVERSOINNOTIFY_H__

// generate by ProtoGen at date: 2017/6/26 21:43:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2N_M2NVERSOINNOTIFY_TYPE 53700

class PtcM2N_M2NVersoinNotify : public CProtocol
{
public:
    explicit PtcM2N_M2NVersoinNotify() : CProtocol(PTCM2N_M2NVERSOINNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2N_M2NVersoinNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SVersion m_Data;
};

#endif