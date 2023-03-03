#ifndef __PTCG2C_POSITIONDEBUG_H__
#define __PTCG2C_POSITIONDEBUG_H__

// generate by ProtoGen at date: 2015/11/2 11:26:44

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_POSITIONDEBUG_TYPE 42493

class PtcG2C_PositionDebug : public CProtocol
{
public:
    explicit PtcG2C_PositionDebug() : CProtocol(PTCG2C_POSITIONDEBUG_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PositionDebug()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PositionCheckList m_Data;
};

#endif