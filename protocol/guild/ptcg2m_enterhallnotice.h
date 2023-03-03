#ifndef __PTCG2M_ENTERHALLNOTICE_H__
#define __PTCG2M_ENTERHALLNOTICE_H__

// generate by ProtoGen at date: 2016/11/28 14:28:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_ENTERHALLNOTICE_TYPE 56094

class PtcG2M_EnterHallNotice : public CProtocol
{
public:
    explicit PtcG2M_EnterHallNotice() : CProtocol(PTCG2M_ENTERHALLNOTICE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_EnterHallNotice()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::EnterHallNotice m_Data;
};

#endif