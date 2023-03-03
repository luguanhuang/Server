#ifndef __PTCM2C_SKYCITYFINALRES_H__
#define __PTCM2C_SKYCITYFINALRES_H__

// generate by ProtoGen at date: 2016/9/29 16:20:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_SKYCITYFINALRES_TYPE 30112

class PtcM2C_SkyCityFinalRes : public CProtocol
{
public:
    explicit PtcM2C_SkyCityFinalRes() : CProtocol(PTCM2C_SKYCITYFINALRES_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_SkyCityFinalRes()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkyCityFinalBaseInfo m_Data;
};

#endif