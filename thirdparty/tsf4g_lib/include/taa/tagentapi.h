/*
**  @file $RCSfile: tagentapi.h,v $
**  general description of this module
**  $Id: tagentapi.h,v 1.16 2009-06-29 06:17:05 sean Exp $
**  @author $Author: sean $
**  @date $Date: 2009-06-29 06:17:05 $
**  @version $Revision: 1.16 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef __TAGENT_API__
#define __TAGENT_API__


#if defined(_WIN32) || defined(_WIN64)
# ifdef _DEBUG
#  define TSF4G_LIB_D    "_d"
# else
#  define TSF4G_LIB_D
# endif /* _DEBUG */



#if defined(_WIN32) || defined(_WIN64)
#ifdef TSF4G_SMALL_LIB
#undef _TSF4G_LARGE_LIB_
#endif

#ifdef _TSF4G_LARGE_LIB_
#ifdef _DEBUG
	#pragma comment(lib, "libtsf4g_d.lib")
#else
	#pragma comment(lib, "libtsf4g.lib")
#endif
#else

#if defined(TSF4G_LIB_D)
#pragma comment(lib, "libtaa"TSF4G_LIB_D".lib" )
#else
#pragma comment(lib, "libtaa.lib")
#endif


#endif
#endif


#endif/*#if defined(_WIN32) || defined(_WIN64)*/

#ifdef __cplusplus
extern "C"
{
#endif

#define EXCHANGESIZE			1024*1024
#define EXCBLOCKSIZE			4096

#define EXCHANGEKEY			    0xFFEE0001
#define EXCHEADSIZE 			64

#define TAGENT_DEFAULT_BUSINESS_ID	0	  //default business id

#define ID_APPID_CENTERD		(0x00)    //默认centerd 	appid
#define ID_APPID_DIRTY			(0x01)    //默认dirtyd  	appid
#define ID_APPID_BUSCONFIG		(0x02)    //默认busconfig appid
#define ID_APPID_TCONND		    (0x03)    //默认tconnd    appid
#define ID_APPID_PROCMNG		(0x04)    //process manage  appid
#define ID_APPID_COLLECT		(0x05)    //TDR-URI and TBUS 数据收集

#define ID_APPID_TMAC			(0xff)	  //默认tmac		appid

#define ID_BUSSID_BROADCAST -1

typedef struct tagExchnageHead
{
	int		iExchangeSize;
	int		iBlockSize;
	int		iCreateTime;
	int		iMagic;
	int		iUsed;
	int		iVersion;

	char	szDesc[EXCHEADSIZE-24];
} EXCHANGEHEAD, *LPEXCHANGEHEAD;

typedef struct tagExchangeBlock
{
	unsigned int	uiAppid;
	unsigned int	uiBusid;
} EXCHANGEBLOCK, *	LPEXCHANGEBLOCK;


typedef struct tagExchangeMng
{
	char*	pShm;
	void*	aSem;

	EXCHANGEHEAD*	pstExcHead;
} EXCHANGEMNG, *LPEXCHANGEMNG;

/*********************************************************************
  Fuction         	:  agent_api_init
  Descption       	:  初始化agent APIs
  Input           	:  ppstExcMng		-- 句柄指针指针
  Output          	:  ppstExcMng		-- 返回句柄实例
  Return          	:  0            	-- 注册成功
                      -1           		-- 内存类错误
  Note            	:  ppstExcMng 初始化为NULL，agent_api_init会分配
  					   内存
*********************************************************************/
int	agent_api_init(LPEXCHANGEMNG *ppstExcMng);

/*********************************************************************
  Fuction         	:  agent_api_register
  Descption       	:  注册到busAgent
  Input           	:  pstExcMng		-- 句柄指针
                       uiAppid			-- 应用ID
			   		   uiBusid			-- 业务ID
  Output          	:  ppstExcMng		-- 返回句柄实例
  Return          	:  0            	-- 注册成功
                      -1           		-- 注册失败
  Note            	:  ppstExcMng 初始化为NULL
*********************************************************************/
int	agent_api_register(LPEXCHANGEMNG pstExcMng, unsigned int uiAppid, unsigned int uiBusid);
/*********************************************************************
  Fuction         	:  agent_api_unregister
  Descption       	:  取消注册
  Input           	:  pstExcMng		-- 句柄指针
                       uiAppid			-- 应用ID
			   		   uiBusid			-- 业务ID
  Output          	:  ppstExcMng		-- 返回句柄实例
  Return          	:  0            	-- 取消注册成功
                      -1           		-- 取消注册失败
  Note            	:  ppstExcMng 初始化为NULL
*********************************************************************/
int	agent_api_unregister(LPEXCHANGEMNG pstExcMng, unsigned int uiAppid, unsigned int uiBusid);

/*********************************************************************
  Fuction         	:  agent_api_get_bussid
  Descption       	:  根据appid获得bussid
  Input           	:  ppstExcMng		-- 句柄指针
                       uiAppid			-- 应用ID
			   		   astBussID		-- 业务ID数组
			   		   piSize			-- 业务ID个数指针
  Output          	:  astBussID		-- 业务ID
			   		   piBussSize		-- 业务 ID个数
  Return          	:  0            	-- 获取成功
                       -1       		-- 获取失败，可能原因是pstExcMng
                       					   没有初始化
  Note            	:
*********************************************************************/
int	agent_api_get_bussid(LPEXCHANGEMNG pstExcMng, unsigned int uiAppid, unsigned int astuiBusid[], int *piLen);

/*********************************************************************
  Fuction         	:  agent_api_get_blocks
  Descption       	:  获取已经注册的blocks信息
  Input           	:  ppstExcMng		-- 句柄指针
                       ppstBlock		-- block数组指针的指针
			   		   piBlockSize		-- block个数
  Output          	:  ppstBlock		-- block数组指针的指针
			   		   piBlockSize		-- block个数
  Return          	:  0            	-- 获取成功
                       -1       		-- 获取失败，可能原因是pstExcMng
                       					   没有初始化
  Note            	:  ppstBlock 需要free
*********************************************************************/
int	agent_api_get_blocks(LPEXCHANGEMNG pstExcMng, LPEXCHANGEBLOCK pstBlock[], int *piLen);

/*********************************************************************
  Fuction         	:  agent_api_get_freebufptr
  Descption       	:  根据appid和bussid返回freebuf指针
  Input           	:  ppstExcMng		-- 句柄指针
                       uiAppid			-- 应用ID
			   		   uBussSize		-- 业务ID
  Output          	:
  Return          	:  0            	-- 获取成功
                       -1       		-- 获取失败
  Note            	:
*********************************************************************/
char * agent_api_get_freebufptr(LPEXCHANGEMNG pstExcMng, unsigned int uiAppid, unsigned int uiBusid);


/*********************************************************************
	Fuction         	:  agent_api_destroy
	Descption       	:  注销句柄
	Input           	:  pstExcMng		-- 句柄
	Output          	:
	Return          	:
	Note            	:
*********************************************************************/

void agent_api_destroy(LPEXCHANGEMNG pstExcMng);


#ifdef __cplusplus
}
#endif

#endif /* __TAGENT_API__*/
