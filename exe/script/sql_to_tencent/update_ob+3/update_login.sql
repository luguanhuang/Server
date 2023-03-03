ALTER TABLE gateinfo ADD COLUMN `free_ipaddr` varchar(255) NOT NULL DEFAULT '';
ALTER TABLE gateinfo ADD COLUMN `backflow_level` int UNSIGNED NOT NULL DEFAULT 0;

CREATE TABLE IF NOT EXISTS bespeak(
	openid varchar(64) NOT NULL,
	serverid int unsigned NOT NULL,
	PRIMARY KEY(openid, serverid),
	KEY(openid)
) ROW_FORMAT=gcs_dynamic;
