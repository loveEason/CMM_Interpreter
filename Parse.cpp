//
// Created by huxijie on 16-11-15.
// 语法分析源文件



#include "Parse.h"
#include "Util.h"
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <iomanip>

set<string> terminalSymbols ;     //终结符集
set<string> noneTerminalSymbols ; //非终结符集
string startToken ;               //文法的开始符
string totalProductions[MAXNT] ;  //存放文法文件的每一行
production productions[MAXNT] ;   //将所有产生式存储为结构体
int productionSum;                //产生式数目
ifstream grammar_file;            //文法文件
map<string,set<string>> firstSet;    //存有所有文法符号的first集合
map<int ,set<string>> productionFirstSet;      //存有所有产生式的first集合
map<string,set<string>> followSet;   //存有所有非终结符的follow集合
map<int,set<string>> selectSet;               //存有所有产生式的select集合
map<string,map<string,string>> predictionTable;     //预测分析表
treeNode *treeRoot = NULL;                    //语法树的根
myStack *analysisStack = new myStack(1);                        //分析栈
treeNode *treeStack[MAXTREENODE];
int top;                                     //分析栈的栈顶指针
fstream parse_file;      //语法分析结果文件
ofstream set_file;       //存放LL1分析中自动生成的集合以及预测分析表




//初始化数据，将所有数据置空
void init() {
    terminalSymbols.clear();
    noneTerminalSymbols.clear();
    for (int i = 0; i < MAXNT; ++i) {
        productions[i].generation.clear();
    }
}

//将文法文件中的产生式存储在数据结构中
void saveProduction(string grammar_name) {
    init();
    char perProduction[MAX];   //每条完整产生式
    char *pos = NULL;
    string productionRight;      //产生式右部
    string noneTerminalSymbol;   //非终结符

    grammar_file.open(grammar_name, ios::in);
    if (grammar_file.is_open() != true) {
        cout<<"打开文件失败"<<endl;
        return;
    }
    productionSum = 0;
    while (!grammar_file.eof()) {
        productionSum++;
        grammar_file.getline(perProduction, MAX);
        totalProductions[productionSum-1] = perProduction;
        pos = strstr(perProduction, "->");  //求出"->"的位置
        productionRight.replace(productionRight.begin(), productionRight.end(), pos + 3);   //取出产生式右部
        noneTerminalSymbol.replace(noneTerminalSymbol.begin(), noneTerminalSymbol.end(), perProduction,
                                   pos - perProduction-1);    //取出产生式左部，也就是非终结符名。再减1的原因是假设非终结符名和->之间有空格

        productions[productionSum-1].noneTerminalSymbol = noneTerminalSymbol;
        //取出产生式右部的每一子项
        list <string> perProRight = strtok_plus(productionRight, " ");
        for (list<string>::const_iterator it = perProRight.cbegin();  it != perProRight.cend() ; ++it) {
            productions[productionSum - 1].generation.push_back(*it);
        }
        //将非终结符名存入非终结符集中
        if (noneTerminalSymbols.find(noneTerminalSymbol) == noneTerminalSymbols.end()) {
            noneTerminalSymbols.insert(noneTerminalSymbol);
        }
        //默认文法第一个产生式左部为开始符
        if (productionSum == 1) {
            startToken = noneTerminalSymbol;
        }
    }

    //将终结符名存入终结符集中
    string tmp;
    for (int i = 0; i < productionSum; ++i) {
        //遍历产生式右部，如果不是非终结符，则是终结符
        for (int j = 0; j < productions[i].generation.size(); ++j) {
            tmp = productions[i].generation[j];
            if (terminalSymbols.find(tmp) == terminalSymbols.end() &&
                noneTerminalSymbols.find(tmp) == noneTerminalSymbols.end()) {
                terminalSymbols.insert(tmp);
            }
        }
    }
    terminalSymbols.insert("#");    //结束符
    terminalSymbols.insert("$");    //空字

    saveNoneTerminal(noneTerminalSymbols,set_file);
    saveTerminal(terminalSymbols,set_file);


}

