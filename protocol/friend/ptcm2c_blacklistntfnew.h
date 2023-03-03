#ifndef __PTCM2C_BLACKLISTNTFNEW_H__
#define __PTCM2C_BLACKLISTNTFNEW_H__

// generate by ProtoGen at date: 2016/7/29 14:27:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_BLACKLISTNTFNEW_TYPE 1537

class PtcM2C_BlackListNtfNew : public CProtocol
{
public:
    explicit PtcM2C_BlackListNtfNew() : CProtocol(PTCM2C_BLACKLISTNTFNEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_BlackListNtfNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BlackListNtf m_Data;
};

#endif