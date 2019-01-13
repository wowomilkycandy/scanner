#include<queue>
#include<string>
#include<iostream>
#include<sstream>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include<stdio.h>
#include "word.h"
#include "report.h"

using namespace std;
stringstream stream;
queue<string> correct;
queue<string> wrong;

int main(int argc, char* argv[]) {
    if(argc==1)
    word("in.txt");
    else while(argc-- > 1) word(*++argv);       //获取外部运行参数
    printf("<< 所有源文件都已经分析完毕 >>\n");
    system("pause");
    return 0;
}

void word(const char *SourceFile) {
    printf("<< 准备分析源文件 \"%s\"，按任意键继续 >>\n",SourceFile); getch(); system("cls");
    if((pF=fopen(SourceFile,"r"))==NULL) {error(9101); return;}   //打开源文件
    state(0); cY=1; char file[255];
    if(outHtmlReport) {
        strcpy(file,SourceFile);
        strcat(file,"_WordReport.html");
        if((pH=fopen(file,"w"))==NULL) error(9102);
    } if(outHtmlReport) {
        fputs(REPORT_Head,pH);
        fputs(REPORT_CodeHead,pH);
        fputs(SourceFile,pH);
        fputs(REPORT_CodeAfter,pH);
        fputs(REPORT_LineHead1,pH);
        fputs("1.",pH);
        fputs(REPORT_LineAfter,pH);
    }
    do {
        if(!back) {
            c=fgetc(pF); cX++;                  //读取中文时是分成两半的
            if(c==9) c=' ';                     //将Tab键换为一个空格
            if(c=='\n') {cY++; cX=0;}           //行数增加，位数置零
        }
        back=0;
        switch(iState) {
        case 5001:                              //注释开始
            if(c=='\n'||c==EOF) state(5002);    //注释结束
            else {
                if(i<255) sTemp[i++]=c;         //注释长度限制
                else error(9201);
            }
            break;
        case 7003:                              //  /
            if(c=='/') state(5001);             //注释
            else state(3004);                   //除号
            break;
        case 7004:                              //  =
            if(c=='=') state(3006);             //等于
            else state(3005);                   //赋值
            break;
        case 7005:                              //  >
            if(c=='=') state(3008);             //大于等于
            else if(c=='>') ;                   //逻辑右移/算数右移
            else state(3007);                   //大于
            break;
        case 7006:                              //  <
            if(c=='=') state(3010);             //小于等于
            else if(c=='<') ;                   //左移
            else if(c=='>') state(3011);        //不等于
            else state(3009);                   //小于
            break;
        case 7001:                              //字母
            if(isID(c)) {                       //标识符字符
                if(i<64) sTemp[i++]=c;          //标识符长度限制
                else error(9202);
            }
            //else state(1001);                   //标识符
            else switch(c) {
                case '/': state(1001); break;
                case '+': state(1001); break;
                case '-': state(1001); break;
                case '*': state(1001); break;
                case '=': state(1001); break;
                case '>': state(1001); break;
                case '<': state(1001); break;
                case ';': state(1001); break;
                case '(': state(1001); break;
                case ')': state(1001); break;
                case '{': state(1001); break;
                case '}': state(1001); break;
                case '[': state(1001); break;
                case ']': state(1001); break;
                case '.': state(1001); break;
                case ',': state(1001); break;
				case '%': state(1001); break;
				case '&': state(1001); break;
				case '?': state(1001); break;
				case '"': state(1001); break;
				case '\'': state(1001); break;
				case ':': state(1001); break;
                case ' ': state(1001); break;
                default: flag=1; sTemp[i++]=c; break;
            }
            break;
        case 7002:                              //数字
            if(isDigit(c)) sTemp[i++]=c;
            else if(c=='E') {                   //科学计数法实数
                sTemp[i++]=c;
                state(7007);
            }
            else if(c=='.') {                   //小数点实数
                sTemp[i++]=c;
                state(7008);
            }
            else state(2001);                   //整数
            break;
        case 7008:                              //小数点
            if(isDigit(c)) sTemp[i++]=c;
            else if(c=='E') {                   //科学计数法实数
                sTemp[i++]=c;
                state(7007);
            }
            else state(2002);                   //小数点实数
            break;
        case 7007:                              //科学计数法E
            if(c=='+'||c=='-') {                //指数符号
                sTemp[i++]=c;
                state(7009);
            }
            else if(isDigit(c)) {               //指数
                sTemp[i++]=c;
                state(7010);
            }
            else error(9301);                   //未正常结束
            break;
        case 7009:                              //指数符号
            if(isDigit(c)) {                    //指数
                sTemp[i++]=c;
                state(7010);
            }
            else error(9301);                   //未正常结束
            break;
        case 7010:                              //指数
            if(isDigit(c)) sTemp[i++]=c;        //数字
            else state(2002);                   //科学计数法实数
            break;
        case 0:
            if(isLetter(c)) {                   //字母
                state(7001);
                sTemp[i++]=c;
            }
            else if(isDigit(c)) {               //数字
                state(7002);
                sTemp[i++]=c;
            }
            else if(outHtmlReport&&c=='\n') {   //没有标识符时，在这里输出换行
                hL=cY;
                fputs(REPORT_LineEnd,pH);
                fputs((cY%2==1)?REPORT_LineHead1:REPORT_LineHead2,pH);
                char t[6]="\0";
                sprintf(t,"%d.",hL);            //输出行号
                fputs(t,pH);
                fputs(REPORT_LineAfter,pH);
            }
            else switch(c) {
                case '/': state(7003); break;
                case '+': state(3001); break;
                case '-': state(3002); break;
                case '*': state(3003); break;
                case '=': state(7004); break;
                case '>': state(7005); break;
                case '<': state(7006); break;
                case ';': state(3012); break;
                case '(': state(3013); break;
                case ')': state(3014); break;
                case '{': state(3015); break;
                case '}': state(3016); break;
                case '[': state(3017); break;
                case ']': state(3018); break;
                case ',': state(3019); break;
                case '.': state(3020); break;
				case '%': state(3021); break;
				case '&': state(3022); break;
				case '?': state(3023); break;
				case '"': state(3024); break;
				case ':': state(3026); break;
				case '\'': state(3025); break;
                default: state(7001); break;
            }
            break;
        }
		if(c == EOF){
			if(wrong.empty()){
				while(!correct.empty()){
					string item = correct.front();
					correct.pop();
					cout<<item<<endl;
				}
			}else{
				if(!wrong.empty()) cout<<"词法错误出现"<<wrong.size()<<"个"<<"\n"<<endl;
				while(!wrong.empty()){
					string item = wrong.front();
					wrong.pop();
					cout<<item;
				}
				cout<<"\n"<<endl;
			}
		}
    } while(c!=EOF);                //文件结束
    fclose(pF);                     //关闭文件
    if(outHtmlReport) {
        fputs(REPORT_LineEnd,pH);
        fputs(REPORT_CodeEnd,pH);
        char t[10]="\0";
        fputs("document.getElementById('line').innerHTML='",pH);
        sprintf(t,"%d",cY); fputs(t,pH); fputs("';",pH);
        fputs("document.getElementById('word').innerHTML='",pH);
        sprintf(t,"%d",wordNum); fputs(t,pH); fputs("';",pH);
        fputs("document.getElementById('note').innerHTML='",pH);
        sprintf(t,"%d",note); fputs(t,pH); fputs(" (",pH);
        sprintf(t,"%4.2f",note*100.0/cY); fputs(t,pH); fputs("%)';",pH);
        fputs("document.getElementById('error').innerHTML='",pH);
        sprintf(t,"%d",err); fputs(t,pH); fputs(" (",pH);
        sprintf(t,"%4.2f",err*100.0/wordNum); fputs(t,pH); fputs("%)';",pH);
        fputs(REPORT_End,pH);
        fclose(pH);                 //关闭文件
        printf("<< 已输出分析报告 \"%s\" >>\n",file);
    }
    printf("<< 源文件 \"%s\" 分析结束，按任意键继续 >>\n",SourceFile); getch();
}

