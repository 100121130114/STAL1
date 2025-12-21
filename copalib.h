//Libreria para COPA

int long lvr(int long numero_base, int long numero_elevar){//ElEvAr
					    //numero_base:numero que se multiplica sobre si mismo; numero_elevar:con el que se eleva; (numero_base^numero elevar)
 int long resultado=1;
 for(int long a=0;a<numero_elevar;a++){
  resultado*=numero_base;
 }
 return resultado;
}

int ces(int numb,int cnc,int* numd){//numb:numero_base(base 10,2,16 ...)	cnc:longuitut del texto		numd:letras(convertidas a numeros [letra-'0'=numero de la letra])
 int numc=0;
 for(int long a=cnc-1,b=0;a>=0;a--){
  if(numd[a]>=0&&(numd[a]<numb)){
   numc+=lvr(numb,cnc-1-a-b)*numd[a];
  }else{
   b+=1;
  }
 }
 return numc;
}
