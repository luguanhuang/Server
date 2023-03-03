#ifndef __PTCM2F_FMRAIDIOCOPY2FMNTF_H__
#define __PTCM2F_FMRAIDIOCOPY2FMNTF_H__

// generate by ProtoGen at date: 2017/2/14 17:13:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2F_FMRAIDIOCOPY2FMNTF_TYPE 38358

class PtcM2F_FMRaidioCopy2FMNtf : public CProtocol
{
public:
    explicit PtcM2F_FMRaidioCopy2FMNtf() : CProtocol(PTCM2F_FMRAIDIOCOPY2FMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2F_FMRaidioCopy2FMNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LargeRoomRoleParam m_Data;
};

#endif