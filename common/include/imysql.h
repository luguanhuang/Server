#ifndef _IMySQL_H__
#define _IMySQL_H__


//mysql interface
enum EMysqlConnEvent
{
	MYSQL_CONN_LOST_EVENT,
	MYSQL_RECONNECT_TRY_MAX_EVENT,
	MYSQL_RECONNECT_SUCCESS_EVENT,
};
typedef void(*PMysqlEventCB)(EMysqlConnEvent nMysqlEvent);


class IMysqlRecordSet
{
public:
	virtual ~IMysqlRecordSet() {}


	// ��ȡ��¼���м�¼����
	virtual UINT32 GetRowNum() = 0;


	// ��ȡ��¼���м�¼����
	virtual UINT32 GetFieldNum() = 0;
	

	// ��ȡ��һ�м�¼
	virtual bool FetchRow() = 0;


	// ��ȡ��ǰ��¼�е�ָ���е��ֶ�ֵ �� ���ݳ���
	virtual bool GetFieldValue(UINT32 dwIndex, char** ppValue, UINT32& rdwLen) = 0;


	virtual void Release() = 0;
};


class IMysqlConn
{
public:
	virtual ~IMysqlConn() {}

	// ���ӵ�ָ��������ָ�����ݿ�
	virtual bool Connect(const char* pszHost, UINT32 dwPort, const char* pszUser, const char* pszPwd, const char* pszDB) = 0;


	// �ر�����
	virtual bool Close() = 0;


	// ִ�в������ؼ�¼����sql��� (insert update)
	virtual bool Query(const char* pszSQL, UINT32 dwLen, UINT32* pdwAffectedRows) = 0;


	// ִ�д����ؽ����¼����sql��� (select)
	virtual bool QueryWithRecord(const char* pszSQL, UINT32 dwLen, IMysqlRecordSet** ppRecordSet, bool bStoreResult = true) = 0;


	// ��pszSrc�е��ַ�������Ϊת��SQL�ַ���,���������pszDest�� (nDestLen ����Ϊ nSrcLen*2+1 �ֽ�)
	// ����ת����ַ����ĳ���, ��������β��'\0', -1��ʾʧ�ܣ��մ�ʱ����0
	virtual INT32 EscapeString(const char* pszSrc, INT32 nSrcLen, char* pszDest, INT32 nDestLen) = 0;


	// ��ȡmysql������
	virtual UINT32 GetErrno() = 0;


	// ��ȡmysql����������Ϣ
	virtual const char* GetError() = 0;


	// ��ȡ���һ��insert ����ʱ�� ����ID
	virtual bool GetLastInsertID(UINT64& rqwLastID) = 0;


	// ����Mysql���Ӷ�ʧ�ص�
	virtual void SetMysqlLostCallBack(UINT32 dwTryInterval, UINT32 dwMaxTryTimes, PMysqlEventCB pMysqEvtCB) = 0;


	virtual void Release() = 0;
};


IMysqlConn* CreateMysqlConn();

#endif