//求所有文法符号的first集合
void buildFirstSet() {
    //初始化所有终结符的first集，为其本身
    for (set<string>::const_iterator ite = terminalSymbols.cbegin(); ite != terminalSymbols.cend(); ite++) {
        firstSet[*ite].insert(*ite);
    }
    //初始化所有非终结符的first集
    int preSize;    //产生式左部非终结符之前的first集元素个数
    int curSize;    //产生式左部非终结符现在的first集元素个数
    bool changed;   //非终结符的first集是否改变
    do {
        changed = false;
        for (int i = 0; i < productionSum; ++i) {
            preSize = firstSet[productions[i].noneTerminalSymbol].size();   //某个非终结符对应的first集中元素个数
            bool shouldContinue = true;     //是否继续求此产生式的first集
            for (int j = 0; j < productions[i].generation.size() && shouldContinue; ++j) {  //对产生式右部每个元素进行判断。比如S->ABc
                for (set<string>::const_iterator ite = firstSet[productions[i].generation[j]].cbegin();
                     ite != firstSet[productions[i].generation[j]].cend(); ite++) {         //获得产生式右部每个元素的first集中的元素，也就是先获得A的first集
                    if (firstSet[productions[i].noneTerminalSymbol].find(*ite) ==
                        firstSet[productions[i].noneTerminalSymbol].end() && *ite != "$") {
                        firstSet[productions[i].noneTerminalSymbol].insert(*ite);           //将A的first集元素都加到S的first集中,除了空字
                    }
                }
                if (firstSet[productions[i].generation[j]].find("$") ==
                    firstSet[productions[i].generation[j]].end()) {
                    shouldContinue = false;                                                 //如果A的first集中没有空字，则结束求此产生式的first集。否则继续对B操作
                }
            }
            if (shouldContinue) firstSet[productions[i].noneTerminalSymbol].insert("$");//若最后一个元素的first集仍有空字，则将空字也加入此产生式左部非终结符的first集中
            curSize = firstSet[productions[i].noneTerminalSymbol].size();
            if (curSize > preSize) changed = true;
        }
    } while (changed);  //若first集改变了，则继续重复操作


    //保存到文件中
    saveFirst(firstSet,set_file);
}

//求所有产生式的first集合
void buildProductionFirstSet() {
    int i,j;
    bool has$;

    for (i = 0; i < productionSum; i++) {
        has$ = true;
        for (j = 0; j < productions[i].generation.size() && has$; j++) {    //遍历产生式右部，如S->ABc,先看first(A)
            for (set<string>::const_iterator ite = firstSet[productions[i].generation[j]].cbegin();
                 ite != firstSet[productions[i].generation[j]].cend(); ite++) {
                //将first(A)都加入first(Xi)中,除了空字
                if (*ite != "$") {
                    productionFirstSet[i].insert(*ite);
                }
            }

            //如果当前符号的first中没有空字，则求产生式first集可以结束了
            if (firstSet[productions[i].generation[j]].find("$") == firstSet[productions[i].generation[j]].end()) {
                has$ = false;
                break;
            }
        }
        //如果遍历到产生式右部末尾，且最后一个符号的first集仍有空字，则将空字也加入产生式的first集中
        if (j == productions[i].generation.size() && has$) {
            productionFirstSet[i].insert("$");
        }
    }

    //保存到文件中
    saveProductionFirst(productionFirstSet,set_file);
}


