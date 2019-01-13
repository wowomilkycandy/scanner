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
    else while(argc-- > 1) word(*++argv);       //��ȡ�ⲿ���в���
    printf("<< ����Դ�ļ����Ѿ�������� >>\n");
    system("pause");
    return 0;
}

void word(const char *SourceFile) {
    printf("<< ׼������Դ�ļ� \"%s\"������������� >>\n",SourceFile); getch(); system("cls");
    if((pF=fopen(SourceFile,"r"))==NULL) {error(9101); return;}   //��Դ�ļ�
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
            c=fgetc(pF); cX++;                  //��ȡ����ʱ�Ƿֳ������
            if(c==9) c=' ';                     //��Tab����Ϊһ���ո�
            if(c=='\n') {cY++; cX=0;}           //�������ӣ�λ������
        }
        back=0;
        switch(iState) {
        case 5001:                              //ע�Ϳ�ʼ
            if(c=='\n'||c==EOF) state(5002);    //ע�ͽ���
            else {
                if(i<255) sTemp[i++]=c;         //ע�ͳ�������
                else error(9201);
            }
            break;
        case 7003:                              //  /
            if(c=='/') state(5001);             //ע��
            else state(3004);                   //����
            break;
        case 7004:                              //  =
            if(c=='=') state(3006);             //����
            else state(3005);                   //��ֵ
            break;
        case 7005:                              //  >
            if(c=='=') state(3008);             //���ڵ���
            else if(c=='>') ;                   //�߼�����/��������
            else state(3007);                   //����
            break;
        case 7006:                              //  <
            if(c=='=') state(3010);             //С�ڵ���
            else if(c=='<') ;                   //����
            else if(c=='>') state(3011);        //������
            else state(3009);                   //С��
            break;
        case 7001:                              //��ĸ
            if(isID(c)) {                       //��ʶ���ַ�
                if(i<64) sTemp[i++]=c;          //��ʶ����������
                else error(9202);
            }
            //else state(1001);                   //��ʶ��
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
        case 7002:                              //����
            if(isDigit(c)) sTemp[i++]=c;
            else if(c=='E') {                   //��ѧ������ʵ��
                sTemp[i++]=c;
                state(7007);
            }
            else if(c=='.') {                   //С����ʵ��
                sTemp[i++]=c;
                state(7008);
            }
            else state(2001);                   //����
            break;
        case 7008:                              //С����
            if(isDigit(c)) sTemp[i++]=c;
            else if(c=='E') {                   //��ѧ������ʵ��
                sTemp[i++]=c;
                state(7007);
            }
            else state(2002);                   //С����ʵ��
            break;
        case 7007:                              //��ѧ������E
            if(c=='+'||c=='-') {                //ָ������
                sTemp[i++]=c;
                state(7009);
            }
            else if(isDigit(c)) {               //ָ��
                sTemp[i++]=c;
                state(7010);
            }
            else error(9301);                   //δ��������
            break;
        case 7009:                              //ָ������
            if(isDigit(c)) {                    //ָ��
                sTemp[i++]=c;
                state(7010);
            }
            else error(9301);                   //δ��������
            break;
        case 7010:                              //ָ��
            if(isDigit(c)) sTemp[i++]=c;        //����
            else state(2002);                   //��ѧ������ʵ��
            break;
        case 0:
            if(isLetter(c)) {                   //��ĸ
                state(7001);
                sTemp[i++]=c;
            }
            else if(isDigit(c)) {               //����
                state(7002);
                sTemp[i++]=c;
            }
            else if(outHtmlReport&&c=='\n') {   //û�б�ʶ��ʱ���������������
                hL=cY;
                fputs(REPORT_LineEnd,pH);
                fputs((cY%2==1)?REPORT_LineHead1:REPORT_LineHead2,pH);
                char t[6]="\0";
                sprintf(t,"%d.",hL);            //����к�
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
				if(!wrong.empty()) cout<<"�ʷ��������"<<wrong.size()<<"��"<<"\n"<<endl;
				while(!wrong.empty()){
					string item = wrong.front();
					wrong.pop();
					cout<<item;
				}
				cout<<"\n"<<endl;
			}
		}
    } while(c!=EOF);                //�ļ�����
    fclose(pF);                     //�ر��ļ�
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
        fclose(pH);                 //�ر��ļ�
        printf("<< ������������� \"%s\" >>\n",file);
    }
    printf("<< Դ�ļ� \"%s\" ��������������������� >>\n",SourceFile); getch();
}

