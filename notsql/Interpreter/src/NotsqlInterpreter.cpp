#include <iostream>
#include <fstream>
#include <string>
#include "NotsqlInterpreter.h" 
#include "NotsqlExpr.h"
#include "NotsqlTable.h"
#include "NotsqlCatalogManager.h"


using namespace std; 
const char NotsqlInterpreter::zText[544] = { 
        'R','E','I','N','D','E','X','E','D','E','S','C','A','P','E','A','C','H',
        'E','C','K','E','Y','B','E','F','O','R','E','I','G','N','O','R','E','G',
        'E','X','P','L','A','I','N','S','T','E','A','D','D','A','T','A','B','A',
        'S','E','L','E','C','T','A','B','L','E','F','T','H','E','N','D','E','F',
        'E','R','R','A','B','L','E','L','S','E','X','C','E','P','T','R','A','N',
        'S','A','C','T','I','O','N','A','T','U','R','A','L','T','E','R','A','I',
        'S','E','X','C','L','U','S','I','V','E','X','I','S','T','S','A','V','E', 
        'P','O','I','N','T','E','R','S','E','C','T','R','I','G','G','E','R','E',
        'F','E','R','E','N','C','E','S','C','O','N','S','T','R','A','I','N','T',
        'O','F','F','S','E','T','E','M','P','O','R','A','R','Y','U','N','I','Q',
        'U','E','R','Y','A','T','T','A','C','H','A','V','I','N','G','R','O','U',
        'P','D','A','T','E','B','E','G','I','N','N','E','R','E','L','E','A','S',
        'E','B','E','T','W','E','E','N','O','T','N','U','L','L','I','K','E','C',
        'A','S','C','A','D','E','L','E','T','E','C','A','S','E','C','O','L','L',
        'A','T','E','C','R','E','A','T','E','C','U','R','R','E','N','T','_','D',
        'A','T','E','D','E','T','A','C','H','I','M','M','E','D','I','A','T','E',
        'J','O','I','N','S','E','R','T','M','A','T','C','H','P','L','A','N','A',
        'L','Y','Z','E','P','R','A','G','M','A','B','O','R','T','V','A','L','U',
        'E','S','V','I','R','T','U','A','L','I','M','I','T','W','H','E','N','W',
        'H','E','R','E','N','A','M','E','A','F','T','E','R','E','P','L','A','C',
        'E','A','N','D','E','F','A','U','L','T','A','U','T','O','I','N','C','R',
        'E','M','E','N','T','C','A','S','T','C','O','L','U','M','N','C','O','M',
        'M','I','T','C','O','N','F','L','I','C','T','C','R','O','S','S','C','U',
        'R','R','E','N','T','_','T','I','M','E','S','T','A','M','P','R','I','M',
        'A','R','Y','D','E','F','E','R','R','E','D','I','S','T','I','N','C','T',
        'D','R','O','P','F','A','I','L','F','R','O','M','F','U','L','L','G','L',
        'O','B','Y','I','F','I','S','N','U','L','L','O','R','D','E','R','E','S',
        'T','R','I','C','T','O','U','T','E','R','I','G','H','T','R','O','L','L',
        'B','A','C','K','R','O','W','U','N','I','O','N','U','S','I','N','G','V',
        'A','C','U','U','M','V','I','E','W','I','N','I','T','I','A','L','L','Y',
        'C','H','A','R',
};

/*
* The index table
*/
const unsigned char NotsqlInterpreter::aHash[127] = {
        72, 101, 114,  70,   0,  45,   0,   0,  78,   0,  73,   0,   0,
        42,  12,  74,  15,   0, 113,  81,  50, 108,   0,  19,   0,   0,
        118,   0, 116, 111,   0,  22,  89,   0,   9,   0,   0,  66,  67,
        0,  65,   6,   0,  48,  86,  98,   0, 115,  97,   0,   0,  44,
        0,  99,  24,   0,  17,   0, 119,  49,  23,   0,   5, 106,  25,
        92,   0,   0, 121, 102,  56, 120,  53,  28,  51,   0,  87,   0,
        96,  26,   0,  95,   0,   0,   0,  91,  88,  93,  84, 105,  14,
        39, 104,   0,  77, 122,  18,  85, 107,  32,   0, 117,  76, 109,
        58,  46,  80,   0,   0,  90,  40,   0, 112,   0,  36,   0,   0,
        29,   0,  82,  59,  60,   0,  20,  57,   0,  52,
};

/*
* Find the next index once collision occurs
*/
const unsigned char NotsqlInterpreter::aNext[121] = {
        0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   2,   0,   0,   0,   0,   0,   0,  13,   0,   0,   0,   0,
        0,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,  33,   0,  21,   0,   0,   0,  43,   3,  47,
        0,   0,   0,   0,  30,   0,  54,   0,  38,   0,   0,   0,   1,
        62,   0,   0,  63,   0,  41,   0,   0,   0,   0,   0,   0,   0,
        61,   0,   0,   0,   0,  31,  55,  16,  34,  10,   0,   0,   0,
        0,   0,   0,   0,  11,  68,  75,   0,   8,   0, 100,  94,   0,
        103,   0,  83,   0,  71,   0,   0, 110,  27,  37,  69,  79,   0,
        35,  64,   0,   0,
};

