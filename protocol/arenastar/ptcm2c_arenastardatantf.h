#ifndef __PTCM2C_ARENASTARDATANTF_H__
#define __PTCM2C_ARENASTARDATANTF_H__

// generate by ProtoGen at date: 2017/3/29 22:27:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_ARENASTARDATANTF_TYPE 11371

class PtcM2C_ArenaStarDataNtf : public CProtocol
{
public:
    explicit PtcM2C_ArenaStarDataNtf() : CProtocol(PTCM2C_ARENASTARDATANTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_ArenaStarDataNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ArenaStarPara m_Data;
};

#endif