void jumpError(int k) {           //������������
    switch(k) {
    case 0:                       //�жϴ��ַ�
        if(c==' '||c=='\n') state(0);
        else jumpError(1);
        break;
    case 1:                       //��ȡ���հף��ո��Ʊ����У�
        c=fgetc(pF); cX++;
        if(c==' '||c==9) state(0);
        else if(c=='\n') {cY++;cX=0;state(0);}
        else jumpError(1);
        break;
    case 2:                       //��ȡ������
        c=fgetc(pF); cX++;
        if(c=='\n') {cY++;cX=0;state(0);}
        else jumpError(2);
        break;
    }
}

void error(int errorNum) {      //�������������
    switch(errorNum) {
    //1: �ļ����ʹ���
    case 9101:
        printf("<< Error 9101: ָ����Դ�ļ������� >>\n");
        //�ڴ������� return   //�˴���ֱ����ֹ��������
        break;
    case 9102:
        printf("<< Error 9102: �����ļ��޷��򿪣����������HTML�������� >>\n");
        outHtmlReport=0;        //���������������������
        break;
    //2: ���/��������
    case 9201:
        printf("Error 9201 @ %d,%d: ע�ͳ��� 255 ���ַ�\n",cL,cW);
        sTemp[i]='\0'; wordNum++; err++; note++;
        addReport(9201);        //�ڱ��������
        jumpError(2);
        break;
    case 9202:
        printf("Error 9202 @ %d,%d: ��ʶ������ 64 ���ַ�\n",cL,cW);
        sTemp[i]='\0'; wordNum++; err++;
        addReport(9202);        //�ڱ��������
        jumpError(1);
        break;
    //3: �ʷ�����
    case 9301:
        printf("Error 9301 @ %d,%d: ʵ��������δ����\n",cL,cW);
        sTemp[i]='\0'; wordNum++; err++;
        addReport(9301);        //�ڱ��������
        jumpError(0);
        break;
    }
}