/*
const unsigned char NotsqlInterpreter::aNext[121] = {
        0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   2,   0,   0,   0,   0,   0,   0,  13,   0,   0,   0,   0,
        0,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,  33,   0,  21,   0,   0,   0,  43,   3,  47,
        0,   0,   0,   0,  30,   0,  54,   0,  38,   0,   0,   0,   1,
        62,   0,   0,  63,   0,  41,   0,   0,   0,   0,   0,   0,   0,
        61,   0,   0,   0,   0,  31,  55,  16,  34,  10,   0,   0,   0,
        0,   0,   0,   0,  11,  68,  75,   0,   8,   0, 100,  94,   0,
        103,   0,  83,   0,  71,   0,   0, 110,  27,  37,  69,  79,   0,
        35,  64,   0,   0,
};
*/
/*
* Length table
*/
const unsigned char NotsqlInterpreter::aLen[122] = {
        7,   7,   5,   4,   6,   4,   5,   3,   6,   7,   3,   6,   6,
        7,   7,   3,   8,   2,   6,   5,   4,   4,   3,  10,   4,   6,
        11,   6,   2,   7,   5,   5,   9,   6,   9,   9,   7,  10,  10,
        4,   6,   2,   3,   9,   4,   2,   6,   5,   6,   6,   5,   6,
        5,   5,   7,   7,   7,   3,   2,   4,   4,   7,   3,   6,   4,
        7,   6,  12,   6,   9,   4,   6,   5,   4,   7,   6,   5,   6,
        7,   5,   4,   5,   6,   5,   7,   3,   7,  13,   2,   2,   4,
        6,   6,   8,   5,  17,  12,   7,   8,   8,   2,   4,   4,   4,
        4,   4,   2,   2,   6,   5,   8,   5,   5,   8,   3,   5,   5,
        6,   4,   9,   3,
};

/*
const unsigned char NotsqlInterpreter::aLen[122] = {
        7,   7,   5,   4,   6,   4,   5,   3,   6,   7,   3,   6,   6,
        7,   7,   3,   8,   2,   6,   5,   4,   4,   3,  10,   4,   6,
        11,   6,   2,   7,   5,   5,   9,   6,   9,   9,   7,  10,  10,
        4,   6,   2,   3,   9,   4,   2,   6,   5,   6,   6,   5,   6,
        5,   5,   7,   7,   7,   3,   2,   4,   4,   7,   3,   6,   4,
        7,   6,  12,   6,   9,   4,   6,   5,   4,   7,   6,   5,   6,
        7,   5,   4,   5,   6,   5,   7,   3,   7,  13,   2,   2,   4,
        6,   6,   8,   5,  17,  12,   7,   8,   8,   2,   4,   4,   4,
        4,   4,   2,   2,   6,   5,   8,   5,   5,   8,   3,   5,   5,
        6,   4,   9,   3,   4,
};
*/
const unsigned short int NotsqlInterpreter::aOffset[122] = {
        0,   2,   2,   8,   9,  14,  16,  20,  23,  25,  25,  29,  33, 
        36,  41,  46,  48,  53,  54,  59,  62,  65,  67,  69,  78,  81,
        86,  91,  95,  96, 101, 105, 109, 117, 122, 128, 136, 142, 152,
        159, 162, 162, 165, 167, 167, 171, 176, 179, 184, 189, 194, 197,
        203, 206, 210, 217, 223, 223, 223, 226, 229, 233, 234, 238, 244,
        248, 255, 261, 273, 279, 288, 290, 296, 301, 303, 310, 315, 320,
        326, 332, 337, 341, 344, 350, 354, 361, 363, 370, 372, 374, 383,
        387, 393, 399, 407, 412, 412, 428, 435, 442, 443, 450, 454, 458,
        462, 466, 469, 471, 473, 479, 483, 491, 495, 500, 508, 511, 516,
        521, 527, 531, 536, 540,
};

