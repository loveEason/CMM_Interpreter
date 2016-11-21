//
// Created by huxijie on 16-10-18.
// 词法分析头文件

#ifndef CMM_INTERPRETER_LEXANALYSIS_H
#define CMM_INTERPRETER_LEXANALYSIS_H

#include "Global.h"
#include <map>

////token的最大长度
//#define MAXTOKENLEN 30

//存放分析得到的token
//extern char tokenString[MAXTOKENLEN + 1];
extern std::string tokenString;
extern map<const string,int> keyMap;     //存放保留字的map,用于检索
extern map<const string,int> specialMap; //存入特殊token的map,用于检索。特殊token指的是双字符的token
extern bool hasError;                           //词法分析中是否出现错误

extern void initKeyMap();                       //初始化keyMap
extern void initSpecialMap();                   //初始化specialMap
extern void initNode();                         //初始化节点，链表都是由空的头节点组织成的


//外部接口,用来从源文件中解析出下个token
//传递参数istream和ostream,实现多态流操作
//最后一个参数是用来分辨控制台输入还是文件输入,以实现不同的控制
extern TokenType getToken(std::istream&,std::ostream&,int);


#endif //CMM_INTERPRETER_LEXANALYSIS_H
