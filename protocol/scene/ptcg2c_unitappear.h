#ifndef __PTCG2C_UNITAPPEAR_H__
#define __PTCG2C_UNITAPPEAR_H__

// generate by ProtoGen at date: 2016/10/28 22:22:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_UNITAPPEAR_TYPE 7458

class PtcG2C_UnitAppear : public CProtocol
{
public:
    explicit PtcG2C_UnitAppear() : CProtocol(PTCG2C_UNITAPPEAR_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_UnitAppear()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UnitAppearList m_Data;
};

#endif