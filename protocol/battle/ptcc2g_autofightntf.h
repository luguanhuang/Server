#ifndef __PTCC2G_AUTOFIGHTNTF_H__
#define __PTCC2G_AUTOFIGHTNTF_H__

// generate by ProtoGen at date: 2016/6/2 15:51:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_AUTOFIGHTNTF_TYPE 25699

class PtcC2G_AutoFightNTF : public CProtocol
{
public:
    explicit PtcC2G_AutoFightNTF() : CProtocol(PTCC2G_AUTOFIGHTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_AutoFightNTF()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AutoFight m_Data;
};

#endif