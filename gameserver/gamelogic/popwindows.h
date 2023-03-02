#ifndef _POP_WINDOWS_H_
#define _POP_WINDOWS_H_

class Role;
#include "table/ShareTable.h"

class  PopWindows
{
	DECLARE_SINGLETON(PopWindows);
public:
	PopWindows(){}
	~PopWindows(){}
public:

	bool LoadFile();

	bool Init();
	void Uninit();
	bool CheckFile();
	void CheckComment(Role *pRole,KKSG::CommentType type,int nParam1=0,int nParam2=0);
	void ClearWindows(Role *pRole);
	void PopPandora(Role *pRole,KKSG::CommentType type,UINT32 nItemID);
	void SyncCommon(Role *pRole,KKSG::CommentType type,UINT32 nIndex);
	void PopQuestionire(Role *pRole);
private:
	ShareTable m_table;

};


#endif