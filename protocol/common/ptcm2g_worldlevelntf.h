#ifndef __PTCM2G_WORLDLEVELNTF_H__
#define __PTCM2G_WORLDLEVELNTF_H__

// generate by ProtoGen at date: 2016/11/15 21:52:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_WORLDLEVELNTF_TYPE 2395

class PtcM2G_WorldLevelNtf : public CProtocol
{
public:
    explicit PtcM2G_WorldLevelNtf() : CProtocol(PTCM2G_WORLDLEVELNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_WorldLevelNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WorldLevel m_Data;
};

#endif