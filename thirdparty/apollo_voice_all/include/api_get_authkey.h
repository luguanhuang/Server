/*
 * get_authkey_api.h
 *
 *  Created on: 2015��12��15��
 *      Author: purperzhang
 */

#ifndef APPS_APOLLO_CHAT_INCLUDE_API_GET_AUTHKEY_H_
#define APPS_APOLLO_CHAT_INCLUDE_API_GET_AUTHKEY_H_
#include<stdint.h>
#include<string.h>
#include"api_log.h"
namespace ns_ga {

enum{
	API_VERSION= 20160921,
	API_SVR_MAX_URL_LEN =128,
	API_STRING_LENGTH =128,
	API_ASYNC_LENGTH =1024,
	API_LARGE_ROOM_MAX_SEND_NUM =64,
	API_MAX_AUTH_KEY_LEN =1024,
};

struct GetAuthkeyPara
{
	char		open_id[API_STRING_LENGTH];		//must set, every request should be different
	char		client_ip[API_STRING_LENGTH];	//mobile out-net ip
	uint32_t 	async_len;						//refer to document
	char 		async_data[API_ASYNC_LENGTH];	//refer to document
};
struct GetAuthkeyResult							//set the following items to mobile sdk api, and you needn't to know the meaning of them
{
	uint32_t	dwMainSvrId;
	char 		dwMainSvrUrl1[API_STRING_LENGTH];
	char		dwMainSvrUrl2[API_STRING_LENGTH];
	uint32_t	dwSlaveSvrId;
	char		dwSlaveSvrUrl1[API_STRING_LENGTH];
	char		dwSlaveSvrUrl2[API_STRING_LENGTH];
	uint32_t	dwAuthkeyLen;
	char		szAuthKey[API_MAX_AUTH_KEY_LEN];
	uint32_t	dwExpireIn;
	uint32_t 	async_len;
	char 		async_data[API_ASYNC_LENGTH];
};

enum ErrorCode
{
	GET_AUTHKEY_TIMEOUT=-1,
};

struct GetAuthkeyError
{
	int			error_code;
	char		error_desc[API_STRING_LENGTH];
	uint32_t 	async_len;
	char 		async_data[API_ASYNC_LENGTH];
};


struct GetAuthkeyConfig
{
	uint32_t	dir_svr_url_count;						//user must set
	char 		dir_svr_url[100][API_SVR_MAX_URL_LEN];	//use strncpy to fill it
	uint32_t	ip_store_capacity;		//use default
	uint32_t	wait_dir_capacity;		//use default or adjust
	uint32_t	send_record_capacity;	//use default or adjust
	uint32_t	dir_server_update_time_interval_s;	//use default
	uint32_t	send_record_timeout_ms;	//use default or adjust
	uint32_t	rudp_send_capacity;	//use default or adjust
	uint32_t	rudp_recv_capacity;	//use default or adjust

	char		business_name[API_STRING_LENGTH];
	char		business_key[API_STRING_LENGTH];

	char		statistic_send_addr[API_SVR_MAX_URL_LEN];
	const int	api_version;	//use default
	GetAuthkeyConfig()
	:dir_svr_url_count(0)
	,ip_store_capacity(100)
	,wait_dir_capacity(512)
	,send_record_capacity(20480)
	,dir_server_update_time_interval_s(30)
	,send_record_timeout_ms(7000)
	,rudp_send_capacity(20480)
	,rudp_recv_capacity(204800)
	,api_version(API_VERSION)
	{
		for(int i=0;i<100;i++)
		{
			dir_svr_url[i][0]='\0';
		}
		strncpy(business_name,"has not set name",sizeof(business_name));
		strncpy(business_key,"has not set key",sizeof(business_key));
		strncpy(statistic_send_addr,"udp://127.0.0.1:31403",sizeof(statistic_send_addr));
	}
};

class get_authkey_event_handler
{
public:
	virtual ~get_authkey_event_handler(){}
	virtual bool on_get_authkey_return(const GetAuthkeyResult *pr) = 0;
	virtual bool on_get_authkey_error(const GetAuthkeyError *pr) = 0;
};

class get_authkey_api_interface
{
public:
	virtual ~get_authkey_api_interface(){}
	virtual bool 				init(const GetAuthkeyConfig&) = 0;
	virtual bool 				send_get_authkey_req(const GetAuthkeyPara *para) = 0;
	virtual bool 				update() = 0;
	virtual bool				set_business_name(const char*)=0;//ignore this api
};

}//namespace



#ifdef linux
extern "C" __attribute__ ((visibility("default"))) ns_ga::get_authkey_api_interface *create_get_authkey_instance(
		ns_ga::get_authkey_event_handler *,ns_log::MLogInterface*);
#elif defined(WIN32) || defined(WIN64)
extern "C" __declspec(dllexport) ns_ga::get_authkey_api_interface* __stdcall create_get_authkey_instance(
		ns_ga::get_authkey_event_handler *,ns_log::MLogInterface*);
#endif


//
#endif /* APPS_APOLLO_CHAT_INCLUDE_API_GET_AUTHKEY_H_ */
