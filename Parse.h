//
// Created by huxijie on 16-11-15.
// 语法分析头文件

#ifndef CMM_INTERPRETER_PARSE_H
#define CMM_INTERPRETER_PARSE_H

#include "Global.h"
#include <vector>
#include <set>
#include <map>
#include <stack>

#define MAXNT 128           //非终结符的最大数量
#define MAX   256           //每行字符串最大长度

//存放产生式的结构
struct production{
    string noneTerminalSymbol;  //非终结符
    vector<string> generation;  //该非终结符的产生式右部
};

extern set<string> terminalSymbols ;                        //终结符集
extern set<string> noneTerminalSymbols ;                    //非终结符集
extern string startToken ;                                  //文法的开始符
extern string totalProductions[MAXNT] ;                     //存放文法文件的每一行
extern production productions[MAXNT] ;                      //将所有产生式存储为结构体。假定产生式"->"左右两边都有空格
extern map<string,set<string>> firstSet;                    //存有所有文法符号的first集合
extern map<int ,set<string>> productionFirstSet;            //存有所有产生式的first集合
extern map<string,set<string>> followSet;                   //存有所有非终结符的follow集合
extern map<int,set<string>> selectSet;                      //存有所有产生式的select集合
extern map<string,map<string,string>> predictionTable;      //预测分析表
extern myStack *analysisStack;                              //分析栈
extern int top;                                             //分析栈的栈顶指针


extern void saveProduction(string grammar_name);            //将文法文件中的产生式存储在数据结构中
extern void buildFirstSet();                                //求所有文法符号的first集合
extern void buildProductionFirstSet();                      //求所有产生式的first集合
extern void buildFollowSet();                               //求所有非终结符的follow集合
extern void buildSelectSet();                               //求所有产生式的select集合
extern void buildPredictionTable();                         //构建预测分析表
extern void showAnalysisStack(myStack *analysisStack);      //输出分析栈的内容
extern void showRemainToken(normalNode *pCurrent);          //输出剩余输入串的内容
extern void analyse();                                      //正式进行语法分析





#endif //CMM_INTERPRETER_PARSE_H
