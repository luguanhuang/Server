ALTER TABLE role Add COLUMN DragonGroupDB mediumblob DEFAULT NULL;

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
