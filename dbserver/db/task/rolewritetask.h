#ifndef _Role_Write_Task_H__
#define _Role_Write_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class CRoleWriteTask : public CMysqlTask
{
public:
	CRoleWriteTask(); 
	virtual ~CRoleWriteTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

	void AddField(const std::string& strField, const std::string& strData);
	UINT32 GetFieldNum() const { return m_oFieldMap.size(); }
public:
	typedef std::unordered_map<std::string, std::string>	CFieldMap;

	UINT32				m_dwRpcDelayed;

	//role
	UINT64				m_qwRoleID;
	CFieldMap			m_oFieldMap;
	UINT32				m_dwTotalSize;

	//accout brief
	std::string			m_strAccount;
	INT32				m_nSlot;
	std::string			m_strBriefData;

	//hidden role
	INT32				m_lastslot;
	std::string			m_lastslotrole;
	std::string			m_hiddenroles;
};

#endif