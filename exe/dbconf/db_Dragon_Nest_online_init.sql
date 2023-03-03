
CREATE DATABASE IF NOT EXISTS db_Dragon_Nest_online DEFAULT CHARACTER SET utf8mb4 DEFAULT COLLATE utf8mb4_general_ci;


USE db_Dragon_Nest_online;

CREATE TABLE IF NOT EXISTS `tb_dragonnest_onlinecnt` (
  `gameappid` varchar(32) NOT NULL DEFAULT '' COMMENT '游戏APPID',
  `timekey` int(11) NOT NULL DEFAULT '0' COMMENT '当前时间的时间戳',
  `gsid` int(11) NOT NULL DEFAULT '0' COMMENT '游戏服务器编号',
  `zoneareaid` int(11) NOT NULL DEFAULT '0' COMMENT '分区分服的游戏填写分区id，用来唯一标示一个区',
  `onlinecntios` int(11) NOT NULL DEFAULT '0' COMMENT 'ios在线人数',
  `onlinecntandroid` int(11) NOT NULL DEFAULT '0' COMMENT 'android在线人数',

  PRIMARY KEY (`timekey`, `gameappid`, `gsid`, `zoneareaid`)
) ROW_FORMAT=DYNAMIC;

