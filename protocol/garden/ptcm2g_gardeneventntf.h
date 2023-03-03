#ifndef __PTCM2G_GARDENEVENTNTF_H__
#define __PTCM2G_GARDENEVENTNTF_H__

// generate by ProtoGen at date: 2016/10/18 12:14:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_GARDENEVENTNTF_TYPE 2774

class PtcM2G_GardenEventNtf : public CProtocol
{
public:
    explicit PtcM2G_GardenEventNtf() : CProtocol(PTCM2G_GARDENEVENTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_GardenEventNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GardenEventNtfArg m_Data;
};

#endif