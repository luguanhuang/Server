//2015-12-28 by purperzhang

#ifndef API_VOIP_H
#define API_VOIP_H
#include<stdint.h>
#include<string.h>
#include"api_log.h"
namespace ns_voip
{
enum {
	API_VERSION=20160921,
};
enum
{
	API_MAX_USER_COUNT=16,
	API_MAX_CALLBACK_LEN=1024,
	API_MAX_ERROR_DESC_LEN=128,
	API_MAX_OPENID_LEN=256,
	API_MAX_MSG_LEN=256,
	API_MAX_ACCESS_URL_COUNT=16,
	API_MAX_AUTH_KEY_LEN=1024,
	API_SVR_MAX_URL_LEN=128,
	API_STRING_LENGTH=128,
};

enum VoipNetType
{
    VOIP_NET_TYPE_DEFAULT=0,//"default"
	VOIP_NET_TYPE_2G=1,//"2g"
	VOIP_NET_TYPE_3G=2,//"3g"
	VOIP_NET_TYPE_4G=3,//"4g"
	VOIP_NET_TYPE_WIFI=10,//"wifi"
};


struct User
{
	char		open_id[API_MAX_OPENID_LEN];	//string flag of client must be set with every request has the different one
	char		client_ip[API_STRING_LENGTH];	//moble user's out-net ip
	VoipNetType	net_type;	//refer to "enum VoipNetType" above
};
struct UserAccess
{
	char		open_id[API_MAX_OPENID_LEN];	//string flag returned from voice server which has been defined in request message;
	uint32_t	member_id;						//transfer it to mobile sdk api
	uint64_t	room_key;						//use it to enter or close the room
	uint64_t	extra_data;						//ignore(fill 0)
	uint16_t	access_ip_count;				//the ip list should be transferred to mobile sdk api without any change
	char		access_ip_list[API_MAX_ACCESS_URL_COUNT][API_SVR_MAX_URL_LEN];

};


struct CreateVoipRoomPara
{
	uint32_t 	async_len;	//refer to document
	char 		async_data[API_MAX_CALLBACK_LEN];	//refer to document
};
struct CreateVoipRoomResult
{
	uint64_t	room_id;	//unique room id number
	uint32_t 	async_len;	//self-defined data len
	char 		async_data[API_MAX_CALLBACK_LEN];	//self-defined data defined by user and returned by voice server when message comes back
};
struct JoinVoipRoomPara
{
	uint64_t	room_id;	//use create result
	User		user;		//refer to "struct User" above
	uint32_t 	async_len;
	char 		async_data[API_MAX_CALLBACK_LEN];
};
struct JoinVoipRoomResult
{
	uint64_t	room_id;
	UserAccess	user_access;
	uint32_t 	async_len;
	char 		async_data[API_MAX_CALLBACK_LEN];

};
struct ExitVoipRoomPara
{
	uint64_t	room_id;	//use join room result
	uint32_t	member_id;	//use join room result
	char		user_open_id[API_MAX_OPENID_LEN];	//the same as the Join room's open_id
	uint32_t	exit_type;	//0:default,1:normal exit, 2:timeout, 3:kick out room
	uint32_t 	async_len;
	char 		async_data[API_MAX_CALLBACK_LEN];
};
struct ExitVoipRoomResult
{
	uint64_t	room_id;
	uint32_t	member_id;
	char		user_open_id[API_MAX_OPENID_LEN];
	uint32_t	member_count_after_exit;
	uint32_t 	async_len;
	char 		async_data[API_MAX_CALLBACK_LEN];
};
struct CloseVoipRoomPara
{
	uint64_t	room_id;	//use create room return
	uint32_t	close_type;	//1:normal close, 2:unnormal close
	uint32_t 	async_len;
	char 		async_data[API_MAX_CALLBACK_LEN];
};
struct CloseVoipRoomResult
{
	uint64_t	room_id;
	uint32_t 	async_len;
	char 		async_data[API_MAX_CALLBACK_LEN];
};

enum ErrorCode{
	VOIP_CMD_CREATE_ROOM=-12,
	VOIP_CMD_JOIN_ROOM=-13,
	VOIP_CMD_EXIT_ROOM=-14,
	VOIP_CMD_CLOSE_ROOM=-15,
	VIOP_CMD_UNKNOWN=-16,
	VOIP_REQ_TIMEOUT=-1,
    VOIP_ERR_CDN_ERROR=1,
    VOIP_ERR_INVALID_ARG=2,
    VOIP_ERR_TIMEOUTED=3,
    VOIP_ERR_SEND_REQ_TO_CDN_FAILED=4,
    VOIP_ERR_ERROR_REQUEST=5,
    VOIP_ERR_BUSINESSID_NOT_FOUND=6,
    VOIP_ERR_CURL_ERROR=7,
};
struct VoipReturnError
{
	int			req_cmd;
	int			error_code;
	char		error_desc[API_MAX_ERROR_DESC_LEN];
	uint32_t 	async_len;
	char 		async_data[API_MAX_CALLBACK_LEN];
};

struct VoipConfig
{
	uint32_t	dir_svr_url_count;//user must set, should not be greater than 100
	char 		dir_svr_url[100][API_SVR_MAX_URL_LEN];	//user set
	uint32_t	ip_store_capacity;	//use default
	uint32_t	wait_dir_capacity;	//use default or adjust
	uint32_t	send_record_capacity;	//use default or adjust
	uint32_t	dir_server_update_time_interval_s;	//use default
	uint32_t	send_record_timeout_ms;	//use default or adjust
	uint32_t	rudp_send_capacity;	//use default or adjust
	uint32_t	rudp_recv_capacity;	//use default or adjust

