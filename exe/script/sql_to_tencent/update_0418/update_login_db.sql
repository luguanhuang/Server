CREATE TABLE IF NOT EXISTS banaccount (
	openid varchar(64) NOT NULL, 
	endtime int NOT NULL,
	reason blob NOT NULL, 
	PRIMARY KEY (openid)
) ROW_FORMAT=gcs_dynamic;
