/*----------------- �¶δ���Ϊ������ֵ -----------------*/
/*                                                      */
int tipsLevel=1;            /* ��ʾ�����趨ֵ��       */
                            /* 0 ������κ���ʾ         */
                            /* 1 �����û�������       */
                            /* 2 ������ע��           */
                            /* 3 �����������           */
                            /* 4 ���׼������           */
int outHtmlReport=0;        /* �Ƿ����HTML����         */
/*                                                      */
/*-------------------- ������ֵ���� --------------------*/

char c;                     //��ȡ���ַ�
int cY,cX,cL,cW,hL=1;       //��ȡ����λ�ã����ʿ�ʼλ�ã�html�к�
int back,wordNum,err,note;  //�Ƿ���ˣ�����������������ע������
int iState,flag=0;            //״̬��ʶ���Ƿ���δʶ���ַ�
char sTemp[255];            //��ʶ����ע����ʱ������
int i;                  //sTemp����
FILE *pF;
FILE *pH;

void word(const char *);
void key(char*);
void state(int);
void error(int);
void jumpError();
void addReport(int);


int isLetter(char c){       //�ж��Ƿ�����ĸ
    return c>='a'&&c<='z'||c>='A'&&c<='Z';
}

int isDigit(char c){        //�ж��Ƿ�������
    return c>='0'&&c<='9';
}

int isID(char c){           //�ж��Ƿ��Ǳ�ʶ���ַ�
    return isLetter(c)||isDigit(c);
}
