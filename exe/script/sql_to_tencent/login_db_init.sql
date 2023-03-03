#CREATE DATABASE IF NOT EXISTS login DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;


CREATE TABLE IF NOT EXISTS users (
	userid varchar(64) NOT NULL, 
	password varchar(128),
	PRIMARY KEY (userid)) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS lastlogin (
	userid varchar(64) NOT NULL,
	serverid int NOT NULL, 
	time timestamp,
	PRIMARY KEY (userid)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS selfserver (
	userid varchar(64) NOT NULL,
	serverid int NOT NULL, 
	level int NOT NULL, 
	time timestamp,
	PRIMARY KEY (userid, serverid)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS gmaccount(
	_id int NOT NULL AUTO_INCREMENT,
	account varchar(64) NOT NULL,
	PRIMARY KEY (_id)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS iplist(
	_id int NOT NULL AUTO_INCREMENT,
	platform varchar(64) NOT NULL,
	ipbegin varchar(64) NOT NULL,
	ipend varchar(64) NOT NULL,
	PRIMARY KEY (_id),
	KEY (platform, ipbegin, ipend)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS notice(
	`type` int NOT NULL,
	`id` int NOT NULL,
	`isopen` int NOT NULL,
	`areaid` int NOT NULL,
	`platid` int NOT NULL,
	`content` blob NOT NULL,
	`updatetime` int NOT NULL,
	PRIMARY KEY (`type`, `areaid`, `platid`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS gateinfo(
	`server_id` int NOT NULL PRIMARY KEY comment '服务器ID',
	`channel` varchar(64) NOT NULL comment '渠道',
	`state`	int NOT NULL DEFAULT 1 comment '设置状态',
	`is_open` int NOT NULL DEFAULT 0 comment '是否开启',
	`server_name` varchar(255) NOT NULL comment '服务器名称',
	`zone_name` varchar(255) NOT NULL comment '区名称',
	`ipaddr` varchar(255) NOT NULL comment 'ip端口地址',
	`free_ipaddr` varchar(255) NOT NULL DEFAULT ''，
	`register_account` int UNSIGNED NOT NULL DEFAULT 0 comment '总注册帐号数',
	`online_role` int UNSIGNED NOT NULL DEFAULT 0 comment '当前在线角色数',
	`open_time` timestamp NOT NULL DEFAULT 0 comment '开服时间',
	`full_register_time` timestamp NOT NULL DEFAULT 0 comment '首次注册突破上限时间',
	`combine_serverid` int unsigned NOT NULL,
	`backflow_level` int UNSIGNED NOT NULL DEFAULT 0
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS banaccount (
	openid varchar(64) NOT NULL, 
	endtime int NOT NULL,
	reason blob NOT NULL, 
	PRIMARY KEY (openid)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS bespeak(
	openid varchar(64) NOT NULL,
	serverid int unsigned NOT NULL,
	PRIMARY KEY(openid, serverid),
	KEY(openid)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS account_backflow(
	openid varchar(64) NOT NULL,
	expiretime timestamp,
	PRIMARY KEY(openid)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS charge_back(
	openid varchar(64) NOT NULL primary key,
	serverid int unsigned,
	roleid bigint unsigned NOT NULL,
	total_pay int unsigned NOT NULL
) ROW_FORMAT=gcs_dynamic;		







