#ifndef __PTCK2M_MAYHEMAWARDNTF_H__
#define __PTCK2M_MAYHEMAWARDNTF_H__

// generate by ProtoGen at date: 2017/6/22 8:19:22

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_MAYHEMAWARDNTF_TYPE 5187

class PtcK2M_MayhemAwardNtf : public CProtocol
{
public:
    explicit PtcK2M_MayhemAwardNtf() : CProtocol(PTCK2M_MAYHEMAWARDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_MayhemAwardNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MayhemAwardList m_Data;
};

#endif