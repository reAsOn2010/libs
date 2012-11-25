CREATE DATABASE NewDB;
USE NewDB;
CREATE TABLE table1 
( col_1  char(10),
    col_2 int,
    col_3 float,
primary key(col_1));

INSERT INTO table1 
values ( "Hello", 10, 9.99);


INSERT INTO table1
values ( "Hello", 10, 9.99);
