#ifndef __PTCG2M_CREATEGUILDBOSSSINGLE_H__
#define __PTCG2M_CREATEGUILDBOSSSINGLE_H__

// generate by ProtoGen at date: 2016/8/8 14:47:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_CREATEGUILDBOSSSINGLE_TYPE 58351

class PtcG2M_CreateGuildBossSingle : public CProtocol
{
public:
    explicit PtcG2M_CreateGuildBossSingle() : CProtocol(PTCG2M_CREATEGUILDBOSSSINGLE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_CreateGuildBossSingle()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::guildbossindex m_Data;
};

#endif