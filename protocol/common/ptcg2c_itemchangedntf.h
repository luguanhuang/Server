#ifndef __PTCG2C_ITEMCHANGEDNTF_H__
#define __PTCG2C_ITEMCHANGEDNTF_H__

// generate by ProtoGen at date: 2015/1/23 11:46:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ITEMCHANGEDNTF_TYPE 20270

class PtcG2C_ItemChangedNtf : public CProtocol
{
public:
    explicit PtcG2C_ItemChangedNtf() : CProtocol(PTCG2C_ITEMCHANGEDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ItemChangedNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ItemChanged m_Data;
};

#endif