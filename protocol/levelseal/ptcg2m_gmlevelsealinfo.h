#ifndef __PTCG2M_GMLEVELSEALINFO_H__
#define __PTCG2M_GMLEVELSEALINFO_H__

// generate by ProtoGen at date: 2016/7/30 20:49:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_GMLEVELSEALINFO_TYPE 44044

class PtcG2M_GmLevelSealInfo : public CProtocol
{
public:
    explicit PtcG2M_GmLevelSealInfo() : CProtocol(PTCG2M_GMLEVELSEALINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_GmLevelSealInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LevelSealInfo m_Data;
};

#endif