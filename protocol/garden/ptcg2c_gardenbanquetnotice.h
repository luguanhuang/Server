#ifndef __PTCG2C_GARDENBANQUETNOTICE_H__
#define __PTCG2C_GARDENBANQUETNOTICE_H__

// generate by ProtoGen at date: 2016/10/27 11:40:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GARDENBANQUETNOTICE_TYPE 36929

class PtcG2C_GardenBanquetNotice : public CProtocol
{
public:
    explicit PtcG2C_GardenBanquetNotice() : CProtocol(PTCG2C_GARDENBANQUETNOTICE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GardenBanquetNotice()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GardenBanquetNtf m_Data;
};

#endif