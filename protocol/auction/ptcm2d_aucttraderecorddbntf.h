#ifndef __PTCM2D_AUCTTRADERECORDDBNTF_H__
#define __PTCM2D_AUCTTRADERECORDDBNTF_H__

// generate by ProtoGen at date: 2016/9/16 22:24:04

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_AUCTTRADERECORDDBNTF_TYPE 56492

class PtcM2D_AuctTradeRecordDBNtf : public CProtocol
{
public:
    explicit PtcM2D_AuctTradeRecordDBNtf() : CProtocol(PTCM2D_AUCTTRADERECORDDBNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_AuctTradeRecordDBNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AuctDBTradeAccount m_Data;
};

#endif