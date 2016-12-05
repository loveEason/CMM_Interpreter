//
// Created by huxijie on 16-10-21.
// 工具源文件，用于打印相关信息

#include <iomanip>
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
        case FOR:
        case INT:
        case REAL:
        case BREAK:
        case SWITCH:
        case CASE:
        case RETURN:
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

//分隔字符串
list<string> strtok_plus(string text,string delim) {
    list<string> result;
    int last_pos = 0;
    int pos = 0;
    int stepLength = delim.length();


    while (true) {
        pos = text.find(delim, last_pos);
        if (pos == string::npos) {
            break;
        } else {
            result.push_back(text.substr(last_pos, pos - last_pos));
            last_pos = pos + stepLength;
        }
    }
    result.push_back(text.substr(last_pos));

    return result;
}

//保存非终结符到文件中
void saveNoneTerminal(set<string> noneTerminalSymbols,ofstream &out) {
    out<<"————————————————————————文法非终结符如下——————————————————————————"<<endl;
    if (out.is_open()) {
        for (set<string>::const_iterator it = noneTerminalSymbols.cbegin(); it != noneTerminalSymbols.cend(); it++) {
            out<<*it<<endl;
        }
    }
    out << endl << endl << endl;
}

//保存终结符到文件中
void saveTerminal(set<string> terminalSymbols,ofstream &out) {
    out<<"————————————————————————文法终结符如下————————————————————————"<<endl;
    if (out.is_open()) {
        for (set<string>::const_iterator it = terminalSymbols.cbegin(); it != terminalSymbols.cend(); it++) {
            out<<*it<<endl;
        }
    }
    out << endl << endl << endl;
}

//保存文法符号的first集到文件中
void saveFirst(map<string,set<string>> firstSet,ofstream &out) {
    out<<"————————————————————————文法符号的first集如下————————————————————————"<<endl;
    if (out.is_open()) {
        for (auto ite = firstSet.cbegin(); ite != firstSet.cend(); ite++) {
            out<<setw(10)<<ite->first;
            out<<" : {";
            for (auto ite2 = ite->second.cbegin(); ite2 != ite->second.cend(); ite2++) {
                if (++ite2 == ite->second.cend()) {
                    ite2--;
                    out<<*ite2<<"}"<<endl;
                } else {
                    ite2--;
                    out<<*ite2<<",";
                }
            }
        }
    }
    out << endl << endl << endl;
}


//保存产生式的first集到文件中
void saveProductionFirst(map<int,set<string>> productionFirstSet,ofstream &out) {
    out<<"————————————————————————文法产生式的first集如下————————————————————————"<<endl;
    if (out.is_open()) {
        for (auto ite = productionFirstSet.cbegin(); ite != productionFirstSet.cend(); ite++) {
            out<<setw(20)<<totalProductions[ite->first]<<" : {";
            for (auto ite2 = ite->second.cbegin(); ite2 != ite->second.cend(); ite2++) {
                if (++ite2 == ite->second.cend()) {
                    ite2--;
                    out<<*ite2<<"}"<<endl;
                } else {
                    ite2--;
                    out<<*ite2<<",";
                }
            }
        }
    }
    out << endl << endl << endl;
}

//保存非终结符的follow集到文件中
void saveFollow(map<string,set<string>> followSet,ofstream &out) {
    out<<"————————————————————————非终结符的follow集如下————————————————————————"<<endl;
    if (out.is_open()) {
        for (auto ite = followSet.cbegin(); ite != followSet.cend(); ite++) {
            out<<setw(20)<<ite->first<<" : {";
            for (auto ite2 = ite->second.cbegin(); ite2 != ite->second.cend(); ite2++) {
                if (++ite2 == ite->second.cend()) {
                    ite2--;
                    out<<*ite2<<"}"<<endl;
                } else {
                    ite2--;
                    out<<*ite2<<",";
                }
            }
        }
    }
    out << endl << endl << endl;
}


//保存产生式的select集到文件中
void saveProductionSelect(map<int,set<string>> selectSet,ofstream &out) {
    out<<"————————————————————————产生式的select集如下————————————————————————"<<endl;
    if (out.is_open()) {
        for (auto ite = selectSet.cbegin(); ite != selectSet.cend(); ite++) {
            out<<setw(20)<<totalProductions[ite->first]<<" : {";
            for (auto ite2 = ite->second.cbegin(); ite2 != ite->second.cend(); ite2++) {
                if (++ite2 == ite->second.cend()) {
                    ite2--;
                    out<<*ite2<<"}"<<endl;
                } else {
                    ite2--;
                    out<<*ite2<<",";
                }
            }
        }
    }
    out << endl << endl << endl;
}


