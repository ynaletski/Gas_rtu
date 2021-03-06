/*�� �८�ࠧ������, ���᫥��� ���⮢ �� � ����஫쭮� �㬬�*/
#include<mem.h>
#include<7188xa.h>
#include<X607.h>

/****** �ନ஢���� 㪠��⥫� ��娢� � ᥣ���� *******************/
unsigned char GetArcPoint (int *pointer,unsigned *segment)
{ /*�ਬ������: ���樠������, ������ � ��娢, ࠡ�� � MMI*/
  unsigned char page;page=ReadNVRAM(12);
  switch (page)
  {
    case 0:*segment=0xd000;*pointer=ReadNVRAM(13)*256+ReadNVRAM(14);
	   break;
    case 1:*segment=0xe000;*pointer=ReadNVRAM(15)*256+ReadNVRAM(16);
	   break;
    case 2:*segment=0xc000;*pointer=ReadNVRAM(17)*256+ReadNVRAM(18);
	   break;
  } return page;
}
/****** ������ ᮡ���,���ଠ,����⭮� ���樨  **************/
void WriteEvent (unsigned char event[], unsigned char type)
{ /*�ਬ������: ���樠������, ������ � ���䨣����, ࠡ�� � MMI*/
  unsigned char i;unsigned number;
  number=ReadNVRAM(6+type*2)*256+ReadNVRAM(7+type*2);
  if (number < 512)
  {X607_WriteFn(0x2000*type+number*16,16,event);number++;}
  if (number >= 512) {WriteNVRAM(6+type*2,0);WriteNVRAM(7+type*2,0);}
  else {i=number/256;WriteNVRAM(6+type*2,i);WriteNVRAM(7+type*2,number-i*256);}
}
/******* ������ ᮡ��� �� ���-������ �᭮���� *****************/
void WriteEventFlash (unsigned char event[])
{
  unsigned char i;for (i=0;i<16;i++) event[i]=0;
}
/************ �८�ࠧ������ 楫��� � 4 ���� *****************/
void ConvLongToBynare (unsigned long val,unsigned char * control)
{
  unsigned int musor;
  musor=val/65536;*(control)=musor/256;
  *(control +1)=musor - *(control)*256;musor=val - musor*65536;
  *(control +2)=musor/256;*(control +3)=musor - *(control +2)*256;
}
/*********** �८�ࠧ������ �������� 楫��� � 4 ���� **********/
void ConvLongToBynNew (unsigned long val,unsigned char *a,unsigned char *b,
		       unsigned char *c, unsigned char *d)
{
  unsigned int musor;
  musor=val/65536; *a=musor/256;
  *b=musor - *a *256; musor=val - musor*65536;
  *c=musor/256;*d=musor - *c *256;
}
/********  ���᫥��� 4-� ���� 横���᪮�� ����   ************/
void CalcCRC32 (unsigned char massive[],unsigned char cnt,
		unsigned long * code)
{
  const unsigned long polinom=0xedb88320;unsigned char i,j,flg;
  unsigned int musor;*code=0xffffffff;
  for (i=0;i<cnt;i++)
  {
   *code=*code ^ massive[i];
    for (j=0;j<8;j++)
    {
      if ((*code & 1)!= 0) flg=1; else flg=0;*code=*code >> 1;
      if (flg==1) *code= *code ^ polinom;
    }
  }
  *code=*code ^ 0xffffffff;/*musor=kode/65536;*(control)=musor/256;
  *(control +1)=musor - *(control)*256;musor=kode - musor*65536;
  *(control +2)=musor/256;*(control +3)=musor - *(control +2)*256;*/
}
/******* ���᫥��� 2-� ���� 横���᪮�� ���� ��� MVS *********/
void CalcCRC16 (unsigned char massive[],unsigned char cnt,
		unsigned char * control)
{
  unsigned char i,j,flg;const unsigned polinom=0xa001;unsigned res;
  res=0xffff;for (i=0;i<cnt;i++)
  {
    res=res ^ massive[i];
    for (j=0;j<8;j++)
    {
      if ((res & 1)!= 0) flg=1; else flg=0;res=res >> 1;
      if (flg==1) res= res ^ polinom;
    }
  } *(control)=res/256;*(control +1)=res - *(control)*256;
}
/****** �८�ࠧ������ 4-� ���� � ����⢨⥫쭮� �᫮ ********/
float ConvToFloat (unsigned char a,unsigned char b,unsigned char c,
		   unsigned char d)
{ /*��אַ� �।������ ��ࠬ��஢ �� ����� ��� ��� ��� ���訩 ����
    �� ���襬� �����*/
  float val;unsigned char buf_char[4];
  buf_char[0]=a;buf_char[1]=b;buf_char[2]=c;buf_char[3]=d;
  /*printf(" %d %d %d %d",buf_f[0],buf_f[1],buf_f[2],buf_f[3]);*/
  memcpy(&val,&buf_char,4);/*printf(" %f",val);*/
  return val;
}
unsigned char ConvToFloatVerify (float *val,unsigned char a,
		    unsigned char b,unsigned char c,unsigned char d)
{
  float value; unsigned char buf_char[4],flag;
  if ((d==255 || d==127) && c>127) {flag=1; goto M;}
  buf_char[0]=a; buf_char[1]=b; buf_char[2]=c; buf_char[3]=d;
  memcpy(&value, &buf_char, 4); *val=value; flag=0;
  M: return flag;
}
/******* �८�ࠧ������ ����⢨�. �᫠ � 4 ���� ****************/
void ConvToBynare (float val,unsigned char * bytes)
{ /*��אַ� �।������ ��ࠬ��஢ �� ����� ������ ��� ��� ���*/
  unsigned char buf_char[4];
  memcpy(&buf_char,&val,4);*(bytes)=buf_char[0];*(bytes+1)=buf_char[1];
  *(bytes+2)=buf_char[2];*(bytes+3)=buf_char[3];
}
/********  ���᫥��� ���� ᨬ����� ����஫쭮� �㬬�   ************/
void CalcCheckSum (unsigned char cmd[],unsigned char cnt_smb,
		   unsigned char *check)
{ /*�ᯮ������ ��� ࠡ��� � ����ﬨ ADAM,ICP*/
  /*���ᨢ ���⮢,�᫮ ���⮢,���塠�⮢� ���ᨢ �����.�㬬�*/
  unsigned char i,sum;sum=0;
  for (i=0;i<cnt_smb;i++) sum=sum+cmd[i];*(check)=sum >> 4;
  *(check +1)=sum & 0xf;
}
/********* ���᫥��� ����஫쭮� �㬬� Modbus **********************/
unsigned char CalcModbusSum (unsigned char buf[],unsigned char count)
{
  unsigned sum;unsigned char i;sum=0;
  for (i=0;i<count;i++) sum=sum+buf[i];return 256-(sum & 0xff);
}
/** ���� �८�ࠧ������ 4-� ���⮢ � ����⢨⥫쭮� �᫮ � �஢�મ�
 �� +-INF � +-NAN *********/
