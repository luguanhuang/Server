#ifndef __PTCG2C_SPRITECHANGEDNTF_H__
#define __PTCG2C_SPRITECHANGEDNTF_H__

// generate by ProtoGen at date: 2016/9/2 17:13:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SPRITECHANGEDNTF_TYPE 197

class PtcG2C_SpriteChangedNtf : public CProtocol
{
public:
    explicit PtcG2C_SpriteChangedNtf() : CProtocol(PTCG2C_SPRITECHANGEDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SpriteChangedNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SpriteChanged m_Data;
};

#endif