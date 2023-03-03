#ifndef __PTCG2C_ITEMFINDBACKNTF_H__
#define __PTCG2C_ITEMFINDBACKNTF_H__

// generate by ProtoGen at date: 2016/8/30 16:46:42

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ITEMFINDBACKNTF_TYPE 28509

class PtcG2C_ItemFindBackNtf : public CProtocol
{
public:
    explicit PtcG2C_ItemFindBackNtf() : CProtocol(PTCG2C_ITEMFINDBACKNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ItemFindBackNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ItemFindBackData m_Data;
};

#endif