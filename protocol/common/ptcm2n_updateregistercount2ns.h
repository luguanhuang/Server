#ifndef __PTCM2N_UPDATEREGISTERCOUNT2NS_H__
#define __PTCM2N_UPDATEREGISTERCOUNT2NS_H__

// generate by ProtoGen at date: 2016/12/24 15:36:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2N_UPDATEREGISTERCOUNT2NS_TYPE 17531

class PtcM2N_UpdateRegisterCount2Ns : public CProtocol
{
public:
    explicit PtcM2N_UpdateRegisterCount2Ns() : CProtocol(PTCM2N_UPDATEREGISTERCOUNT2NS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2N_UpdateRegisterCount2Ns()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateRegisterCount2Ns m_Data;
};

#endif