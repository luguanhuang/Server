#ifndef __PTCG2C_HORSEAWARDALLNTF_H__
#define __PTCG2C_HORSEAWARDALLNTF_H__

// generate by ProtoGen at date: 2016/12/6 20:12:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HORSEAWARDALLNTF_TYPE 5990

class PtcG2C_HorseAwardAllNtf : public CProtocol
{
public:
    explicit PtcG2C_HorseAwardAllNtf() : CProtocol(PTCG2C_HORSEAWARDALLNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HorseAwardAllNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HorseAwardAll m_Data;
};

#endif