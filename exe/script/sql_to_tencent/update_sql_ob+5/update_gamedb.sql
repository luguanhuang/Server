ALTER TABLE role Add COLUMN BattleFieldData mediumblob DEFAULT NULL;
ALTER TABLE role Add COLUMN NpcFeelingRecord mediumblob DEFAULT NULL;
ALTER TABLE role Add COLUMN CompeteDragonInfo2DB mediumblob DEFAULT NULL;
ALTER TABLE role ADD COLUMN DragonGuildRecordData mediumblob DEFAULT NULL;

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