const unsigned char NotsqlInterpreter::aCode[122] = {
        TK_REINDEX,    TK_INDEXED,    TK_INDEX,      TK_DESC,       TK_ESCAPE,     
        TK_EACH,       TK_CHECK,      TK_KEY,        TK_BEFORE,     TK_FOREIGN,    
        TK_FOR,        TK_IGNORE,     TK_LIKE_KW,    TK_EXPLAIN,    TK_INSTEAD,    
        TK_ADD,        TK_DATABASE,   TK_AS,         TK_SELECT,     TK_TABLE,      
        TK_JOIN_KW,    TK_THEN,       TK_END,        TK_DEFERRABLE, TK_ELSE,       
        TK_EXCEPT,     TK_TRANSACTION,TK_ACTION,     TK_ON,         TK_JOIN_KW,    
        TK_ALTER,      TK_RAISE,      TK_EXCLUSIVE,  TK_EXISTS,     TK_SAVEPOINT,  
        TK_INTERSECT,  TK_TRIGGER,    TK_REFERENCES, TK_CONSTRAINT, TK_INTO,       
        TK_OFFSET,     TK_OF,         TK_SET,        TK_TEMP,       TK_TEMP,       
        TK_OR,         TK_UNIQUE,     TK_QUERY,      TK_ATTACH,     TK_HAVING,     
        TK_GROUP,      TK_UPDATE,     TK_BEGIN,      TK_JOIN_KW,    TK_RELEASE,    
        TK_BETWEEN,    TK_NOTNULL,    TK_NOT,        TK_NO,         TK_NULL,       
        TK_LIKE_KW,    TK_CASCADE,    TK_ASC,        TK_DELETE,     TK_CASE,       
        TK_COLLATE,    TK_CREATE,     TK_CTIME_KW,   TK_DETACH,     TK_IMMEDIATE,  
        TK_JOIN,       TK_INSERT,     TK_MATCH,      TK_PLAN,       TK_ANALYZE,    
        TK_PRAGMA,     TK_ABORT,      TK_VALUES,     TK_VIRTUAL,    TK_LIMIT,      
        TK_WHEN,       TK_WHERE,      TK_RENAME,     TK_AFTER,      TK_REPLACE,    
        TK_AND,        TK_DEFAULT,    TK_AUTOINCR,   TK_TO,         TK_IN,         
        TK_CAST,       TK_COLUMNKW,   TK_COMMIT,     TK_CONFLICT,   TK_JOIN_KW,    
        TK_CTIME_KW,   TK_CTIME_KW,   TK_PRIMARY,    TK_DEFERRED,   TK_DISTINCT,   
        TK_IS,         TK_DROP,       TK_FAIL,       TK_FROM,       TK_JOIN_KW,    
        TK_LIKE_KW,    TK_BY,         TK_IF,         TK_ISNULL,     TK_ORDER,      
        TK_RESTRICT,   TK_JOIN_KW,    TK_JOIN_KW,    TK_ROLLBACK,   TK_ROW,        
        TK_UNION,      TK_USING,      TK_VACUUM,     TK_VIEW,       TK_INITIALLY,  
        TK_ALL,        TK_CHAR,
};

const unsigned char NotsqlInterpreter::toUpperTable[256] = {
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
     18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
     36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
     54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
     72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
     90, 91, 92, 93, 94, 95, 96, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
     76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 89, 90,122,123,124,125,
    126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
    144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,
    162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,
    180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,
    198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,
    216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,
    234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,
    252,253,254,255
};

const unsigned char NotsqlInterpreter::toLowerTable[256] = {
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
     18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
     36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
     54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 97, 98, 99,100,101,102,103,
    104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,
     90, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,
    108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,
    126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
    144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,
    162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,
    180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,
    198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,
    216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,
    234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,
    252,253,254,255
};

void NotsqlInterpreter::prepare(){
    index = 0;
    tokens.clear();
}


void NotsqlInterpreter::parseConf(){
    fstream fin("notsql.conf");
    if(!fin.is_open()){
        throw(NotsqlFileException("Cannot find config file."));
    }
    string line;
    getline(fin, line);
    string::iterator it = find(line.begin(), line.end(), '=');
    if(it == line.end()){
        PREFIX = "./";
    }else{
        string new_str = line.substr(it - line.begin() + 1, line.end() - it + 1);
        for(unsigned int i = 0; i != new_str.size(); ++i){
            if(i != ' '){
                PREFIX = new_str.substr(i, new_str.size() - i);
                break;
            }
        }
        if(PREFIX == ""){
            PREFIX = "./";
        }
    }
}

