alter table account add column serverid int unsigned not null DEFAULT 0;


alter table ranklist add column serverid int unsigned not null DEFAULT 0;


alter table role add index(`account`);


CREATE TABLE IF NOT EXISTS serverids(
	`serverid` int UNSIGNED not null,
	PRIMARY KEY(`serverid`)
) ROW_FORMAT=gcs_dynamic;
