//
// Created by huxijie on 16-10-18.
// 词法分析源文件

#include "Global.h"
#include "LexAnalysis.h"
#include "Util.h"
#include <string.h>
#include <map>
#include <iomanip>


using namespace std;

//DFA的状态
typedef enum {
    START,
    INNUM,INREAL0,INREAL,INID,INPLUS,INMIN,INMUL,INDIV,INMOD,INSPECIAL,
    INASSIGN,INLES,INGRT,
    INLINECOMMENT,INMULCOMMENT1,INMULCOMMENT2,
    DONE
} StateType;



//每一行输入缓存的最大长度
#define BUFLEN 256

string tokenString;
static char lineBuf[BUFLEN];    //存放每一行
static int bufSize = 0;         //目前行的长度
static int linepos = 0;         //目前行中的字符当前位置
static int EOF_flag = FALSE;
static map<const string,int> keyMap;     //存放保留字的map,用于检索
static map<const string,int> specialMap; //存入特殊token的map,用于检索。特殊token指的是双字符的token
static char check[] = {'+','-','*','/','%','=','<','>','(',')','[',']','{','}',',',';','\'','\"'};
map<const string,int>::iterator iter;

//初始化keyMap
void initKeyMap() {
    keyMap.insert(make_pair("if", IF));
    keyMap.insert(make_pair("else", ELSE));
    keyMap.insert(make_pair("while", WHILE));
    keyMap.insert(make_pair("read", READ));
    keyMap.insert(make_pair("write", WRITE));
    keyMap.insert(make_pair("int", INT));
    keyMap.insert(make_pair("real", REAL));
    keyMap.insert(make_pair("break", BREAK));
    keyMap.insert(make_pair("switch", SWITCH));
    keyMap.insert(make_pair("case", CASE));
}



//初始化specialMap
void initSpecialMap() {
    specialMap.insert(make_pair("++", PLUS_PLUS));
    specialMap.insert(make_pair("+=", PLUS_EQL));
    specialMap.insert(make_pair("--", MINUS_MINUS));
    specialMap.insert(make_pair("-=", MINUS_EQL));
    specialMap.insert(make_pair("*=", MUL_EQL));
    specialMap.insert(make_pair("/=", DIV_EQL));
    specialMap.insert(make_pair("%=", MOD_EQL));
    specialMap.insert(make_pair("<=", LES_EQL));
    specialMap.insert(make_pair(">=", GRT_EQL));
    specialMap.insert(make_pair("==", EQL));
    specialMap.insert(make_pair("<>", NOT_EQL));

}

//从lineBuf中获取下个字符
static char getNextChar(istream &source_file,ostream &lex_file,int flag) {
    if (!(linepos < bufSize)) { //说明此行已经读完了
        lineNo++;
        if (!source_file.eof()) {  //再读取一行
            source_file.getline(lineBuf,BUFLEN);
            if (flag == 0 && lineBuf[0] == '#') {   //flag为0表示是控制台输入,设定#是程序结束标志
                EOF_flag = TRUE;
                return EOF;
            }
            lex_file<<setw(4)<<lineNo<<": "<<lineBuf<<endl;       //同时将此行内容写入词法分析结果文件
            linepos = 0;
            bufSize = strlen(lineBuf);
            return lineBuf[linepos++];
        } else {
            if (flag == 1) {                //flag为1表示是文件输入,设定到达文件结尾表示程序结束标志
                EOF_flag = TRUE;
                return EOF;
            }
        }
    } else {
        return lineBuf[linepos++];
    }
}

//取消获取下一个字符，也就是撤消上一次的取字符操作
static void unGetNextChar() {
    if (!EOF_flag) linepos--;
}


//判断字符是否为空白
bool isBlank(char c) {
    if ((c == ' ') || (c == '\t') || c == '\n') {
        return true;
    }
    else return false;
}


//判断字符是否为运算符、限界符、注释的第一个字符
static bool isFirst(char c) {
    int n = sizeof(check)/ sizeof(check[0]);
    for (int i = 0; i < n; ++i) {
        if (c == check[i]) return true;
    }
    return false;
}