void key(char *word) { //ʶ��ؼ���
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

void state(int stateNum) {  //״̬˵�������
    iState=stateNum;
    switch(stateNum) {
    case 0:
        i=0;
        if(tipsLevel>3) printf("State 0000: ׼����������ʼʶ����һ����\n");
        flag=0;
        break;
    //1: �ɽ�����ĸ��
    case 1001:
        sTemp[i]='\0'; back=1;
        //printf("%d\n",strlen(sTemp));
        if(strlen(sTemp)==0 && !((sTemp[0]>= 'a'&& sTemp[0] <= 'z') || (sTemp[0]>='A' && sTemp[0] <= 'Z'))){
            state(0);
            break;
        }
        if(tipsLevel>2) printf("1001 @ %d,%d: �����δʶ��ı�ʶ��,cL,cW\n");
        key(sTemp); state(0); break;
    //��ñ�ʶ��
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
			// printf("1301 @ %d,%d: ��ô����ʶ��: %s\n",cL,cW,sTemp);
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
				// printf("1002 @ %d,%d: ��ñ�ʶ��: %s\n",cL,cW,sTemp);
			}
        }
        addReport(1002);      //�ڱ��������
        wordNum++; break;

    //��ùؼ���int
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
				// printf("1101 @ %d,%d: ��ùؼ���: int\n",cL,cW);
			}
        addReport(1101);      //�ڱ��������
        wordNum++; break;

    //��ùؼ���real
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
				// printf("1102 @ %d,%d: ��ùؼ���: real\n",cL,cW);
			}
        addReport(1102);      //�ڱ��������
        wordNum++; break;

    //��ùؼ���if
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
				// printf("1201 @ %d,%d: ��ùؼ���: if\n",cL,cW);
			}
        addReport(1201);      //�ڱ��������
        wordNum++; break;

    //��ùؼ���then
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
				// printf("1202 @ %d,%d: ��ùؼ���: then\n",cL,cW);
			}
        addReport(1202);      //�ڱ��������
        wordNum++; break;

    //��ùؼ���: else
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
				// printf("1203 @ %d,%d: ��ùؼ���: else\n",cL,cW);
			}
        addReport(1203);      //�ڱ��������
        wordNum++; break;

    //��ùؼ���: while
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
				// printf("1204 @ %d,%d: ��ùؼ���: while\n",cL,cW);
			}
        addReport(1204);      //�ڱ��������
        wordNum++; break;

    //��ùؼ���: begin
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
				// printf("1205 @ %d,%d: ��ùؼ���: begin\n",cL,cW);
			}
        addReport(1205);      //�ڱ��������
        wordNum++; break;

    //��ùؼ���: end
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
				// printf("1206 @ %d,%d: ��ùؼ���: end\n",cL,cW);
			}
        addReport(1206);      //�ڱ��������
        wordNum++; break;

    //��ùؼ���: case
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
				// printf("1207 @ %d,%d: ��ùؼ���: case\n",cL,cW);
			}
        addReport(1207);      //�ڱ��������
        wordNum++; break;

    //��ùؼ���: and
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
				 // printf("1208 @ %d,%d: ��ùؼ���: and\n",cL,cW);
			}
        addReport(1208);      //�ڱ��������
        wordNum++; break;

    //��ùؼ���: or
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
				// printf("1209 @ %d,%d: ��ùؼ���: or\n",cL,cW);
			}
        addReport(1209);      //�ڱ��������
        wordNum++; break;

    //��ùؼ���: not
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
				// printf("1210 @ %d,%d: ��ùؼ���: not\n",cL,cW);
			}
        addReport(1210);      //�ڱ��������
        wordNum++; break;
    //2: �ɽ���������

    //����
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
				// printf("2001 @ %d,%d: �������: %s\n",cL,cW,sTemp);
			}
        addReport(2001);      //�ڱ��������
        state(0); wordNum++; break;

    //ʵ��
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
				// printf("2002 @ %d,%d: ���ʵ��: %s\n",cL,cW,sTemp);
			}
        addReport(2002);      //�ڱ��������
        state(0); wordNum++; break;
    //3: �ɽ��ܷ�����

    //�Ӻ�
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
				// printf("3001 @ %d,%d: ��üӺ�: +\n",cL,cW);
			}
        addReport(3001);      //�ڱ��������
        state(0); wordNum++; break;

    //����
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
				// printf("3002 @ %d,%d: ��ü���: -\n",cL,cW);
			}
        addReport(3002);      //�ڱ��������
        state(0); wordNum++; break;

    //�˺�
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
				// printf("3003 @ %d,%d: ��ó˺�: *\n",cL,cW);
			}
        addReport(3003);      //�ڱ��������
        state(0); wordNum++; break;

    //����
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
				// printf("3004 @ %d,%d: ��ó���: /\n",cL,cW);
			}
        addReport(3004);      //�ڱ��������
        back=1; state(0); wordNum++; break;

    //��ֵ��
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
				// printf("3005 @ %d,%d: ��ø�ֵ��: =\n",cL,cW);
			}
        addReport(3005);      //�ڱ��������
        back=1; state(0); wordNum++; break;

    //��ֵ��
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
				// printf("3006 @ %d,%d: ��ø�ֵ��: ==\n",cL,cW);
			}
        addReport(3006);      //�ڱ��������
        state(0); wordNum++; break;

    //���ں�
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
				// printf("3007 @ %d,%d: ��ô��ں�: >\n",cL,cW);
			}
        addReport(3007);      //�ڱ��������
        back=1; state(0); wordNum++; break;

    //���ڵ���
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
				// printf("3008 @ %d,%d: ��ô��ڵ��ں�: >=\n",cL,cW);
			}
        addReport(3008);      //�ڱ��������
        state(0); wordNum++; break;

    //С��
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
				// printf("3009 @ %d,%d: ���С�ں�: <\n",cL,cW);
			}
        addReport(3009);      //�ڱ��������
        back=1; state(0); wordNum++; break;

    //С�ڵ���
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
				// printf("3010 @ %d,%d: ���С�ڵ��ں�: <=\n",cL,cW);
			}
        addReport(3010);      //�ڱ��������
        state(0); wordNum++; break;

    //������<>
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
				// printf("3011 @ %d,%d: ��ò����ں�: <>\n",cL,cW);
			}
        addReport(3011);      //�ڱ��������
        state(0); wordNum++; break;

    //������
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
				// printf("3012 @ %d,%d: ��ý�����: ;\n",cL,cW);
			}
        addReport(3012);      //�ڱ��������
        state(0); wordNum++; break;

    //������
    case 3013:      //������Ҫ����Ϊ�����ἶ�Ż򴫵ݲ�����
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
				// printf("3013 @ %d,%d: ���������: (\n",cL,cW);
			}
        addReport(3013);      //�ڱ��������
        state(0); wordNum++; break;

    //������
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
				// printf("3014 @ %d,%d: ���������: )\n",cL,cW);
			}
        addReport(3014);      //�ڱ��������
        state(0); wordNum++; break;

    //������
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
				// printf("3015 @ %d,%d: ���������: {\n",cL,cW);
			}
        addReport(3015);      //�ڱ��������
        state(0); wordNum++; break;

    //������
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
				// printf("3016 @ %d,%d: ���������: }\n",cL,cW);
			}
        addReport(3016);      //�ڱ��������
        state(0); wordNum++; break;

    //���±��
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
				// printf("3017 @ %d,%d: ������±��: [\n",cL,cW);
			}
        addReport(3017);      //�ڱ��������
        state(0); wordNum++; break;

    //���±��
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
				// printf("3018 @ %d,%d: ������±��: ]\n",cL,cW);
			}
        addReport(3018);      //�ڱ��������
        state(0); wordNum++; break;

    //����
    case 3019:      //�����������
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
			// printf("3019 @ %d,%d: ��÷ָ���: ,\n",cL,cW);
		}
        addReport(3019);      //�ڱ��������
        state(0); wordNum++; break;

    //Ӣ�ľ��
    case 3020:      //��������
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
			// printf("3020 @ %d,%d: ��÷ָ���: .\n",cL,cW);
		}
        addReport(3020);      //�ڱ��������
        state(0); wordNum++; break;
		
	//�ٷֺ�
	case 3021:		//�ٷֺ������
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
			// printf("3021 @ %d,%d: ��ðٷֺ�: %\n",cL,cW);
		}
		addReport(3021);      //�ڱ��������
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
			// printf("3022 @ %d,%d: ��ðٷֺ�: &\n",cL,cW);
		}
		addReport(3022);      //�ڱ��������
        state(0); wordNum++; break;
		
	//��
	case 3023:		//��
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
			// printf("3023 @ %d,%d: ��ðٷֺ�: ?\n",cL,cW);
		}
		addReport(3023);      //�ڱ��������
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
			// printf("3024 @ %d,%d: ��ðٷֺ�: "\n",cL,cW);
		}
		addReport(3024);      //�ڱ��������
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
			// printf("3025 @ %d,%d: ��ðٷֺ�: '\n",cL,cW);
		}
		addReport(3025);      //�ڱ��������
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
			// printf("3026 @ %d,%d: ��ðٷֺ�: :\n",cL,cW);
		}
		addReport(3026);      //�ڱ��������
        state(0); wordNum++; break;
	
    //5: ������
    case 5001:
        if(tipsLevel>2){
			stream.str("");
			string str = "3019 : ע�Ϳ�ʼ\n";
			correct.push(str);
			//printf("5001: ע�Ϳ�ʼ\n");
		}
        break;

    //ע��
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
			string str = "5002 "+ CL +","+ CW +": ���ע��: "+ sTemp +"\n";
			correct.push(str);
			// printf("5002 @ %d,%d: ���ע��: %s\n",cL,cW,sTemp);
		}
        addReport(5002);      //�ڱ��������
        state(0); break;
    //7: ��δ������

    //��ĸ
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
			// printf("7001 @ %d,%d: �յ�һ����ĸ\n",cY,cX);
		}
        cL=cY; cW=cX; break;

    //����
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
			// printf("7002 @ %d,%d: �յ�һ������\n",cY,cX);
		}
        cL=cY; cW=cX; break;

    // �յ�һ��/
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
			// printf("7003 @ %d,%d: �յ�һ��/\n",cY,cX);
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
			// printf("7004 @ %d,%d: �յ�һ�� =\n",cY,cX);
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
			// printf("7005 @ %d,%d: �յ�һ�� >\n",cY,cX);
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
			//  printf("7006 @ %d,%d: �յ�һ�� <\n",cY,cX);
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
			//  printf("7007 @ %d,%d: �յ�һ�� E\n",cY,cX);
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
			//  printf("7008 @ %d,%d: �յ�һ�� .\n",cY,cX);
		}
        break;

    //�յ�һ��C
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
			//  printf("7009 @ %d,%d: �յ�һ�� %c\n",cY,cX,c);
		}
        break;

    //����
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
			// printf("7010 @ %d,%d: �յ�һ������\n",cY,cX);
		}
        break;
    //8: ���ɽ�����
    //9: �����࣬�ڳ������ж���
    }
}

