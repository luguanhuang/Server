#ifndef __PTCG2M_SYNGUILDCONTRIBUTE_H__
#define __PTCG2M_SYNGUILDCONTRIBUTE_H__

// generate by ProtoGen at date: 2016/9/24 17:05:29

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SYNGUILDCONTRIBUTE_TYPE 29191

class PtcG2M_SynGuildContribute : public CProtocol
{
public:
    explicit PtcG2M_SynGuildContribute() : CProtocol(PTCG2M_SYNGUILDCONTRIBUTE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SynGuildContribute()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildContribute m_Data;
};

#endif