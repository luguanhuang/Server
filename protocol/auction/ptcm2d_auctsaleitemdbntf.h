#ifndef __PTCM2D_AUCTSALEITEMDBNTF_H__
#define __PTCM2D_AUCTSALEITEMDBNTF_H__

// generate by ProtoGen at date: 2016/9/16 22:22:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_AUCTSALEITEMDBNTF_TYPE 34550

class PtcM2D_AuctSaleItemDBNtf : public CProtocol
{
public:
    explicit PtcM2D_AuctSaleItemDBNtf() : CProtocol(PTCM2D_AUCTSALEITEMDBNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_AuctSaleItemDBNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AuctDBItemDale m_Data;
};

#endif