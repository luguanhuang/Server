#ifndef __PTCK2M_KMATCHEDK2MNTF_H__
#define __PTCK2M_KMATCHEDK2MNTF_H__

// generate by ProtoGen at date: 2017/5/5 21:21:03

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_KMATCHEDK2MNTF_TYPE 28109

class PtcK2M_KMatchedk2mNtf : public CProtocol
{
public:
    explicit PtcK2M_KMatchedk2mNtf() : CProtocol(PTCK2M_KMATCHEDK2MNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_KMatchedk2mNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::KMatchedPara m_Data;
};

#endif