pair<TK_TYPE, string> NotsqlInterpreter::parseWord(){
    string word;
    TK_TYPE type;
    int pre_index = index;
    bool in_word = false;
    for(unsigned int i = index; i != cmd.length(); ++i){
        index++;
        if((!isAlpha(cmd[i]) && !isDigit(cmd[i]) && !isUnderline(cmd[i]))){
            if(in_word){
                //Is it a float number?
                if(cmd[i] == '.' && isNum(word)){
                    word += '.';
                    for(unsigned int ii = i + 1; ii != cmd.length(); ++ii){
                        if(isAlpha(cmd[ii])){
                            index = pre_index;
                            throw(NotsqlParseException(ii));
                        }else if(!isDigit(cmd[ii])){
                            break;
                        }else{
                            word += cmd[ii];
                            index = ii + 1;
                        }
                    }
                    type = TK_FLOAT;
                    break;
                }else{
                    index--;
                    type = getKeywordCode(word);
                    if(type == TK_ID){
                        // It may be an integer
                        if(isNum(word)){
                            type = TK_INTEGER;
                        }
                    }
                    break;
                }
            }else if(isBlank(cmd[i])){
                continue;
            }else{
                char quote;
                bool balance;
                word += cmd[i];
                switch(cmd[i]){
                    case '.':
                        type = TK_DOT;
                        break;
                    case ',': type = TK_COMMA;
                        break;
                    case '\"':
                    case '\'':
                        word = "";
                        quote = cmd[i];
                        balance = false;
                        for(unsigned int ii = i + 1; ii != cmd.length(); ++ii){
                            if(cmd[ii] == quote){
                                index = ii + 1;
                                balance = true;
                                break;
                            }
                            else{
                                word += cmd[ii];
                            }
                        }
                        if(!balance){
                            index = pre_index;
                            throw(NotsqlParseException(index));
                        }
                        type = TK_STRING;
                        break;
                    case '`':
                        type = TK_BACKQUOTE;
                        break;
                    case '=':
                        type = TK_EQ;
                        break;
                    case '<':
                        // Is it <=?
                        if(parseChar('=')){
                            word = "<=";
                            type = TK_LE;
                        // Is it <>?
                        }else if(parseChar('>')){
                            word = "<>";
                            type = TK_NE;
                        }else{
                            word = "<";
                            type = TK_LT;
                        }
                        break;
                    case '>':
                        if(parseChar('=')){
                            word = ">=";
                            type = TK_GE;
                        }else{
                            word = ">";
                            type = TK_GT;
                        }
                        break;
                    case '*':
                        type = TK_STAR;
                        break;
                    case '(':
                        type = TK_LEFT_BRACKET;
                        break;
                    case ')':
                        type = TK_RIGHT_BRACKET;
                        break;
                    case ';':
                        type = TK_NONE;
                        break;
                    default:
                        index = pre_index;
                        throw(NotsqlParseException(pre_index));
                }
                break;
            }
        }else{
            in_word = true;
            word += cmd[i];
        }
    }
    // Reach the end of the command
    if(word.length() == 0){
        type = TK_NONE;
    }
    return pair<TK_TYPE, string>(type, word);
}
    
bool NotsqlInterpreter::parseChar(char ch){
    int pre_index = index;
    for(unsigned int i = index; i != cmd.length(); ++i){
        index++;
        if(isBlank(cmd[i])){
            continue;
        }else if(cmd[i] == ch){
            return true;
        }else{
            break;
        }
    }
    index = pre_index;
    return false;
}

/*
 * if text > word : return 1
 * elif text < word : return -1
 * else return 0;
 */
int NotsqlInterpreter::strcmp_upper(const char * text, string word, int word_len){
    for(int i = 0; i < word_len; i++){
        if(text[i] > toUpper(word[i]))
            return 1;
        else if(text[i] < toUpper(word[i])){
            return -1;
        }
    }
    return 0;
}


/*
 * Determine what keyword the word is 
 */
int NotsqlInterpreter::getKeywordCode(string word){
    int word_len = word.length();
    if(word_len == 0)
        return NO_SENSE;
    if(word_len < 2)
        return TK_ID;
    if(word_len == 3 && strcmp_upper( "INT", word, word_len) == 0){
        return TK_INTEGER;
    }
    if(word_len == 3 && strcmp_upper( "USE", word, word_len) == 0){
        return TK_USE;
    }
    if(word_len == 4 && strcmp_upper( "CHAR", word, word_len) == 0){
        return TK_CHAR;
    }
    if(word_len == 4 && strcmp_upper( "QUIT", word, word_len) == 0){
        return TK_QUIT;
    }
    if(word_len == 5 && strcmp_upper("FLOAT", word, word_len) == 0){
        return TK_FLOAT;
    }
    if(word_len == 6 && strcmp_upper("DOUBLE", word, word_len) == 0){
        return TK_DOUBLE;
    }
    if(word_len == 7 && strcmp_upper("PRIMARY", word, word_len) == 0){
        return TK_PRIMARY;
    }

    int hash_val, i;
    hash_val = ((toLower(word[0]) * 4) ^ 
                (toLower(word[word_len - 1]) * 3) ^ 
                word_len) % 127;

    for(i = ((int)NotsqlInterpreter::aHash[hash_val]) - 1; i >= 0; i = ((int)NotsqlInterpreter::aNext[i]) - 1){
        if( (int)NotsqlInterpreter::aLen[i] == word_len && strcmp_upper( &NotsqlInterpreter::zText[NotsqlInterpreter::aOffset[i]], word, word_len ) == 0 )
            return NotsqlInterpreter::aCode[i];
    }
    return TK_ID;
}

void NotsqlInterpreter::tokenize(){
    pair<TK_TYPE, string> token;
    while(true){
        try{
            token = parseWord();
        }catch(NotsqlParseException e){
            throw e;
        }
        if(token.first == TK_NONE){
            break;
        }else{
            tokens.push_back(token);
        }
    }
}

void NotsqlInterpreter::run(){
    if(tokens.empty()){
        return;
    }

    this->res_table.reset();
    this->res_str.clear();

    bool fail = false;
    bool quit = false;
    TK_TYPE opcode = tokens[0].first;
    switch(opcode){
        case TK_USE:
            parseUse();
            break;
        case TK_SELECT:
            parseSelect();
            break;
        case TK_CREATE:
            parseCreate();
            break;
        case TK_INSERT:
            parseInsert();
            break;
        case TK_DELETE:
            parseDelete();
            break;
        case TK_DROP:
            parseDrop();
            break;
        case TK_QUIT:
            quit = true;
            break;
        default:
            fail = true;
            break;
    }
    if(quit){
        throw(NotsqlQuitException());
    }
    if(fail){
        throw(NotsqlSyntaxException(tokens[0].second, 
                    "Invalid operation"));
    }
}


