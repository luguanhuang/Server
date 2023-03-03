//2016-01-04 by purperzhang

#ifndef STT_INCLUDE_API_STT_H_
#define STT_INCLUDE_API_STT_H_

#include<stdint.h>
#include<string.h>
#include"api_log.h"
namespace ns_stt {

enum{
	API_VERSION= 20160921,
	API_SVR_MAX_URL_LEN =128,
	API_STRING_LENGTH =128,
	API_FILE_ID_LENGTH=512,
	API_KEY_TEXT_LENGTH=1024,
	API_ASYNC_LENGTH =1024,
};

struct SttPara
{
	char 		file_id[API_FILE_ID_LENGTH];	//obtained when mobile up-transfer the voice file
	uint32_t 	async_len;						//refer to document
	char 		async_data[API_ASYNC_LENGTH];	//refer to document
};
struct SttResult
{
	char 		file_id[API_FILE_ID_LENGTH];
	char 		file_text[API_KEY_TEXT_LENGTH];	//the translated result
	uint32_t 	async_len;
	char 		async_data[API_ASYNC_LENGTH];
};

enum ErrorCode
{
	STT_LOCAL_TIMEOUT=-1,
};

struct SttError
{
	char 		file_id[API_FILE_ID_LENGTH];
	int			error_code;
	char		error_desc[API_STRING_LENGTH];
	uint32_t 	async_len;
	char 		async_data[API_ASYNC_LENGTH];
};

struct SttConfig
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
	SttConfig()
	:dir_svr_url_count(0)
	,ip_store_capacity(100)
	,wait_dir_capacity(512)
	,send_record_capacity(20480)
	,dir_server_update_time_interval_s(30)
	,send_record_timeout_ms(13000)
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
		strncpy(statistic_send_addr,"udp://127.0.0.1:31404",sizeof(statistic_send_addr));
	}
};
class stt_event_handler
{
public:
	virtual ~stt_event_handler(){}
	virtual bool on_stt_return(const SttResult *pr) = 0;
	virtual bool on_stt_error(const SttError *pr) = 0;
};

class stt_api_interface
{
public:
	virtual ~stt_api_interface(){}
	virtual bool 				init(const SttConfig&) = 0;
	virtual bool 				send_stt_req(const SttPara *para) = 0;
	virtual bool 				update() = 0;
	virtual bool				set_business_name(const char*)=0;//ignore this api
};
}//namespace
#ifdef linux

extern "C" __attribute__ ((visibility("default"))) ns_stt::stt_api_interface *create_stt_instance(
		ns_stt::stt_event_handler *,ns_log::MLogInterface*);

#elif defined(WIN32) || defined(WIN64)

extern "C" __declspec(dllexport) ns_stt::stt_api_interface* __stdcall create_stt_instance(
		ns_stt::stt_event_handler *,ns_log::MLogInterface*);
#endif

#endif /* STT_INCLUDE_API_STT_H_ */
