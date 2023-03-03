#ifndef __PTCM2F_FMRELOADCOPY2FMNTF_H__
#define __PTCM2F_FMRELOADCOPY2FMNTF_H__

// generate by ProtoGen at date: 2017/2/15 16:36:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2F_FMRELOADCOPY2FMNTF_TYPE 47232

class PtcM2F_FMReloadCopy2FMNtf : public CProtocol
{
public:
    explicit PtcM2F_FMReloadCopy2FMNtf() : CProtocol(PTCM2F_FMRELOADCOPY2FMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2F_FMReloadCopy2FMNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LargeRoomRoleParam m_Data;
};

#endif