#ifndef __PTCM2D_SAVEGARDENINFO_H__
#define __PTCM2D_SAVEGARDENINFO_H__

// generate by ProtoGen at date: 2016/10/5 10:02:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVEGARDENINFO_TYPE 61597

class PtcM2D_SaveGardenInfo : public CProtocol
{
public:
    explicit PtcM2D_SaveGardenInfo() : CProtocol(PTCM2D_SAVEGARDENINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SaveGardenInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GardenInfo m_Data;
};

#endif