#ifndef __PTCM2C_GARDENBANQUETNTF_H__
#define __PTCM2C_GARDENBANQUETNTF_H__

// generate by ProtoGen at date: 2016/10/27 11:57:21

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GARDENBANQUETNTF_TYPE 21287

class PtcM2C_GardenBanquetNtf : public CProtocol
{
public:
    explicit PtcM2C_GardenBanquetNtf() : CProtocol(PTCM2C_GARDENBANQUETNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GardenBanquetNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BanquetNtfArg m_Data;
};

#endif