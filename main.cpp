#include <iosfwd>
#include <iostream>
#include <fstream>
#include "Global.h"
#include "LexAnalysis.h"
#include "Util.h"
#include "Parse.h"

using namespace std;


int lineNo;          //源文件中行的序号
ofstream parseTree_file; //语法树结果文件
#define BUFLEN 256

int main(int argc,char *argv[]) {
    string suffix = ".cmm"; //后缀名
    int pos;                //字符串中后缀名出现的位置
    string source_name;  //源文件的文件名
    string lex_name;     //词法分析结果文件的文件名


    initKeyMap();
    initSpecialMap();
    initNode();

    if (argc == 1) {    //不提供参数,默认为从控制台输入源程序
        lex_file.open("./console.lex",ios::out);
        cout << "请输入源程序,#表示结束输入" << endl;
        lex_file << "词法分析结果如下：" << endl;
        while (getToken(cin,lex_file,0) != ENDFILE) ;
        lex_file.close();
        lex_file.open("./console.lex", ios::in);
        char readStr[BUFLEN];
        while (!lex_file.eof()) {
            lex_file.getline(readStr, BUFLEN);
            cout << readStr << endl;
        }

    } else if (argc == 2) {     //从参数中打开源代码文件
        source_name.append(argv[1]);
        pos = source_name.find(suffix);
        if (pos == string::npos) {     //加上后缀名
            source_name.append(suffix);
        }
        source_file.open(source_name, ios::in);
        if (!source_file.is_open()) {
            cout << "找不到或者打不开文件 " << source_name << endl;
            return 1;
        }
        lex_name.append(argv[1]);
        if (pos == string::npos) {
            lex_name.append(".lex");
        } else {
            lex_name.replace(pos, 4, ".lex");
        }
        lex_file.open(lex_name, ios::out);
        lex_file << "词法分析结果如下：" << endl;
        while (getToken(source_file,lex_file,1) != ENDFILE);         //一直分析token,直到源代码已经分析结束
    } else {
        printf("用法：%s or %s filename\n",argv[0],argv[0]);
    }

    source_file.close();
    lex_file.close();

    if (hasError) {
        cout<<"词法分析中有无法识别的token,请先修改:"<<endl;
        errorNode *p = errorHead;
        while (p->next != NULL) {
            cout<<"位于程序第"<<p->next->line<<"行的"<<p->next->content<<endl;
            p = p->next;
        }
    } else {    //词法分析正确,开始语法分析
        parse_file.open("./parseResult.txt", ios::out);
        set_file.open("./set.txt", ios::trunc);
        string grammar_name = "./grammar.txt";
        saveProduction(grammar_name);
        buildFirstSet();
        buildProductionFirstSet();
        buildFollowSet();
        buildSelectSet();
        buildPredictionTable();
        analyse();
        parseTree_file.open("./tree.xml");
        parseTree_file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
        saveTree(treeRoot, parseTree_file);
        parseTree_file.close();
        set_file.close();
        parse_file.close();
    }


    return 0;
}