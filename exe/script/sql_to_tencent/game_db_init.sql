#CREATE DATABASE IF NOT EXISTS world1 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;

CREATE TABLE IF NOT EXISTS account(
	_id varchar(64) NOT NULL, 
	lastSelect int   DEFAULT -1, 
	role1 blob DEFAULT NULL, 
	role2 blob DEFAULT NULL, 
	role3 blob DEFAULT NULL, 
	role4 blob DEFAULT NULL, 
	role5 blob DEFAULT NULL, 
	role6 blob DEFAULT NULL, 
	role7 blob DEFAULT NULL, 
	role8 blob DEFAULT NULL, 
	role9 blob DEFAULT NULL, 
	hiddenroles blob DEFAULT NULL, 
	ctime timestamp, 
	mtime timestamp,
	serverid int unsigned not null DEFAULT 0,
	PRIMARY KEY (_id, serverid)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS role (
	_id bigint NOT NULL, 
	name char(64) NOT NULL, 
	account varchar(64),
	ctime timestamp, 
	mtime timestamp, 
	nickid int(11) NOT NULL AUTO_INCREMENT,

	RoleBrief           mediumblob DEFAULT NULL,
	Attribute           mediumblob DEFAULT NULL,
	BagContent          mediumblob DEFAULT NULL,
	RoleLotteryInfo     mediumblob DEFAULT NULL,
	StageInfo           mediumblob DEFAULT NULL,
	CheckinRecord       mediumblob DEFAULT NULL,
	ActivityRecord      mediumblob DEFAULT NULL,
	ArenaRecord         mediumblob DEFAULT NULL,
	RewardRecord        mediumblob DEFAULT NULL,
	BuyGoldFatInfo      mediumblob DEFAULT NULL,
	ShopRecord          mediumblob DEFAULT NULL,
	FlowerRecord        mediumblob DEFAULT NULL,
	GuildRecord         mediumblob DEFAULT NULL,
	PkRecord            mediumblob DEFAULT NULL,
	RoleConfig          mediumblob DEFAULT NULL,
	TShowVoteRecord     mediumblob DEFAULT NULL,
	CampRoleRecord      mediumblob DEFAULT NULL,
	RoleFindBackRecord  mediumblob DEFAULT NULL,
	RoleExtraInfo       mediumblob DEFAULT NULL,
	TowerRecord2DB      mediumblob DEFAULT NULL,
	LoginRecord         mediumblob DEFAULT NULL,
	PvpData             mediumblob DEFAULT NULL,
	SQARecord           mediumblob DEFAULT NULL,
	DragonRecord2DB     mediumblob DEFAULT NULL,
	FashionRecord       mediumblob DEFAULT NULL,
	LiveRecord          mediumblob DEFAULT NULL,
	PayV2Record         mediumblob DEFAULT NULL,
	PetSysData          mediumblob DEFAULT NULL,
	FirstPassRecord     mediumblob DEFAULT NULL,
	IBShopAllRecord     mediumblob DEFAULT NULL,
	SpriteRecord        mediumblob DEFAULT NULL,
	SAtlasRecord        mediumblob DEFAULT NULL,
	RiskInfo2DB         mediumblob DEFAULT NULL,
	RoleTask            mediumblob DEFAULT NULL,
	IdipData            mediumblob DEFAULT NULL,
	SpActivity          mediumblob DEFAULT NULL,
	Designation2DB      mediumblob DEFAULT NULL,
	LevelSealRecord     mediumblob DEFAULT NULL,
	SBuffRecord         mediumblob DEFAULT NULL,
	RolePushInfo        mediumblob DEFAULT NULL,
	QQVipInfo           mediumblob DEFAULT NULL,
	TeamRecord          mediumblob DEFAULT NULL,
	RoleMiscData        mediumblob DEFAULT NULL,
	RolePartnerData     mediumblob DEFAULT NULL,
	AchieveDbInfo       mediumblob DEFAULT NULL,
	SkillRecord         mediumblob DEFAULT NULL,
	SChatRecord         mediumblob DEFAULT NULL,
	HeroBattleRecord    mediumblob DEFAULT NULL,
	ReportDataRecord    mediumblob DEFAULT NULL,
	RoleSystem          mediumblob DEFAULT NULL,
	MilitaryRecord      mediumblob DEFAULT NULL,
	PlatformShareResult mediumblob DEFAULT NULL,
	WeekEnd4v4Data		mediumblob DEFAULT NULL,
	TajieHelp2DB 		mediumblob DEFAULT NULL,
	DragonGroupDB 		mediumblob DEFAULT NULL,
	BattleFieldData 	mediumblob DEFAULT NULL,
	NpcFeelingRecord 	mediumblob DEFAULT NULL,
	CompeteDragonInfo2DB 	mediumblob DEFAULT NULL,
	DragonGuildRecordData	mediumblob DEFAULT NULL,
	
	PRIMARY KEY (_id), 
	INDEX (name),
	INDEX (account),
	KEY(nickid)) AUTO_INCREMENT=10000 ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS rolesummary (
	_id bigint NOT NULL, 
	data blob NOT NULL, 
	PRIMARY KEY (_id)) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS ranklist (
	_id int(11) NOT NULL, 
	data mediumblob NOT NULL, 
	serverid int unsigned not null DEFAULT 0,
	PRIMARY KEY (_id, serverid)) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS globalconfig (
	_id int(11) NOT NULL, 
	data blob DEFAULT NULL, 
	PRIMARY KEY (_id)) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS reward (
	_id bigint NOT NULL,
	roleid bigint NOT NULL,
	rewardinfo blob DEFAULT NULL,
	time timestamp,
	PRIMARY KEY (_id),
	INDEX (roleid)) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS social (
	_id bigint NOT NULL,
	flower blob DEFAULT NULL,
	gaojigu blob DEFAULT NULL,
	tshow blob DEFAULT NULL,
	PRIMARY KEY (_id)) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS friend (
	_id bigint NOT NULL,
	name char(64) NOT NULL,
	friend blob DEFAULT NULL,
	ctime timestamp,
	mtime timestamp,
	PRIMARY KEY (_id)) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS guild (
	_id bigint NOT NULL,
	name char(64) NOT NULL,
	brief mediumblob NOT NULL,
	application mediumblob,
	history mediumblob,
	card mediumblob,
	checkin mediumblob,
	guildbonus mediumblob,
	guildgoblin mediumblob,
	guildactivity mediumblob,
	guildwage mediumblob,
	guildcamp mediumblob,
	garden mediumblob,
	guilddonate mediumblob,
	ctime timestamp,
	mtime timestamp,
	PRIMARY KEY (_id),
	INDEX (name)) ROW_FORMAT=gcs_dynamic;
	
CREATE TABLE IF NOT EXISTS camp (
	_id bigint NOT NULL,
	fortinfo blob NOT NULL,
	ctime timestamp,
	mtime timestamp,
	PRIMARY KEY (_id)) ROW_FORMAT=gcs_dynamic;
	
CREATE TABLE IF NOT EXISTS auctsaleitem(
    _id bigint(20) unsigned NOT NULL PRIMARY KEY,
    roleid bigint(20) NOT NULL,
    roleaccount varchar(128) NOT NULL,
    state int NOT NULL,
    perprice int unsigned NOT NULL,
    reqsaletime int unsigned NOT NULL,
    onsaletime int unsigned NOT NULL,
    outsaletime int unsigned NOT NULL,
    itemdata blob NOT NULL
    ) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS aucttraderecord(
    _id bigint(20) unsigned NOT NULL PRIMARY KEY,
    itemid int unsigned NOT NULL,
    itemcount int unsigned NOT NULL,
    perprice int unsigned NOT NULL,
    attime int unsigned NOT NULL,
    auctuid bigint(20) NOT NULL,
    buyaccount varchar(128) NOT NULL,
    saleaccount varchar(128) NOT NULL
    ) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS guildauctitem(
	_id bigint(20) unsigned NOT NULL PRIMARY KEY,
	alldata blob NOT NULL
	) ROW_FORMAT=gcs_dynamic;
	
CREATE TABLE IF NOT EXISTS mailtemplate (
    	_id bigint NOT NULL PRIMARY KEY,
	mail blob NOT NULL
    ) ROW_FORMAT=gcs_dynamic;
	
CREATE TABLE IF NOT EXISTS mail (
    	_id bigint NOT NULL PRIMARY KEY,
	roleid bigint NOT NULL,
	mail blob NOT NULL,
	INDEX (roleid)
    ) ROW_FORMAT=gcs_dynamic;
   
CREATE TABLE IF NOT EXISTS mailrecord (
    	roleid bigint NOT NULL PRIMARY KEY,
	maxtemplateid bigint NOT NULL
    ) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS chatrecord(
	`_id` bigint(20) NOT NULL AUTO_INCREMENT, 
	`channel` int unsigned NOT NULL default 0,
	`storekey` bigint(20) NOT NULL default 0,
	`chatinfolist` blob NOT NULL,
	PRIMARY KEY(`_id`),
	index(`channel`),
	index(`storekey`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS designationevent(
  `roleid` bigint(20) NOT NULL DEFAULT '0',
  `data` mediumblob,
  PRIMARY KEY (`roleid`)
) ROW_FORMAT=gcs_dynamic;



CREATE TABLE IF NOT EXISTS privatechatlist (
  `_id` bigint(20) NOT NULL,
  `privatechatlist` blob NOT NULL,
  PRIMARY KEY (`_id`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS punishmentlist (
  `account` varchar(64) NOT NULL COMMENT '账号',
  `roleid` bigint(20) NOT NULL COMMENT '角色ID或公会ID',
  `punishtype` int(11) NOT NULL COMMENT '惩罚方式0正常 1封号 2禁言 3临时封禁 4世界boss 5个人公会boss 6天梯 7竞技场 8爬塔 9魅力 10公会 11公会boss',
  `punishdate` int(11) NOT NULL COMMENT '操作的时间',
  `bantime` int(11) NOT NULL COMMENT '处罚时长',
  `reason` blob COMMENT '处罚原因',
  `rankdata` blob,
  PRIMARY KEY (`roleid`,`punishtype`)
) ROW_FORMAT=gcs_dynamic;


CREATE TABLE IF NOT EXISTS garden (  
  `roleid` bigint(20) NOT NULL COMMENT '角色ID',
  `rolename` char(64) NOT NULL COMMENT '角色名称',
  `plant` blob COMMENT '种植信息',
  `cooking` blob COMMENT '烹饪信息',
  `fishing` blob COMMENT '钓鱼信息',
  `banquet` blob COMMENT '宴会信息',
  `eventlog` blob COMMENT '日志信息',  
  PRIMARY KEY (`roleid`)
) ROW_FORMAT=gcs_dynamic;


CREATE TABLE IF NOT EXISTS platformfriendinfo (
	_id varchar(64) NOT NULL,  
	data blob NOT NULL, 
	PRIMARY KEY (_id)) ROW_FORMAT=gcs_dynamic;
	
CREATE TABLE IF NOT EXISTS notice(
	`type` int NOT NULL,
	`id` int NOT NULL,
	`isopen` int NOT NULL,
	`areaid` int NOT NULL,
	`platid` int NOT NULL,
	`content` blob NOT NULL,
	`updatetime` int NOT NULL,
	`isnew` int NOT NULL,
	`title` blob NOT NULL,
	PRIMARY KEY (`type`, `id`, `areaid`, `platid`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS partner (
	`_id` bigint UNSIGNED NOT NULL,
	`base` blob NOT NULL,
	`liveness` blob NOT NULL,
	`shop` blob NOT NULL,
	PRIMARY KEY(`_id`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS pay (
	`roleid` bigint(20) NOT NULL COMMENT '角色ID',
	`saveamt` int(11) not null default '0' comment '累计充值金额的游戏币数量',
	`nosaveamt` int(11) not null default '0' comment '未到账的充值金额的游戏币数量',
	`payotherlist` blob NOT NULL comment '非游戏币直充未到帐的订单信息',
	`consumelist` blob NOT NULL comment '消费游戏币未完成的订单list',
	`sendlist` blob NOT NULL comment '赠送游戏币未完成的订单list',
	PRIMARY KEY(`roleid`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS mentorship (
	`_id` bigint(20) NOT NULL COMMENT '角色ID',
	`data` mediumblob NOT NULL comment '师徒信息',
	PRIMARY KEY(`_id`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS leagueteam (
	`_id` bigint NOT NULL,
	`base` blob NOT NULL,
    `battlerecord` blob NOT NULL,
	PRIMARY KEY(`_id`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS cross_rolesummary (
	`_id` bigint NOT NULL,
	`data` blob NOT NULL,
	PRIMARY KEY(`_id`)
)  ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS openserverdate(
	`_id` int not NULL,
	`date` char(24) NOT NULL,
	PRIMARY KEY(`_id`)
)  ROW_FORMAT=gcs_dynamic;
INSERT INTO openserverdate(`_id`, `date`) VALUES(1,DATE_FORMAT(NOW(),'%Y-%m-%d'));


CREATE TABLE IF NOT EXISTS wxpush(
	`_id` int not NULL,
	`isopen` INT NOT NULL DEFAULT '0' COMMENT '0:否 1：是微信订阅推送服',
	PRIMARY KEY(`_id`)
)  ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS serverids(
	`serverid` int UNSIGNED not null,
	PRIMARY KEY(`serverid`)
) ROW_FORMAT=gcs_dynamic;


CREATE TABLE IF NOT EXISTS payibitem(
	`billno` varchar(32) NOT NULL,
	`fromroleid` bigint(20) NOT NULL DEFAULT '0',
	`toroleid` bigint(20) NOT NULL DEFAULT '0',
	`goodsid` blob NOT NULL,
	`itemid` int unsigned NOT NULL DEFAULT '0',
	`count` int unsigned NOT NULL DEFAULT '0',
	`paydegree` int unsigned NOT NULL DEFAULT '0',
	`text` varchar(64) NOT NULL,
	`ts` int unsigned NOT NULL,
	`status` int NOT NULL,
	PRIMARY KEY(`billno`)
) ROW_FORMAT=gcs_dynamic;


CREATE TABLE IF NOT EXISTS skyteam(
	`_id` bigint NOT NULL,
	`base` blob NOT NULL,
	`record` blob NOT NULL,
	PRIMARY KEY(`_id`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS arenastarroledata(
    `_id` bigint(20) unsigned NOT NULL PRIMARY KEY,
    `data` blob NOT NULL
) ROW_FORMAT=gcs_dynamic;


CREATE TABLE IF NOT EXISTS giftrecord(
	_id bigint NOT NULL, 
  `giftself` blob DEFAULT NULL,
  `gifttarget` blob DEFAULT NULL,
  PRIMARY KEY (`_id`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS custombattle(
	_id bigint NOT NULL,
	custombattle mediumblob DEFAULT NULL,
	ctime timestamp,
	mtime timestamp,
	PRIMARY KEY (_id)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS custombattlerole(
	_id bigint NOT NULL,
	system blob DEFAULT NULL,
	custom blob DEFAULT NULL,
	ctime timestamp,
	mtime timestamp,
	PRIMARY KEY (_id)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS rolereport(
	_id bigint NOT NULL,
	data mediumblob DEFAULT NULL,
	PRIMARY KEY(_id)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS backflow(
	level INT UNSIGNED NOT NULL DEFAULT 0,			
	PRIMARY KEY(level)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS mobarecord(
	_id bigint NOT NULL,
	record mediumblob DEFAULT NULL,
	PRIMARY KEY(_id)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS marriage (
	`_id` bigint(20) NOT NULL COMMENT '角色ID',
	`data` mediumblob NOT NULL comment '婚姻信息',
	PRIMARY KEY(`_id`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS groupchatroleissue (
	`_id` bigint NOT NULL,
	`ctime` int unsigned NOT NULL,
	`data` mediumblob DEFAULT NULL,
	PRIMARY KEY(`_id`),
	index(`ctime`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS groupchatgroupissue (
	`_id` bigint NOT NULL,
	`ctime` int unsigned NOT NULL,
	`data` mediumblob DEFAULT NULL,
	PRIMARY KEY(`_id`),
	index(`ctime`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS groupchatgroup (
	`_id` bigint NOT NULL,
	`ctime` int unsigned NOT NULL,
	`data` mediumblob DEFAULT NULL,
	PRIMARY KEY(`_id`),
	index(`ctime`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS groupchatrole (
	`_id` bigint NOT NULL,
	`ctime` int unsigned NOT NULL,
	`data` mediumblob DEFAULT NULL,
	PRIMARY KEY(`_id`),
	index(`ctime`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS dragonguild (
	`_id` BIGINT NOT NULL,
	`name` CHAR(64) NOT NULL,
	`base` BLOB NOT NULL,
	`liveness` BLOB NOT NULL,
	`shop` BLOB NOT NULL,
	`applications` BLOB NOT NULL,
	`bindinfo` BLOB NOT NULL,
	`task` BLOB NOT NULL,
	`ctime` TIMESTAMP,
	`mtime` TIMESTAMP,
	PRIMARY KEY (`_id`),
	INDEX (`ctime`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS goalawards (
	`_id` bigint NOT NULL,
	`data` mediumblob DEFAULT NULL,
	PRIMARY KEY(`_id`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS roletask (
	`_id` bigint UNSIGNED NOT NULL,
	`data` mediumblob NOT NULL,
	PRIMARY KEY(`_id`)
) ROW_FORMAT=gcs_dynamic;
