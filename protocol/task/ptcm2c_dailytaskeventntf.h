#ifndef __PTCM2C_DAILYTASKEVENTNTF_H__
#define __PTCM2C_DAILYTASKEVENTNTF_H__

// generate by ProtoGen at date: 2017/9/19 19:53:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_DAILYTASKEVENTNTF_TYPE 26376

class PtcM2C_DailyTaskEventNtf : public CProtocol
{
public:
    explicit PtcM2C_DailyTaskEventNtf() : CProtocol(PTCM2C_DAILYTASKEVENTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_DailyTaskEventNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DailyTaskEventNtf m_Data;
};

#endif