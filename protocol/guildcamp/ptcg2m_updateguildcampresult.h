#ifndef __PTCG2M_UPDATEGUILDCAMPRESULT_H__
#define __PTCG2M_UPDATEGUILDCAMPRESULT_H__

// generate by ProtoGen at date: 2016/11/16 14:34:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_UPDATEGUILDCAMPRESULT_TYPE 45716

class PtcG2M_UpdateGuildCampResult : public CProtocol
{
public:
    explicit PtcG2M_UpdateGuildCampResult() : CProtocol(PTCG2M_UPDATEGUILDCAMPRESULT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_UpdateGuildCampResult()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildCampResult m_Data;
};

#endif