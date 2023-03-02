#ifndef _INet_Process_H__
#define _INet_Process_H__

#include "define.h"

class INetWorkProcess
{
public:
	virtual ~INetWorkProcess() {}


	/* ��������Connectʱ,�ɹ���ص�, pUserDataΪConnectʱ����
	 * */
	virtual void OnConnectSucceed(UINT32 dwConnID, void* pUserData) = 0; 


	/* ��������Connectʱ,ʧ�ܺ�ص�
	 * ��nNetErrCodeΪϵͳ������nSysErrCodeΪϵͳ������
	 * ��������£�nSysErrCode��Ч
	 * pUserDataΪConnectʱ����
	 * */
	virtual void OnConnectFailed(INT32 nNetErrCode, INT32 nSysErrCode, void* pUserData) = 0;


	/* ���ܱ�������ʱ, pUserDataΪListenʱ����
	 * */
	virtual void OnPassiveConnect(UINT32 dwConnID, void* pUserData) = 0;


	/* �������ӱ��ر�ʱ�ص�������Ϊ���󱻹رգ�dwErrCodeΪ������
	 * ��nNetErrCodeΪϵͳ������nSysErrCodeΪϵͳ������
	 * ��������£�nSysErrCode��Ч
	 * pUserDataΪConnectʱ���� �� Listener Listenʱ����
	 * */
	virtual void OnClose(UINT32 dwConnID, INT32 nNetErrCode, INT32 nSysErrCode, void* pUserData) = 0;


	/* �����������յ�����ʱ�ص����ϲ㴦��
	 * pUserDataΪConnectʱ���� �� Listener Listenʱ����
	 * */
	virtual void OnRecv(UINT32 dwConnID, const char* pcData, UINT32 dwLen, void* pUserData) = 0;


	/* ����㷢������ʱ�ص� (��ĳ�������ϵĴ���)
	 * ��nNetErrCodeΪϵͳ������nSysErrCodeΪϵͳ������
	 * ��������£�nSysErrCode��Ч
	 * */
	virtual void OnError(INT32 nNetErrCode, INT32 nSysErrCode) = 0;
};

#endif
