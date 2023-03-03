#ifndef __PTCG2C_MOBAMINIMAPNTF_H__
#define __PTCG2C_MOBAMINIMAPNTF_H__

// generate by ProtoGen at date: 2017/6/20 22:13:22

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_MOBAMINIMAPNTF_TYPE 32069

class PtcG2C_MobaMiniMapNtf : public CProtocol
{
public:
    explicit PtcG2C_MobaMiniMapNtf() : CProtocol(PTCG2C_MOBAMINIMAPNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_MobaMiniMapNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MobaMiniMapData m_Data;
};

#endif