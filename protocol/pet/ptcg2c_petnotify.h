#ifndef __PTCG2C_PETNOTIFY_H__
#define __PTCG2C_PETNOTIFY_H__

// generate by ProtoGen at date: 2016/4/15 11:13:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PETNOTIFY_TYPE 17722

class PtcG2C_PetNotify : public CProtocol
{
public:
    explicit PtcG2C_PetNotify() : CProtocol(PTCG2C_PETNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PetNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PetNotify m_Data;
};

#endif