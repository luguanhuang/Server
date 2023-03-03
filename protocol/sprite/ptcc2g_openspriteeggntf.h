#ifndef __PTCC2G_OPENSPRITEEGGNTF_H__
#define __PTCC2G_OPENSPRITEEGGNTF_H__

// generate by ProtoGen at date: 2016/9/14 14:37:07

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_OPENSPRITEEGGNTF_TYPE 47965

class PtcC2G_OpenSpriteEggNtf : public CProtocol
{
public:
    explicit PtcC2G_OpenSpriteEggNtf() : CProtocol(PTCC2G_OPENSPRITEEGGNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_OpenSpriteEggNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::OpenSpriteEgg m_Data;
};

#endif