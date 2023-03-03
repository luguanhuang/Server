#CREATE DATABASE IF NOT EXISTS fm_db DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;


CREATE TABLE IF NOT EXISTS masterfmid(
	`_id` int NOT NULL,
	PRIMARY KEY(`_id`)
) ROW_FORMAT=gcs_dynamic;


