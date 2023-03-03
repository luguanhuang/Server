//2015-12-21 by purperzhang

#ifndef APPS_APOLLO_CHAT_INCLUDE_API_LARGE_ROOM_H_
#define APPS_APOLLO_CHAT_INCLUDE_API_LARGE_ROOM_H_
#include<stdint.h>
#include<string.h>
#include"api_log.h"

namespace ns_lr
{

int get_apollo_chat_api_version();
const  char *get_apollo_version();

enum{
 API_VERSION =20160921,
 API_SVR_MAX_URL_LEN =128,
 API_STRING_LENGTH= 128,
 API_ASYNC_LENGTH =1024,
 API_LARGE_ROOM_MAX_SEND_NUM =16,
};
struct LargeRoomConfig
{
	uint32_t	dir_svr_url_count;	//user set
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
	LargeRoomConfig()
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
		strncpy(statistic_send_addr,"udp://127.0.0.1:31402",sizeof(statistic_send_addr));
	}
};

struct CreateLargeRoomReqPara
{
	uint64_t	roomid;		//(fill 0, use gid instead)
	uint64_t	roomkey;	//ignore
	uint32_t	user_openid;				//client id(user must defined, can't be empty and every request should be different)
	uint32_t	cycle_report_switch;		//ignore(fill 0)
	uint32_t	encrypt_size;				//ignore(fill 0)
	char		uuid[API_STRING_LENGTH];	//user open id(user must set, can't be empty and every request should be different)
	uint32_t	async_len;					//refer to document
	char		async_data[API_ASYNC_LENGTH];	//refer to document
};
struct CreateLargeRoomRspResult
{
	uint64_t	gid;			//use it to enter room(it is the main flag for large_room, usually use it instead of roomid)
	uint64_t	roomid;			//ignore
	uint64_t	roomkey;		//use it when join room
	uint32_t	user_openid;
	char		uuid[API_STRING_LENGTH];	//the same as open_id but uuid would be checked first
	uint32_t 	business_id;				//this will be used in mobile sdk api
	uint32_t	async_len;
	char		async_data[API_ASYNC_LENGTH];
};
struct CloseLargeRoomReqPara
{
	uint64_t	roomid;			//changeless name of room use,  fill 0 or not
	uint64_t	roomkey;		//use create room result
	uint32_t	user_openid;	//must set, every request should be different
	uint32_t	type;			//1:normal close	2:timeout close
	uint64_t	gid;			//use create room result
	char		uuid[API_STRING_LENGTH];	//user must set, every request should be different, can be same as open_id
	uint32_t	async_len;
	char		async_data[API_ASYNC_LENGTH];
};
struct CloseLargeRoomRspResult
{
	uint64_t	roomid;
	uint64_t	roomkey;
	uint32_t	user_openid;
	uint64_t	gid;
	char		uuid[API_STRING_LENGTH];
	uint32_t	async_len;
	char		async_data[API_ASYNC_LENGTH];
};
struct EnterReqInfo
{
	uint64_t	roomid;			//changeless name of room use,  fill 0 or not
	uint64_t	roomkey;		//use create room result
	uint32_t	memberid;		//fill 0
	uint32_t	user_openid;	//user must set, every request should be different
	char		user_ip[API_STRING_LENGTH];	//user must set the mobile user out-net ip
	uint32_t	dump_switch;	//ignore(fill 0)
	uint32_t	client_link_info_report_switch;	//ignore(fill 0)
	uint32_t	user_ability;	//ignore(fill 0)
	uint32_t	mix_voice_ability;		//ignore(fill 0)
	uint32_t	net_type;		//0:default	1:2G	2:3G	3:4G	10:WIFI
	uint64_t	gid;			//use create room result
	char		uuid[API_STRING_LENGTH];	//user must set, every request should be different, can be same as open_id
	int32_t	 	role;			//1:speaker	2:audience	0:no meaning
};
struct EnterRspInfo
{
	uint64_t	gid;
	uint64_t	roomid;
	uint64_t	roomkey;
	uint32_t	memberid;		//transfer it to mobile apollo_voice sdk api
	uint32_t	user_openid;
	char		user_ip[API_STRING_LENGTH];
	char	 	user_access[API_STRING_LENGTH];	//the net_address user's mobile is going to connect, just transfer it to mobile apollo_voice sdk api without any change
	int32_t 	entrypt_switch;		//ignore(fill 0)
	uint32_t	mix_voice_ability;	//ignore(fill 0)
	char		uuid[API_STRING_LENGTH];
};
struct MemberEnterReqPara
{
	uint32_t	num;			//the number of member to enter, suggest setting only one within a request
	EnterReqInfo req_info[API_LARGE_ROOM_MAX_SEND_NUM];
	uint32_t	async_len;
	char		async_data[API_ASYNC_LENGTH];
};
struct MemberEnterRspResult
{
	uint32_t	num;
	EnterRspInfo rsp_info[API_LARGE_ROOM_MAX_SEND_NUM];
	uint32_t	async_len;
	char		async_data[API_ASYNC_LENGTH];
};
struct CheckRoomExistPara
{
	uint64_t	roomid;
	uint64_t	roomkey;
	uint64_t	gid;
	uint32_t	async_len;
	char		async_data[API_ASYNC_LENGTH];
};
struct CheckRoomExistResult
{
	uint64_t	roomid;		//changeless name of room use,  fill 0 or not
	uint64_t	roomkey;	//use create room result
	uint64_t	gid;		//use create room result
	uint32_t	exist;		//0:not exist	1:exist
	uint32_t	async_len;
	char		async_data[API_ASYNC_LENGTH];
};
enum LargeRoomErrorCode{
	ERROR_REQ_CMD_CREATE_LARGE_ROOM=-10,
	ERROR_REQ_CMD_ENTER_LARGE_ROOM=-11,
	ERROR_REQ_CMD_CLOSE_LARGE_ROOM=-12,
	ERROR_REQ_CMD_UNKNOWN=-13,
	ERROR_REQ_CMD_CHECK_ROOM_EXIST=-14,
	LARGE_ROOM_REQ_TIMEOUT=-1,
    LARGE_ROOM_NO_ERROR=0,
    LARGE_ROOM_ERROR_FAILED_TO_SEND_REQ_FOR_NO_CONNECTION_AVAIBLE=1,
    LARGE_ROOM_ERROR_SESSION_TIMEOUT=2,
    LARGE_ROOM_ERROR_NOT_ENOUGH_SESSION=3,
	LARGE_ROOM_BUSINESS_FIXED_ROOM_ID_WRONG=4,
};
struct LargeRoomErrorResult
{
	int		error_req_cmd;
	int		error_code;
	char	error_desc[API_STRING_LENGTH];
	uint32_t	async_len;
	char 	async_data[API_ASYNC_LENGTH];
};


