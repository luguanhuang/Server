CREATE TABLE IF NOT EXISTS custombattle(
	_id bigint NOT NULL,
	custombattle mediumblob DEFAULT NULL,
	ctime timestamp,
	mtime timestamp,
	PRIMARY KEY (_id)
) ROW_FORMAT=gcs_dynamic;

