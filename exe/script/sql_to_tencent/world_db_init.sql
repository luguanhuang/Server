#CREATE DATABASE IF NOT EXISTS globalworld DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;


CREATE TABLE IF NOT EXISTS cross_league_team (
	`_id` bigint NOT NULL,
	`data` blob NOT NULL,
	PRIMARY KEY(`_id`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS cross_rolesummary (
	`_id` bigint NOT NULL,
	`data` blob NOT NULL,
	PRIMARY KEY(`_id`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS keyvalueconfig (
	`_id` int NOT NULL,
	`data` blob DEFAULT NULL,
	PRIMARY KEY(`_id`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS masterid(
	`_id` int NOT NULL,
	PRIMARY KEY(`_id`)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS custombattle(
	_id bigint NOT NULL,
	custombattle mediumblob DEFAULT NULL,
	ctime timestamp,
	mtime timestamp,
	PRIMARY KEY (_id)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS mayhem(
	_id int unsigned NOT NULL,
	data mediumblob DEFAULT NULL,
	PRIMARY KEY (_id)
) ROW_FORMAT=gcs_dynamic;


