#ifndef __PTCM2C_RESWARRANKDATANTF_H__
#define __PTCM2C_RESWARRANKDATANTF_H__

// generate by ProtoGen at date: 2016/10/28 17:13:37

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_RESWARRANKDATANTF_TYPE 6844

class PtcM2C_ResWarRankDataNtf : public CProtocol
{
public:
    explicit PtcM2C_ResWarRankDataNtf() : CProtocol(PTCM2C_RESWARRANKDATANTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_ResWarRankDataNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarRankData m_Data;
};

#endif