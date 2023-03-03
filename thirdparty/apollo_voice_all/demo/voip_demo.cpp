//2015-12-28 by purperzhang

#define DONT_USE_TSF4G_TLOG
#include<iostream>
#include<string.h>
#include<time.h>
#include<string>
#include<stdlib.h>
#include"api_log.h"
#include"api_log_demo.h"
#include"api_voip.h"
using namespace std;
#define PR(x) cout<<#x<<":"<<x<<endl

class VoipEvent:public ns_voip::VoipEventHandler
{
public:
	ns_voip::VoipInterface*pvi;

	virtual bool	on_create_voiproom_rsp(const ns_voip::CreateVoipRoomResult*pr)
	{
		if(pr)
		{
			cout<<"on_create_voiproom_rsp----->"<<endl;
			PR(pr->async_data);
			PR(pr->async_len);
			PR(pr->room_id);
		}
		{
			ns_voip::JoinVoipRoomPara para;
			para.room_id=pr->room_id;
			strncpy(para.user.client_ip,"255.255.255.255",sizeof(para.user.client_ip));
			para.user.net_type=ns_voip::VOIP_NET_TYPE_DEFAULT;
			strncpy(para.user.open_id,"open_id",sizeof(para.user.open_id));
			para.async_len=snprintf(para.async_data,sizeof(para.async_data),"join_room")+1;
			pvi->send_join_voiproom_req(&para);

		}
		return true;
	}
	virtual	bool	on_join_voiproom_rsp(const ns_voip::JoinVoipRoomResult*pr)
	{
		if(pr)
		{
			cout<<"on_join_voiproom_rsp----->"<<endl;
			PR(pr->async_data);
			PR(pr->async_len);
			PR(pr->room_id);
			PR(pr->user_access.access_ip_count);
			for(int j=0;j<pr->user_access.access_ip_count;j++)
			{
				PR(pr->user_access.access_ip_list[j]);
			}
			PR(pr->user_access.extra_data);
			PR(pr->user_access.member_id);
			PR(pr->user_access.open_id);
			PR(pr->user_access.room_key);
		}
		{
			ns_voip::ExitVoipRoomPara para;
			para.room_id=pr->room_id;
			para.member_id=pr->user_access.member_id;
			strncpy(para.user_open_id,"open_id",sizeof(para.user_open_id));
			para.exit_type=1;
			para.async_len=snprintf(para.async_data,sizeof(para.async_data),"exit room")+1;
			pvi->send_exit_voiproom_req(&para);
		}
		return true;
	}
	virtual bool	on_exit_voiproom_rsp(const ns_voip::ExitVoipRoomResult*pr)
	{
		if(pr)
		{
			cout<<"on exit voiproom_rsp"<<endl;
			PR(pr->room_id);
			PR(pr->member_id);
			PR(pr->user_open_id);
			PR(pr->member_count_after_exit);
			PR(pr->async_len);
			PR(pr->async_data);
		}
		{
			ns_voip::CloseVoipRoomPara para;
			para.room_id=pr->room_id;
			para.close_type=1;
			para.async_len=snprintf(para.async_data,sizeof(para.async_data),"close room");
			pvi->send_close_voiproom_req(&para);
		}
		return true;
	}
	virtual bool	on_close_voiproom_rsp(const ns_voip::CloseVoipRoomResult*pr)
	{
		if(pr)
		{
			cout<<"on close voiproom rsp"<<endl;
			PR(pr->room_id);
			PR(pr->async_len);
			PR(pr->async_data);
		}
		return true;
	}
	virtual bool	on_recv_error(const ns_voip::VoipReturnError*pr)
	{
		if(pr)
		{
			cout<<"on_recv_error----->"<<endl;
			PR(pr->async_data);
			PR(pr->async_len);
			PR(pr->req_cmd);
			PR(pr->error_code);
			PR(pr->error_desc);
		}
		switch(pr->req_cmd)
		{
		case ns_voip::VOIP_CMD_CREATE_ROOM:
			break;
		case ns_voip::VOIP_CMD_JOIN_ROOM:
			break;
		case ns_voip::VOIP_CMD_EXIT_ROOM:
			break;
		case ns_voip::VOIP_CMD_CLOSE_ROOM:
			break;
		default:
			cout<<"unknown command"<<endl;
		}
		return true;
	}
};

int init_voip()
{

	VoipEvent ve;
	ns_log::MLog*pmlog=new ns_log::MLog();
//	pmlog->set_log_level(ns_log::CS_LOG_ERROR);

	ns_voip::VoipInterface* pvi=create_voip_instance(&ve,pmlog);
	if(!pvi)
	{
		cout<<"create_voip_instance error"<<endl;
		return -1;
	}
	ns_voip::VoipConfig config;
	config.dir_svr_url_count=1;
	strncpy(config.dir_svr_url[0],"udp://10.185.12.32:5307",sizeof(config.dir_svr_url[0]));
	strncpy(config.business_name,"apollo.demo_name",sizeof(config.business_name));
	strncpy(config.business_key,"761fc9007d584a7001693a19216efc962b6f46fc3448eb8ece8f898bae9aa20d",sizeof(config.business_key));

	if(!pvi->init(config))
	{
		cout<<"pvi init error"<<endl;
		return -1;
	}

	{
		ns_voip::CreateVoipRoomPara para;
		para.async_len=snprintf(para.async_data,sizeof(para.async_data),"create_room")+1;
		if(!pvi->send_create_voiproom_req(&para))
		{
			cout<<"send_create_voiproom_req error"<<endl;
			return -1;
		}
	}

	ve.pvi=pvi;
	while(true)
	{
		pvi->update();
		//usleep(1);
	}
	delete pmlog;
	delete pvi;
	return 0;
}