unsigned char SecurityConvert (int a,int b,int c,int d,
			       unsigned char * bytes)
{
  unsigned char i;
  if ((d==255 || d==128) && c>127) i=1;else
  {*(bytes)=a;*(bytes +1)=b;*(bytes +2)=c;*(bytes +3)=d;i=0;}
  return i;
}
/******** �८�ࠧ������ ⥪�� � �� 8-�� ����⢨⥫��� �ᥫ *****/
void TextToFloat (unsigned char ind_com,unsigned char inbuf[],
		  float param[])
{
  unsigned char i,j,k,l,flg,buf[10];i=1;j=k=flg=0;
  while (i<ind_com-3)
  {
    if (inbuf[i]==0x2b || inbuf[i]==0x2d)
    {
      if (flg == 0) flg=1;else
      {
	j=0;param[k]=atof(buf);k++;for (l=0;l<10;l++) buf[l]=0;
      } buf[j]=inbuf[i];
    } else buf[j]=inbuf[i];
    i++;j++;if (j > 10) break;
  } if (j <= 10) param[k]=atof(buf);
}
/*******************************************************/
void FloatToText (float val,unsigned char buf_str[],
		   unsigned char offset)
{ /*�ᯮ������ �� �뢮�� ���祭�� ����������� ��室�*/

  unsigned char i,j,dot,count;float a,b,c;unsigned long d;
  count=0;if (val<10) {buf_str[count+offset]=0x30;count=1;}
  if (val < 100)
  {
    if (val != 0)
    {
      b=c=10000;a=1;dot=0;i=0;
      while (val/a>=10) {a=a*10;dot++;b=b/10;} val=val*b;
      while (c>=1)
      {
	d=val/c;j=d;buf_str[count+offset]=hex_to_ascii[j & 0x0f];count++;
	if (dot==i) {buf_str[count+offset]=0x2e;count++;}
	val=val-d*c;c=c/10;i++;
      }
    } else {buf_str[offset]=0x30;buf_str[offset+1]=0x30;
    buf_str[offset+2]=0x2e;buf_str[offset+3]=0x30;buf_str[offset+4]=0x30;
    buf_str[offset+5]=0x30;}
  } /*�᫮ ����� 8 ������ ᨬ��� "#"*/
}
/*void TextToFloat (unsigned char ind_com,unsigned char inbuf[],
		  float param[])
{
  unsigned char i,j,flg,flg_dot,cnt[8],zn[8],dot[8],buf[8][10];
  for (i=0;i<8;i++) {cnt[i]=zn[i]=dot[i]=0;param[i]=0.0;}
  i=1;j=flg=flg_dot=0;
  while (i<ind_com-3)
  {
    if (inbuf[i]==0x2e) flg_dot=1;
    if ((inbuf[i]==0x2b)||(inbuf[i]==0x2d))
    {if (flg != 0) {j++;flg_dot=0;} flg=1;}
    if (inbuf[i]==0x2b) zn[j]=0;if (inbuf[i]==0x2d) zn[j]=1;
    if ((inbuf[i]>=0x30)&&(inbuf[i]<=0x39))
    {
      buf[j][cnt[j]]=ascii_to_hex(inbuf[i]);cnt[j]++;
      if (flg_dot != 0) dot[j]++;
    } i++;
  }
  for (i=0;i<8;i++) if (cnt[i]>0)
  { /*���᫥��� ���祭�� � ��室��� �������*/
/*    for (j=0;j<cnt[i];j++)
    param[i]=param[i]+buf[i][j]*pow10(cnt[i]-j-1);
    param[i]=param[i]/pow10(dot[i]);
    if (zn[i]==1) param[i]=-param[i];
  }
}
/********* �ନ஢���� 蠡���� ����� ᮡ���/���ଠ **************/
void  FormateEvent (unsigned char buf[])
{
  unsigned char i;
  for (i=0;i<6;i++) buf[i]=ReadNVRAM(i);for (i=0;i<10;i++) buf[i+6]=0;
}
/******************************************************************/