void NotsqlInterpreter::init(const string & db_name){
    if(!NotsqlCatalogManager::DatabaseIsExist(db_name, PREFIX)){
        throw(NotsqlDBNotExistException(db_name));
    }

    // Delete the previous database session
    if(init_flag){
        delete api;
    }
    init_flag = true;
    api = new NotsqlAPI(db_name, PREFIX);
}

void NotsqlInterpreter::interpret(const string & cmd){
    // Do the cleanup work
    prepare();
    this->cmd = cmd + " ";
    tokenize();
    run();
}



void NotsqlInterpreter::parseUse(){
    unsigned int index = 1;
    if(!expect(index, TK_ID)){
        throw(NotsqlSyntaxException(tokens[index - 1].second, 
                    "Invalid database name"));
    }
    string db_name = tokens[index].second;
    index++;
    if(index != tokens.size()){
        throw(NotsqlSyntaxException(tokens[index - 1].second,
                    "Unexpected end"));
    }
    // Initialize the database
    init(db_name);
}


void NotsqlInterpreter::parseCreate(){
    unsigned int index = 1;

    if(!expect(index, TK_TABLE) && !expect(index, TK_INDEX) && 
            !expect(index, TK_DATABASE)){
        throw NotsqlSyntaxException(tokens[index - 1].second, "");
    }


    // TABLE
    if(tokens[index].first == TK_TABLE){
        if(!init_flag){
            throw(NotsqlNoDBSelectedException());
        }
        string table_name;
        vector<string> col_names;
        vector<TK_TYPE> types;
        vector<int> type_sizes;
        vector<string> primary_keys;
        vector<bool> not_nulls;
        // Create table
        index++;
        if(!expect(index, TK_ID)){
            throw NotsqlSyntaxException(tokens[index - 1].second, 
                    "Invalid table name");
        }

        table_name = tokens[index].second;
        index++;

        // (
        if(!expect(index, TK_LEFT_BRACKET)){
            throw NotsqlSyntaxException(tokens[index - 1].second, "");
        }
        index++;
        TK_TYPE type;
        // The types, if TK_TYPE is char, need a extra size field
        while(true){
            if(expect(index, TK_ID)){
                // It's a new field
                col_names.push_back(tokens[index].second);
                index++;

                if(!expect(index, TK_CHAR) && 
                        !expect(index, TK_FLOAT) &&
                        !expect(index, TK_DOUBLE) &&
                        !expect(index, TK_INTEGER)){
                    throw NotsqlSyntaxException(tokens[index - 1].second, 
                            "Invalid field type");
                }
                type = tokens[index].first;
                index++;
                // Char
                if(type == TK_CHAR){
                    if(!expect(index, TK_LEFT_BRACKET)){
                        throw NotsqlSyntaxException(tokens[index - 1].second, "");
                    }
                    index++;
                    if(!expect(index, TK_INTEGER)){
                        throw NotsqlSyntaxException(tokens[index - 1].second, "");
                    }
                    int sz;
                    try{
                        sz = (int)toNum(tokens[index].second);
                    }catch(NotsqlNotANumException & e){
                        throw NotsqlSyntaxException(tokens[index - 1].second, 
                                e.getMsg());
                    }
                    index++;
                    if(!expect(index, TK_RIGHT_BRACKET)){
                        throw NotsqlSyntaxException(tokens[index - 1].second, 
                                "Unbalance brackets");
                    }
                    index++;
                    types.push_back(type);
                    type_sizes.push_back(sz);
                }else if(type == TK_DOUBLE){
                    types.push_back(type);
                    type_sizes.push_back(8);
                }else{
                    types.push_back(type);
                    type_sizes.push_back(4);
                }

                // NOT NULL
                if(expect(index, TK_NOT)){
                    index++;
                    if(!expect(index, TK_NULL)){
                        throw NotsqlSyntaxException(tokens[index - 1].second, "");
                    }
                    not_nulls.push_back(true);
                    index++;
                }else{
                    not_nulls.push_back(false);
                }
            }else if(expect(index, TK_PRIMARY)){
                // PRIMARY KEY
                index++;
                if(!expect(index, TK_KEY)){
                    throw NotsqlSyntaxException(tokens[index - 1].second, "");
                }
                index++;
                // '('
                if(!expect(index, TK_LEFT_BRACKET)){
                    throw NotsqlSyntaxException(tokens[index - 1].second, "");
                }
                index++;
                while(true){
                    if(!expect(index, TK_ID)){
                        throw NotsqlSyntaxException(tokens[index - 1].second, "");
                    }
                    if(find(col_names.begin(), 
                                col_names.end(),
                                tokens[index].second) == col_names.end()){
                        throw NotsqlSyntaxException(tokens[index - 1].second, 
                                "Undefined field name.");
                    }
                    primary_keys.push_back(tokens[index].second);
                    index++;
                    if(expect(index, TK_COMMA)){
                        index++;
                    }else{
                        break;
                    }
                }
                if(!expect(index, TK_RIGHT_BRACKET)){
                    throw NotsqlSyntaxException(tokens[index - 1].second, 
                            "Unbalance brackets");
                }
                index++;
            }
            if(!expect(index, TK_COMMA)){
                if(!expect(index, TK_RIGHT_BRACKET)){
                    throw NotsqlSyntaxException(tokens[index - 1].second, 
                            "Unbalance brackets");
                }
                break;
            }
            // Meet comma, run next loop
            index++;
        }
        index++;
        if(index != tokens.size()){
            throw NotsqlSyntaxException(tokens[index - 1].second, "");
        }
        api->createTable(
                table_name,
                col_names,
                types,
                type_sizes,
                primary_keys,
                not_nulls
                );
    }else if(tokens[index].first == TK_INDEX){
        if(!init_flag){
            throw(NotsqlNoDBSelectedException());
        }
        string index_name;
        string table_name;
        vector<string> col_names;
        index++;
        // Index name
        if(!expect(index, TK_ID)){
            throw NotsqlSyntaxException(tokens[index - 1].second, "");
        }
        index_name = tokens[index].second;
        index++;
        // ON
        if(!expect(index, TK_ON)){
            throw NotsqlSyntaxException(tokens[index - 1].second, "");
        }
        index++;
        // Table name
        if(!expect(index, TK_ID)){
            throw NotsqlSyntaxException(tokens[index - 1].second, "");
        }
        table_name = tokens[index].second;
        index++;
        // (
        if(!expect(index, TK_LEFT_BRACKET)){
            throw NotsqlSyntaxException(tokens[index - 1].second, "");
        }
        index++;
        while(true){
            // Parse column name
            if(!expect(index, TK_ID)){
                throw NotsqlSyntaxException(tokens[index - 1].second, 
                        "Invalid column name");
            }
            col_names.push_back(tokens[index].second);
            index++;
            if(index >= tokens.size()){
                throw NotsqlSyntaxException(tokens[index - 1].second, "");
            }
            if(expect(index, TK_COMMA)){
                index++;
            }else{
                break;
            }
        }
        // )
        if(!expect(index, TK_RIGHT_BRACKET)){
            throw NotsqlSyntaxException(tokens[index - 1].second, 
                    "Unbalance brackets");
        }
        // END
        if(index != tokens.size() - 1){
            throw NotsqlSyntaxException(tokens[index - 1].second, "");
        }
        // Sorry.. Only create one index one time in this version
        api->createIndex(table_name, index_name, col_names[0]);
    }else if(tokens[index].first == TK_DATABASE){
        string db_name;
        index++;
        if(!expect(index, TK_ID)){
            throw(NotsqlSyntaxException(tokens[index - 1].second,
                        "Invalid database name"));
        }
        db_name = tokens[index].second;
        index++;
        if(index != tokens.size()){
            throw(NotsqlSyntaxException(tokens[index - 1].second,
                        "Unexpected end"));
        }
        if(NotsqlCatalogManager::DatabaseIsExist(db_name, PREFIX)){
            throw(CatalogDatabaseDuplicateError(db_name + " is already existed."));
        }
        NotsqlCatalogManager::CreateDatabase(db_name, PREFIX);
    }
}


