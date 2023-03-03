
CREATE DATABASE IF NOT EXISTS gamelog DEFAULT CHARACTER SET utf8mb4 DEFAULT COLLATE utf8mb4_general_ci;
CREATE DATABASE IF NOT EXISTS tlog DEFAULT CHARACTER SET utf8mb4 DEFAULT COLLATE utf8mb4_general_ci;


USE gamelog;

CREATE TABLE IF NOT EXISTS account (
	account varchar(64) NOT NULL, 
	action  varchar(128) ,
	ctime timestamp DEFAULT CURRENT_TIMESTAMP,
	logintime timestamp,
	logouttime timestamp,
	PRIMARY KEY (account)) ROW_FORMAT=DYNAMIC;

CREATE TABLE IF NOT EXISTS roleaction (
	roleid bigint NOT NULL, 
	rolename char(64) NOT NULL,
	action varchar(128) NOT NULL, 
	level int,
	param varchar(128),
	param2 varchar(128),
	ctime timestamp DEFAULT CURRENT_TIMESTAMP) ROW_FORMAT=DYNAMIC;

CREATE TABLE IF NOT EXISTS rolestate (
	roleid bigint NOT NULL, 
	rolename char(64),
	account varchar(64),
	level int,
	profession int,
	powerpoint int,
	friendnum int,
	serverid int,
	ctime timestamp  DEFAULT CURRENT_TIMESTAMP,
	logintime timestamp,
	logouttime timestamp,
	PRIMARY KEY (roleid)) ROW_FORMAT=DYNAMIC;

CREATE TABLE IF NOT EXISTS battle (
	battleid bigint,
	roleid bigint, 
	rolename char(64),
	rolelevel smallint, 
	profession smallint, 
	sceneid int, 
	entertime timestamp, 
	leavetime timestamp, 
	winorloss char(8), 
	rank smallint, 
	reward varchar(1024)) ROW_FORMAT=DYNAMIC;

CREATE TABLE IF NOT EXISTS operaterecord (
  account varchar(64),
	roleid bigint, 
	rolename char(64),
	position int(11),
	arg varchar(256),
	time timestamp DEFAULT CURRENT_TIMESTAMP) ROW_FORMAT=DYNAMIC;

CREATE TABLE IF NOT EXISTS moneychange (
	roleid bigint, 
	type   smallint,
	action smallint,
	amount int,
	method int,
	time timestamp DEFAULT CURRENT_TIMESTAMP
) ROW_FORMAT=DYNAMIC;

CREATE TABLE IF NOT EXISTS auction_log(
	_id bigint NOT NULL AUTO_INCREMENT PRIMARY KEY, 
	serverid int NOT NULL,
    operuserid bigint(20) NOT NULL,
	auctuserid bigint(20) NOT NULL,
	auctionid bigint(20) NOT NULL,
	time timestamp NOT NULL,
	type int NOT NULL,
	para varchar(64)
) ROW_FORMAT=DYNAMIC;

