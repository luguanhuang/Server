#ifndef __PTCG2C_SKYCITYTIMERES_H__
#define __PTCG2C_SKYCITYTIMERES_H__

// generate by ProtoGen at date: 2016/9/29 19:17:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SKYCITYTIMERES_TYPE 30724

class PtcG2C_SkyCityTimeRes : public CProtocol
{
public:
    explicit PtcG2C_SkyCityTimeRes() : CProtocol(PTCG2C_SKYCITYTIMERES_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SkyCityTimeRes()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkyCityTimeInfo m_Data;
};

#endif