//求所有非终结符的follow集合
void buildFollowSet() {
    followSet[startToken].insert("#");  //文法的开始符号的follow集中包含{#}
    int preSize;    //非终结符之前的follow集元素个数
    int curSize;    //非终结符现在的follow集元素个数
    bool changed;   //非终结符的follow集是否改变
    bool has$;  //是否有空字
    int i,j,k;
    int lastNoneTerminal = -1;   //产生式右部最后一个非终结符的位置

    do {
        changed = false;
        for (i = 0; i < productionSum; ++i) {
            for (j = 0; j < productions[i].generation.size(); ++j) {    //对于每条产生式x,如 A->aBβα
                //如果x[j]是非终结符,即读到B，则求B的follow集
                if (noneTerminalSymbols.find(productions[i].generation[j]) != noneTerminalSymbols.end()) {
                    preSize = followSet[productions[i].generation[j]].size();   //先求此时B的follow集元素个数
                    for (k = j+1; k < productions[i].generation.size(); ++k) {  //遍历B之后的文法符号，即βα
                        has$ = false;
                        for (set<string>::const_iterator ite = firstSet[productions[i].generation[k]].cbegin();
                             ite != firstSet[productions[i].generation[k]].cend(); ite++) { //将first(β)的非空元素加入follow(B)中
                            if (*ite == "$") {
                                has$ = true;
                            } else if (followSet[productions[i].generation[j]].find(*ite) ==
                                       followSet[productions[i].generation[j]].end() && *ite != "$") {
                                followSet[productions[i].generation[j]].insert(*ite);
                            }
                        }
                        if (!has$) {    //如果first(β)不包含空字，则退出
                            break;
                        }
                    }   //继续遍历之后的文法符号
                    if (k == productions[i].generation.size() && has$) {    //如果读到产生式末尾，并且最后一个仍包含空字，则把follow(A)也加入follow(B)中
                        for (set<string>::const_iterator ite = followSet[productions[i].noneTerminalSymbol].cbegin();
                             ite != followSet[productions[i].noneTerminalSymbol].cend(); ite++) {
                            if (followSet[productions[i].generation[j]].find(*ite) ==
                                followSet[productions[i].generation[j]].end()) {
                                followSet[productions[i].generation[j]].insert(*ite);
                            }
                        }
                    }
                    curSize = followSet[productions[i].generation[j]].size();   //再求此时B的follow集元素个数
                    if (curSize > preSize) changed = true;

                    lastNoneTerminal = j;   //记录最后产生式最后一个非终结符的位置
                }
            }
            if (lastNoneTerminal == j - 1) {    //如果产生式最后一个符号是非终结符，则该follow集将无法由上述步骤求出。因此直接将follow(A)加入该follow集中
                for (set<string>::const_iterator ite = followSet[productions[i].noneTerminalSymbol].cbegin();
                     ite != followSet[productions[i].noneTerminalSymbol].cend(); ite++) {
                    if (followSet[productions[i].generation[lastNoneTerminal]].find(*ite) ==
                        followSet[productions[i].generation[lastNoneTerminal]].end()) {
                        followSet[productions[i].generation[lastNoneTerminal]].insert(*ite);
                    }
                }
                lastNoneTerminal = -1;      //重新恢复初始值
            } else {
                lastNoneTerminal = -1;      //重新恢复初始值
            }
        }
    } while (changed);  //若follow集改变了，则继续重复操作

    //保存到文件中
    saveFollow(followSet,set_file);
}

//求所有产生式的select集合
void buildSelectSet() {
    int i,j;

    for (i = 0; i < productionSum; i++) {
        //对于A->α
        //若α不能推出空字，则select(A->α) = first(α)
        if (productionFirstSet[i].find("$") == productionFirstSet[i].end()) {
            for (set<string>::const_iterator ite = productionFirstSet[i].cbegin();
                 ite != productionFirstSet[i].cend(); ite++) {
                selectSet[i].insert(*ite);
            }
        } else {    //如果α能推出空字，则select(A->α) = (first(α) - {$}) U follow(A)
            for (set<string>::const_iterator ite = productionFirstSet[i].cbegin();
                 ite != productionFirstSet[i].cend(); ite++) {
                if (*ite != "$") {
                    selectSet[i].insert(*ite);
                }
            }
            //先取出产生式左部非终结符
            int pos = totalProductions[i].find(" ->", 0);
            string noneTerminal = totalProductions[i].substr(0, pos - 0);
            //把follow(A)加入select中
            for (set<string>::const_iterator ite = followSet[noneTerminal].cbegin();
                 ite != followSet[noneTerminal].cend(); ite++) {
                selectSet[i].insert(*ite);
            }
        }
    }

    //保存到文件中
    saveProductionSelect(selectSet,set_file);
}

//构建预测分析表
void buildPredictionTable() {
    //若a ∈ select(A -> α),则把A -> α放入predictionTable[A,a]中
    int i;
    for (i = 0; i < productionSum; i++) {   //对于每个产生式
        for (set<string>::const_iterator ite = selectSet[i].cbegin(); ite != selectSet[i].cend(); ite++) {  //对于select集中的每个终结符
            predictionTable[productions[i].noneTerminalSymbol][*ite] = totalProductions[i];
        }
    }

    //保存到文件中
    savePredictionTable(predictionTable,set_file);
}

//输出分析栈的内容
void showAnalysisStack(myStack *analysisStack) {
    for (int i = 0; i < analysisStack->n; ++i) {
        parse_file << analysisStack->item[i] << " ";
    }
    parse_file<<endl;
}

//输出剩余输入串的内容
void showRemainToken(normalNode *pCurrent) {
    normalNode *p = pCurrent;
    while (p != NULL) {
        parse_file<<p->tokenStr<<" ";
        p = p->next;
    }
    parse_file<<endl;
}