//外部接口
TokenType getToken(istream &source_file,ostream &lex_file,int flag) {
    initKeyMap();
    initSpecialMap();
    return getTokenPrivate(source_file,lex_file,flag);
}

//内部实现,用来从源文件中解析出下个token
TokenType getTokenPrivate(istream &source_file,ostream &lex_file,int flag) {
    TokenType currentToken;
    StateType stateType = START;    //DFA的开始
    int isSave;                     //用于判断是否将字符存进当前tokenString
    while (stateType != DONE) {
        char c = getNextChar(source_file,lex_file,flag);
        if (EOF_flag) {             //如果经过取字符发现已经到达文件尾,则直接返回token:ENDFILE
            return ENDFILE;
        }
        isSave = TRUE;
        switch (stateType) {
            case START:
                if (isBlank(c)) {   //是空白的则跳过
                    isSave = FALSE;
                } else if (isdigit(c)) {
                    stateType = INNUM;
                } else if (isalpha(c)) {
                    stateType = INID;
                } else if (c == '+') {
                    stateType = INPLUS;
                } else if (c == '-') {
                    stateType = INMIN;
                } else if (c == '*') {
                    stateType = INMUL;
                } else if (c == '/') {
                    stateType = INDIV;
                } else if (c == '%') {
                    stateType = INMOD;
                } else if (c == '=') {
                    stateType = INASSIGN;
                } else if (c == '<') {
                    stateType = INLES;
                } else if (c == '>') {
                    stateType = INGRT;
                } else if (c == '(') {
                    currentToken = LEFT_BRA;
                    stateType = DONE;
                } else if (c == ')') {
                    currentToken = RIGHT_BRA;
                    stateType = DONE;
                } else if (c == '[') {
                    currentToken = LEFT_INDEX;
                    stateType = DONE;
                } else if (c == ']') {
                    currentToken = RIGHT_INDEX;
                    stateType = DONE;
                } else if (c == '{') {
                    currentToken = LEFT_BOUNDER;
                    stateType = DONE;
                } else if (c == '}') {
                    currentToken = RIGHT_BOUNDER;
                    stateType = DONE;
                } else if (c == '.') {
                    currentToken = POINTER;
                    stateType = DONE;
                } else if (c == ',') {
                    currentToken = COMMA;
                    stateType = DONE;
                } else if (c == ';') {
                    currentToken = SEMI;
                    stateType = DONE;
                } else if (c == '\'') {
                    currentToken = SIN_QUE;
                    stateType = DONE;
                } else if (c == '\"') {
                    currentToken = DOU_QUE;
                    stateType = DONE;
                } else {
                    currentToken = ERROR;
                    stateType = DONE;
                }
                break;
            case INNUM:
                if (isdigit(c)) {
                    stateType = INNUM;
                }else if (c == '.') {
                    stateType = INREAL0;
                } else if (isBlank(c) || isFirst(c)) {
                    isSave = FALSE;     //当前字符不能存入tokenString中，因为已经识别出是个token了
                    unGetNextChar();
                    stateType = DONE;
                    currentToken = INT_VAL;
                } else {
                    currentToken = ERROR;
                    stateType = DONE;
                }
                break;
            case INREAL0:
                if (isdigit(c)) {
                    stateType = INREAL;
                } else {
                    currentToken = ERROR;
                    stateType = DONE;
                }
                break;
            case INREAL:
                if (isdigit(c)) {
                    stateType = INREAL;
                }else if (isBlank(c) || isFirst(c)) {
                    isSave = FALSE;
                    unGetNextChar();
                    stateType = DONE;
                    currentToken = FLOAT_VAL;
                } else {
                    currentToken = ERROR;
                    stateType = DONE;
                }
                break;
            case INID:
                if (isalpha(c) || isdigit(c) || c == '_') {
                    stateType = INID;
                } else if (isBlank(c) || isFirst(c)) {
                    isSave = FALSE;
                    unGetNextChar();
                    stateType = DONE;
                    currentToken = IDENTIFIER;
                } else {
                    currentToken = ERROR;
                    stateType = DONE;
                }
                break;
            case INPLUS:
                if (c == '+' || c == '=') {
                    stateType = INSPECIAL;
                } else {
                    isSave = FALSE;
                    unGetNextChar();
                    stateType = DONE;
                    currentToken = PLUS;
                }
                break;
            case INMIN:
                if (c == '-' || c == '=') {
                    stateType = INSPECIAL;
                } else {
                    isSave = FALSE;
                    unGetNextChar();
                    stateType = DONE;
                    currentToken = MINUS;
                }
                break;
            case INMUL:
                if (c == '=') {
                    stateType = INSPECIAL;
                } else {
                    isSave = FALSE;
                    unGetNextChar();
                    stateType = DONE;
                    currentToken = MUL;
                }
                break;
            case INDIV:
                if ( c == '=') {
                    stateType = INSPECIAL;
                } else if (c == '/') {
                    isSave = FALSE;
                    stateType = INLINECOMMENT;
                } else if (c == '*') {
                    isSave = FALSE;
                    stateType = INMULCOMMENT1;
                } else {
                    isSave = FALSE;
                    unGetNextChar();
                    stateType = DONE;
                    currentToken = DIV;
                }
                break;
            case INLINECOMMENT:
                isSave = FALSE;
                stateType = INLINECOMMENT;
                if (linepos>=bufSize) {
                    currentToken = LINE_NOTE;
                    stateType = DONE;
                }
                break;
            case INMULCOMMENT1:
                isSave = FALSE;
                if (c == '*') {
                    stateType = INMULCOMMENT2;
                }
                break;
            case INMULCOMMENT2:
                isSave = FALSE;
                if (c == '*') {
                    stateType = INMULCOMMENT2;
                }else if (linepos>=bufSize) {
                    currentToken = MUL_NOTE;
                    stateType = DONE;
                } else {
                    stateType = INMULCOMMENT1;
                }
                break;
            case INMOD:
                if (c == '=') {
                    stateType = INSPECIAL;
                } else {
                    isSave = FALSE;
                    unGetNextChar();
                    stateType = DONE;
                    currentToken = MOD;
                }
                break;
            case INASSIGN:
                if (c == '=') {
                    stateType = INSPECIAL;
                } else {
                    isSave = FALSE;
                    unGetNextChar();
                    stateType = DONE;
                    currentToken = ASSIGN;
                }
                break;
            case INLES:
                if (c == '=' || c == '>') {
                    stateType = INSPECIAL;
                } else {
                    isSave = FALSE;
                    unGetNextChar();
                    stateType = DONE;
                    currentToken = LES;
                }
                break;
            case INGRT:
                if (c == '=' ) {
                    stateType = INSPECIAL;
                } else {
                    isSave = FALSE;
                    unGetNextChar();
                    stateType = DONE;
                    currentToken = GRT;
                }
                break;
            case INSPECIAL:     //对于特殊的token进行分析处理,在此状态下不进行存储新字符
                isSave = FALSE;
                unGetNextChar();
                stateType = DONE;
                currentToken = (TokenType)(specialMap.find(tokenString)->second);
                break;
        }
        if (isSave) {
            tokenString.append(1,c);
        }
        if (stateType == DONE) {
            if (currentToken == IDENTIFIER) {       //在标识符中进一步分析，看看是不是保留字
                if ((iter = keyMap.find(tokenString)) != keyMap.end()) {
                    currentToken = (TokenType) iter->second;
                }
            } else if (currentToken == LINE_NOTE || currentToken == MUL_NOTE) {
                tokenString.erase(0,tokenString.length());  //将tokenString中的元素都擦除
                return currentToken;
            }
        }
    }
    lex_file<<setw(8)<<lineNo<<":";
    printToken(source_file,lex_file,currentToken, tokenString);
    tokenString.erase(0,tokenString.length());  //将tokenString中的元素都擦除
    return currentToken;
}

