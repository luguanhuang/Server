#ifndef __PTCG2C_DOODADITEMUSENTF_H__
#define __PTCG2C_DOODADITEMUSENTF_H__

// generate by ProtoGen at date: 2016/12/24 10:23:12

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_DOODADITEMUSENTF_TYPE 13498

class PtcG2C_DoodadItemUseNtf : public CProtocol
{
public:
    explicit PtcG2C_DoodadItemUseNtf() : CProtocol(PTCG2C_DOODADITEMUSENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_DoodadItemUseNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DoodadItemUseNtf m_Data;
};

#endif