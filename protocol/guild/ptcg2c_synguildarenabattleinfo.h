#ifndef __PTCG2C_SYNGUILDARENABATTLEINFO_H__
#define __PTCG2C_SYNGUILDARENABATTLEINFO_H__

// generate by ProtoGen at date: 2016/7/19 19:57:02

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SYNGUILDARENABATTLEINFO_TYPE 1906

class PtcG2C_SynGuildArenaBattleInfo : public CProtocol
{
public:
    explicit PtcG2C_SynGuildArenaBattleInfo() : CProtocol(PTCG2C_SYNGUILDARENABATTLEINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SynGuildArenaBattleInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildArenaBattleInfo m_Data;
};

#endif