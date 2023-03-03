#ifndef __PTCG2T_CHANGEGSLINE_H__
#define __PTCG2T_CHANGEGSLINE_H__

// generate by ProtoGen at date: 2016/10/28 19:13:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2T_CHANGEGSLINE_TYPE 3517

class PtcG2T_ChangeGsLine : public CProtocol
{
public:
    explicit PtcG2T_ChangeGsLine() : CProtocol(PTCG2T_CHANGEGSLINE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2T_ChangeGsLine()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChangeGsLineData m_Data;
};

#endif