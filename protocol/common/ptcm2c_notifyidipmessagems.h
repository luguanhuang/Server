#ifndef __PTCM2C_NOTIFYIDIPMESSAGEMS_H__
#define __PTCM2C_NOTIFYIDIPMESSAGEMS_H__

// generate by ProtoGen at date: 2016/9/19 10:27:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTIFYIDIPMESSAGEMS_TYPE 48558

class PtcM2C_NotifyIdipMessageMs : public CProtocol
{
public:
    explicit PtcM2C_NotifyIdipMessageMs() : CProtocol(PTCM2C_NOTIFYIDIPMESSAGEMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NotifyIdipMessageMs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::IdipMessage m_Data;
};

#endif