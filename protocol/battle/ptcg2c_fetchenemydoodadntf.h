#ifndef __PTCG2C_FETCHENEMYDOODADNTF_H__
#define __PTCG2C_FETCHENEMYDOODADNTF_H__

// generate by ProtoGen at date: 2015/12/2 14:51:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_FETCHENEMYDOODADNTF_TYPE 50480

class PtcG2C_FetchEnemyDoodadNtf : public CProtocol
{
public:
    explicit PtcG2C_FetchEnemyDoodadNtf() : CProtocol(PTCG2C_FETCHENEMYDOODADNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_FetchEnemyDoodadNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::OtherFetchDoodadRes m_Data;
};

#endif