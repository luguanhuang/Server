#ifndef __PTCG2D_UPDATEFIELD_H__
#define __PTCG2D_UPDATEFIELD_H__

// generate by ProtoGen at date: 2014/12/2 11:17:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2D_UPDATEFIELD_TYPE 29364

class PtcG2D_UpdateField : public CProtocol
{
public:
    explicit PtcG2D_UpdateField() : CProtocol(PTCG2D_UPDATEFIELD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2D_UpdateField()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateField m_Data;
};

#endif