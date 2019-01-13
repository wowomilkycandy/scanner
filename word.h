/*----------------- 下段代码为可设置值 -----------------*/
/*                                                      */
int tipsLevel=1;            /* 提示级别，设定值：       */
                            /* 0 不输出任何提示         */
                            /* 1 输出获得基本单词       */
                            /* 2 输出获得注释           */
                            /* 3 输出分析过程           */
                            /* 4 输出准备就绪           */
int outHtmlReport=0;        /* 是否输出HTML报告         */
/*                                                      */
/*-------------------- 可设置值结束 --------------------*/

char c;                     //读取的字符
int cY,cX,cL,cW,hL=1;       //读取到的位置，单词开始位置，html行号
int back,wordNum,err,note;  //是否回退，单词数，错误数，注释行数
int iState,flag=0;            //状态标识，是否有未识别字符
char sTemp[255];            //标识符或注释临时接收区
int i;                  //sTemp长度
FILE *pF;
FILE *pH;

void word(const char *);
void key(char*);
void state(int);
void error(int);
void jumpError();
void addReport(int);


int isLetter(char c){       //判断是否是字母
    return c>='a'&&c<='z'||c>='A'&&c<='Z';
}

int isDigit(char c){        //判断是否是数字
    return c>='0'&&c<='9';
}

int isID(char c){           //判断是否是标识符字符
    return isLetter(c)||isDigit(c);
}