CREATE TABLE IF NOT EXISTS `ccu_log` (
  `dDateTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '采集时间 yyyy-mm-dd hh:ii:ss',
  `iCCU` int(11) NOT NULL DEFAULT '0' COMMENT '同时在线人数',
  `iChannelID` int(11) NOT NULL DEFAULT '0' COMMENT '区ID',
  `vPlatform` varchar(32) NOT NULL COMMENT '用户平台',
  KEY `oper_channel_idx` (`iChannelID`,`dDateTime`) USING BTREE,
  KEY `time` (`dDateTime`) USING BTREE,
  KEY `platform` (`vPlatform`) USING BTREE
) ENGINE=InnoDB COMMENT='CCU日志' ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `user_login_log` (
  `vAccountName` varchar(16) NOT NULL COMMENT '玩家的帐户名',
  `iRoleID` int(11) NOT NULL COMMENT '玩家角色ID',
  `vRoleName` varchar(255) DEFAULT NULL COMMENT '玩家角色名',
  `iClassID` int(11) DEFAULT NULL COMMENT '角色职业/种类 ID',
  `iRoleLevel` smallint(6) DEFAULT '0' COMMENT '玩家角色等级',
  `vIP` varchar(255) DEFAULT NULL COMMENT '玩家IP地址',
  `dDateTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '登陆时间 yyyy-mm-dd hh:ii:ss',
  `iChannelID` int(11) NOT NULL DEFAULT '0' COMMENT '区ID',
  `vDeviceID` varchar(64) DEFAULT NULL COMMENT '设备唯一标示',
  `vDeviceModel` varchar(255) DEFAULT NULL COMMENT '设备型号',
  `vDeviceOS` varchar(255) DEFAULT NULL COMMENT '设备操作系统',
  `vDeviceRatio` varchar(255) DEFAULT NULL COMMENT '设备分辨率',
  `vDeviceNet` varchar(255) DEFAULT NULL COMMENT '设备联网方式（2G 3G 4G WIFI）',
  `vDeviceCarrier` varchar(255) DEFAULT NULL COMMENT '设备运营商',
  `vAccountType` varchar(255) DEFAULT NULL COMMENT '用户账户类型',
  `vAccountSource` varchar(255) DEFAULT NULL COMMENT '用户来源渠道',
  `vPlatform` varchar(32) NOT NULL COMMENT '平台',
  KEY `oper_uin_idx` (`iRoleID`,`iChannelID`,`dDateTime`),
  KEY `dDateTime` (`dDateTime`),
  KEY `vPlatform` (`vPlatform`),
  KEY `iChannelID` (`iChannelID`),
  KEY `iRoleID` (`iRoleID`),
  KEY `vDeviceID` (`vDeviceID`)
) ENGINE=InnoDB COMMENT='玩家登陆信息日志' ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `user_logout_log` (
  `vAccountName` varchar(16) NOT NULL COMMENT '玩家的帐户名',
  `iRoleID` int(11) NOT NULL COMMENT '玩家角色ID',
  `vRoleName` varchar(255) DEFAULT NULL COMMENT '玩家角色名',
  `iClassID` int(11) DEFAULT NULL COMMENT '角色职业/种类 ID',
  `iRoleLevel` smallint(6) DEFAULT '0' COMMENT '玩家角色等级',
  `vIP` varchar(255) DEFAULT NULL COMMENT '玩家IP地址',
  `dLogoutTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '玩家下线时间 yyyy-mm-dd hh:ii:ss',
  `dLoginTime` datetime DEFAULT '0000-00-00 00:00:00' COMMENT '上一次登陆时间 yyyy-mm-dd hh:ii:ss',
  `iChannelID` int(11) NOT NULL DEFAULT '0' COMMENT '区ID',
  `vDeviceID` varchar(64) DEFAULT NULL COMMENT '设备唯一标示',
  `vDeviceModel` varchar(255) DEFAULT NULL COMMENT '设备型号',
  `vDeviceOS` varchar(255) DEFAULT NULL COMMENT '设备操作系统',
  `vDeviceRatio` varchar(255) DEFAULT NULL COMMENT '设备分辨率',
  `vDeviceNet` varchar(255) DEFAULT NULL COMMENT '设备联网方式（2G 3G 4G WIFI）',
  `vDeviceCarrier` varchar(255) DEFAULT NULL COMMENT '设备运营商',
  `vAccountType` varchar(255) DEFAULT NULL COMMENT '用户账户类型',
  `vAccountSource` varchar(255) DEFAULT NULL COMMENT '用户来源渠道',
  `vPlatform` varchar(32) NOT NULL COMMENT '用户平台',
  KEY `oper_uin_idx` (`iRoleID`,`iChannelID`,`dLogoutTime`),
  KEY `dLogoutTime` (`dLogoutTime`),
  KEY `iRoleID` (`iRoleID`),
  KEY `vAccountName` (`vAccountName`),
  KEY `vDeviceID` (`vDeviceID`),
  KEY `vPlatform` (`vPlatform`) USING BTREE,
  KEY `dlogintime` (`dLoginTime`) USING BTREE
) ENGINE=InnoDB COMMENT='用户登出信息日志' ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `user_register_log` (
  `vAccountName` varchar(16) NOT NULL COMMENT '玩家的帐户名',
  `iRoleID` int(11) NOT NULL COMMENT '玩家角色ID',
  `vRoleName` varchar(255) DEFAULT NULL COMMENT '玩家角色名',
  `iClassID` int(11) DEFAULT NULL COMMENT '角色职业/种类 ID',
  `iRoleLevel` smallint(6) DEFAULT '0' COMMENT '玩家角色等级',
  `vIP` varchar(255) DEFAULT NULL COMMENT '玩家IP地址',
  `dDateTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '建造角色的时间 yyyy-mm-dd hh:ii:ss',
  `iChannelID` int(11) NOT NULL DEFAULT '0' COMMENT '区ID',
  `vDeviceID` varchar(64) DEFAULT NULL COMMENT '设备唯一标示',
  `vDeviceModel` varchar(255) DEFAULT NULL COMMENT '设备型号',
  `vDeviceOS` varchar(255) DEFAULT NULL COMMENT '设备操作系统',
  `vDeviceRatio` varchar(255) DEFAULT NULL COMMENT '设备分辨率',
  `vDeviceNet` varchar(255) DEFAULT NULL COMMENT '设备联网方式（2G 3G 4G WIFI）',
  `vDeviceCarrier` varchar(255) DEFAULT NULL COMMENT '设备运营商',
  `vAccountType` varchar(255) DEFAULT NULL COMMENT '用户账户类型',
  `vAccountSource` varchar(255) DEFAULT NULL COMMENT '用户来源渠道',
  `vPlatform` varchar(32) NOT NULL COMMENT '用户平台',
  `iDeviceOSType` tinyint(1) DEFAULT NULL COMMENT '设备操作系统类型（1-安卓，2-ios，3-wp）',
  `vPk` varchar(60) DEFAULT NULL COMMENT '广告主推广应用包名',
  `vMac` varchar(100) DEFAULT NULL COMMENT 'mac地址',
  `vIdfa` varchar(100) DEFAULT NULL COMMENT 'ios专用',
  KEY `dDatetime` (`dDateTime`) USING BTREE,
  KEY `vDeviceID` (`vDeviceID`) USING HASH,
  KEY `vAccountName` (`vAccountName`) USING HASH,
  KEY `idfa` (`vIdfa`),
  KEY `rid` (`iRoleID`) USING BTREE,
  KEY `pf` (`vPlatform`) USING BTREE,
  KEY `group` (`iChannelID`),
  KEY `vMac` (`vMac`)
) ENGINE=InnoDB COMMENT='新增用户信息日志' ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `user_effective_log` (
  `vAccountName` varchar(16) NOT NULL COMMENT '玩家的帐户名',
  `iRoleID` int(11) NOT NULL COMMENT '玩家角色ID',
  `vRoleName` varchar(255) DEFAULT NULL COMMENT '玩家角色名',
  `iClassID` int(11) DEFAULT NULL COMMENT '角色职业/种类 ID',
  `iRoleLevel` smallint(6) DEFAULT '0' COMMENT '玩家角色等级',
  `vIP` varchar(255) DEFAULT NULL COMMENT '玩家IP地址',
  `dDateTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '建造角色的时间 yyyy-mm-dd hh:ii:ss',
  `iChannelID` int(11) NOT NULL DEFAULT '0' COMMENT '区ID',
  `vDeviceID` varchar(64) DEFAULT NULL COMMENT '设备唯一标示',
  `vDeviceModel` varchar(255) DEFAULT NULL COMMENT '设备型号',
  `vDeviceOS` varchar(255) DEFAULT NULL COMMENT '设备操作系统',
  `vDeviceRatio` varchar(255) DEFAULT NULL COMMENT '设备分辨率',
  `vDeviceNet` varchar(255) DEFAULT NULL COMMENT '设备联网方式（2G 3G 4G WIFI）',
  `vDeviceCarrier` varchar(255) DEFAULT NULL COMMENT '设备运营商',
  `vAccountType` varchar(255) DEFAULT NULL COMMENT '用户账户类型',
  `vAccountSource` varchar(255) DEFAULT NULL COMMENT '用户来源渠道',
  `vPlatform` varchar(32) NOT NULL COMMENT '用户平台',
  `iDeviceOSType` tinyint(1) DEFAULT NULL COMMENT '设备操作系统类型（1-安卓，2-ios，3-wp）',
  `vPk` varchar(60) DEFAULT NULL COMMENT '广告主推广应用包名',
  `vMac` varchar(30) DEFAULT NULL COMMENT 'mac地址',
  `vIdfa` varchar(30) DEFAULT NULL COMMENT 'ios专用',
  KEY `oper_uin_idx` (`iRoleID`,`iChannelID`,`dDateTime`),
  KEY `dDateTime` (`dDateTime`),
  KEY `vPlatform` (`vPlatform`),
  KEY `iChannelID` (`iChannelID`),
  KEY `vDeviceID` (`vDeviceID`),
  KEY `iRoleID` (`iRoleID`)
) ENGINE=InnoDB COMMENT='有效新增用户信息日志' ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `user_strong_effective_log` (
  `vAccountName` varchar(16) NOT NULL COMMENT '玩家的帐户名',
  `iRoleID` int(11) NOT NULL COMMENT '玩家角色ID',
  `vRoleName` varchar(255) DEFAULT NULL COMMENT '玩家角色名',
  `iClassID` int(11) DEFAULT NULL COMMENT '角色职业/种类 ID',
  `iRoleLevel` smallint(6) DEFAULT '0' COMMENT '玩家角色等级',
  `vIP` varchar(255) DEFAULT NULL COMMENT '玩家IP地址',
  `dDateTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '建造角色的时间 yyyy-mm-dd hh:ii:ss',
  `iChannelID` int(11) NOT NULL DEFAULT '0' COMMENT '区ID',
  `vDeviceID` varchar(64) DEFAULT NULL COMMENT '设备唯一标示',
  `vDeviceModel` varchar(255) DEFAULT NULL COMMENT '设备型号',
  `vDeviceOS` varchar(255) DEFAULT NULL COMMENT '设备操作系统',
  `vDeviceRatio` varchar(255) DEFAULT NULL COMMENT '设备分辨率',
  `vDeviceNet` varchar(255) DEFAULT NULL COMMENT '设备联网方式（2G 3G 4G WIFI）',
  `vDeviceCarrier` varchar(255) DEFAULT NULL COMMENT '设备运营商',
  `vAccountType` varchar(255) DEFAULT NULL COMMENT '用户账户类型',
  `vAccountSource` varchar(255) DEFAULT NULL COMMENT '用户来源渠道',
  `vPlatform` varchar(32) NOT NULL COMMENT '用户平台',
  `iDeviceOSType` tinyint(1) DEFAULT NULL COMMENT '设备操作系统类型（1-安卓，2-ios，3-wp）',
  `vPk` varchar(60) DEFAULT NULL COMMENT '广告主推广应用包名',
  `vMac` varchar(30) DEFAULT NULL COMMENT 'mac地址',
  `vIdfa` varchar(30) DEFAULT NULL COMMENT 'ios专用',
  KEY `oper_uin_idx` (`iRoleID`,`iChannelID`,`dDateTime`),
  KEY `dDateTime` (`dDateTime`),
  KEY `vPlatform` (`vPlatform`),
  KEY `iChannelID` (`iChannelID`),
  KEY `vDeviceID` (`vDeviceID`),
  KEY `iRoleID` (`iRoleID`)
) ENGINE=InnoDB COMMENT='有效新增用户信息日志' ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `pay_log` (
  `vAccountName` varchar(16) NOT NULL COMMENT '玩家的帐户名',
  `iRoleID` int(11) NOT NULL COMMENT '玩家角色ID',
  `vRoleName` varchar(100) DEFAULT NULL COMMENT '玩家角色名',
  `iClassID` int(11) DEFAULT NULL COMMENT '角色职业/种类 ID',
  `iRoleLevel` smallint(6) DEFAULT '0' COMMENT '玩家角色等级',
  `vIP` varchar(255) DEFAULT NULL COMMENT '玩家IP地址',
  `dDateTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '获得时间 yyyy-mm-dd hh:ii:ss',
  `iMoney` int(11) DEFAULT NULL COMMENT '玩家获得货币数量',
  `iPreMoney` int(11) unsigned DEFAULT NULL COMMENT '交易前货币数量',
  `iAftMoney` int(11) unsigned DEFAULT NULL COMMENT '交易后货币数量',
  `iTotalOnlineTime` int(11) unsigned DEFAULT '0' COMMENT '玩家总在线时长--单位（秒）',
  `iCumLoginDays` int(11) unsigned DEFAULT '0' COMMENT '玩家登陆天数',
  `iExtraGiveMoney` int(11) unsigned DEFAULT '0' COMMENT 'iExtraGiveMoney',
  `iChannelID` int(11) NOT NULL DEFAULT '0' COMMENT '区ID',
  `iError` tinyint(4) DEFAULT NULL COMMENT '0 – 成功, 1 – 失败',
  `vAccountSource` varchar(255) DEFAULT NULL COMMENT '用户来源渠道',
  `vChargeType` varchar(255) DEFAULT NULL COMMENT '玩家充值包类型',
  `vPlatform` varchar(32) NOT NULL COMMENT '用户平台',
  `vOrderID` varchar(255) DEFAULT NULL COMMENT '订单号',
  `vSerialID` varchar(255) DEFAULT NULL COMMENT '流水号',
  `iProductID` int(11) DEFAULT NULL COMMENT '产品ID',
  KEY `oper_uin_idx` (`iRoleID`,`iChannelID`,`dDateTime`),
  KEY `iRoleID` (`iRoleID`) USING BTREE,
  KEY `vAccountName` (`vAccountName`) USING BTREE,
  KEY `pf` (`vPlatform`) USING BTREE,
  KEY `rname` (`vRoleName`) USING BTREE,
  KEY `time` (`dDateTime`) USING BTREE
) ENGINE=InnoDB COMMENT='充值日志' ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `user_upgrade_log` (
  `vAccountName` varchar(16) NOT NULL COMMENT '玩家的帐户名',
  `iRoleID` int(11) NOT NULL COMMENT '玩家角色ID',
  `vRoleName` varchar(255) DEFAULT NULL COMMENT '玩家角色名',
  `iClassID` int(11) DEFAULT NULL COMMENT '角色职业/种类 ID',
  `iRoleLevel` smallint(6) DEFAULT '0' COMMENT '玩家角色等级',
  `vIP` varchar(255) DEFAULT NULL COMMENT '玩家IP地址',
  `dDateTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '行为时间',
  `iRoleExp` int(11) unsigned DEFAULT '0' COMMENT '角色经验值',
  `iTotalOnlineTime` int(11) unsigned DEFAULT NULL COMMENT '玩家总在线时长',
  `iUpgradeTime` int(11) unsigned DEFAULT NULL COMMENT '本次升级耗时',
  `vReason` varchar(255) DEFAULT NULL COMMENT '升级的原因',
  `iMoneyTotal` int(11) unsigned DEFAULT '0' COMMENT '付费金额',
  `iMoneyTime` int(11) DEFAULT '0' COMMENT '付费次数',
  `iChannelID` int(11) NOT NULL DEFAULT '0' COMMENT '区ID',
  `vPlatform` varchar(32) NOT NULL COMMENT '用户平台',
  `vAccountSource` varchar(32) DEFAULT NULL COMMENT '用户渠道来源',
  KEY `oper_uin_idx` (`iRoleID`,`iChannelID`,`dDateTime`),
  KEY `dDateTime` (`dDateTime`),
  KEY `iRoleID` (`iRoleID`),
  KEY `vPlatform` (`vPlatform`) USING BTREE,
  KEY `iRolelevel` (`iRoleLevel`) USING BTREE,
  KEY `p_r` (`vPlatform`,`iRoleID`) USING BTREE
) ENGINE=InnoDB COMMENT='角色升级日志' ROW_FORMAT=DYNAMIC;




