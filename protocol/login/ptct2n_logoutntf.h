#ifndef __PTCT2N_LOGOUTNTF_H__
#define __PTCT2N_LOGOUTNTF_H__

// generate by ProtoGen at date: 2016/11/3 20:17:42

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCT2N_LOGOUTNTF_TYPE 6890

class PtcT2N_LogoutNtf : public CProtocol
{
public:
    explicit PtcT2N_LogoutNtf() : CProtocol(PTCT2N_LOGOUTNTF_TYPE)
    {
		
    }

    virtual ~PtcT2N_LogoutNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	
};

#endif