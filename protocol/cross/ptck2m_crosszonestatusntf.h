#ifndef __PTCK2M_CROSSZONESTATUSNTF_H__
#define __PTCK2M_CROSSZONESTATUSNTF_H__

// generate by ProtoGen at date: 2016/11/18 16:06:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_CROSSZONESTATUSNTF_TYPE 3748

class PtcK2M_CrossZoneStatusNtf : public CProtocol
{
public:
    explicit PtcK2M_CrossZoneStatusNtf() : CProtocol(PTCK2M_CROSSZONESTATUSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_CrossZoneStatusNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CrossZoneStatusNtf m_Data;
};

#endif