USE tlog;

CREATE TABLE IF NOT EXISTS `GameSvrState` (
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)时间 yyyy-mm-dd hh:ii:ss',
  `vGameIP` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)服务器IP',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',
  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `PlayerRegister` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',

  `ClientVersion` varchar(64) DEFAULT NULL COMMENT '(必填)客户端版本',
  `SystemSoftware` varchar(64) DEFAULT NULL COMMENT '(可选)移动终端操作系统版本',
  `SystemHardware` varchar(64) DEFAULT NULL COMMENT '(必填)移动终端机型',
  `TelecomOper` varchar(64) DEFAULT NULL COMMENT '(必填)运营商',
  `Network` varchar(64) DEFAULT NULL COMMENT '(必填)3G/WIFI/2G',

  `ScreenWidth` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)显示屏宽度',
  `ScreenHight` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)显示屏高度',
  `Density` float NOT NULL DEFAULT '0' COMMENT '(可选)像素密度',
  `RegChannel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)注册渠道',

  `CpuHardware` varchar(64) DEFAULT NULL COMMENT '(可选)cpu类型-频率-核数',
  `Memory` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)内存信息单位M',

  `GLRender` varchar(64) DEFAULT NULL COMMENT '(可选)opengl render信息',
  `GLVersion` varchar(64) DEFAULT NULL COMMENT '(可选)opengl版本信息',
  `DeviceId` varchar(64) DEFAULT NULL COMMENT '(可选)设备ID',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `PlayerLogin` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',
  `Level` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)等级', 
  `PlayerFriendsNum` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)玩家好友数量', 

  `ClientVersion` varchar(64) DEFAULT NULL COMMENT '(必填)客户端版本',
  `SystemSoftware` varchar(64) DEFAULT NULL COMMENT '(可选)移动终端操作系统版本',
  `SystemHardware` varchar(64) DEFAULT NULL COMMENT '(必填)移动终端机型',
  `TelecomOper` varchar(64) DEFAULT NULL COMMENT '(必填)运营商',
  `Network` varchar(64) DEFAULT NULL COMMENT '(必填)3G/WIFI/2G',

  `ScreenWidth` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)显示屏宽度',
  `ScreenHight` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)显示屏高度',
  `Density` float NOT NULL DEFAULT '0' COMMENT '(可选)像素密度',
  `LoginChannel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)登录渠道',

  `vRoleID` varchar(64) DEFAULT NULL COMMENT '(必填)玩家角色ID',
  `vRoleName` varchar(64) DEFAULT NULL COMMENT '(必填)玩家角色名',
  `CpuHardware` varchar(64) DEFAULT NULL COMMENT '(可选)cpu类型-频率-核数',
  `Memory` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)内存信息单位M',

  `GLRender` varchar(64) DEFAULT NULL COMMENT '(可选)opengl render信息',
  `GLVersion` varchar(64) DEFAULT NULL COMMENT '(可选)opengl版本信息',
  `DeviceId` varchar(64) DEFAULT NULL COMMENT '(可选)设备ID',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `PlayerLogout` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',
  `OnlineTime` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)本次登录在线时间(秒)', 
  `Level` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)等级', 
  `PlayerFriendsNum` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)玩家好友数量', 

  `ClientVersion` varchar(64) DEFAULT NULL COMMENT '(必填)客户端版本',
  `SystemSoftware` varchar(64) DEFAULT NULL COMMENT '(可选)移动终端操作系统版本',
  `SystemHardware` varchar(64) DEFAULT NULL COMMENT '(必填)移动终端机型',
  `TelecomOper` varchar(64) DEFAULT NULL COMMENT '(必填)运营商',
  `Network` varchar(64) DEFAULT NULL COMMENT '(必填)3G/WIFI/2G',

  `ScreenWidth` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)显示屏宽度',
  `ScreenHight` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)显示屏高度',
  `Density` float NOT NULL DEFAULT '0' COMMENT '(可选)像素密度',
  `LoginChannel` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)登录渠道',

  `CpuHardware` varchar(64) DEFAULT NULL COMMENT '(可选)cpu类型-频率-核数',
  `Memory` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)内存信息单位M',

  `GLRender` varchar(64) DEFAULT NULL COMMENT '(可选)opengl render信息',
  `GLVersion` varchar(64) DEFAULT NULL COMMENT '(可选)opengl版本信息',
  `DeviceId` varchar(64) DEFAULT NULL COMMENT '(可选)设备ID',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `MoneyFlow` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',
  `Sequence` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)用于关联一次动作产生多条不同类型的货币流动日志', 
  `Level` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)等级', 
  `AfterMoney` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)动作后的金钱数', 

  `iMoney` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)动作涉及的金钱数',
  `Reason` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)货币流动一级原因',
  `SubReason` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)货币流动二级原因',
  `AddOrReduce` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)增加 0/减少 1',
  `iMoneyType` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)钱的类型MONEYTYPE,其它货币类型参考FAQ文档',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `ItemFlow` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',
  `Level` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)等级', 
  `Sequence` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)用于关联一次动作产生多条不同类型的货币流动日志', 

  `iGoodsType` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)道具类型', 
  `iGoodsId` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)道具ID',
  `Count` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)数量',
  `AfterCount` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)动作后的物品存量',

  `Reason` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)道具流动一级原因',
  `SubReason` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)道具流动二级原因',
  `iMoney` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)花费代币或金币购买道具情况下输出消耗的钱数量，否则填0',
  `iMoneyType` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)钱的类型MONEYTYPE,其它货币类型参考FAQ文档',
  `AddOrReduce` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)增加 0/减少 1',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `PlayerExpFlow` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',
  `ExpChange` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)经验变化', 
  `BeforeLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)动作前等级', 
  `AfterLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)动作后等级', 

  `Time` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)升级所用时间(秒)',
  `Reason` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)经验流动一级原因',
  `SubReason` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)经验流动二级原因',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `SnsFlow` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `ActorOpenID` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)动作发起玩家OPENID号',
  `RecNum` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)接收玩家个数', 
  `Count` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)发送的数量', 
  `SNSType` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)交互一级类型,其它说明参考FAQ文档', 
  `SNSSubType` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)交互二级类型',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `RoundFlow` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',
  `BattleID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)关卡id或者副本id[副本ID必须有序]，对局时填0', 
  `BattleType` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)战斗类型 对应BATTLETYPE,其它说明参考FAQ文档', 
  `RoundScore` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)本局分数,无得分的填0',
  `RoundTime` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)对局时长(秒)',

  `Result` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)单局结果，参考 Result',
  `Rank` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)排名',
  `Gold` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)金钱',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `GuideFlow` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',
  `iGuideID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)节点ID(节点ID按实际步骤递增编号)', 
  `iLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(可选)等级', 
  `vRoleID` varchar(64) DEFAULT NULL COMMENT '(必填)玩家角色ID',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `VipLevelFlow` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',
  `iLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)玩家等级', 
  `iBeforeVipLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)动作前等级', 
  `iAfterVipLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)动作后等级', 
  `vRoleID` varchar(64) DEFAULT NULL COMMENT '(必填)玩家角色ID',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `ActivityFlow` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',
  `iLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)玩家等级', 
  `iVipLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)VIP等级', 
  `iActivityType` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)活动类型，如充值类的，登录类的，活动为运营过程中做的一些活动', 
  `iActivityID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)活动ID，如首冲6元送XXX活动', 

  `vRoleID` varchar(64) DEFAULT NULL COMMENT '(必填)玩家角色ID',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `TaskFlow` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',
  `iLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)玩家等级', 
  `iVipLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)VIP等级', 
  `iTaskType` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)任务类型', 
  `iTaskID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)任务ID', 
  `iState` int(11) NOT NULL DEFAULT '1' COMMENT '(必填)任务操作状态', 

  `vRoleID` varchar(64) DEFAULT NULL COMMENT '(必填)玩家角色ID',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `LotteryFlow` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',
  `iLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)玩家等级', 
  `iVipLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)VIP等级', 
  `iLotteryType` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)抽取类型 参考iLotteryType，如钻石十连抽，金币抽一次等', 
  `iMoney` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)金额', 
  `iState` int(11) NOT NULL DEFAULT '1' COMMENT '(必填)任务操作状态', 

  `vRoleID` varchar(64) DEFAULT NULL COMMENT '(必填)玩家角色ID',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;


