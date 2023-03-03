#ifndef __PTCG2C_CHATNOTIFYG2C_H__
#define __PTCG2C_CHATNOTIFYG2C_H__

// generate by ProtoGen at date: 2017/6/20 15:09:31

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_CHATNOTIFYG2C_TYPE 48111

class PtcG2C_ChatNotifyG2C : public CProtocol
{
public:
    explicit PtcG2C_ChatNotifyG2C() : CProtocol(PTCG2C_CHATNOTIFYG2C_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ChatNotifyG2C()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChatNotify m_Data;
};

#endif