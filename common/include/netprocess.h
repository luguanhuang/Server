#ifndef _INet_Process_H__
#define _INet_Process_H__

#include "define.h"

class INetWorkProcess
{
public:
	virtual ~INetWorkProcess() {}


	/* 主动发起Connect时,成功后回调, pUserData为Connect时传入
	 * */
	virtual void OnConnectSucceed(UINT32 dwConnID, void* pUserData) = 0; 


	/* 主动发起Connect时,失败后回调
	 * 若nNetErrCode为系统错误，则nSysErrCode为系统错误码
	 * 其他情况下，nSysErrCode无效
	 * pUserData为Connect时传入
	 * */
	virtual void OnConnectFailed(INT32 nNetErrCode, INT32 nSysErrCode, void* pUserData) = 0;


	/* 接受被动连接时, pUserData为Listen时传入
	 * */
	virtual void OnPassiveConnect(UINT32 dwConnID, void* pUserData) = 0;


	/* 网络连接被关闭时回调，若因为错误被关闭，dwErrCode为错误码
	 * 若nNetErrCode为系统错误，则nSysErrCode为系统错误码
	 * 其他情况下，nSysErrCode无效
	 * pUserData为Connect时传入 或 Listener Listen时传入
	 * */
	virtual void OnClose(UINT32 dwConnID, INT32 nNetErrCode, INT32 nSysErrCode, void* pUserData) = 0;


	/* 网络连接上收到数据时回调，上层处理
	 * pUserData为Connect时传入 或 Listener Listen时传入
	 * */
	virtual void OnRecv(UINT32 dwConnID, const char* pcData, UINT32 dwLen, void* pUserData) = 0;


	/* 网络层发生错误时回调 (非某个连接上的错误)
	 * 若nNetErrCode为系统错误，则nSysErrCode为系统错误码
	 * 其他情况下，nSysErrCode无效
	 * */
	virtual void OnError(INT32 nNetErrCode, INT32 nSysErrCode) = 0;
};

#endif
