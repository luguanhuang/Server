#ifndef __PTCG2C_SPACTIVITYOFFSETDAYNTF_H__
#define __PTCG2C_SPACTIVITYOFFSETDAYNTF_H__

// generate by ProtoGen at date: 2016/10/28 16:43:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SPACTIVITYOFFSETDAYNTF_TYPE 4059

class PtcG2C_SpActivityOffsetDayNtf : public CProtocol
{
public:
    explicit PtcG2C_SpActivityOffsetDayNtf() : CProtocol(PTCG2C_SPACTIVITYOFFSETDAYNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SpActivityOffsetDayNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SpActivityOffsetDay m_Data;
};

#endif