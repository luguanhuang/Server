CREATE TABLE IF NOT EXISTS charge_back(
	openid varchar(64) NOT NULL primary key,
	serverid int unsigned,
	roleid bigint unsigned NOT NULL,
	total_pay int unsigned NOT NULL
) ROW_FORMAT=gcs_dynamic;
		
