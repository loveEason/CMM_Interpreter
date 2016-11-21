//
// Created by huxijie on 16-10-21.
// 工具头文件，用于打印相关信息

#ifndef CMM_INTERPRETER_UTIL_H
#define CMM_INTERPRETER_UTIL_H

#include "Global.h"
#include "Parse.h"
#include <list>
#include <set>
#include <map>

//打印token,通过istream和ostream实现多态,达到控制台输入输出或者文件输入输出
extern void printToken(std::istream &,std::ostream &,TokenType,const std::string);
//分隔字符串
extern list<string> strtok_plus(string text,string delim);
//保存非终结符到文件中
extern void saveNoneTerminal(set<string> noneTerminalSymbols,ofstream &out);
//保存终结符到文件中
extern void saveTerminal(set<string> terminalSymbols,ofstream &out);
//保存文法符号的first集到文件中
extern void saveFirst(map<string,set<string>> firstSet,ofstream &out);
//保存产生式的first集到文件中
extern void saveProductionFirst(map<int,set<string>> productionFirstSet,ofstream &out);
//保存非终结符的follow集到文件中
extern void saveFollow(map<string,set<string>> followSet,ofstream &out);
//保存产生式的select集到文件中
extern void saveProductionSelect(map<int,set<string>> selectSet,ofstream &out);
//保存预测分析表到文件中
extern void savePredictionTable(map<string,map<string,string>> predictionTable,ofstream &out);
//存入树中
extern void addToTree(treeNode *tNode);
//得到语法树栈的当前根节点
extern treeNode *getTopNode();
//保存语法树到xml文件中
extern void saveTree(treeNode *pTree, ofstream &out);
//处理文法中的运算符和限界符符号,转换为对应的英文说明
extern string dealTerminalSymbols(string oldStr);
//处理文法中的非终结符符号,去掉包围的<>
extern string dealNoneTerminalSymbols(string oldStr);


#endif //CMM_INTERPRETER_UTIL_H
