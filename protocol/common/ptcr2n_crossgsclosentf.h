#ifndef __PTCR2N_CROSSGSCLOSENTF_H__
#define __PTCR2N_CROSSGSCLOSENTF_H__

// generate by ProtoGen at date: 2016/11/18 14:22:32

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCR2N_CROSSGSCLOSENTF_TYPE 52951

class PtcR2N_CrossGsCloseNtf : public CProtocol
{
public:
    explicit PtcR2N_CrossGsCloseNtf() : CProtocol(PTCR2N_CROSSGSCLOSENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcR2N_CrossGsCloseNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CrossGsCloseNtf m_Data;
};

#endif