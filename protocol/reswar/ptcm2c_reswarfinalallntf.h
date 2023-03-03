#ifndef __PTCM2C_RESWARFINALALLNTF_H__
#define __PTCM2C_RESWARFINALALLNTF_H__

// generate by ProtoGen at date: 2016/11/8 14:55:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_RESWARFINALALLNTF_TYPE 35696

class PtcM2C_ResWarFinalAllNtf : public CProtocol
{
public:
    explicit PtcM2C_ResWarFinalAllNtf() : CProtocol(PTCM2C_RESWARFINALALLNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_ResWarFinalAllNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarFinalAll m_Data;
};

#endif