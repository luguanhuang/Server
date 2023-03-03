#ifndef __PTCM2G_GIVEANSERQAREWARDNTF_H__
#define __PTCM2G_GIVEANSERQAREWARDNTF_H__

// generate by ProtoGen at date: 2016/10/8 22:44:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_GIVEANSERQAREWARDNTF_TYPE 54848

class PtcM2G_GiveAnserQARewardNtf : public CProtocol
{
public:
    explicit PtcM2G_GiveAnserQARewardNtf() : CProtocol(PTCM2G_GIVEANSERQAREWARDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_GiveAnserQARewardNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GiveAnserQARewardNtf m_Data;
};

#endif