#ifndef __PTCG2C_SCENEPREPAREINFONTF_H__
#define __PTCG2C_SCENEPREPAREINFONTF_H__

// generate by ProtoGen at date: 2016/12/16 20:58:08

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SCENEPREPAREINFONTF_TYPE 65478

class PtcG2C_ScenePrepareInfoNtf : public CProtocol
{
public:
    explicit PtcG2C_ScenePrepareInfoNtf() : CProtocol(PTCG2C_SCENEPREPAREINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ScenePrepareInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ScenePrepareInfoNtf m_Data;
};

#endif