void addReport(int n) {
    if(!outHtmlReport) return;
    if(cL>hL) {                     //���ֱ�ʶ��ʱ���ж��Ƿ����������
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
        fputs("ע�ͳ��� 255 ���ַ�",pH);
        REPORT_TipAfter="');\" style=\"color:red;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 9202:
        fputs("��ʶ������ 64 ���ַ�",pH);
        REPORT_TipAfter="');\" style=\"color:red;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 9301:
        fputs("ʵ��������δ����",pH);
        REPORT_TipAfter="');\" style=\"color:red;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1002:
        fputs("��ʶ��",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1101:
        fputs("int ��������",pH);
        REPORT_TipAfter="');\" style=\"color:blue;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1102:
        fputs("real ʵ������",pH);
        REPORT_TipAfter="');\" style=\"color:blue;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1201:
        fputs("if �ж�",pH);
        REPORT_TipAfter="');\" style=\"color:blue;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1202:
        fputs("then ���֧",pH);
        REPORT_TipAfter="');\" style=\"color:blue;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1203:
        fputs("else �ٷ�֧",pH);
        REPORT_TipAfter="');\" style=\"color:blue;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 1204:
        fputs("while ѭ��",pH);
        REPORT_TipAfter="');\" style=\"color:blue;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 2001:
        fputs("����",pH);
        REPORT_TipAfter="');\" style=\"color:#FF50A0;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 2002:
        fputs("ʵ��",pH);
        REPORT_TipAfter="');\" style=\"color:#FF50A0;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(sTemp,pH); break;
    case 3001:
        fputs("�Ӻ�",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("+",pH); break;
    case 3002:
        fputs("����",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("-",pH); break;
    case 3003:
        fputs("�˺�",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("*",pH); break;
    case 3004:
        fputs("����",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("/",pH); break;
    case 3005:
        fputs("��ֵ��",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("=",pH); break;
    case 3006:
        fputs("���ں�",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("==",pH); break;
    case 3007:
        fputs("���ں�",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(">",pH); break;
    case 3008:
        fputs("���ڵ��ں�",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(">=",pH); break;
    case 3009:
        fputs("С�ں�",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("<",pH); break;
    case 3010:
        fputs("С�ڵ��ں�",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("<=",pH); break;
    case 3011:
        fputs("�����ں�",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("<>",pH); break;
    case 3012:
        fputs("������",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(";",pH); break;
    case 3013:
        fputs("������",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("(",pH); break;
    case 3014:
        fputs("������",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(")",pH); break;
    case 3015:
        fputs("������",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("{",pH); break;
    case 3016:
        fputs("������",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("}",pH); break;
    case 3017:
        fputs("���±��",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("[",pH); break;
    case 3018:
        fputs("���±��",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("]",pH); break;
    case 3019:
        fputs("�ָ���",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(",",pH); break;
	case 3020:
        fputs("Ӣ�ľ��",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs(".",pH); break;
	case 3021:
        fputs("�ٷֺ�",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("%",pH); break;
	case 3022:
        fputs("&",pH);
        REPORT_TipAfter="');\" style=\"color:black;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("&",pH); break;
    case 5002:
        fputs("ע��",pH);
        REPORT_TipAfter="');\" style=\"color:green;\">";
        fputs(REPORT_TipAfter,pH);
        fputs("//",pH); fputs(sTemp,pH); break;
    }
    fputs(REPORT_WordAfter,pH);
}
