#ifndef __GIFTIBHIST_TASK_H__
#define __GIFTIBHIST_TASK_H__

#include "mysqltask.h"

class GiftIBHistTask : public CMysqlTask
{
public:
	GiftIBHistTask();
	virtual ~GiftIBHistTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();
	void SetData(const KKSG::SaveIbGiftRecordArg& oData);

private:
	KKSG::SaveIbGiftRecordArg m_oData;
};
#endif

