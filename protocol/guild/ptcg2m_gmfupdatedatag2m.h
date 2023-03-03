#ifndef __PTCG2M_GMFUPDATEDATAG2M_H__
#define __PTCG2M_GMFUPDATEDATAG2M_H__

// generate by ProtoGen at date: 2016/9/20 18:11:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_GMFUPDATEDATAG2M_TYPE 54928

class PtcG2M_GmfUpdateDataG2M : public CProtocol
{
public:
    explicit PtcG2M_GmfUpdateDataG2M() : CProtocol(PTCG2M_GMFUPDATEDATAG2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_GmfUpdateDataG2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfUpdateData m_Data;
};

#endif