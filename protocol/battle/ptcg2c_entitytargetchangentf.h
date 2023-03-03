#ifndef __PTCG2C_ENTITYTARGETCHANGENTF_H__
#define __PTCG2C_ENTITYTARGETCHANGENTF_H__

// generate by ProtoGen at date: 2017/6/13 11:51:13

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ENTITYTARGETCHANGENTF_TYPE 9303

class PtcG2C_EntityTargetChangeNtf : public CProtocol
{
public:
    explicit PtcG2C_EntityTargetChangeNtf() : CProtocol(PTCG2C_ENTITYTARGETCHANGENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_EntityTargetChangeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::EntityTargetData m_Data;
};

#endif