void jumpError(int k) {           //出错跳过处理
    switch(k) {
    case 0:                       //判断此字符
        if(c==' '||c=='\n') state(0);
        else jumpError(1);
        break;
    case 1:                       //读取到空白（空格、制表、换行）
        c=fgetc(pF); cX++;
        if(c==' '||c==9) state(0);
        else if(c=='\n') {cY++;cX=0;state(0);}
        else jumpError(1);
        break;
    case 2:                       //读取到换行
        c=fgetc(pF); cX++;
        if(c=='\n') {cY++;cX=0;state(0);}
        else jumpError(2);
        break;
    }
}

void error(int errorNum) {      //出错输出及处理
    switch(errorNum) {
    //1: 文件类型错误
    case 9101:
        printf("<< Error 9101: 指定的源文件不存在 >>\n");
        //在错误发生处 return   //此错误将直接终止分析过程
        break;
    case 9102:
        printf("<< Error 9102: 报告文件无法打开，将不再输出HTML分析报告 >>\n");
        outHtmlReport=0;        //正常分析，但不输出报告
        break;
    //2: 溢出/超长错误
    case 9201:
        printf("Error 9201 @ %d,%d: 注释超过 255 个字符\n",cL,cW);
        sTemp[i]='\0'; wordNum++; err++; note++;
        addReport(9201);        //在报告中输出
        jumpError(2);
        break;
    case 9202:
        printf("Error 9202 @ %d,%d: 标识符超过 64 个字符\n",cL,cW);
        sTemp[i]='\0'; wordNum++; err++;
        addReport(9202);        //在报告中输出
        jumpError(1);
        break;
    //3: 词法错误
    case 9301:
        printf("Error 9301 @ %d,%d: 实数定义尚未结束\n",cL,cW);
        sTemp[i]='\0'; wordNum++; err++;
        addReport(9301);        //在报告中输出
        jumpError(0);
        break;
    }
}

