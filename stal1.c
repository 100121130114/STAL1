#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "copalib.h"

int main(){
 //extractor de la configuracion
 FILE *fconf = fopen("conf.conf", "r");
 int conf[17];
 char conft[999];
 int numd[999];
 for(int a=0, b=0;fgets(conft, 999, fconf);){
  for(int c=0, d=0;c<999;c++){
   if((d==1)&&((conft[c]>='0')&&(conft[c]<='9'))){
    numd[b]=(int)(conft[c]-'0');
    b++;
   }
   if((conft[c]==';')&&(d==1)){d=0;break;}
   if((conft[c]==';')&&(d==0)){d=1;}
   if(conft[c]=='#'){b=0;break;}
  }
  if(b>0){
   if(a<17){
   conf[a]=ces(10,b,numd);
   a++;
   }else{
    break;
   }
  }
  b=0;
 }

 //variables de configuracion
 int clt=conf[0];//cantidad de lineas de trabajo
 int tlt=conf[1];//tamaño de las lineas de trabajo
 int lmte=conf[2];//longuitut maxima del texto de entrada
 int cv=conf[3];//cantidad de variables
 int cmla=conf[4];//cantidad maxima de lineas de comandos procedentes de un script
 //variables de almacenamiento
 int pt[2]={conf[5],conf[6]};//punto de trabajo
 int pv[2*3]={conf[7],conf[8],conf[9],conf[10],conf[11],conf[12]};//punto de vista
 int lt[2]={conf[13],conf[14]};//lineas de trabajo
 int lines[clt*tlt];
 int var[cv];
 bool mhel=0;//modo help activado o no, vista de las lineas de trabajo i variables
 //variables de procesamiento
 int ccea=38;//cantidad de comandos existentes ahora
 int tac=conf[15];//tamaño automatico para los comandos
 int lms=conf[16];//longuitut maxima para el nombre de el script a cargar
 bool ace=1;//alguna cosa ejecutada 
 char ent[lmte];//texto plano que se recibe
 bool ta=true;//true:modo terminal; false:modo archivo;
 int uvu=0;//ultima variable en usarse
 char arch[cmla][lmte];//arch actual
 int lau=-1;//linea de el archivo en uso
 int lrau=0;//lineas reales del archivo actual
 int combd[ccea][4];//comandos, base de datos [comando][0 y 1:cantidad de datos de entrada; 2 y 3:cantidad de datos de salida;]

 //preparador de variables
 for(int a=0;a<clt*tlt;a++){
  lines[a]=0;
 }
 for(int a=0;a<lmte;a++){
  ent[a]=0;
 }
 for(int a=0;a<cv;a++){
  var[a]=0;
 }
 for(int a=0;a<lmte;a++){
  for(int b=0;b<cmla;b++){
   arch[b][a]=0;
  }
 }
 combd[0][0]=2;combd[0][1]=0;combd[0][2]=0;combd[0][3]=1;// +
 combd[1][0]=2;combd[1][1]=0;combd[1][2]=0;combd[1][3]=1;// -
 combd[2][0]=2;combd[2][1]=0;combd[2][2]=0;combd[2][3]=1;// *
 combd[3][0]=2;combd[3][1]=0;combd[3][2]=0;combd[3][3]=1;// /
 combd[4][0]=2;combd[4][1]=0;combd[4][2]=0;combd[4][3]=1;// n=
 combd[5][0]=2;combd[5][1]=0;combd[5][2]=0;combd[5][3]=1;// b=
 combd[6][0]=2;combd[6][1]=0;combd[6][2]=0;combd[6][3]=1;// n<
 combd[7][0]=2;combd[7][1]=0;combd[7][2]=0;combd[7][3]=1;// n>
 combd[8][0]=2;combd[8][1]=0;combd[8][2]=0;combd[8][3]=1;// <=
 combd[9][0]=2;combd[9][1]=0;combd[9][2]=0;combd[9][3]=1;// >=
 combd[10][0]=2;combd[10][1]=0;combd[10][2]=0;combd[10][3]=1;// and
 combd[11][0]=2;combd[11][1]=0;combd[11][2]=0;combd[11][3]=1;// or
 combd[12][0]=2;combd[12][1]=0;combd[12][2]=0;combd[12][3]=1;// xor
 combd[13][0]=1;combd[13][1]=0;combd[13][2]=0;combd[13][3]=1;// not
 combd[14][0]=2;combd[14][1]=0;combd[14][2]=0;combd[14][3]=0;// var
 combd[15][0]=1;combd[15][1]=0;combd[15][2]=0;combd[15][3]=1;// 0>1
 combd[16][0]=0;combd[16][1]=1;combd[16][2]=1;combd[16][3]=0;// 1>0
 combd[17][0]=1;combd[17][1]=0;combd[17][2]=0;combd[17][3]=0;// rm0
 combd[18][0]=0;combd[18][1]=1;combd[18][2]=0;combd[18][3]=0;// rm1
 combd[19][0]=1;combd[19][1]=0;combd[19][2]=2;combd[19][3]=0;// cp0
 combd[20][0]=0;combd[20][1]=1;combd[20][2]=0;combd[20][3]=2;// cp1
 combd[21][0]=1;combd[21][1]=0;combd[21][2]=0;combd[21][3]=0;// if
 combd[22][0]=0;combd[22][1]=0;combd[22][2]=0;combd[22][3]=0;// cc
 combd[23][0]=2;combd[23][1]=0;combd[23][2]=0;combd[23][3]=1;// %
 combd[24][0]=0;combd[24][1]=0;combd[24][2]=0;combd[24][3]=0;// b<
 combd[25][0]=2;combd[25][1]=0;combd[25][2]=0;combd[25][3]=1;// b>
 combd[26][0]=1;combd[26][1]=0;combd[26][2]=0;combd[26][3]=0;// jc
 combd[27][0]=2;combd[27][1]=0;combd[27][2]=0;combd[27][3]=0;// jls
 combd[28][0]=0;combd[28][1]=0;combd[28][2]=0;combd[28][3]=0;// # (script)
 combd[29][0]=1;combd[29][1]=0;combd[29][2]=0;combd[29][3]=0;// pt0
 combd[30][0]=1;combd[30][1]=0;combd[30][2]=0;combd[30][3]=0;// pt1
 combd[31][0]=1;combd[31][1]=0;combd[31][2]=0;combd[31][3]=0;// lt0
 combd[32][0]=1;combd[32][1]=0;combd[32][2]=0;combd[32][3]=0;// lt1
 combd[33][0]=1;combd[33][1]=0;combd[33][2]=0;combd[33][3]=1;// in (inputs)
 combd[34][0]=2;combd[34][1]=0;combd[34][2]=0;combd[34][3]=0;// out (outputs)
 combd[35][0]=0;combd[35][1]=0;combd[35][2]=0;combd[35][3]=0;// hel
 combd[36][0]=3;combd[36][1]=0;combd[36][2]=0;combd[36][3]=1;// ?
 combd[36][0]=1;combd[36][1]=0;combd[36][2]=0;combd[36][3]=0;// &

 //definisions de interpretasio de les variables
 #define li(a,b) lines[a*tlt+b]//li[linea a usar][punto exacto de trabajo]

 //entrada de comandos(consola o archivo)
 printf("STAL Version 1.\n");//titulo
 FILE *flogo = fopen("logo.ascii", "r");
 char line[999];
 while(fgets(line, 999, flogo)){
  printf("%s",line);
 }
 printf("By CodeHat or 100121130114.\n\n\n");//titulo
 while(ace){
  ace=0;
  //zona de entradas
  for(int a=0;a<lmte;a++){
   ent[a]=0;
  }
  if(ta){
   printf("\t@ ");
   scanf("%s",ent);
  }
  if(!ta){
   if(((lau>=0)&&(lau<lrau))&&(lau<cmla)){
    for(int a=0;a<lmte;a++){
     ent[a]=arch[lau][a];
    }
    lau++;
   }else{
    ta=1;
    lau=-1;
    lrau=0;
    ace=1;
    for(int a=0;a<lmte;a++){
     for(int b=0;b<cmla;b++){
      arch[b][a]=0;
     }
    }
    goto cont;
   }
  }
  //comandos primarios
  if((ent[0]=='e')&&((ent[1]=='n')&&(ent[2]=='d'))){
   exit(0);
   ace=1;
   goto cont;
  }
  //zona de procesamiento de datos
  for(int a=0,td=-1,res=-1;a<lmte;){//a:punto en el texto real, el que se procesa ahora td:tipo de dato	res:dato
   if(false){
    error:
    printf("\n[");
    for(int b=0;b<lmte;b++){
     if((ent[b]>=32)&&(ent[b]<=126)){
      printf("%c",ent[b]);
     }else{
      printf(" ");
     }
    }
    printf("]\n[");
    for(int b=0;b<lmte;b++){
     if(b==a){
      printf("E");
     }else{
      printf(" ");
     }
    }
    printf("]\n");
    exit(0);
   }
   td=-1;
   res=-1;
   if(ent[a]==':'){break;}//escape en el final del texto
   if((ent[a]>' ')&&(ent[a]!='_')){
    
    if((ent[a]=='.')&&((td==-1)&&(res==-1))){td=0;a++;}
    if((ent[a]=='$')&&((td==-1)&&(res==-1))){td=1;a++;}
    if((ent[a]=='/')&&((td==-1)&&(res==-1))){td=2;a++;}
    if((ent[a]==',')&&((td==-1)&&(res==-1))){td=3;a++;}
    
    if(((td>=0)&&(td<=1))&&(res==-1)){//procesamiento de numeros
     int lon=0;//longuitut del numero
     //ver la lonquitut del numero
     for(int b=0;b<lmte-a;b++){
      if(!((ent[a+b]>='0')&&(ent[a+b]<='9'))){
       break;
      }else{
       lon++;
      }
     }
     int numd[lon];//numero a procesar
     //recoger los datos del numero
     for(int b=0;b<lon;b++){
      if((ent[b+a]>='0')&&(ent[b+a]<='9')){
       numd[b]=(int)(ent[b+a]-'0');
      }else{
       printf("ERROR: Caracteres incorectos en el numero.");
       goto error;
      }
     }
     //calculo del numero
     res=ces(10,lon,numd);
     a=a+lon; 
    }else{
     if(!((td>=0)&&(td<=3))){
      printf("ERROR: Tipo de dato incorecto.");
      goto error;
     }
    }
    
    //detecion de los comandos
    if((td==2)&&(res==-1)){
     res=-1;
     if((ent[a]=='a')&&((ent[a+1]=='n')&&((ent[a+2]=='d')&&(res==-1)))){a+=3;res=10;}
     if((ent[a]=='x')&&((ent[a+1]=='o')&&((ent[a+2]=='r')&&(res==-1)))){a+=3;res=12;}
     if((ent[a]=='n')&&((ent[a+1]=='o')&&((ent[a+2]=='t')&&(res==-1)))){a+=3;res=13;}
     if((ent[a]=='v')&&((ent[a+1]=='a')&&((ent[a+2]=='r')&&(res==-1)))){a+=3;res=14;}
     if((ent[a]=='0')&&((ent[a+1]=='>')&&((ent[a+2]=='1')&&(res==-1)))){a+=3;res=15;}
     if((ent[a]=='1')&&((ent[a+1]=='>')&&((ent[a+2]=='0')&&(res==-1)))){a+=3;res=16;}
     if((ent[a]=='r')&&((ent[a+1]=='m')&&((ent[a+2]=='0')&&(res==-1)))){a+=3;res=17;}
     if((ent[a]=='r')&&((ent[a+1]=='m')&&((ent[a+2]=='1')&&(res==-1)))){a+=3;res=18;}
     if((ent[a]=='c')&&((ent[a+1]=='p')&&((ent[a+2]=='0')&&(res==-1)))){a+=3;res=19;}
     if((ent[a]=='c')&&((ent[a+1]=='p')&&((ent[a+2]=='1')&&(res==-1)))){a+=3;res=20;}
     if((ent[a]=='j')&&((ent[a+1]=='l')&&((ent[a+2]=='s')&&(res==-1)))){a+=3;res=27;}
     if((ent[a]=='p')&&((ent[a+1]=='t')&&((ent[a+2]=='0')&&(res==-1)))){a+=3;res=29;}
     if((ent[a]=='p')&&((ent[a+1]=='t')&&((ent[a+2]=='1')&&(res==-1)))){a+=3;res=30;}
     if((ent[a]=='l')&&((ent[a+1]=='t')&&((ent[a+2]=='0')&&(res==-1)))){a+=3;res=31;}
     if((ent[a]=='l')&&((ent[a+1]=='t')&&((ent[a+2]=='1')&&(res==-1)))){a+=3;res=32;}
     if((ent[a]=='o')&&((ent[a+1]=='u')&&((ent[a+2]=='t')&&(res==-1)))){a+=3;res=34;}
     if((ent[a]=='h')&&((ent[a+1]=='e')&&((ent[a+2]=='l')&&(res==-1)))){a+=3;res=35;}
     if((ent[a]=='n')&&((ent[a+1]=='=')&&(res==-1))){a+=2;res=4;}
     if((ent[a]=='b')&&((ent[a+1]=='=')&&(res==-1))){a+=2;res=5;}
     if((ent[a]=='<')&&((ent[a+1]=='=')&&(res==-1))){a+=2;res=8;}
     if((ent[a]=='>')&&((ent[a+1]=='=')&&(res==-1))){a+=2;res=9;}
     if((ent[a]=='o')&&((ent[a+1]=='r')&&(res==-1))){a+=2;res=11;}
     if((ent[a]=='i')&&((ent[a+1]=='f')&&(res==-1))){a+=2;res=21;}
     if((ent[a]=='c')&&((ent[a+1]=='c')&&(res==-1))){a+=2;res=22;}
     if((ent[a]=='n')&&((ent[a+1]=='<')&&(res==-1))){a+=2;res=6;}
     if((ent[a]=='n')&&((ent[a+1]=='>')&&(res==-1))){a+=2;res=7;}
     if((ent[a]=='b')&&((ent[a+1]=='<')&&(res==-1))){a+=2;res=24;}
     if((ent[a]=='b')&&((ent[a+1]=='>')&&(res==-1))){a+=2;res=25;}
     if((ent[a]=='j')&&((ent[a+1]=='c')&&(res==-1))){a+=2;res=26;}
     if((ent[a]=='i')&&((ent[a+1]=='n')&&(res==-1))){a+=2;res=33;}
     if((ent[a]=='+')&&(res==-1)){a+=1;res=0;}
     if((ent[a]=='-')&&(res==-1)){a+=1;res=1;}
     if((ent[a]=='*')&&(res==-1)){a+=1;res=2;}
     if((ent[a]=='/')&&(res==-1)){a+=1;res=3;}
     if((ent[a]=='%')&&(res==-1)){a+=1;res=23;}
     if((ent[a]=='#')&&(res==-1)){a+=1;res=28;}
     if((ent[a]=='?')&&(res==-1)){a+=1;res=36;}
     if((ent[a]=='&')&&(res==-1)){a+=1;res=37;}
     td=2;
    }

    //detecion de los comandos
    if((td==3)&&(res==-1)){
     if(ent[a]==','){
      switch(ent[a+1]){
       case ',':{
	res=',';
	a+1;
       }break;
       case 'l':{//linea
	res='\n';
	a+1;
       }break;
       case 't':{//tabulador
	res='\t';
	a+1;
       }break;
       case 'S':{
	res=(int)ent[a+2];
	for(int b=a+3;b<lmte;b++){
	 if((ent[b]==',')&&(ent[b+1]=='S')){
	  a=b+2;
	  break;
	 }else{
          li(lt[0],pt[0])=res;
          pt[0]++;
          if(pt[0]>=tlt){pt[0]=0;}
          if(pt[0]<0){pt[0]=tlt-1;}
	  res=(int)ent[b];
	 }
	}
       }break;
      };
     }else{
      res=(int)ent[a];
      a+=1;
     }
    }
    
    //zona de ejecucion
    if((td==0)||(td==3)){//numero
     li(lt[0],pt[0])=res;
     pt[0]++;
     if(pt[0]>=tlt){pt[0]=0;}
     if(pt[0]<0){pt[0]=tlt-1;}
     ace=1;
    }
    if(td==1){//variable
     res=res%cv;
     li(lt[0],pt[0])=var[res];
     pt[0]++;
     if(pt[0]>=tlt){pt[0]=0;}
     if(pt[0]<0){pt[0]=tlt-1;}
     ace=1;
    }
    if(td==2){//comandos
     bool ife=0;//funcion de salida para el if
     int c[tac];//input de lt 0
     int d[tac];//input de lt 1
     int e[tac];//output de lt 0
     int f[tac];//output de lt 1
     //limpieza de las variables
     for(int g=0;g<tac;g++){
      c[g]=0;d[g]=0;e[g]=0;f[g]=0;
     }
     if((res>=0)&&(res<ccea)){//detecion de que el comando existe
      //buscado i recolecion de datos
      for(int b=0;b<combd[res][0];b++){
       pt[0]--;
       if(pt[0]>=tlt){pt[0]=0;}
       if(pt[0]<0){pt[0]=tlt-1;}
       c[b]=li(lt[0],pt[0]);
       li(lt[0],pt[0])=0;
      }
      for(int b=0;b<combd[res][1];b++){
       pt[1]--;
       if(pt[1]>=tlt){pt[1]=0;}
       if(pt[1]<0){pt[1]=tlt-1;}
       d[b]=li(lt[1],pt[1]);
       li(lt[1],pt[1])=0;
      }
     }
     switch(res){
      case 0:{//+
       f[0]=c[1]+c[0];
       ace=1;
      }break;
      case 1:{//-
       f[0]=c[1]-c[0];
       ace=1;
      }break;
      case 2:{//*
       f[0]=c[1]*c[0];
       ace=1;
      }break;
      case 3:{// /
       if(c[0]>0){
        f[0]=c[1]/c[0];
       }else{
        printf("0 detectado, no se permite en esta operacion.\n");
       }
       ace=1;
      }break;
      case 4:{//n=
       f[0]=(int)(c[1]==c[0]);
       ace=1;
      }break;
      case 5:{//b=
       f[0]=(int)((bool)(c[1]%2)==(bool)(c[0]%2));
       ace=1;
      }break;
      case 6:{//n<
       f[0]=(int)(c[1]<c[0]);
       ace=1;
      }break;
      case 7:{//n>
       f[0]=(int)(c[1]>c[0]);
       ace=1;
      }break;
      case 8:{//<=
       f[0]=(int)(c[1]<=c[0]);
       ace=1;
      }break;
      case 9:{//>=
       f[0]=(int)(c[1]>=c[0]);
       ace=1;
      }break;
      case 10:{//and
       f[0]=(int)((bool)(c[1]%2)&&(bool)(c[0]%2));
       ace=1;
      }break;
      case 11:{//or
       f[0]=(int)((bool)(c[1]%2)||(bool)(c[0]%2));
       ace=1;
      }break;
      case 12:{//xor
       f[0]=(int)((bool)(c[1]%2)!=(bool)(c[0]%2));
       ace=1;
      }break;
      case 13:{//not
       f[0]=(int)(!((bool)(c[0]%2)));
       ace=1;
      }break;
      case 14:{//var
       var[c[0]%cv]=c[1];
       uvu=c[0];
       ace=1;
      }break;
      case 15:{//0>1
       f[0]=c[0];
       ace=1;
      }break;
      case 16:{//1>0
       e[0]=d[0];
       ace=1;
      }break;
      case 17:{//rm0
       //solo se solicita 1 en c
       ace=1;
      }break;
      case 18:{//rm1
       //solo se solicita 1 en d
       ace=1;
      }break;
      case 19:{//cp0
       e[0]=c[0];
       e[1]=c[0];
       ace=1;
      }break;
      case 20:{//cp1
       f[0]=d[0];
       f[1]=d[0];
       ace=1;
      }break;
      case 21:{//if
       if(!((bool)(c[0]%2))){ife=1;}
       ace=1;
      }break;
      case 22:{//cc
       //comando que no hace nada
       ace=1;
      }break;
      case 23:{//%
       if(c[0]>0){
        f[0]=c[1]%c[0];
       }else{
        printf("0 detectado, no se permite en esta operacion.\n");
       }
       ace=1;
      }break;
      case 24:{//b<
       f[0]=(int)((bool)(c[1]%2)<(bool)(c[0]%2));
       ace=1;
      }break;
      case 25:{//b>
       f[0]=(int)((bool)(c[1]%2)>(bool)(c[0]%2));
       ace=1;
      }break;
      case 26:{//jc
       if((bool)(c[0]%2)){a=0;}
       ace=1;
      }break;
      case 27:{//jls
       if(!(ta)){
        if((bool)(c[0]%2)){lau=(lau+c[1])%lrau;}
       }
       ace=1;
      }break;
      case 28:{//# (script)
       char arch_name[lms];
       for(int b=0;b<lms;b++){
	if(ent[a+b]=='#'){
         arch_name[b]='.';
         arch_name[b+1]='s';
         arch_name[b+2]='t';
	 for(int c=b+3;c<lms;c++){
	  arch_name[c]=0;
	 }
	 a+=b;
	 break;
	}else{
         arch_name[b]=ent[a+b];
	}
       }
       FILE *fptr = fopen(arch_name, "r");
       for(int b=0;(b<cmla)&&(fgets(arch[b], lmte, fptr));b++){//cargar los datos i vista de los datos
        for(int c=0;c<lmte;c++){
         if(arch[b][c]=='\n'){arch[b][c]=0;}
         if(arch[b][c]<' '){arch[b][c]=0;}
        }
        lrau=b+1;
       }
       lau=0;
       ta=0;
       ace=1;
       ife=1;
      }break;
      case 29:{//pt0
       pt[0]=c[0]%tlt;
       ace=1;
      }break;
      case 30:{//pt1
       pt[1]=c[0]%tlt;
       ace=1;
      }break;
      case 31:{//lt0
       lt[0]=c[0]%clt;
       ace=1;
      }break;
      case 32:{//lt1
       lt[1]=c[0]%clt;
       ace=1;
      }break;
      case 33:{// in
       switch(c[0]){
        case 0:{// scan-int
	 int g;
	 scanf("%d",&g);
	 f[0]=g;
	}break;
        case 1:{// print-char
	 char g;
	 printf("%c",&g);
	 f[0]=(int)(g);
	}break;
       };
       ace=1;
      }break;
      case 34:{// out
       switch(c[0]){
        case 0:{// print-int
	 printf("%d",c[1]);
	}break;
        case 1:{// print-char
	 printf("%c",(char)c[1]);
	}break;
       };
       ace=1;
      }break;
      case 35:{//hel
       if(ta){
        mhel=1;
       }
       ace=1;
      }break;
      case 36:{//? (if de valores)
       if((bool)(c[2]%2)){f[0]=c[1];}else{f[0]=c[0];}
       ace=1;
      }break;
      case 37:{//& (reflejo en la misma posicion)
       c[0]=c[0]%tlt;
       int var1=li(lt[1],c[0]);
       li(lt[1],c[0])=li(lt[0],c[0]);
       li(lt[0],c[0])=var1;
       ace=1;
      }break;
      default:{
       printf("ERROR: Comando no identificado.");
       goto error;
      }break;
     };
     if((res>=0)&&(res<ccea)){//detecion de que el comando existe  
      //envio del resultado
      for(int b=0;b<combd[res][2];b++){
       li(lt[0],pt[0])=e[b];
       pt[0]++;
       if(pt[0]>=tlt){pt[0]=0;}
       if(pt[0]<0){pt[0]=tlt-1;}
      }
      for(int b=0;b<combd[res][3];b++){
       li(lt[1],pt[1])=f[b];
       pt[1]++;
       if(pt[1]>=tlt){pt[1]=0;}
       if(pt[1]<0){pt[1]=tlt-1;}
      }
     }
     if(ife){break;}
    }
   }else{
    a++;
   }
  }
  cont: 
  if((ta)&&(mhel)){
   printf("\n");
   for(int a=0;a<2;a++){
    printf("LTA%d[PA%d/PM%d->LA%d/LM%d] ",a,pt[a],tlt-1,lt[a],clt-1);
    for(int b=pv[a*2];b<pv[a*2+1];b++){
     if(b<tlt){
      if(b==pt[a]){
       printf(">");
      }
      printf("%d ",li(lt[a],b));
     }else{
      printf("C ");
     }
    }
    printf("\n");
   }
   printf("VTA%d[VA%d/VM%d] ",uvu,pt[4],cv-1);
   for(int b=pv[4];b<pv[5];b++){
    if(b<cv){
     if(b==uvu){
      printf(">");
     }
     printf("%d ",var[b]);
    }else{
     printf("C ");
    }
   }
   printf("\n");
  } 
 }
 if(!(ace)){
  printf("ERROR: No se a ejecutado nada.\n");
 }
 return 0;
}