void NotsqlInterpreter::parseDrop(){
    unsigned int index = 1;

    if(expect(index, TK_DATABASE)){
        index++;
        if(!expect(index, TK_ID)){
            throw(NotsqlSyntaxException(tokens[index - 1].second,
                        "Invalid database name"));
        }
        string db_name = tokens[index].second;
        init(db_name);
    }

    if(!init_flag){
        throw(NotsqlNoDBSelectedException());
    }
    if(expect(index, TK_TABLE)){
        index++;
        string table_name;
        if(!expect(index, TK_ID)){
            throw(NotsqlSyntaxException(tokens[index - 1].second,
                        "Invalid table name"));
        }
        table_name = tokens[index].second;

        index++;
        if(index != tokens.size()){
            throw(NotsqlSyntaxException(tokens[index - 1].second,
                        "Unexpect end"));
        }
        api->dropTable(table_name);
    }else if(expect(index, TK_INDEX)){
        index++;
        string index_name;
        string table_name;
        if(!expect(index, TK_ID)){
            throw(NotsqlSyntaxException(tokens[index - 1].second,
                        "Invalid index name"));
        }
        index_name = tokens[index].second;

        index++;
        if(!expect(index, TK_FROM)){
            throw(NotsqlSyntaxException(tokens[index - 1].second,
                        "Expect \"FROM\""));
        }
        index++;
        if(!expect(index, TK_ID)){
            throw(NotsqlSyntaxException(tokens[index - 1].second,
                        "Invalid table name"));
        }
        table_name = tokens[index].second;

        index++;
        if(index != tokens.size()){
            throw(NotsqlSyntaxException(tokens[index - 1].second,
                        "Unexpect end"));
        }else{
            api->dropIndex(table_name, index_name);
        }
    }else{
        throw(NotsqlSyntaxException(tokens[index - 1].second, ""));
    }
}


