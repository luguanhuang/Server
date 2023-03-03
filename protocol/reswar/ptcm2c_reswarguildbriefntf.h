#ifndef __PTCM2C_RESWARGUILDBRIEFNTF_H__
#define __PTCM2C_RESWARGUILDBRIEFNTF_H__

// generate by ProtoGen at date: 2016/11/3 16:21:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_RESWARGUILDBRIEFNTF_TYPE 35338

class PtcM2C_ResWarGuildBriefNtf : public CProtocol
{
public:
    explicit PtcM2C_ResWarGuildBriefNtf() : CProtocol(PTCM2C_RESWARGUILDBRIEFNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_ResWarGuildBriefNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarGuildBrief m_Data;
};

#endif