#ifndef __PTCM2C_HINTNOTIFYMS_H__
#define __PTCM2C_HINTNOTIFYMS_H__

// generate by ProtoGen at date: 2017/3/15 10:57:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_HINTNOTIFYMS_TYPE 15542

class PtcM2C_HintNotifyMS : public CProtocol
{
public:
    explicit PtcM2C_HintNotifyMS() : CProtocol(PTCM2C_HINTNOTIFYMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_HintNotifyMS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HintNotify m_Data;
};

#endif