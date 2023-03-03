#ifndef __PTCG2C_GMFBASEDATANTF_H__
#define __PTCG2C_GMFBASEDATANTF_H__

// generate by ProtoGen at date: 2016/7/18 10:56:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GMFBASEDATANTF_TYPE 4338

class PtcG2C_GmfBaseDataNtf : public CProtocol
{
public:
    explicit PtcG2C_GmfBaseDataNtf() : CProtocol(PTCG2C_GMFBASEDATANTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GmfBaseDataNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfRoleDatas m_Data;
};

#endif