void key(char *word) { //识别关键字
    if(strcmp(word,"int")==0)           state(1101);
    else if(strcmp(word,"real")==0)     state(1102);
    else if(strcmp(word,"if")==0)       state(1201);
    else if(strcmp(word,"then")==0)     state(1202);
    else if(strcmp(word,"else")==0)     state(1203);
    else if(strcmp(word,"while")==0)    state(1204);
    else if(strcmp(word,"begin")==0)    state(1205);
    else if(strcmp(word,"end")==0)      state(1206);
    else if(strcmp(word,"case")==0)     state(1207);
    else if(strcmp(word,"and")==0)      state(1208);
    else if(strcmp(word,"or")==0)       state(1209);
    else if(strcmp(word,"not")==0)      state(1210);
    else                                state(1002);
}

void state(int stateNum) {  //状态说明及输出
    iState=stateNum;
    switch(stateNum) {
    case 0:
        i=0;
        if(tipsLevel>3) printf("State 0000: 准备就绪，开始识别下一单词\n");
        flag=0;
        break;
    //1: 可接受字母类
    case 1001:
        sTemp[i]='\0'; back=1;
        //printf("%d\n",strlen(sTemp));
        if(strlen(sTemp)==0 && !((sTemp[0]>= 'a'&& sTemp[0] <= 'z') || (sTemp[0]>='A' && sTemp[0] <= 'Z'))){
            state(0);
            break;
        }
        if(tipsLevel>2) printf("1001 @ %d,%d: 获得尚未识别的标识符,cL,cW\n");
        key(sTemp); state(0); break;
    //获得标识符
    case 1002:
        if(flag == 1 && tipsLevel>0){
			stream.str("");
			stream<<cL;
			string CL = stream.str();
			stream.str("");
			stream<<cW;
			string CW = stream.str();
			stream.str("");
			stream<<sTemp;
			string sTemp = stream.str();
			string str = CL +","+ CW +" ";
			wrong.push(str);
			// printf("1301 @ %d,%d: 获得错误标识符: %s\n",cL,cW,sTemp);
		}
        else{
            if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				stream.str("");
				stream<<sTemp;
				string sTemp = stream.str();
				string str = "1002 "+ CL +","+ CW +" "+ sTemp +"\n";
				correct.push(str);
				// printf("1002 @ %d,%d: 获得标识符: %s\n",cL,cW,sTemp);
			}
        }
        addReport(1002);      //在报告中输出
        wordNum++; break;

    //获得关键字int
    case 1101:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "1101 "+ CL +","+ CW +" int\n";
				correct.push(str);
				// printf("1101 @ %d,%d: 获得关键字: int\n",cL,cW);
			}
        addReport(1101);      //在报告中输出
        wordNum++; break;

    //获得关键字real
    case 1102:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "1102 "+ CL +","+ CW +" real\n";
				correct.push(str);
				// printf("1102 @ %d,%d: 获得关键字: real\n",cL,cW);
			}
        addReport(1102);      //在报告中输出
        wordNum++; break;

    //获得关键字if
    case 1201:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "1201 "+ CL +","+ CW +" if\n";
				correct.push(str);
				// printf("1201 @ %d,%d: 获得关键字: if\n",cL,cW);
			}
        addReport(1201);      //在报告中输出
        wordNum++; break;

    //获得关键字then
    case 1202:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "1202 "+ CL +","+ CW +" then\n";
				correct.push(str);
				// printf("1202 @ %d,%d: 获得关键字: then\n",cL,cW);
			}
        addReport(1202);      //在报告中输出
        wordNum++; break;

    //获得关键字: else
    case 1203:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "1203 "+ CL +","+ CW +" else\n";
				correct.push(str);
				// printf("1203 @ %d,%d: 获得关键字: else\n",cL,cW);
			}
        addReport(1203);      //在报告中输出
        wordNum++; break;

    //获得关键字: while
    case 1204:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "1204 "+ CL +","+ CW +" while\n";
				correct.push(str);
				// printf("1204 @ %d,%d: 获得关键字: while\n",cL,cW);
			}
        addReport(1204);      //在报告中输出
        wordNum++; break;

    //获得关键字: begin
    case 1205:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "1205 "+ CL +","+ CW +" begin\n";
				correct.push(str);
				// printf("1205 @ %d,%d: 获得关键字: begin\n",cL,cW);
			}
        addReport(1205);      //在报告中输出
        wordNum++; break;

    //获得关键字: end
    case 1206:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "1206 "+ CL +","+ CW +" end\n";
				correct.push(str);
				// printf("1206 @ %d,%d: 获得关键字: end\n",cL,cW);
			}
        addReport(1206);      //在报告中输出
        wordNum++; break;

    //获得关键字: case
    case 1207:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "1207 "+ CL +","+ CW +" case\n";
				correct.push(str);
				// printf("1207 @ %d,%d: 获得关键字: case\n",cL,cW);
			}
        addReport(1207);      //在报告中输出
        wordNum++; break;

    //获得关键字: and
    case 1208:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "1208 "+ CL +","+ CW +" and\n";
				correct.push(str);
				 // printf("1208 @ %d,%d: 获得关键字: and\n",cL,cW);
			}
        addReport(1208);      //在报告中输出
        wordNum++; break;

    //获得关键字: or
    case 1209:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "1209 "+ CL +","+ CW +" or\n";
				correct.push(str);
				// printf("1209 @ %d,%d: 获得关键字: or\n",cL,cW);
			}
        addReport(1209);      //在报告中输出
        wordNum++; break;

    //获得关键字: not
    case 1210:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "1210 "+ CL +","+ CW +" not\n";
				correct.push(str);
				// printf("1210 @ %d,%d: 获得关键字: not\n",cL,cW);
			}
        addReport(1210);      //在报告中输出
        wordNum++; break;
    //2: 可接受数字类

    //整数
    case 2001:
        sTemp[i]='\0'; back=1;
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				stream.str("");
				stream<<sTemp;
				string sTemp = stream.str();
				string str = "2001 "+ CL +","+ CW +" "+ sTemp +"\n";
				correct.push(str);
				// printf("2001 @ %d,%d: 获得整数: %s\n",cL,cW,sTemp);
			}
        addReport(2001);      //在报告中输出
        state(0); wordNum++; break;

    //实数
    case 2002:
        sTemp[i]='\0'; back=1;
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				stream.str("");
				stream<<sTemp;
				string sTemp = stream.str();
				string str = "2002 "+ CL +","+ CW +" "+ sTemp +"\n";
				correct.push(str);
				// printf("2002 @ %d,%d: 获得实数: %s\n",cL,cW,sTemp);
			}
        addReport(2002);      //在报告中输出
        state(0); wordNum++; break;
    //3: 可接受符号类

    //加号
    case 3001:
        cL=cY; cW=cX;
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3001 "+ CL +","+ CW +" +\n";
				correct.push(str);
				// printf("3001 @ %d,%d: 获得加号: +\n",cL,cW);
			}
        addReport(3001);      //在报告中输出
        state(0); wordNum++; break;

    //减号
    case 3002:
        cL=cY; cW=cX;
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3002 "+ CL +","+ CW +" -\n";
				correct.push(str);
				// printf("3002 @ %d,%d: 获得减号: -\n",cL,cW);
			}
        addReport(3002);      //在报告中输出
        state(0); wordNum++; break;

    //乘号
    case 3003:
        cL=cY; cW=cX;
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3003 "+ CL +","+ CW +" *\n";
				correct.push(str);
				// printf("3003 @ %d,%d: 获得乘号: *\n",cL,cW);
			}
        addReport(3003);      //在报告中输出
        state(0); wordNum++; break;

    //除号
    case 3004:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3004 "+ CL +","+ CW +" \n";
				correct.push(str);
				// printf("3004 @ %d,%d: 获得除号: /\n",cL,cW);
			}
        addReport(3004);      //在报告中输出
        back=1; state(0); wordNum++; break;

    //赋值号
    case 3005:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3005 "+ CL +","+ CW +" =\n";
				correct.push(str);
				// printf("3005 @ %d,%d: 获得赋值号: =\n",cL,cW);
			}
        addReport(3005);      //在报告中输出
        back=1; state(0); wordNum++; break;

    //赋值号
    case 3006:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3006 "+ CL +","+ CW +" ==\n";
				correct.push(str);
				// printf("3006 @ %d,%d: 获得赋值号: ==\n",cL,cW);
			}
        addReport(3006);      //在报告中输出
        state(0); wordNum++; break;

    //大于号
    case 3007:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3007 "+ CL +","+ CW +" >\n";
				correct.push(str);
				// printf("3007 @ %d,%d: 获得大于号: >\n",cL,cW);
			}
        addReport(3007);      //在报告中输出
        back=1; state(0); wordNum++; break;

    //大于等于
    case 3008:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3008 "+ CL +","+ CW +" >=\n";
				correct.push(str);
				// printf("3008 @ %d,%d: 获得大于等于号: >=\n",cL,cW);
			}
        addReport(3008);      //在报告中输出
        state(0); wordNum++; break;

    //小于
    case 3009:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3009 "+ CL +","+ CW +" <\n";
				correct.push(str);
				// printf("3009 @ %d,%d: 获得小于号: <\n",cL,cW);
			}
        addReport(3009);      //在报告中输出
        back=1; state(0); wordNum++; break;

    //小于等于
    case 3010:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3010 "+ CL +","+ CW +" <=\n";
				correct.push(str);
				// printf("3010 @ %d,%d: 获得小于等于号: <=\n",cL,cW);
			}
        addReport(3010);      //在报告中输出
        state(0); wordNum++; break;

    //不等于<>
    case 3011:
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3011 "+ CL +","+ CW +" <>\n";
				correct.push(str);
				// printf("3011 @ %d,%d: 获得不等于号: <>\n",cL,cW);
			}
        addReport(3011);      //在报告中输出
        state(0); wordNum++; break;

    //结束号
    case 3012:
        cL=cY; cW=cX;
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3012 "+ CL +","+ CW +" ;\n";
				correct.push(str);
				// printf("3012 @ %d,%d: 获得结束号: ;\n",cL,cW);
			}
        addReport(3012);      //在报告中输出
        state(0); wordNum++; break;

    //左括号
    case 3013:      //括号需要区分为运算提级号或传递参数号
        cL=cY; cW=cX;
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3013 "+ CL +","+ CW +" (\n";
				correct.push(str);
				// printf("3013 @ %d,%d: 获得左括号: (\n",cL,cW);
			}
        addReport(3013);      //在报告中输出
        state(0); wordNum++; break;

    //右括号
    case 3014:
        cL=cY; cW=cX;
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3014 "+ CL +","+ CW +" )\n";
				correct.push(str);
				// printf("3014 @ %d,%d: 获得右括号: )\n",cL,cW);
			}
        addReport(3014);      //在报告中输出
        state(0); wordNum++; break;

    //左区号
    case 3015:
        cL=cY; cW=cX;
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3015 "+ CL +","+ CW +" {\n";
				correct.push(str);
				// printf("3015 @ %d,%d: 获得左区号: {\n",cL,cW);
			}
        addReport(3015);      //在报告中输出
        state(0); wordNum++; break;

    //右区号
    case 3016:
        cL=cY; cW=cX;
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3016 "+ CL +","+ CW +" }\n";
				correct.push(str);
				// printf("3016 @ %d,%d: 获得右区号: }\n",cL,cW);
			}
        addReport(3016);      //在报告中输出
        state(0); wordNum++; break;

    //左下标号
    case 3017:
        cL=cY; cW=cX;
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3017 "+ CL +","+ CW +" [\n";
				correct.push(str);
				// printf("3017 @ %d,%d: 获得左下标号: [\n",cL,cW);
			}
        addReport(3017);      //在报告中输出
        state(0); wordNum++; break;

    //右下标号
    case 3018:
        cL=cY; cW=cX;
        if(tipsLevel>0){
				stream.str("");
				stream<<cL;
				string CL = stream.str();
				stream.str("");
				stream<<cW;
				string CW = stream.str();
				string str = "3018 "+ CL +","+ CW +" ]\n";
				correct.push(str);
				// printf("3018 @ %d,%d: 获得右下标号: ]\n",cL,cW);
			}
        addReport(3018);      //在报告中输出
        state(0); wordNum++; break;

    //逗号
    case 3019:      //逗号运算符？
        cL=cY; cW=cX;
        if(tipsLevel>0){
			stream.str("");
			stream<<cL;
			string CL = stream.str();
			stream.str("");
			stream<<cW;
			string CW = stream.str();
			string str = "3019 "+ CL +","+ CW +" ,\n";
			correct.push(str);
			// printf("3019 @ %d,%d: 获得分隔号: ,\n",cL,cW);
		}
        addReport(3019);      //在报告中输出
        state(0); wordNum++; break;

    //英文句号
    case 3020:      //句号运算符
        cL=cY; cW=cX;
        if(tipsLevel>0){
			stream.str("");
			stream<<cL;
			string CL = stream.str();
			stream.str("");
			stream<<cW;
			string CW = stream.str();
			string str = "3020 "+ CL +","+ CW +" .\n";
			correct.push(str);
			// printf("3020 @ %d,%d: 获得分隔号: .\n",cL,cW);
		}
        addReport(3020);      //在报告中输出
        state(0); wordNum++; break;
		
	//百分号
	case 3021:		//百分号运算符
		cL=cY; cW=cX;
		if(tipsLevel>0){
			stream.str("");
			stream<<cL;
			string CL = stream.str();
			stream.str("");
			stream<<cW;
			string CW = stream.str();
			string str = "3021 "+ CL +","+ CW +" %\n";
			correct.push(str);
			// printf("3021 @ %d,%d: 获得百分号: %\n",cL,cW);
		}
		addReport(3021);      //在报告中输出
        state(0); wordNum++; break;
		
	//&
	case 3022:		//&
		cL=cY; cW=cX;
		if(tipsLevel>0){
			stream.str("");
			stream<<cL;
			string CL = stream.str();
			stream.str("");
			stream<<cW;
			string CW = stream.str();
			string str = "3022 "+ CL +","+ CW +" &\n";
			correct.push(str);
			// printf("3022 @ %d,%d: 获得百分号: &\n",cL,cW);
		}
		addReport(3022);      //在报告中输出
        state(0); wordNum++; break;
		
	//？
	case 3023:		//？
		cL=cY; cW=cX;
		if(tipsLevel>0){
			stream.str("");
			stream<<cL;
			string CL = stream.str();
			stream.str("");
			stream<<cW;
			string CW = stream.str();
			string str = "3023 "+ CL +","+ CW +" ?\n";
			correct.push(str);
			// printf("3023 @ %d,%d: 获得百分号: ?\n",cL,cW);
		}
		addReport(3023);      //在报告中输出
        state(0); wordNum++; break;
		
	//"
	case 3024:		//"
		cL=cY; cW=cX;
		if(tipsLevel>0){
			stream.str("");
			stream<<cL;
			string CL = stream.str();
			stream.str("");
			stream<<cW;
			string CW = stream.str();
			string str = "3024 "+ CL +","+ CW +" \"\n";
			correct.push(str);
			// printf("3024 @ %d,%d: 获得百分号: "\n",cL,cW);
		}
		addReport(3024);      //在报告中输出
        state(0); wordNum++; break;
		
	//'
	case 3025:		//'
		cL=cY; cW=cX;
		if(tipsLevel>0){
			stream.str("");
			stream<<cL;
			string CL = stream.str();
			stream.str("");
			stream<<cW;
			string CW = stream.str();
			string str = "3025 "+ CL +","+ CW +" '\n";
			correct.push(str);
			// printf("3025 @ %d,%d: 获得百分号: '\n",cL,cW);
		}
		addReport(3025);      //在报告中输出
        state(0); wordNum++; break;
		
	//:
	case 3026:		//:
		cL=cY; cW=cX;
		if(tipsLevel>0){
			stream.str("");
			stream<<cL;
			string CL = stream.str();
			stream.str("");
			stream<<cW;
			string CW = stream.str();
			string str = "3026 "+ CL +","+ CW +" :\n";
			correct.push(str);
			// printf("3026 @ %d,%d: 获得百分号: :\n",cL,cW);
		}
		addReport(3026);      //在报告中输出
        state(0); wordNum++; break;
	
    //5: 其他类
    case 5001:
        if(tipsLevel>2){
			stream.str("");
			string str = "3019 : 注释开始\n";
			correct.push(str);
			//printf("5001: 注释开始\n");
		}
        break;

    //注释
    case 5002:
        sTemp[i]='\0'; back=1; note++; wordNum++;
        if(tipsLevel>1){
			stream.str("");
			stream<<cL;
			string CL = stream.str();
			stream.str("");
			stream<<cW;
			string CW = stream.str();
			stream.str("");
			stream<<sTemp;
			string sTemp = stream.str();
			string str = "5002 "+ CL +","+ CW +": 获得注释: "+ sTemp +"\n";
			correct.push(str);
			// printf("5002 @ %d,%d: 获得注释: %s\n",cL,cW,sTemp);
		}
        addReport(5002);      //在报告中输出
        state(0); break;
    //7: 尚未结束类

    //字母
    case 7001:
        if(tipsLevel>2){
			stream.str("");
			stream<<cY;
			string CY = stream.str();
			stream.str("");
			stream<<cX;
			string CX = stream.str();
			string str = "7001 "+ CY +","+ CX +" \n";
			correct.push(str);
			// printf("7001 @ %d,%d: 收到一个字母\n",cY,cX);
		}
        cL=cY; cW=cX; break;

    //数字
    case 7002:
        if(tipsLevel>2){
			stream.str("");
			stream<<cY;
			string CY = stream.str();
			stream.str("");
			stream<<cX;
			string CX = stream.str();
			string str = "7002 "+ CY +","+ CX +" \n";
			correct.push(str);
			// printf("7002 @ %d,%d: 收到一个数字\n",cY,cX);
		}
        cL=cY; cW=cX; break;

    // 收到一个/
    case 7003:
        if(tipsLevel>2){
			stream.str("");
			stream<<cY;
			string CY = stream.str();
			stream.str("");
			stream<<cX;
			string CX = stream.str();
			string str = "7003 "+ CY +","+ CX +" /\n";
			correct.push(str);
			// printf("7003 @ %d,%d: 收到一个/\n",cY,cX);
		}
        cL=cY; cW=cX; break;

    // =
    case 7004:
        if(tipsLevel>2){
			stream.str("");
			stream<<cY;
			string CY = stream.str();
			stream.str("");
			stream<<cX;
			string CX = stream.str();
			string str = "7004 "+ CY +","+ CX +" =\n";
			correct.push(str);
			// printf("7004 @ %d,%d: 收到一个 =\n",cY,cX);
		}
        cL=cY; cW=cX; break;

    // >
    case 7005:
        if(tipsLevel>2){
			stream.str("");
			stream<<cY;
			string CY = stream.str();
			stream.str("");
			stream<<cX;
			string CX = stream.str();
			string str = "7005 "+ CY +","+ CX +" >\n";
			correct.push(str);
			// printf("7005 @ %d,%d: 收到一个 >\n",cY,cX);
		}
        cL=cY; cW=cX; break;

    // <
    case 7006:
        if(tipsLevel>2){
			stream.str("");
			stream<<cY;
			string CY = stream.str();
			stream.str("");
			stream<<cX;
			string CX = stream.str();
			string str = "7006 "+ CY +","+ CX +" <\n";
			correct.push(str);
			//  printf("7006 @ %d,%d: 收到一个 <\n",cY,cX);
		}
        cL=cY; cW=cX; break;

    // E
    case 7007:
        if(tipsLevel>2){
			stream.str("");
			stream<<cY;
			string CY = stream.str();
			stream.str("");
			stream<<cX;
			string CX = stream.str();
			string str = "7007 "+ CY +","+ CX +" E\n";
			correct.push(str);
			//  printf("7007 @ %d,%d: 收到一个 E\n",cY,cX);
		}
        break;

    // .
    case 7008:
        if(tipsLevel>2){
			stream.str("");
			stream<<cY;
			string CY = stream.str();
			stream.str("");
			stream<<cX;
			string CX = stream.str();
			string str = "7008 "+ CY +","+ CX +" .\n";
			correct.push(str);
			//  printf("7008 @ %d,%d: 收到一个 .\n",cY,cX);
		}
        break;

    //收到一个C
    case 7009:
        if(tipsLevel>2){
			stream.str("");
			stream<<cY;
			string CY = stream.str();
			stream.str("");
			stream<<cX;
			string CX = stream.str();
			stream.str("");
			stream<<c;
			string c = stream.str();
			string str = "7009 "+ CY +","+ CX +" "+ c +"\n";
			correct.push(str);
			//  printf("7009 @ %d,%d: 收到一个 %c\n",cY,cX,c);
		}
        break;

    //数字
    case 7010:
        if(tipsLevel>2){
			stream.str("");
			stream<<cY;
			string CY = stream.str();
			stream.str("");
			stream<<cX;
			string CX = stream.str();
			string str = "7010 "+ CY +","+ CX +" \n";
			correct.push(str);
			// printf("7010 @ %d,%d: 收到一个数字\n",cY,cX);
		}
        break;
    //8: 不可接受类
    //9: 出错类，在出错处理中定义
    }
}

