drop table ulord_master;
create table   ulord_master(
addr char(50) not null primary key,
amount bigint NOT NULL DEFAULT '0',
txid       char(100) null,
hostname   char(50) NULL DEFAULT ' ',
ip         char(50) NULL DEFAULT ' ',
disksize     int NOT NULL DEFAULT '0',
netsize      int NOT NULL DEFAULT '0',
cpusize      int NOT NULL DEFAULT '0',
ramsize      int NOT NULL DEFAULT '0',
score        int NOT NULL DEFAULT '0',
revser1    char(50) NULL ,
revser2    char(50) NULL ,
revser3    char(50) NULL ,
revser4    char(50) NULL ,
index(hostname)
);
