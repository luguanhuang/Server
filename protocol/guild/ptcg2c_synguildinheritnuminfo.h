#ifndef __PTCG2C_SYNGUILDINHERITNUMINFO_H__
#define __PTCG2C_SYNGUILDINHERITNUMINFO_H__

// generate by ProtoGen at date: 2016/11/26 17:08:12

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SYNGUILDINHERITNUMINFO_TYPE 54442

class PtcG2C_SynGuildInheritNumInfo : public CProtocol
{
public:
    explicit PtcG2C_SynGuildInheritNumInfo() : CProtocol(PTCG2C_SYNGUILDINHERITNUMINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SynGuildInheritNumInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildInheritNumInfo m_Data;
};

#endif