void NotsqlInterpreter::parseSelect(){
    unsigned int index = 1;
    bool distinct = false;
    bool all_fields = false;

    if(!init_flag){
        throw(NotsqlNoDBSelectedException());
    }
    if(tokens.size() <= NOTSQL_MIN_SELECT_LENGTH)
        throw NotsqlSyntaxException(tokens[tokens.size() - 1].second, "Statement too short");

    if(expect(index, TK_DISTINCT)){
        distinct = true;
        index++;
    }

    vector< pair<TABLE_NAME, FIELD_NAME> > fields;

    vector<TABLE_NAME> tables;

    // The expressions may be multi-part
    // Because the OR relation will split them
    vector< vector<NotsqlExpr> > exprs;

    vector<TK_TYPE> rels;

    string tmp_token0;

    if(expect(index, TK_STAR)){
        // Meet '*'
        all_fields = true;
        index++;
    }else{
        // Parse the field names
        while(true){
            if(!expect(index, TK_ID)){
                throw NotsqlSyntaxException(tokens[index - 1].second, "");
            }
            // Table.Field
            if(expect(index + 1, TK_DOT)){
                index += 2;
                if(!expect(index, TK_ID)){
                    throw NotsqlSyntaxException(tokens[index - 1].second, "");
                }
                fields.push_back(
                        pair<TABLE_NAME, FIELD_NAME>(tokens[index - 2].second, 
                            tokens[index].second));
            }else{
                // Field
                fields.push_back(pair<TABLE_NAME, FIELD_NAME>
                        ("", tokens[index].second));
            }
            index++;
            // ,
            if(!expect(index, TK_COMMA)){
                break;
            }else{
                index++;
            }
        }
    }

    // From
    if(!expect(index, TK_FROM)){
        throw NotsqlSyntaxException(tokens[index - 1].second, "");
    }
    index++;
    while(true){
        // Source tables
        if(!expect(index, TK_ID)){
            throw NotsqlSyntaxException(tokens[index - 1].second, "");
        }
        tables.push_back(tokens[index].second);
        index++;
        if(!expect(index, TK_COMMA)){
            break;
        }else{
            index++;
        }
    }

    // Where 
    if(expect(index, TK_WHERE)){
        index++;

        // Current expressions 
        vector<NotsqlExpr> curr_exprs;
        while(true){
            // Expressions

            if(!expect(index, TK_ID)){
                throw NotsqlSyntaxException(tokens[index - 1].second, "");
            }
            NotsqlExpr expr;

            // Table.
            if(expect(index + 1, TK_DOT)){
                expr.table = tokens[index].second;
                index += 2;
            }

            // Field 
            if(!expect(index, TK_ID)){
                throw NotsqlSyntaxException(tokens[index - 1].second, "");
            }
            expr.col = tokens[index].second;
            index++;

            // Operator
            if(!expect(index, TK_EQ) &&
                    !expect(index, TK_NE) &&
                    !expect(index, TK_LT) &&
                    !expect(index, TK_LE) &&
                    !expect(index, TK_GT) &&
                    !expect(index, TK_GE) &&
                    !expect(index, TK_LIKE_KW)){
                throw NotsqlSyntaxException(tokens[index].second, 
                        "Invalid operator");
            }
            index++;

            // Operand
            if(!expect(index, TK_INTEGER) &&
                    !expect(index, TK_STRING) &&
                    !expect(index, TK_FLOAT) &&
                    !expect(index, TK_DOUBLE &&
                        !expect(index, TK_ID))){
                throw NotsqlSyntaxException(tokens[index - 1].second, 
                        "Invalid operand");
            }
            if(expect(index, TK_ID)){
                expr.opr = tokens[index - 1].first;

                if(expect(index + 1, TK_DOT)){
                    expr.table2 = tokens[index].second;
                    index += 2;
                }
                if(!expect(index, TK_ID)){
                    throw NotsqlSyntaxException(tokens[index - 1].second, 
                            "Invalid operand");
                }
                expr.col2 = tokens[index].second;
            }else{
                expr.opr = tokens[index - 1].first;
                expr.value = tokens[index].second;
                expr.type = tokens[index].first;
            }
            curr_exprs.push_back(expr);

            index++;
            if(!expect(index, TK_AND) && !expect(index, TK_OR)){
                exprs.push_back(curr_exprs);
                break;
            }else{
                if(tokens[index].first == TK_OR){
                    exprs.push_back(curr_exprs);
                    curr_exprs = vector<NotsqlExpr>();
                }
                index++;
            }
        }
    }


    // If it does not reach the end
    // Something was wrong
    if(index < tokens.size())
        throw NotsqlSyntaxException(tokens[index - 1].second, "");

    // It's time to call the api's select interface!!
    this->res_table = api->select(all_fields, distinct, fields, tables, exprs);
    this->res_table.showTable();
}


