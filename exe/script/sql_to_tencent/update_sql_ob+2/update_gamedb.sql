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
  `giftself` blob NOT NULL,
  `gifttarget` blob NOT NULL,
  PRIMARY KEY (`_id`)
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


ALTER TABLE role Add COLUMN MilitaryRecord mediumblob DEFAULT NULL;

CREATE TABLE IF NOT EXISTS rolereport(
	_id bigint NOT NULL,
	data mediumblob DEFAULT NULL,
	PRIMARY KEY(_id)
) ROW_FORMAT=gcs_dynamic;

