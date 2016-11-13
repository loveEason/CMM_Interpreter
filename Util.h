//
// Created by huxijie on 16-10-21.
// 工具头文件，用于打印相关信息

#ifndef CMM_INTERPRETER_UTIL_H
#define CMM_INTERPRETER_UTIL_H

#include "Global.h"

//打印token,通过istream和ostream实现多态,达到控制台输入输出或者文件输入输出
void printToken(std::istream &,std::ostream &,TokenType,const std::string);

#endif //CMM_INTERPRETER_UTIL_H
