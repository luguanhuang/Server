#ifndef __PTCG2C_CHANGESUPPLEMENTNTF_H__
#define __PTCG2C_CHANGESUPPLEMENTNTF_H__

// generate by ProtoGen at date: 2016/1/18 17:25:44

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_CHANGESUPPLEMENTNTF_TYPE 11250

class PtcG2C_ChangeSupplementNtf : public CProtocol
{
public:
    explicit PtcG2C_ChangeSupplementNtf() : CProtocol(PTCG2C_CHANGESUPPLEMENTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ChangeSupplementNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChangeSupplementNtf m_Data;
};

#endif