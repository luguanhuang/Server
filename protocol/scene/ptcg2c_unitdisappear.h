#ifndef __PTCG2C_UNITDISAPPEAR_H__
#define __PTCG2C_UNITDISAPPEAR_H__

// generate by ProtoGen at date: 2016/12/25 18:48:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_UNITDISAPPEAR_TYPE 26347

class PtcG2C_UnitDisappear : public CProtocol
{
public:
    explicit PtcG2C_UnitDisappear() : CProtocol(PTCG2C_UNITDISAPPEAR_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_UnitDisappear()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UnitAppearance m_Data;
};

#endif