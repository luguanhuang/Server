#ifndef _MySql_Mgr_H__
#define _MySql_Mgr_H__

#include "imysql.h"


class CMysqlMgr
{
	CMysqlMgr();
	~CMysqlMgr();
	DECLARE_SINGLETON(CMysqlMgr)

public:
	bool Init();
	void Uninit();


	IMysqlConn* GetMysqConn() { return m_poMysqlConn; }
private:
	IMysqlConn*		m_poMysqlConn;
};

#endif