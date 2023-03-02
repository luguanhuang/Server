#ifndef __PAYIBITEM_TASK_H__
#define __PAYIBITEM_TASK_H__

#include "mysqltask.h"

class CPayIbItemTask : public CMysqlTask
{
public:
	CPayIbItemTask();
	virtual ~CPayIbItemTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();
	void SetData(KKSG::MysqlOpType op, const KKSG::PayGiftIbItem& oData);

private:
	INT32 m_Op;
	KKSG::PayGiftIbItem m_oData;
};
#endif

