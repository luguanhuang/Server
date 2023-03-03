#ifndef __PTCM2K_BMENTERREADYSCENENTF_H__
#define __PTCM2K_BMENTERREADYSCENENTF_H__

// generate by ProtoGen at date: 2017/6/22 8:18:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2K_BMENTERREADYSCENENTF_TYPE 27074

class PtcM2K_BMEnterReadySceneNtf : public CProtocol
{
public:
    explicit PtcM2K_BMEnterReadySceneNtf() : CProtocol(PTCM2K_BMENTERREADYSCENENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2K_BMEnterReadySceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BMEnterSvrInfo m_Data;
};

#endif