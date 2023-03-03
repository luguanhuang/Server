#ifndef __PTCG2M_SYNGUILDBOSSATTR_H__
#define __PTCG2M_SYNGUILDBOSSATTR_H__

// generate by ProtoGen at date: 2017/1/15 15:47:37

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SYNGUILDBOSSATTR_TYPE 7318

class PtcG2M_SynGuildBossAttr : public CProtocol
{
public:
    explicit PtcG2M_SynGuildBossAttr() : CProtocol(PTCG2M_SYNGUILDBOSSATTR_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SynGuildBossAttr()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildBossAttr m_Data;
};

#endif