	char		business_name[API_STRING_LENGTH];	//user set
	char		business_key[API_STRING_LENGTH];	//user set

	char		statistic_send_addr[API_SVR_MAX_URL_LEN];	//use default
	const int	api_version;	//use default
	VoipConfig()
	:dir_svr_url_count(0)
	,ip_store_capacity(100)
	,wait_dir_capacity(512/2)
	,send_record_capacity(20480)
	,dir_server_update_time_interval_s(30)
	,send_record_timeout_ms(7000)
	,rudp_send_capacity(20480/2)
	,rudp_recv_capacity(204800/2)
	,api_version(API_VERSION)
	{
		for(int i=0;i<100;i++)
		{
			dir_svr_url[i][0]='\0';
		}
		strncpy(business_name,"has not set name",sizeof(business_name));
		strncpy(business_key,"has not set key",sizeof(business_key));
		strncpy(statistic_send_addr,"udp://127.0.0.1:31401",sizeof(statistic_send_addr));
	}
};



class VoipInterface
{
public:
	virtual ~VoipInterface(){}
	virtual bool	init(const VoipConfig&config)=0;
	virtual bool	send_create_voiproom_req(const CreateVoipRoomPara*para)=0;
	virtual bool	send_join_voiproom_req	(const JoinVoipRoomPara*para)=0;
	virtual bool	send_exit_voiproom_req	(const ExitVoipRoomPara*para)=0;
	virtual bool	send_close_voiproom_req	(const CloseVoipRoomPara*para)=0;
	virtual bool	update()=0;
	virtual bool	set_business_name(const char*)=0;//ignore this api
};
class VoipEventHandler
{
public:
	virtual ~VoipEventHandler(){}
	virtual bool	on_create_voiproom_rsp	(const CreateVoipRoomResult*pr)=0;
	virtual	bool	on_join_voiproom_rsp	(const JoinVoipRoomResult*pr)=0;
	virtual bool	on_exit_voiproom_rsp	(const ExitVoipRoomResult*pr)=0;
	virtual bool	on_close_voiproom_rsp	(const CloseVoipRoomResult*pr)=0;
	virtual bool	on_recv_error			(const VoipReturnError*pr)=0;
};

}//namespace

#ifdef linux

extern "C" __attribute__((visibility("default"))) ns_voip::VoipInterface* create_voip_instance(
		ns_voip::VoipEventHandler*,ns_log::MLogInterface*);

#endif



//the following is used for producing windows C++ api dll
#if defined(WIN32) || defined(WIN64)

extern "C" __declspec(dllexport) ns_voip::VoipInterface* __stdcall create_voip_instance(
		ns_voip::VoipEventHandler*,ns_log::MLogInterface*);
#endif


//the following is used for producing windows C api dll
#if defined(WIN32) || defined(WIN64)
extern "C"
{
typedef int(__stdcall *ftype_on_create_voiproom_rsp)	(struct ns_voip::CreateVoipRoomResult*);
typedef int(__stdcall *ftype_on_join_voiproom_rsp)		(struct ns_voip::JoinVoipRoomResult*);
typedef int(__stdcall *ftype_on_exit_voiproom_rsp)		(struct ns_voip::ExitVoipRoomResult*);
typedef int(__stdcall *ftype_on_close_voiproom_rsp)		(struct ns_voip::CloseVoipRoomResult*);
typedef int(__stdcall *ftype_on_recv_error)				(struct ns_voip::VoipReturnError*);

__declspec(dllexport) void* __stdcall create_voip_instance_windows(
		ftype_on_create_voiproom_rsp	fcreate,
		ftype_on_join_voiproom_rsp		fjoin,
		ftype_on_exit_voiproom_rsp		fexit,
		ftype_on_close_voiproom_rsp		fclose,
		ftype_on_recv_error				ferror
		);
__declspec(dllexport) int __stdcall voip_init(void *handle,struct ns_voip::VoipConfig*pconfig);
__declspec(dllexport) int __stdcall voip_send_create_voiproom_req(void *handle,struct ns_voip::CreateVoipRoomPara*para);
__declspec(dllexport) int __stdcall voip_send_join_voiproom_req(void *handle,struct ns_voip::JoinVoipRoomPara*para);
__declspec(dllexport) int __stdcall voip_send_exit_voiproom_req(void *handle,struct ns_voip::ExitVoipRoomPara*para);
__declspec(dllexport) int __stdcall voip_send_close_voiproom_req(void *handle,struct ns_voip::CloseVoipRoomPara*para);
__declspec(dllexport) int __stdcall voip_update(void *handle);
__declspec(dllexport) void __stdcall voip_set_log_level(void *handle,ns_log::LogLevel log_level);

}
#endif

#endif //API_VOIP_H
