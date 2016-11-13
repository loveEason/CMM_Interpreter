#include <iosfwd>
#include <iostream>
#include <string>
#include <fstream>
#include "Global.h"
#include "LexAnalysis.h"
#include "Util.h"

using namespace std;


int lineNo;          //源文件中行的序号
#define BUFLEN 256

int main(int argc,char *argv[]) {
    ifstream source_file;    //源文件
    fstream lex_file;       //词法分析结果文件
    string suffix = ".cmm"; //后缀名
    int pos;                //字符串中后缀名出现的位置
    string source_name;  //源文件的文件名
    string lex_name;     //词法分析结果文件的文件名


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
            exit(1);
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
    return 0;
}