CREATE TABLE IF NOT EXISTS `GuildFlow` (
  `GameSvrId` varchar(25) NOT NULL DEFAULT '' COMMENT '(必填)登录的游戏服务器编号',
  `dtEventTime` datetime NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '(必填)游戏事件时间 yyyy-mm-dd hh:ii:ss',
  `vGameAppid` varchar(32) NOT NULL DEFAULT '' COMMENT '(必填)游戏APPID',
  `PlatID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)ios 0/android 1',
  `iZoneAreaID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)针对分区分服的游戏填写分区id，用来唯一标示一个区；非分区分服游戏请填写0',

  `vopenid` varchar(64) NOT NULL DEFAULT '' COMMENT '(必填)用户OPENID号',
  `iLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)玩家等级', 
  `iVipLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)VIP等级', 
  `iActType` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)操作类型(自动升级时openid可为空)', 
  `iGuildID` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)公会ID', 
  `iGuildLevel` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)公会等级', 
  `iMemberNum` int(11) NOT NULL DEFAULT '0' COMMENT '(必填)操作后公会人数(解散时成员为0)', 

  `vRoleID` varchar(64) DEFAULT NULL COMMENT '(必填)玩家角色ID',

  KEY `idx_zone` (`iZoneAreaID`)
) ROW_FORMAT=DYNAMIC;



USE gamelog;

