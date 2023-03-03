CREATE TABLE IF NOT EXISTS mayhem(
	_id int unsigned NOT NULL,
	data mediumblob DEFAULT NULL,
	PRIMARY KEY (_id)
) ROW_FORMAT=gcs_dynamic;