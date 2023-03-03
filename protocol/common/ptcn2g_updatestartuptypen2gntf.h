#ifndef __PTCN2G_UPDATESTARTUPTYPEN2GNTF_H__
#define __PTCN2G_UPDATESTARTUPTYPEN2GNTF_H__

// generate by ProtoGen at date: 2016/12/22 9:47:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2G_UPDATESTARTUPTYPEN2GNTF_TYPE 50760

class PtcN2G_UpdateStartUpTypeN2GNtf : public CProtocol
{
public:
    explicit PtcN2G_UpdateStartUpTypeN2GNtf() : CProtocol(PTCN2G_UPDATESTARTUPTYPEN2GNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2G_UpdateStartUpTypeN2GNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateStartUpType m_Data;
};

#endif