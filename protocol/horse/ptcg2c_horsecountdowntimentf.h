#ifndef __PTCG2C_HORSECOUNTDOWNTIMENTF_H__
#define __PTCG2C_HORSECOUNTDOWNTIMENTF_H__

// generate by ProtoGen at date: 2016/12/14 11:05:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HORSECOUNTDOWNTIMENTF_TYPE 65307

class PtcG2C_HorseCountDownTimeNtf : public CProtocol
{
public:
    explicit PtcG2C_HorseCountDownTimeNtf() : CProtocol(PTCG2C_HORSECOUNTDOWNTIMENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HorseCountDownTimeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HorseCountDownTime m_Data;
};

#endif