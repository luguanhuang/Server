#ifndef __PTCG2C_ALLGIFTIBITEMNTF_H__
#define __PTCG2C_ALLGIFTIBITEMNTF_H__

// generate by ProtoGen at date: 2017/3/14 10:59:17

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ALLGIFTIBITEMNTF_TYPE 2916

class PtcG2C_AllGiftIBItemNtf : public CProtocol
{
public:
    explicit PtcG2C_AllGiftIBItemNtf() : CProtocol(PTCG2C_ALLGIFTIBITEMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_AllGiftIBItemNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AllGiftIBItem m_Data;
};

#endif