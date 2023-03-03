ALTER TABLE role Add COLUMN PlatformShareResult mediumblob DEFAULT NULL;
ALTER TABLE role Add COLUMN WeekEnd4v4Data mediumblob DEFAULT NULL;

CREATE TABLE IF NOT EXISTS backflow(
	level INT UNSIGNED NOT NULL DEFAULT 0,			
	PRIMARY KEY(level)
) ROW_FORMAT=gcs_dynamic;

CREATE TABLE IF NOT EXISTS mobarecord(
	_id bigint NOT NULL,
	record mediumblob DEFAULT NULL,
	PRIMARY KEY(_id)
) ROW_FORMAT=gcs_dynamic;
