#ifndef __PTCG2C_RESWARENDNTF_H__
#define __PTCG2C_RESWARENDNTF_H__

// generate by ProtoGen at date: 2016/11/3 16:15:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_RESWARENDNTF_TYPE 4850

class PtcG2C_ResWarEndNtf : public CProtocol
{
public:
    explicit PtcG2C_ResWarEndNtf() : CProtocol(PTCG2C_RESWARENDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ResWarEndNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarEnd m_Data;
};

#endif