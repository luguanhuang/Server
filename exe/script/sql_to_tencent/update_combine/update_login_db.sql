alter table gateinfo add column combine_serverid int unsigned;
update gateinfo set combine_serverid=server_id;
