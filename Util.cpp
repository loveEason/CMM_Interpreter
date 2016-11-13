//
// Created by huxijie on 16-10-21.
// 工具源文件，用于打印相关信息

#include "Util.h"


using namespace std;

void printToken(istream &source_file,ostream &lex_file,TokenType token, const string tokenString) {
    switch (token) {
        case IDENTIFIER:
            lex_file<<"标识符, 名字为： "<<tokenString<<endl;
            break;
        case INT_VAL:
            lex_file<<"整数, 值为： "<<tokenString<<endl;
            break;
        case FLOAT_VAL:
            lex_file<<"实数, 值为： "<<tokenString<<endl;
            break;
        case IF:
        case ELSE:
        case WHILE:
        case READ:
        case WRITE:
        case INT:
        case REAL:
        case BREAK:
        case SWITCH:
        case CASE:
            lex_file<<"保留字: "<<tokenString<<endl;
            break;
        case PLUS:
        case PLUS_PLUS:
        case PLUS_EQL:
        case MINUS:
        case MINUS_MINUS:
        case MINUS_EQL:
        case MUL:
        case MUL_EQL:
        case DIV:
        case DIV_EQL:
        case MOD:
        case MOD_EQL:
        case ASSIGN:
        case LES:
        case LES_EQL:
        case GRT:
        case GRT_EQL:
        case EQL:
        case NOT_EQL:
            lex_file<<"运算符: "<<tokenString<<endl;
            break;
        case LEFT_BRA:
        case RIGHT_BRA:
        case LEFT_INDEX:
        case RIGHT_INDEX:
        case LEFT_BOUNDER:
        case RIGHT_BOUNDER:
        case POINTER:
        case COMMA:
        case SEMI:
        case SIN_QUE:
        case DOU_QUE:
            lex_file<<"限界符: "<<tokenString<<endl;
            break;
        case ERROR:
            lex_file<<"错误,无法识别token: "<<tokenString<<endl;
            break;
    }
}

