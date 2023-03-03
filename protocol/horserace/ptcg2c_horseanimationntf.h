#ifndef __PTCG2C_HORSEANIMATIONNTF_H__
#define __PTCG2C_HORSEANIMATIONNTF_H__

// generate by ProtoGen at date: 2016/12/6 20:55:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HORSEANIMATIONNTF_TYPE 21212

class PtcG2C_HorseAnimationNtf : public CProtocol
{
public:
    explicit PtcG2C_HorseAnimationNtf() : CProtocol(PTCG2C_HORSEANIMATIONNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HorseAnimationNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HorseAnimation m_Data;
};

#endif