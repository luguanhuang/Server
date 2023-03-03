#ifndef __PTCG2C_BMFIGHTTIMENTF_H__
#define __PTCG2C_BMFIGHTTIMENTF_H__

// generate by ProtoGen at date: 2017/6/22 8:18:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BMFIGHTTIMENTF_TYPE 4101

class PtcG2C_BMFightTimeNtf : public CProtocol
{
public:
    explicit PtcG2C_BMFightTimeNtf() : CProtocol(PTCG2C_BMFIGHTTIMENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BMFightTimeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BMFightTime m_Data;
};

#endif