void addReport(int n) {
    if(!outHtmlReport) return;
    if(cL>hL) {                     //发现标识符时，判断是否已输出换行
        fputs(REPORT_LineEnd,pH);
        fputs((++hL%2==1)?REPORT_LineHead1:REPORT_LineHead2,pH);
        char t[6]="\0";
        sprintf(t,"%d.",hL);
        fputs(t,pH);
        fputs(REPORT_LineAfter,pH);
    }
    fputs(REPORT_TipHead,pH);
    switch(n) {
    case 9201:
        fputs("注释超过 255 个字符",pH);
        REPORT_TipAfter="');\" style=\"color:red;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 9202:
        fputs("标识符超过 64 个字符",pH);
        REPORT_TipAfter="');\" style=\"color:red;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 9301:
        fputs("实数定义尚未结束",pH);
        REPORT_TipAfter="');\" style=\"color:red;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1002:
        fputs("标识符",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1101:
        fputs("int 整型声明",pH);
        REPORT_TipAfter="');\" style=\"color:blue;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1102:
        fputs("real 实型声明",pH);
        REPORT_TipAfter="');\" style=\"color:blue;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1201:
        fputs("if 判断",pH);
        REPORT_TipAfter="');\" style=\"color:blue;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1202:
        fputs("then 真分支",pH);
        REPORT_TipAfter="');\" style=\"color:blue;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1203:
        fputs("else 假分支",pH);
        REPORT_TipAfter="');\" style=\"color:blue;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1204:
        fputs("while 循环",pH);
        REPORT_TipAfter="');\" style=\"color:blue;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 2001:
        fputs("整数",pH);
        REPORT_TipAfter="');\" style=\"color:#FF50A0;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 2002:
        fputs("实数",pH);
        REPORT_TipAfter="');\" style=\"color:#FF50A0;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 3001:
        fputs("加号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("+",pH); break;
    case 3002:
        fputs("减号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("-",pH); break;
    case 3003:
        fputs("乘号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("*",pH); break;
    case 3004:
        fputs("除号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("/",pH); break;
    case 3005:
        fputs("赋值号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("=",pH); break;
    case 3006:
        fputs("等于号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("==",pH); break;
    case 3007:
        fputs("大于号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(">",pH); break;
    case 3008:
        fputs("大于等于号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(">=",pH); break;
    case 3009:
        fputs("小于号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("<",pH); break;
    case 3010:
        fputs("小于等于号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("<=",pH); break;
    case 3011:
        fputs("不等于号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("<>",pH); break;
    case 3012:
        fputs("结束号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(";",pH); break;
    case 3013:
        fputs("左括号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("(",pH); break;
    case 3014:
        fputs("右括号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(")",pH); break;
    case 3015:
        fputs("左区号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("{",pH); break;
    case 3016:
        fputs("右区号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("}",pH); break;
    case 3017:
        fputs("左下标号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("[",pH); break;
    case 3018:
        fputs("右下标号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("]",pH); break;
    case 3019:
        fputs("分隔符",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(",",pH); break;
	case 3020:
        fputs("英文句号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(".",pH); break;
	case 3021:
        fputs("百分号",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("%",pH); break;
	case 3022:
        fputs("&",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("&",pH); break;
    case 5002:
        fputs("注释",pH);
        REPORT_TipAfter="');\" style=\"color:green;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("//",pH); fputs(sTemp,pH); break;
    }
    fputs(REPORT_WordAfter,pH);
}
