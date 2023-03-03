#ifndef __PTCG2C_PETCHANGENOTFIY_H__
#define __PTCG2C_PETCHANGENOTFIY_H__

// generate by ProtoGen at date: 2016/4/19 15:29:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PETCHANGENOTFIY_TYPE 22264

class PtcG2C_PetChangeNotfiy : public CProtocol
{
public:
    explicit PtcG2C_PetChangeNotfiy() : CProtocol(PTCG2C_PETCHANGENOTFIY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PetChangeNotfiy()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PetChangeNotfiy m_Data;
};

#endif