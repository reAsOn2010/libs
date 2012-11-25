#ifndef _NOTSQL_CONSTANTS_H_

#include<iostream>
using namespace std;


typedef int TK_TYPE;
typedef string TABLE_NAME;
typedef string FIELD_NAME;

#define _NOTSQL_CONSTANTS_H_

#define NO_SENSE    -2
#define ILLEGAL     -1
#define TK_ID       0
#define TK_REINDEX  1    
#define TK_INDEXED  2    
#define TK_INDEX    3
#define TK_DESC     4
#define TK_ESCAPE   5   
#define TK_EACH     6
#define TK_CHECK    7
#define TK_KEY      8
#define TK_BEFORE   9
#define TK_FOREIGN  10
#define TK_FOR      11
#define TK_IGNORE   12
#define TK_LIKE_KW  13
#define TK_EXPLAIN  14
#define TK_INSTEAD  15
#define TK_ADD      16
#define TK_DATABASE 17
#define TK_AS       18
#define TK_SELECT   19
#define TK_TABLE    20
#define TK_JOIN_KW  21
#define TK_THEN     22
#define TK_END      23
#define TK_DEFERRABLE   24
#define TK_ELSE     25
#define TK_EXCEPT   26
#define TK_TRANSACTION  27
#define TK_ACTION   28
#define TK_ON       29
#define TK_ALTER    31
#define TK_RAISE    32
#define TK_EXCLUSIVE    33
#define TK_EXISTS   34
#define TK_SAVEPOINT    35
#define TK_INTERSECT    36
#define TK_TRIGGER  37
#define TK_REFERENCES   38
#define TK_CONSTRAINT   39
#define TK_INTO     40
#define TK_OFFSET   41
#define TK_OF       42
#define TK_SET      43
#define TK_TEMP     45
#define TK_OR       46
#define TK_UNIQUE   47
#define TK_QUERY    48
#define TK_ATTACH   49
#define TK_HAVING   50
#define TK_GROUP    51
#define TK_UPDATE   52
#define TK_BEGIN    53
#define TK_RELEASE  55
#define TK_BETWEEN  56
#define TK_NOTNULL  57
#define TK_NOT      58
#define TK_NO       59
#define TK_NULL     60
#define TK_CASCADE  62
#define TK_ASC      63
#define TK_DELETE   64
#define TK_CASE     65
#define TK_COLLATE  66
#define TK_CREATE   67
#define TK_DETACH   69
#define TK_IMMEDIATE    70
#define TK_JOIN     71
#define TK_INSERT   72
#define TK_MATCH    73
#define TK_PLAN     74
#define TK_ANALYZE  75
#define TK_PRAGMA   76
#define TK_ABORT    77
#define TK_VALUES   78
#define TK_VIRTUAL  79
#define TK_LIMIT    80
#define TK_WHEN     81
#define TK_WHERE    82
#define TK_RENAME   83
#define TK_AFTER    84
#define TK_REPLACE  85
#define TK_AND      86
#define TK_DEFAULT  87
#define TK_AUTOINCR 88
#define TK_TO       89
#define TK_IN       90
#define TK_CAST     91
#define TK_COLUMNKW 92
#define TK_COMMIT   93
#define TK_CONFLICT 94
#define TK_CTIME_KW 96
#define TK_PRIMARY  98
#define TK_DEFERRED 99
#define TK_DISTINCT 100
#define TK_IS       101
#define TK_DROP     102
#define TK_FAIL     103
#define TK_FROM     104
#define TK_BY       107
#define TK_IF       108
#define TK_ISNULL   109
#define TK_ORDER    110
#define TK_RESTRICT 111
#define TK_ROLLBACK 114
#define TK_ROW      115
#define TK_UNION    116
#define TK_USING    117
#define TK_VACUUM   118
#define TK_VIEW     119
#define TK_INITIALLY    120
#define TK_ALL      121
#define TK_SPACE    122
#define TK_PLUS     123
#define TK_MINUS    124
#define TK_LP       125 //(
#define TK_RP       126 //)
#define TK_SEMI     127 //;
#define TK_STAR     128 //*
#define TK_SLASH    129 //\  ..
#define TK_REM      130 //%
#define TK_EQ       131 //=
#define TK_LE       132 //<=
#define TK_NE       133 //<>
#define TK_LSHIFT   134 //<<
#define TK_LT       135 //<
#define TK_GE       136 //>=
#define TK_RSHIFT   137 //>>
#define TK_GT       138 //>
#define TK_BITOR    139 //|
#define TK_CONCAT   140 //||
#define TK_COMMA    141 //,
#define TK_BITAND   142 //&
#define TK_BITNOT   143 //~
#define TK_STRING   157
#define TK_DOT      145 //.
#define TK_INTEGER  146 
#define TK_FLOAT    147 
#define TK_REGISTER 148
#define TK_BLOB     149
#define TK_DOUBLE   147
#define TK_QUOTE    151 //' or "
#define TK_BACKQUOTE    152 // `
#define TK_LEFT_BRACKET 154
#define TK_RIGHT_BRACKET 155
#define TK_NONE     156
#define TK_CHAR     157 // Same to TK_STRING
#define TK_FIELD    158 
#define TK_USE      159
#define TK_QUIT     160

#define NOTSQL_DEBUG 1
#endif