void NotsqlInterpreter::parseInsert(){
    index = 1;
    string table_name;
    vector<string> col_names;
    vector< pair<TK_TYPE, string> > values;

    if(!init_flag){
        throw(NotsqlNoDBSelectedException());
    }
    // INTO
    if(!expect(index, TK_INTO)){
        throw NotsqlSyntaxException(tokens[index - 1].second, "");
    }
    index++;
    // Table name
    if(!expect(index, TK_ID)){
        throw NotsqlSyntaxException(tokens[index - 1].second, 
                "Invalid table name");
    }
    table_name = tokens[index].second;
    index++;
    // ( field1, field2 ** )
    if(expect(index, TK_LEFT_BRACKET)){
        while(true){
            // Field name
            index++;
            if(!expect(index, TK_ID)){
                throw NotsqlSyntaxException(tokens[index - 1].second, 
                        "Invalid field name");
            }
            col_names.push_back(tokens[index].second);
            index++;
            // ,
            if(!expect(index, TK_COMMA)){
                break;
            }
        }
        if(!expect(index, TK_RIGHT_BRACKET)){
            throw NotsqlSyntaxException(tokens[index - 1].second, 
                    "Unbalance brackets");
        }
        index++;
    }
    // VALUES
    if(!expect(index, TK_VALUES)){
        throw NotsqlSyntaxException(tokens[index - 1].second, "");
    }
    index++;
    // (
    if(!expect(index, TK_LEFT_BRACKET)){
        throw NotsqlSyntaxException(tokens[index - 1].second, "");
    }
    index++;
    while(true){
        // Value
        if(!expect(index, TK_STRING) &&
                !expect(index, TK_INTEGER) &&
                !expect(index, TK_FLOAT) &&
                !expect(index, TK_DOUBLE)){
            throw NotsqlSyntaxException(tokens[index - 1].second, 
                    "Invalid value");
        }
        values.push_back(pair<TK_TYPE, string>(
                    tokens[index].first, 
                    tokens[index].second));
        index++;
        // ,
        if(!expect(index, TK_COMMA)){
            break;
        }else{
            index++;
        }
    }
    // )
    if(!expect(index, TK_RIGHT_BRACKET)){
        throw NotsqlSyntaxException(tokens[index - 1].second, 
                "Unbalance brackets");
    }
    api->insert(table_name, col_names, values);
}


void NotsqlInterpreter::parseDelete(){
    string table_name;
    vector< vector<NotsqlExpr> > expr_groups;
    unsigned int index = 1;

    if(!init_flag){
        throw(NotsqlNoDBSelectedException());
    }

    // FROM
    if(!expect(index, TK_FROM)){
        throw NotsqlSyntaxException(tokens[index - 1].second, "");
    }
    index++;
    // *
    if(expect(index, TK_STAR)){
        index++;
    }
    // Table_name
    if(!expect(index, TK_ID)){
        throw NotsqlSyntaxException(tokens[index - 1].second, 
                "Invalid table name");
    }
    table_name = tokens[index].second;
    index++;
    // WHERE
    if(expect(index, TK_WHERE)){
        vector<NotsqlExpr> exprs;
        while(true){
            NotsqlExpr expr;
            expr.table = table_name;
            index++;
            if(!expect(index, TK_ID)){
                throw NotsqlSyntaxException(tokens[index - 1].second, 
                        "Invalid field name");
            }
            expr.col = tokens[index].second;
            index++;
            if(!expect(index, TK_EQ) &&
                    !expect(index, TK_NE) &&
                    !expect(index, TK_GE) &&
                    !expect(index, TK_LE) &&
                    !expect(index, TK_LT) &&
                    !expect(index, TK_GT) &&
                    !expect(index, TK_LIKE_KW)){
                throw NotsqlSyntaxException(tokens[index - 1].second, 
                        "Invalid operator");
            }
            expr.opr = tokens[index].first;
            index++;
            if(!expect(index, TK_STRING) &&
                    !expect(index, TK_INTEGER) &&
                    !expect(index, TK_FLOAT)   &&
                    !expect(index, TK_DOUBLE)){
                throw NotsqlSyntaxException(tokens[index - 1].second, 
                        "Invalid operand");
            }
            expr.type = tokens[index].first;
            expr.value = tokens[index].second;
            exprs.push_back(expr);
            index++;
            if(!expect(index, TK_AND) &&
                    !expect(index, TK_OR)){
                expr_groups.push_back(exprs);
                break;
            }else if(expect(index, TK_OR)){
                expr_groups.push_back(exprs);
                exprs = vector<NotsqlExpr>();
            }
        }
    }
    if(index != tokens.size()){
        throw NotsqlSyntaxException(tokens[index - 1].second, 
                "Unexpect end");
    }

    api->delete_(table_name, expr_groups);
}


/*******************************
 * Static utils
 */


bool NotsqlInterpreter::isNum(const string & str){
    for(unsigned int i = 0; i != str.size(); ++i){
        if(!isDigit(str[i]))
            return false;
    }
    return true;
}


double NotsqlInterpreter::toNum(const string & str){
    stringstream ss;
    double res;
    ss << str;
    ss >> res;
    if(ss.fail()){
        throw(NotsqlNotANumException(str));
    }
    return res;
}


