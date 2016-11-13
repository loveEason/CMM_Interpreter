//
// Created by huxijie on 16-10-18.
// 全局设置的头文件


#ifndef CMM_INTERPRETER_GLOBAL_H
#define CMM_INTERPRETER_GLOBAL_H

#include <iostream>
#include <fstream>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//token的类型
typedef enum {
    ENDFILE,ERROR,
    //标识符
    IDENTIFIER,
    //整数与实数
    INT_VAL,FLOAT_VAL,
    //保留字
    IF,ELSE,WHILE,READ,WRITE,INT,REAL,
    BREAK,SWITCH,CASE,
    //运算符
    PLUS,PLUS_PLUS,PLUS_EQL,                // + ++ +=
    MINUS,MINUS_MINUS,MINUS_EQL,            // - -- -=
    MUL,MUL_EQL,                            // * *=
    DIV,DIV_EQL,                            // / /=
    MOD,MOD_EQL,                            // % %=
    ASSIGN,LES,LES_EQL,GRT,GRT_EQL,EQL,NOT_EQL,    // = < <= > >= == <>
    //限界符
    LEFT_BRA,RIGHT_BRA,                     // ( )
    LEFT_INDEX,RIGHT_INDEX,                 // [ ]
    LEFT_BOUNDER,RIGHT_BOUNDER,             // { }
    POINTER,COMMA,SEMI,SIN_QUE,DOU_QUE,   // . , ; ' "
    //注释
    LINE_NOTE,                              // //注释
    MUL_NOTE                                // /**/注释
} TokenType;

/*extern std::istream &source_file;    //源程序,可以是控制台输入也可以是文件输入
extern std::ostream &lex_file;       //词法分析结果,可以是控制台输出也可以是文件输出*/

extern int lineNo;          //源文件中行的序号

#endif //CMM_INTERPRETER_GLOBAL_H


