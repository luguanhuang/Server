#ifndef _DBGOALAWARDSSAVE_H_
#define _DBGOALAWARDSSAVE_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CTaskGoalAwardsSave : public CMysqlTask
{
public:
    CTaskGoalAwardsSave(); 
    virtual ~CTaskGoalAwardsSave();


    virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
    virtual void OnReply();

public:
    UINT64	            m_Index;
    KKSG::DBGoalAwardsSave m_data;
};

#endif
