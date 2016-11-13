//
// Created by huxijie on 16-10-18.
// 词法分析头文件

#ifndef CMM_INTERPRETER_LEXANALYSIS_H
#define CMM_INTERPRETER_LEXANALYSIS_H

#include "Global.h"

////token的最大长度
//#define MAXTOKENLEN 30

//存放分析得到的token
//extern char tokenString[MAXTOKENLEN + 1];
extern std::string tokenString;

//内部的getToken
static TokenType getTokenPrivate(std::istream&,std::ostream&,int);

//外部接口,用来从源文件中解析出下个token
//传递参数istream和ostream,实现多态流操作
//最后一个参数是用来分辨控制台输入还是文件输入,以实现不同的控制
TokenType getToken(std::istream&,std::ostream&,int);


#endif //CMM_INTERPRETER_LEXANALYSIS_H
