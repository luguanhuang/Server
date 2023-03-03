#ifndef __PTCG2C_WORLDBOSSATTRNTF_H__
#define __PTCG2C_WORLDBOSSATTRNTF_H__

// generate by ProtoGen at date: 2016/8/9 0:42:05

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WORLDBOSSATTRNTF_TYPE 31578

class PtcG2C_WorldBossAttrNtf : public CProtocol
{
public:
    explicit PtcG2C_WorldBossAttrNtf() : CProtocol(PTCG2C_WORLDBOSSATTRNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WorldBossAttrNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WorldBossAttrNtf m_Data;
};

#endif