//保存预测分析表到文件中
void savePredictionTable(map<string,map<string,string>> predictionTable,ofstream &out) {
    out<<"————————————————————————预测分析表如下————————————————————————"<<endl;
    if (out.is_open()) {
        for (auto ite1 = predictionTable.cbegin(); ite1 != predictionTable.cend(); ite1++) {
            for (auto ite2 = ite1->second.cbegin(); ite2 != ite1->second.cend(); ite2++) {
                out << setw(5) << ite1->first << " 遇到了 " << ite2->first << ",采用产生式:"  << ite2->second << endl;
            }
        }
    }
    out << endl << endl << endl;
}

//存入树中
void addToTree(treeNode *tNode) {
    treeStack[top] = tNode;
    top++;
}

//得到语法树栈的当前根节点
treeNode *getTopNode() {
    return treeStack[--top];
}


//处理文法中的运算符和限界符符号,转换为对应的英文说明
string dealTerminalSymbols(string oldStr) {
    if (oldStr == "+") {
        return "PLUS";
    } else if (oldStr == "++") {
        return "PLUS_PLUS";
    }else if (oldStr == "+=") {
        return "PLUS_EQL";
    }else if (oldStr == "-") {
        return "MINUS";
    }else if (oldStr == "--") {
        return "MINUS_MINUS";
    }else if (oldStr == "-=") {
        return "MINUS_EQL";
    }else if (oldStr == "*") {
        return "MUL";
    }else if (oldStr == "*=") {
        return "MUL_EQL";
    }else if (oldStr == "/") {
        return "DIV";
    }else if (oldStr == "/=") {
        return "DIV_EQL";
    }else if (oldStr == "%") {
        return "MOD";
    }else if (oldStr == "%=") {
        return "MOD_EQL";
    }else if (oldStr == "=") {
        return "ASSIGN";
    }else if (oldStr == "<") {
        return "LES";
    }else if (oldStr == "<=") {
        return "LES_EQL";
    }else if (oldStr == ">") {
        return "GRT";
    }else if (oldStr == ">=") {
        return "GRT_EQL";
    }else if (oldStr == "==") {
        return "EQL";
    }else if (oldStr == "<>") {
        return "NOT_EQL";
    }else if (oldStr == "(") {
        return "LEFT_BRA";
    }else if (oldStr == ")") {
        return "RIGHT_BRA";
    }else if (oldStr == "[") {
        return "LEFT_INDEX";
    }else if (oldStr == "]") {
        return "RIGHT_INDEX";
    }else if (oldStr == "{") {
        return "LEFT_BOUNDER";
    }else if (oldStr == "}") {
        return "RIGHT_BOUNDER";
    }else if (oldStr == ".") {
        return "POINTER";
    }else if (oldStr == ",") {
        return "COMMA";
    }else if (oldStr == ";") {
        return "SEMI";
    }else if (oldStr == "\'") {
        return "SIN_QUOTE";
    }else if (oldStr == "\"") {
        return "DOU_QUOTE";
    } else {
        return oldStr;
    }
}


//处理文法中的符号,如果是由<>包围的，则去掉
string dealNoneTerminalSymbols(string oldStr) {
    int pos1 = 0;
    int pos2 = 0;
    string result;

    pos1 = oldStr.find("<", 0);
    pos2 = oldStr.find(">", 0);
    if (pos1 == 0 && pos2 == oldStr.length() - 1 && pos2 != 1) {
        result = oldStr.substr(pos1 + 1, pos2 - pos1 - 1);
    } else {
        result = oldStr;
    }
    return result;
}

//保存语法树到xml文件中
void saveTree(treeNode *pTree, ofstream &out) {
    int i ;
    if( pTree == NULL ) return ;
    /*当前节点为叶子节点*/
    if( pTree->childNum == 0 && terminalSymbols.find(pTree->content) != terminalSymbols.end() ) {
        if (pTree->content == "$" || pTree->content=="(" || pTree->content==")") {    //空字和括号不输出

        } else {
            out << dealTerminalSymbols(pTree->content) << endl;
        }
        return;
    } else {
        out << "<" << dealNoneTerminalSymbols(pTree->content) << ">" << endl ;
    }

    for ( i = 0 ; i < pTree->childNum ; i++)
    {
        saveTree(pTree->children[i],out);
    }

    out << "</" << dealNoneTerminalSymbols(pTree->content) << ">" << endl ;
}