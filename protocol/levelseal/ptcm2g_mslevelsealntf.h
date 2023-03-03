#ifndef __PTCM2G_MSLEVELSEALNTF_H__
#define __PTCM2G_MSLEVELSEALNTF_H__

// generate by ProtoGen at date: 2016/7/29 18:34:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_MSLEVELSEALNTF_TYPE 17527

class PtcM2G_MsLevelSealNtf : public CProtocol
{
public:
    explicit PtcM2G_MsLevelSealNtf() : CProtocol(PTCM2G_MSLEVELSEALNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_MsLevelSealNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MsLevelSealInfo m_Data;
};

#endif