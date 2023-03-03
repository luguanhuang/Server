#ifndef __PTCG2C_BFFIGHTTIMENTF_H__
#define __PTCG2C_BFFIGHTTIMENTF_H__

// generate by ProtoGen at date: 2017/9/6 14:38:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BFFIGHTTIMENTF_TYPE 39352

class PtcG2C_BFFightTimeNtf : public CProtocol
{
public:
    explicit PtcG2C_BFFightTimeNtf() : CProtocol(PTCG2C_BFFIGHTTIMENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BFFightTimeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BFFightTime m_Data;
};

#endif