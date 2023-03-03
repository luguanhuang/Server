CREATE TABLE IF NOT EXISTS account_backflow(
	openid varchar(64) NOT NULL,
	expiretime timestamp,
	PRIMARY KEY(openid)
) ROW_FORMAT=gcs_dynamic;