class large_room_api_interface;
class large_room_event_handler
{
public:
	virtual bool on_create_large_room_rsp(const CreateLargeRoomRspResult*presult)=0;
	virtual bool on_member_enter_rsp(const MemberEnterRspResult*presult)=0;
	virtual bool on_close_large_room_rsp(const CloseLargeRoomRspResult*presult)=0;
	virtual bool on_large_room_error_rsp(const LargeRoomErrorResult*perror)=0;
	virtual bool on_check_room_exist_rsp(const CheckRoomExistResult*presult)=0;
	virtual ~large_room_event_handler()
	{
	}
};

class large_room_api_interface
{
public:
	virtual bool init(const LargeRoomConfig& config)=0;

	virtual bool send_create_large_room_req(const CreateLargeRoomReqPara*para)=0;
	virtual bool send_member_enter_req(const MemberEnterReqPara*para)=0;
	virtual bool send_close_large_room_req(const CloseLargeRoomReqPara*para)=0;
	virtual bool send_check_room_exist_req(const CheckRoomExistPara*para)=0;

	virtual bool update() = 0;
	virtual ~large_room_api_interface()
	{
	}

	virtual bool				set_business_name(const char*)=0;//ignore this api
};

} //namespace

#ifdef linux

extern "C" __attribute__ ((visibility("default"))) ns_lr::large_room_api_interface *create_large_room_instance(
		ns_lr::large_room_event_handler *cb,
		ns_log::MLogInterface *plogger);

#elif defined(WIN32) || defined(WIN64)

extern "C" __declspec(dllexport) ns_lr::large_room_api_interface* __stdcall create_large_room_instance(
		ns_lr::large_room_event_handler *cb,
		ns_log::MLogInterface *plogger);
#endif



#endif /* APPS_APOLLO_CHAT_INCLUDE_API_LARGE_ROOM_H_ */
