#ifndef __PTCD2G_D2GDESIGNATIONNTF_H__
#define __PTCD2G_D2GDESIGNATIONNTF_H__

// generate by ProtoGen at date: 2016/7/29 0:53:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCD2G_D2GDESIGNATIONNTF_TYPE 22625

class PtcD2G_D2GDesignationNtf : public CProtocol
{
public:
    explicit PtcD2G_D2GDesignationNtf() : CProtocol(PTCD2G_D2GDESIGNATIONNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcD2G_D2GDesignationNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::D2GDesignationNtf m_Data;
};

#endif