//正式用预测分析表进行语法分析
void analyse() {
    //将#添加到normalNode链表的结尾
    normalNode *p = normalHead;
    normalNode *tmp = new normalNode();
    tmp->content = "#";
    tmp->type = -1;
    tmp->tokenStr = "#";
    tmp->addr = -1;
    tmp->line = -1;
    tmp->next = NULL;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = tmp;

    int i,j,pos;
    list<string> usedProduction;
    list<string>::const_iterator ite;
    list<string> usedProductionRight;
    string analysisStkTop;
    normalNode *preTop = NULL;
    normalNode *remainTokenTop = NULL;
    treeNode *tmpTreeNode;

    top = 0;
    //将#和开始字符依次进分析栈
    analysisStack->push("#");
    addToTree(new treeNode("#"));
    analysisStack->push(startToken);
    addToTree(new treeNode(startToken));


    treeRoot = treeStack[top-1];    //得到语法树的根

    preTop = normalHead;                 //重新指向头结点（注意，头结点为空的）
    for (i=0;;i++) {                //边分析边将过程写入语法分析结果文件中
        parse_file<<"第"<<i+1<<"步"<<endl;
        parse_file<<"当前分析栈:";
        showAnalysisStack(analysisStack);
        parse_file<<"剩余输入串:";
        showRemainToken(preTop->next);

        //取出分析栈的栈顶
        analysisStkTop = analysisStack->top();
        //取出剩余输入串的栈顶
        remainTokenTop = preTop->next;

        //如果分析栈和输入串都只剩余#,说明分析成功
        if (analysisStkTop == "#" && remainTokenTop->tokenStr == "#") {
            parse_file<<"当前程序语法分析成功！"<<endl;
            break;
        } else if (noneTerminalSymbols.find(analysisStkTop) != noneTerminalSymbols.end()) {      //如果分析栈的栈顶是非终结符
            //如果剩余输入符号串的栈顶是终结符并且在预测分析表中有对应的产生式
            if (terminalSymbols.find(remainTokenTop->tokenStr) != terminalSymbols.end() &&
                predictionTable[analysisStkTop].find(remainTokenTop->tokenStr) != predictionTable[analysisStkTop].end()) {
                parse_file<<"推导所用产生式:"<<predictionTable[analysisStkTop][remainTokenTop->tokenStr]<<endl<<endl;
                //得到此时应使用的产生式
                usedProduction = strtok_plus(predictionTable[analysisStkTop][remainTokenTop->tokenStr], " -> ");
                //第二项为产生式右部
                ite = usedProduction.cbegin();
                ite++;
                //取出产生式右部中的每一项
                usedProductionRight = strtok_plus(*ite, " ");
//                //去掉空字
//                list<string> proTmp;
//                for (ite = usedProductionRight.cbegin(); ite != usedProductionRight.cend(); ite++) {
//                    if (*ite == "$") {
//
//                    }else {
//                        proTmp.push_back(*ite);
//                    }
//                }
                list<string> proTmp = usedProductionRight;
                //初始化语法树节点
                tmpTreeNode = getTopNode();
                tmpTreeNode->childNum = proTmp.size();

                //分析栈进行弹栈,再把产生式每一项倒序进栈
                analysisStack->pop();
                j = proTmp.size()-1;
                string tokenString;
                for (ite = proTmp.cend();ite != proTmp.cbegin();) {
                    tokenString = *(--ite);
                    tmpTreeNode->children[j] = new treeNode(tokenString);
                    if (terminalSymbols.find(tokenString) == terminalSymbols.end()) {
                        addToTree(tmpTreeNode->children[j]);
                    }
                    if (tokenString != "$") {               //空字不入栈
                        analysisStack->push(tokenString);
                    }
                    j--;
                }
            } else if (terminalSymbols.find(remainTokenTop->tokenStr) == terminalSymbols.end()) {
                //当前输入串栈顶不是终结符
                parse_file<<"错误！位于程序第"<<remainTokenTop->line<<"行的token:"<<remainTokenTop->tokenStr<<"不是终结符"<<endl<<endl;
                break;
            } else {
                //在预测分析表中找不到相关产生式
                parse_file<<"错误！当前token不可被识别:"<<"位于程序第"<<remainTokenTop->line<<"行的"<<remainTokenTop->tokenStr<<endl<<endl;
                break;
            }
        } else if (terminalSymbols.find(analysisStkTop) != terminalSymbols.end()) {            //如果分析栈的栈顶是终结符
            if (analysisStkTop == remainTokenTop->tokenStr) {
                //匹配
                //分析栈弹出
                analysisStack->pop();
                //让p指向剩余输入串下一个token,以使下次取出新栈顶
                preTop = remainTokenTop;
                parse_file<<"匹配:"<<remainTokenTop->tokenStr<<endl<<endl;
            } else {
                parse_file<<"错误！匹配失败:"<<"位于程序第"<<remainTokenTop->line<<"行的"<<remainTokenTop->tokenStr<<endl<<endl;
            }
        }
    }
}


