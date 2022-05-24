#include <stdio.h>
#include <iostream>
#include <time.h>
#include <graphics.h>

typedef struct{
        int color;
}Malla;

typedef struct{
   Malla **malla;
}Imagen;

typedef struct bomba
{
        struct nodo *pos;
        struct bomba *ant,*sig;
        struct jug *j;
        int numEx;
        int num;
}Bomba;

typedef struct{
        Bomba *prim,*ult;
        Imagen IB[5];
}LBombas;

typedef struct enem
{
        struct nodo *pos;
        struct enem *ant,*sig;
        int num;
        int tipo;
        int dir;
        int mueve;
        int tiemp;
}Enemigo;

typedef struct{
        Enemigo *prim,*ult;
}LEnemigo;

typedef struct nodo{
        struct nodo *ant,*sig,*arri,*aba;
        struct nodo *SI;  
        struct nodo *SD;  
        struct nodo *II;  
        struct nodo *ID;  
        int X;
        int Y;
        struct jug *posJN;
        Bomba *BN;
        Enemigo *EN;
        int obs;
}Nodo;     

typedef struct{
        char nombre[10];
        int puntos;
}Record; 

typedef struct jug{
  Nodo *pos;
  Imagen imaJ[20];
  Record record;
  char nombre[6];
  int dir;
  int pasos;
  int bombas;
  int vidas;
  int imuerto;
  int numExp;
  int puntos;
  bool muerto;
  bool gana;
}Jugador;
 
void nivel(int nivel,int puntos,int vidas,char nombre[]);           
void imprimirRecords();
void insertarRecord(Record bomber);
void iniciaRecords();
void colores(int color);
void PintaIcono(Malla **m,int posx,int posy,int modo);
Malla **Abre(char nom[]);  
Nodo *CreaNodo(int i, int j);
Nodo *GeneraMalla();
void asdf(Jugador JUG,int x,char aux[]);

Malla **CreaMalla(int tamx,int tamy);
void mueveJuga(Jugador *JUG,int *tecla,LBombas *bombas,Imagen bomba[],int numE,int nivel);
void juego(Nodo *prin,Jugador JUG,LBombas *bombas,Imagen bomba[],LEnemigo LE,Imagen enemigos[],Imagen obs[]);
void creaJugador(Jugador *JUG,Nodo *pos);
void jugadorMuerto(Jugador *JUG,Nodo *prin);

Nodo *CargaEscenario(Nodo *Principal,int nivel,Imagen obst[],LEnemigo *LE,Imagen enemigos[]);
void dibuja(Nodo *principal,Imagen obs[],int nivel);
void cargaObstaculos(Imagen obs[]);
void EliminaEscenario(Nodo *Principal);
void imaBomba(Imagen bomba[]);
Bomba *iniciaBomba(Nodo *Pos);
void ponBomba(LBombas *bombas,Jugador *JUG,Imagen bomba[]);
void TiempoExplocion(LBombas *bombas,Imagen bomba[],LEnemigo *LE,Jugador *JUG,int nivel);
void Explocion(Bomba *b,Imagen bomba[],LEnemigo *LE,Jugador *JUG,int nivel);
void explota(int num,Nodo *pos,Imagen bomba[],int nivel);
void EliminaBomba(LBombas  *bom,int numero);
void eliminaLista(LEnemigo *le);

void imaEnem(Imagen enemigo[]);
Enemigo *CreaEnemigo(int Direccion,Nodo *Pos,int tipo);
void EliminaEnemigo(LEnemigo *LE,int numero,Jugador *JUG);
void MueveEnemigos(LEnemigo *lE,Imagen enemigo[],Imagen obs[],Nodo *p,int tiempo);
void MueveEnemigo(Enemigo *en,Imagen enemigo[],Imagen obs[],Nodo *p);
int cuentalistaE(LEnemigo le);
void portada();
int opcion(Jugador JUG);
void ayuda();
void menu(Nodo *p,Nodo *posJ,Jugador JUG,LEnemigo le,LBombas lb,Imagen obs[],Imagen bombas[],Imagen enemigos[]);
void ingresaNombre(int x,int y, char cadena[5]);

int gameover1(Jugador JUG);
Nodo *buscaPosicion(Nodo *Principal);

int main( )
{
	initwindow( 600 , 570 , "WinBGIm" );    
	Nodo *p;
	Nodo *posJ;
	p=GeneraMalla(); 
    srand( (unsigned)time( NULL ) );
    Jugador JUG;
    
    LBombas lb;
    LEnemigo le;
    lb.prim=NULL;
    lb.ult=NULL;
    
    le.prim=NULL;
    le.ult=NULL;
    Imagen obs[15];
    Imagen bombas[15];
    Imagen enemigos[10];
    imaBomba(bombas);
    imaEnem(enemigos);
    cargaObstaculos(obs);
    
    menu(p,posJ,JUG,le,lb,obs,bombas,enemigos);
  
	closegraph( );
	return( 0 );
}
void menu(Nodo *p,Nodo *posJ,Jugador JUG,LEnemigo le,LBombas lb,Imagen obs[],Imagen bombas[],Imagen enemigos[])
{
    int op;
    posJ=CargaEscenario(p,1,obs,&le,enemigos);
    creaJugador(&JUG,posJ);
    setfillstyle(SOLID_FILL,6);
    portada();
    do
    {
         op = opcion(JUG);
         cleardevice();
         switch(op)
         {
                      case 1:
                           eliminaLista(&le);
                           EliminaEscenario(p);
                           posJ=CargaEscenario(p,1,obs,&le,enemigos);
                           creaJugador(&JUG,posJ);
                           ingresaNombre(300,200,JUG.nombre);
                           nivel(1,0,3,JUG.nombre);
                           //setactivepage(1); 
                           dibuja(p,obs,1);
                           juego(p,JUG,&lb,bombas,le,enemigos,obs);
                      break;
                      case 2:imprimirRecords();
                      break;
                      case 3:ayuda();
                      break;
                      case 4:op=27;
                      break;
         }
    }while(op!=27);     
     
}


void juego(Nodo *prin,Jugador JUG,LBombas *bombas,Imagen bomba[],LEnemigo LE,Imagen enemigos[],Imagen obs[])
{
     int numE;
     int tecla;
     Record r;
     bool salir =false;
     int n=1;
     char cVidas[50];
     char cNombre[50];
     char cPuntos[50];
     char cTiempo[50];
     int tiempoInicial,tiempoActual,tiempoLimite=180;
     int tiempo;
     tiempoInicial=time(NULL);
     tiempoActual=time(NULL);
     
     settextstyle(1,0,1);
     outtextxy(365,10,JUG.nombre); 
     jugadorMuerto(&JUG,prin);
     itoa(JUG.vidas,cVidas,10); 
     outtextxy(450,10,cVidas);
     int pulso=0;
     do{
     
            tiempoActual=time(NULL);            
            tiempo=tiempoActual-tiempoInicial;
            if(pulso==tiempo)
            {
              printf("Mueve");
              pulso++;
            }
            itoa(tiempo,cTiempo,10); 
            outtextxy(280,10,cTiempo);
            if(tiempo==180)
            {
                salir=true;
                cleardevice();
                outtextxy(200,250,"Tiempo Terminado");             
            }
            asdf(JUG,170,cPuntos);
            numE=cuentalistaE(LE);
            MueveEnemigos(&LE,enemigos,obs,prin,tiempo);
            TiempoExplocion(bombas,bomba,&LE,&JUG,n);
            
            if(JUG.muerto==true)
            {
                jugadorMuerto(&JUG,prin);
                itoa(JUG.vidas,cVidas,10); 
                outtextxy(450,10,cVidas);
                if(JUG.vidas==0)
                salir = true;   
            }
            else
            mueveJuga(&JUG,&tecla,bombas,bomba,numE,n);  
            
            if(JUG.gana==true)
            {
                n+=1; 
                if(n==6)
                {
                    salir=true;
                    cleardevice(); 
                     outtextxy(200,250,"Gracias por Jugar");
                     outtextxy(210,300,"Nuevo Record"); 
                     outtextxy(230,350,JUG.nombre);
                }
                else
                {
                    eliminaLista(&LE);
                    EliminaEscenario(prin);
                    JUG.pos=CargaEscenario(prin,n,obs,&LE,enemigos);
                    nivel(n,JUG.puntos,JUG.vidas,JUG.nombre);
                     jugadorMuerto(&JUG,prin);
                     itoa(JUG.vidas,cVidas,10); 
                     outtextxy(450,10,cVidas);
                     dibuja(prin,obs,n); 
                     outtextxy(365,10,JUG.nombre); 
                     tiempoInicial=time(NULL);
                    JUG.gana=false;
                }              
            }
                         
     }while(salir==false);
     outtextxy(200,400,"Juego terminado");
     delay(10000);
     r.puntos=JUG.puntos;
     strcpy(r.nombre,JUG.nombre);
     insertarRecord(r);
     imprimirRecords();
}
void asdf(Jugador JUG,int x,char aux[])
{ 
        if(JUG.puntos>=10)
        x-=17;
        if(JUG.puntos>=100)
        x-=17;
        if(JUG.puntos>=1000)
        x-=17;
        if(JUG.puntos>=10000)
        x-=17;
        itoa(JUG.puntos,aux,10); 
        outtextxy(x,10,aux);            
}
void mueveJuga(Jugador *JUG,int *tecla,LBombas *bombas,Imagen bomba[],int numE,int nivel)
{       
      if(kbhit())
      {
        *tecla=getch();
        switch(*tecla)
        {
        case 77://derecha
              if(JUG->pos->sig!=NULL && JUG->dir==0 && JUG->pos->sig->BN==NULL )  
              {
                if(JUG->pos->sig->obs==0 || JUG->pos->sig->obs==70 || JUG->pos->sig->obs==71)
                {
                       setfillstyle(SOLID_FILL,6);
                       if(JUG->pos->obs==70)
                       PintaIcono(JUG->imaJ[19].malla,JUG->pos->X,JUG->pos->Y,0);
                       else
                       bar(JUG->pos->X,JUG->pos->Y,JUG->pos->X+40,JUG->pos->Y+40);
                       
                       JUG->pos->posJN=NULL;
                       JUG->pos=JUG->pos->sig;
                       JUG->pos->posJN=JUG; 
                       

                      if(JUG->pasos==0)PintaIcono(JUG->imaJ[4].malla,JUG->pos->X,JUG->pos->Y,0);
                      if(JUG->pasos==1)PintaIcono(JUG->imaJ[5].malla,JUG->pos->X,JUG->pos->Y,0);
                      delay(100);
                      JUG->pasos++;
                      if(JUG->pasos==2)
                      JUG->pasos=0;
                }
              }
              else
              {
                JUG->dir=0;    
              }
         break; 
         case 80://abajo
              if(JUG->pos->aba!=NULL && JUG->dir==1 && JUG->pos->aba->BN==NULL)  
              {
                   if(JUG->pos->aba->obs==0 || JUG->pos->aba->obs==70 || JUG->pos->aba->obs==71)
                   {
                       setfillstyle(SOLID_FILL,6);
                       if(JUG->pos->obs==70)
                       PintaIcono(JUG->imaJ[19].malla,JUG->pos->X,JUG->pos->Y,0);
                       else
                      bar(JUG->pos->X,JUG->pos->Y,JUG->pos->X+40,JUG->pos->Y+40);
                      JUG->pos->posJN=NULL;
                      JUG->pos=JUG->pos->aba;
                      JUG->pos->posJN=JUG; 

                      if(JUG->pasos==0)PintaIcono(JUG->imaJ[1].malla,JUG->pos->X,JUG->pos->Y,0);
                      if(JUG->pasos==1)PintaIcono(JUG->imaJ[2].malla,JUG->pos->X,JUG->pos->Y,0);
                      delay(100);
                      JUG->pasos++;
                      if(JUG->pasos==2)
                      JUG->pasos=0;
                      }
              }
              else
              {
                  JUG->dir=1;
              }  
         break;
         case 75:
              if(JUG->pos->ant!=NULL && JUG->dir==2 && JUG->pos->ant->BN==NULL)  
              {
                 if(JUG->pos->ant->obs==0 || JUG->pos->ant->obs==70 || JUG->pos->ant->obs==71)
                 {
                       setfillstyle(SOLID_FILL,6);
                       if(JUG->pos->obs==70)
                       PintaIcono(JUG->imaJ[19].malla,JUG->pos->X,JUG->pos->Y,0);
                       else
                       bar(JUG->pos->X,JUG->pos->Y,JUG->pos->X+40,JUG->pos->Y+40); 
                      JUG->pos->posJN=NULL;
                      JUG->pos=JUG->pos->ant;
                      JUG->pos->posJN=JUG; 

                      if(JUG->pasos==0)PintaIcono(JUG->imaJ[10].malla,JUG->pos->X,JUG->pos->Y,0);
                      if(JUG->pasos==1)PintaIcono(JUG->imaJ[11].malla,JUG->pos->X,JUG->pos->Y,0);
                      delay(100);
                      JUG->pasos++;
                      if(JUG->pasos==2)
                      JUG->pasos=0;
                 }
              }
              else
              {
                 JUG->dir=2;
              }
         break;  
         case 72://arriba
              if(JUG->pos->arri!=NULL && JUG->dir==3 && JUG->pos->arri->BN==NULL)  
              {
                 if(JUG->pos->arri->obs==0 || JUG->pos->arri->obs==70 || JUG->pos->arri->obs==71)
                 {
                       setfillstyle(SOLID_FILL,6);
                       if(JUG->pos->obs==70)
                       PintaIcono(JUG->imaJ[19].malla,JUG->pos->X,JUG->pos->Y,0);
                       else
                       bar(JUG->pos->X,JUG->pos->Y,JUG->pos->X+40,JUG->pos->Y+40);
                      JUG->pos->posJN=NULL;
                      JUG->pos=JUG->pos->arri;
                      JUG->pos->posJN=JUG; 
                      if(JUG->pasos==0)PintaIcono(JUG->imaJ[7].malla,JUG->pos->X,JUG->pos->Y,0);
                      if(JUG->pasos==1)PintaIcono(JUG->imaJ[8].malla,JUG->pos->X,JUG->pos->Y,0);
                      delay(100);
                      JUG->pasos++;
                      if(JUG->pasos==2)
                      JUG->pasos=0;
                 }
              }
              else
              {
                 JUG->dir=3;  
              }
         break;
         case 32:
              if(JUG->bombas!=0)
              {
                 JUG->bombas--;
                 ponBomba(bombas,JUG,bomba);
              }
         break;    
        }
      }
      else
      {
         if(JUG->dir==0){PintaIcono(JUG->imaJ[3].malla,JUG->pos->X,JUG->pos->Y,0);}
         if(JUG->dir==1){PintaIcono(JUG->imaJ[0].malla,JUG->pos->X,JUG->pos->Y,0);}
         if(JUG->dir==2){PintaIcono(JUG->imaJ[9].malla,JUG->pos->X,JUG->pos->Y,0);}
         if(JUG->dir==3){PintaIcono(JUG->imaJ[6].malla,JUG->pos->X,JUG->pos->Y,0);}
      }
      if(JUG->pos->EN!=NULL)
      {
       JUG->muerto=true; 
      }
      if(JUG->pos->obs==70 && numE==0)
       JUG->gana=true;
      if(JUG->pos->obs==71)
      {
          JUG->pos->obs=0;
          if(nivel==1)
          JUG->bombas+=1;
          if(nivel==2)
          JUG->numExp+=1;
          if(nivel==3)
          JUG->puntos+=100; 
          JUG->puntos+=1;              
      }
}
void MueveEnemigos(LEnemigo *lE,Imagen enemigo[],Imagen obs[],Nodo *p,int tiempo)
{     
   Enemigo *aux=lE->prim;
   
   while(aux)
   { 
             aux->mueve++;
             if(aux->tipo==0)
             {
                 if(aux->mueve==50)
                 {   
                      MueveEnemigo(aux,enemigo,obs,p);
                      PintaIcono(enemigo[aux->tipo+1].malla,aux->pos->X,aux->pos->Y,0); 
                 }
                 if(aux->mueve==100)
                 {
                    aux->mueve=0;  
                    MueveEnemigo(aux,enemigo,obs,p);
                    PintaIcono(enemigo[aux->tipo].malla,aux->pos->X,aux->pos->Y,0);
                 } 
             }  
             if(aux->tipo==2)
             {
                 if(aux->mueve==30)
                 {   
                      MueveEnemigo(aux,enemigo,obs,p);
                      PintaIcono(enemigo[aux->tipo+1].malla,aux->pos->X,aux->pos->Y,0); 
                 }
                 if(aux->mueve==60)
                 {
                    aux->mueve=0;  
                    MueveEnemigo(aux,enemigo,obs,p);
                    PintaIcono(enemigo[aux->tipo].malla,aux->pos->X,aux->pos->Y,0);
                 }              
             }         
             if(aux->tipo==4)
             {
                 if(aux->mueve==30)
                 {   
                      MueveEnemigo(aux,enemigo,obs,p);
                      PintaIcono(enemigo[aux->tipo+1].malla,aux->pos->X,aux->pos->Y,0); 
                 }
                 if(aux->mueve==60)
                 {
                    aux->mueve=0;  
                    MueveEnemigo(aux,enemigo,obs,p);
                    PintaIcono(enemigo[aux->tipo].malla,aux->pos->X,aux->pos->Y,0);
                 }              
             }    
             if(aux->tipo==6)
             {
                 if(aux->mueve==25)
                 {   
                      MueveEnemigo(aux,enemigo,obs,p);
                      PintaIcono(enemigo[aux->tipo].malla,aux->pos->X,aux->pos->Y,0); 
                 }
                 if(aux->mueve==50)
                 { 
                    MueveEnemigo(aux,enemigo,obs,p);
                    PintaIcono(enemigo[aux->tipo+1].malla,aux->pos->X,aux->pos->Y,0);
                 }    
                 if(aux->mueve==75)
                 {  
                    MueveEnemigo(aux,enemigo,obs,p);
                    PintaIcono(enemigo[aux->tipo].malla,aux->pos->X,aux->pos->Y,0);
                 } 
                 if(aux->mueve==100)
                 {
                    aux->mueve=0;  
                    MueveEnemigo(aux,enemigo,obs,p);
                    PintaIcono(enemigo[aux->tipo+2].malla,aux->pos->X,aux->pos->Y,0);
                 }            
             }           
      aux=aux->sig;       
   }
}

void MueveEnemigo(Enemigo *en,Imagen enemigo[],Imagen obs[],Nodo *p)
{
    if(en->pos->posJN!=NULL)
    {
       en->pos->posJN->muerto=true;
       en->pos->posJN=NULL;                                             
    } 
    switch(en->dir)
    {
     case 0:
          if(en->tipo==0 || en->tipo==4)
          if(en->pos->sig!=NULL && en->pos->sig->BN==NULL && en->pos->sig->EN==NULL && en->pos->sig->obs==0)  
          {
              setfillstyle(SOLID_FILL,6);
              bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
              en->pos->EN=NULL;
              en->pos=en->pos->sig;
              en->pos->EN=en;
          }
          else
          en->dir=1;
          if(en->tipo==2)
          if(en->pos->sig!=NULL && en->pos->sig->BN==NULL && en->pos->sig->EN==NULL && en->pos->sig->obs==0 || en->pos->sig->obs==12)  
          {
              setfillstyle(SOLID_FILL,6);
              if(en->pos->obs==0)
              bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
              else
              PintaIcono(obs[12].malla,en->pos->X,en->pos->Y,0);
              en->pos->EN=NULL;
              en->pos=en->pos->sig;
              en->pos->EN=en;
          }
          else
          en->dir=rand()%4;
          if(en->tipo==6)
          if(en->pos->sig!=NULL && en->pos->sig->BN==NULL && en->pos->sig->EN==NULL && en->pos->sig->obs==0 || en->pos->sig->obs==12)  
          {
              setfillstyle(SOLID_FILL,6);
              if(en->pos->obs==0)
              bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
              else
              PintaIcono(obs[12].malla,en->pos->X,en->pos->Y,0);
              en->pos->EN=NULL;
              en->pos=en->pos->sig;
              en->pos->EN=en;
          }
          else
          {
              setfillstyle(SOLID_FILL,6);
              if(en->pos->obs==0)
              bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
              else
              PintaIcono(obs[12].malla,en->pos->X,en->pos->Y,0);
              Nodo *pos;
              pos=buscaPosicion(p);
              if(pos!=NULL){en->pos->EN=NULL;en->pos=pos;en->pos->EN=en;}
              en->dir=rand()%4; 
          }
          break;
     case 1:
          if(en->tipo==0 || en->tipo==4)
          if(en->pos->aba!=NULL && en->pos->aba->BN==NULL && en->pos->aba->EN==NULL && en->pos->aba->obs==0 )  
          {
             setfillstyle(SOLID_FILL,6);
              bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
              en->pos->EN=NULL;
             en->pos=en->pos->aba;
             en->pos->EN=en;
          }
          else
          en->dir=2;
          if(en->tipo==2)
          if(en->pos->aba!=NULL && en->pos->aba->BN==NULL && en->pos->aba->EN==NULL && en->pos->aba->obs==0 || en->pos->aba->obs==12)  
          {
             setfillstyle(SOLID_FILL,6);
             if( en->pos->obs==0)
              bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
              else
              PintaIcono(obs[12].malla,en->pos->X,en->pos->Y,0);
              en->pos->EN=NULL;
             en->pos=en->pos->aba;
             en->pos->EN=en;
          }
          else
          en->dir=rand()%4;
          if(en->tipo==6)
          if(en->pos->aba!=NULL && en->pos->aba->BN==NULL && en->pos->aba->EN==NULL && en->pos->aba->obs==0 || en->pos->aba->obs==12)  
          {
             setfillstyle(SOLID_FILL,6);
             if( en->pos->obs==0)
              bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
              else
              PintaIcono(obs[12].malla,en->pos->X,en->pos->Y,0);
              en->pos->EN=NULL;
             en->pos=en->pos->aba;
             en->pos->EN=en;
          }
          else
          {
              setfillstyle(SOLID_FILL,6);
              if(en->pos->obs==0)
              bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
              else
              PintaIcono(obs[12].malla,en->pos->X,en->pos->Y,0);
              Nodo *pos;
              pos=buscaPosicion(p);
              if(pos!=NULL){en->pos->EN=NULL;en->pos=pos;en->pos->EN=en;}
              en->dir=rand()%4;     
          }
          break;
     case 2:
          if(en->tipo==0 || en->tipo==4)
          if(en->pos->ant!=NULL && en->pos->ant->BN==NULL && en->pos->ant->EN==NULL && en->pos->ant->obs==0)  
          {
             setfillstyle(SOLID_FILL,6);
             bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
             en->pos->EN=NULL;
             en->pos=en->pos->ant;
             en->pos->EN=en; 
          }
          else
          en->dir=3;
          if(en->tipo==2)
          if(en->pos->ant!=NULL && en->pos->ant->BN==NULL && en->pos->ant->EN==NULL && en->pos->ant->obs==0 || en->pos->ant->obs==12)  
          {
             setfillstyle(SOLID_FILL,6);
             if(en->pos->obs==0)
             bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
             else
              PintaIcono(obs[12].malla,en->pos->X,en->pos->Y,0);
             en->pos->EN=NULL;
             en->pos=en->pos->ant;
             en->pos->EN=en; 
          }
          else
          en->dir=rand()%4;
           if(en->tipo==6)
          if(en->pos->ant!=NULL && en->pos->ant->BN==NULL && en->pos->ant->EN==NULL && en->pos->ant->obs==0 || en->pos->ant->obs==12)  
          {
             setfillstyle(SOLID_FILL,6);
             if(en->pos->obs==0)
             bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
             else
              PintaIcono(obs[12].malla,en->pos->X,en->pos->Y,0);
             en->pos->EN=NULL;
             en->pos=en->pos->ant;
             en->pos->EN=en; 
          }
          else
         {
              setfillstyle(SOLID_FILL,6);
              if(en->pos->obs==0)
              bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
              else
              PintaIcono(obs[12].malla,en->pos->X,en->pos->Y,0);
              Nodo *pos;
              pos=buscaPosicion(p);
              if(pos!=NULL){en->pos->EN=NULL;en->pos=pos;en->pos->EN=en;}
              en->dir=rand()%4;        
         }
          break;
     case 3:
          if(en->tipo==0 || en->tipo==4)
          if(en->pos->arri!=NULL && en->pos->arri->BN==NULL &&  en->pos->arri->EN==NULL && en->pos->arri->obs==0)  
          {
             setfillstyle(SOLID_FILL,6);
             bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
             en->pos->EN=NULL;
             en->pos=en->pos->arri; 
             en->pos->EN=en;
          }
          else
          en->dir=0;
          if(en->tipo==2)
          if(en->pos->arri!=NULL && en->pos->arri->BN==NULL &&  en->pos->arri->EN==NULL && en->pos->arri->obs==0 || en->pos->arri->obs==12)  
          {
             setfillstyle(SOLID_FILL,6);
             if(en->pos->obs==0)
             bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
             else
             PintaIcono(obs[12].malla,en->pos->X,en->pos->Y,0);
             en->pos->EN=NULL;
             en->pos=en->pos->arri; 
             en->pos->EN=en;
          }
          else
          en->dir=rand()%4;
             if(en->tipo==6)
          if(en->pos->arri!=NULL && en->pos->arri->BN==NULL &&  en->pos->arri->EN==NULL && en->pos->arri->obs==0 || en->pos->arri->obs==12)  
          {
             setfillstyle(SOLID_FILL,6);
             if(en->pos->obs==0)
             bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
             else
             PintaIcono(obs[12].malla,en->pos->X,en->pos->Y,0);
             en->pos->EN=NULL;
             en->pos=en->pos->arri; 
             en->pos->EN=en;
          }
          else
          {
              setfillstyle(SOLID_FILL,6);
              if(en->pos->obs==0)
              bar(en->pos->X,en->pos->Y,en->pos->X+40,en->pos->Y+40);
              else
              PintaIcono(obs[12].malla,en->pos->X,en->pos->Y,0);
              Nodo *pos;
              pos=buscaPosicion(p);
              if(pos!=NULL){en->pos->EN=NULL;en->pos=pos;en->pos->EN=en;}
              en->dir=rand()%4;     
          }
          break;                             
    }
}


void InsertaEnemigo(LEnemigo *lE,Nodo *pos,int tipo,Imagen enemigos[])
{
      Enemigo *nuevoE;  
      Nodo *aux = pos;        
      nuevoE=CreaEnemigo(rand()%3,aux,tipo);
      if(nuevoE)
      {
                if(lE->prim==NULL)
                {
                      lE->prim=nuevoE;
                      lE->ult=nuevoE;
                      lE->prim->num+=lE->prim->num;
                }
                else
                {
                      nuevoE->sig=lE->prim;
                      lE->prim->ant=nuevoE;
                      lE->prim=nuevoE;
                      lE->prim->num+=lE->prim->sig->num+1;  
                }
      aux->EN=nuevoE;
      return;
      } 
}


void EliminaEnemigo(LEnemigo *LE,int numero,Jugador *JUG)
{ 
   Enemigo *aux2=LE->ult;  
   Enemigo *aux=LE->prim;
   JUG->puntos+=10;
   if(LE->prim==LE->ult)
   {
        aux->pos->EN=NULL;
        free(aux);
        LE->prim=LE->ult=NULL;
   }
   else 
   if(LE->ult->num==numero)
   {                                 
        LE->ult=LE->ult->ant;
        LE->ult->sig=NULL;
        aux2->pos->EN=NULL;
        free(aux2);         
   }
   else 
   if(LE->prim->num==numero)
   {       
        LE->prim=LE->prim->sig;
        LE->prim->ant=NULL;
        aux->pos->EN=NULL;
        free(aux);           
   }
   else
   {
          while(aux != NULL && aux->num != numero)
            {
                aux=aux->sig;
            }
            if(aux->num == numero)
            {  
                aux->sig->ant=aux->ant;
                aux->ant->sig=aux->sig;
                aux->sig=NULL;
                aux->ant=NULL;
                aux->pos->EN=NULL;
                free(aux);
            }
   }                                
}


void imaEnem(Imagen enemigo[])
{
    char nomArch[10];
     
    for(int i=0;i<6;i++)
    {  
            sprintf(nomArch,"en%d",i); 
            enemigo[i].malla=Abre(nomArch);   
    }
    enemigo[6].malla=Abre("f1"); 
    enemigo[7].malla=Abre("f2");   
    enemigo[8].malla=Abre("f3");       
}

void ponBomba(LBombas *bombas,Jugador *JUG,Imagen bomba[])
{
      Bomba *nuevoB;  
      Nodo *aux = JUG->pos;        
      nuevoB=iniciaBomba(aux);
      PintaIcono(bomba[1].malla,aux->X,aux->Y,0);
      nuevoB->j=JUG;
      if(nuevoB)
      {
                if(bombas->prim==NULL)
                {
                      bombas->prim=nuevoB;
                      bombas->ult=nuevoB;
                      bombas->prim->num+=bombas->prim->num;
                }
                else
                {
                      nuevoB->sig=bombas->prim;
                      bombas->prim->ant=nuevoB;
                      bombas->prim=nuevoB;
                      bombas->prim->num+=bombas->prim->sig->num+1;
                       
                }
      aux->BN=nuevoB;
      }
    
}
void TiempoExplocion(LBombas *bombas,Imagen bomba[],LEnemigo *LE,Jugador *JUG,int nivel)
{     
   Bomba *aux=bombas->prim;
   while(aux)
   {   
      aux->numEx+=1;
      if(aux->numEx>275)
      {
         Explocion(aux,bomba,LE,JUG,nivel);
      }
      else
      {
      if(aux->numEx==75 || aux->numEx==175 || aux->numEx==275)
      PintaIcono(bomba[1].malla,aux->pos->X,aux->pos->Y,0);
      if(aux->numEx==5|| aux->numEx==50 || aux->numEx==100 || aux->numEx==150 || aux->numEx==200 || aux->numEx==250)
      PintaIcono(bomba[2].malla,aux->pos->X,aux->pos->Y,0);
      if(aux->numEx==25 || aux->numEx==125 || aux->numEx==225)
      PintaIcono(bomba[3].malla,aux->pos->X,aux->pos->Y,0);
      }
      
      if(aux->numEx>366){EliminaBomba(bombas,aux->num);}

      aux=aux->sig;        
   }
}
void Explocion(Bomba *b,Imagen bomba[],LEnemigo *LE,Jugador *JUG,int nivel)
{
     explota(b->numEx,b->pos,bomba,nivel);
     if(b->pos->posJN!=NULL){b->pos->posJN->muerto=true;b->pos->posJN=NULL;}
     if(b->pos->EN!=NULL){EliminaEnemigo(LE,b->pos->EN->num,JUG);}
     
     if(JUG->numExp==1 || JUG->numExp==2)
     {
             if(b->pos->arri!=NULL && b->pos->arri->obs==0 || b->pos->arri->obs==12 || b->pos->arri->obs==13 || b->pos->arri->obs==14)
             {
                                   
                                   explota(b->numEx,b->pos->arri,bomba,nivel);
                                   if(b->pos->arri->EN!=NULL){EliminaEnemigo(LE,b->pos->arri->EN->num,JUG);}
                                   if(b->pos->arri->posJN!=NULL){b->pos->arri->posJN->muerto=true;b->pos->arri->posJN=NULL;} 
                                   if(b->pos->arri->obs==12)
                                   b->pos->arri->obs=0;
                                   else
                                   if(b->pos->arri->arri!=NULL && JUG->numExp==2 && b->pos->arri->arri->obs==0 || b->pos->arri->arri->obs==12 || b->pos->arri->arri->obs==13 || b->pos->arri->arri->obs==14) 
                                   {
                                       if(b->pos->arri->arri->obs==12)
                                       b->pos->arri->arri->obs=0;
                                       explota(b->numEx,b->pos->arri->arri,bomba,nivel);
                                       if(b->pos->arri->arri->EN!=NULL){EliminaEnemigo(LE,b->pos->arri->arri->EN->num,JUG);}
                                       if(b->pos->arri->arri->posJN!=NULL){b->pos->arri->arri->posJN->muerto=true;b->pos->arri->arri->posJN=NULL;}                         
                                   }  
             } 
             if(b->pos->aba!=NULL && b->pos->aba->obs==0 || b->pos->aba->obs==12 || b->pos->aba->obs==13 || b->pos->aba->obs==14)
             {
                                  
                                  explota(b->numEx,b->pos->aba,bomba,nivel);
                                  if(b->pos->aba->EN!=NULL){EliminaEnemigo(LE,b->pos->aba->EN->num,JUG);}
                                  if(b->pos->aba->posJN!=NULL){b->pos->aba->posJN->muerto=true;b->pos->aba->posJN=NULL;}
                                  if(b->pos->aba->obs==12)
                                  b->pos->aba->obs=0;
                                  else
                                  if(b->pos->aba->aba!=NULL && JUG->numExp==2 && b->pos->aba->aba->obs==0 || b->pos->aba->aba->obs==12 || b->pos->aba->aba->obs==13 || b->pos->aba->aba->obs==14) 
                                   {
                                       if(b->pos->aba->aba->obs==12)
                                       b->pos->aba->aba->obs=0;
                                       explota(b->numEx,b->pos->aba->aba,bomba,nivel);
                                       if(b->pos->aba->aba->EN!=NULL){EliminaEnemigo(LE,b->pos->aba->aba->EN->num,JUG);}
                                       if(b->pos->aba->aba->posJN!=NULL){b->pos->aba->aba->posJN->muerto=true;b->pos->aba->aba->posJN=NULL;}                         
                                   }
                                  
             }
             if(b->pos->ant!=NULL && b->pos->ant->obs==0 || b->pos->ant->obs==12 || b->pos->ant->obs==13 || b->pos->ant->obs==14)
             {
                                  
                                  explota(b->numEx,b->pos->ant,bomba,nivel);
                                  if(b->pos->ant->EN!=NULL){EliminaEnemigo(LE,b->pos->ant->EN->num,JUG);}
                                  if(b->pos->ant->posJN!=NULL){b->pos->ant->posJN->muerto=true;b->pos->ant->posJN=NULL;}
                                  if(b->pos->ant->obs==12)
                                  b->pos->ant->obs=0;
                                  else
                                  if(b->pos->ant->ant!=NULL && JUG->numExp==2 && b->pos->ant->ant->obs==0 || b->pos->ant->ant->obs==12 || b->pos->ant->ant->obs==13 || b->pos->ant->ant->obs==14) 
                                   {
                                       if(b->pos->ant->ant->obs==12)
                                       b->pos->ant->ant->obs=0;
                                       explota(b->numEx,b->pos->ant->ant,bomba,nivel);
                                       if(b->pos->ant->ant->EN!=NULL){EliminaEnemigo(LE,b->pos->ant->ant->EN->num,JUG);}
                                       if(b->pos->ant->ant->posJN!=NULL){b->pos->ant->ant->posJN->muerto=true;b->pos->ant->ant->posJN=NULL;}                         
                                   }
                                  
             }
             if(b->pos->sig!=NULL && b->pos->sig->obs==0 || b->pos->sig->obs==12 || b->pos->sig->obs==13 || b->pos->sig->obs==14)
             {
                                  
                                  explota(b->numEx,b->pos->sig,bomba,nivel);
                                  if(b->pos->sig->EN!=NULL){EliminaEnemigo(LE,b->pos->sig->EN->num,JUG);}
                                  if(b->pos->sig->posJN!=NULL){b->pos->sig->posJN->muerto=true;b->pos->sig->posJN=NULL;}
                                  if(b->pos->sig->obs==12)
                                  b->pos->sig->obs=0;
                                  else
                                  if(b->pos->sig->sig!=NULL && JUG->numExp==2 && b->pos->sig->sig->obs==0 || b->pos->sig->sig->obs==12 || b->pos->sig->sig->obs==13 || b->pos->sig->sig->obs==14) 
                                   {
                                       if(b->pos->sig->sig->obs==12)
                                       b->pos->sig->sig->obs=0;
                                       explota(b->numEx,b->pos->sig->sig,bomba,nivel);
                                       if(b->pos->sig->sig->EN!=NULL){EliminaEnemigo(LE,b->pos->sig->sig->EN->num,JUG);}
                                       if(b->pos->sig->sig->posJN!=NULL){b->pos->sig->sig->posJN->muerto=true;b->pos->sig->sig->posJN=NULL;}                         
                                   }
             }
     }
}
void explota(int num,Nodo *pos,Imagen bomba[],int nivel)
{
      if(num==275 || num==365)
      PintaIcono(bomba[4].malla,pos->X,pos->Y,0);
      if(num==290 || num==350)
      PintaIcono(bomba[5].malla,pos->X,pos->Y,0);
      if(num==305 || num==335)
      PintaIcono(bomba[6].malla,pos->X,pos->Y,0);
      if(num==320)
      PintaIcono(bomba[7].malla,pos->X,pos->Y,0);
      if(num==366)
      {             
             setfillstyle(SOLID_FILL,6);
             if(pos->obs==12 || pos->obs==0)
             bar(pos->X,pos->Y,pos->X+40,pos->Y+40); 
             if(pos->obs==13)   
             {
                 pos->obs=70;
                 PintaIcono(bomba[11].malla,pos->X,pos->Y,0); 
             } 
             if(pos->obs==14)
             { 
                 pos->obs=71; 
                 if(nivel==1)
                 PintaIcono(bomba[8].malla,pos->X,pos->Y,0);  
                 if(nivel==2)
                 PintaIcono(bomba[9].malla,pos->X,pos->Y,0); 
                 if(nivel==3)
                 PintaIcono(bomba[10].malla,pos->X,pos->Y,0);   
             }     
      }
}
void EliminaBomba(LBombas  *bom,int numero)
{ 
  Bomba *aux2=bom->ult;  
   Bomba *aux=bom->prim;
   
   if(bom->prim==bom->ult)
   {
        bom->prim->j->bombas++;
        bom->prim->pos->BN=NULL;
        free(bom->prim);
        bom->prim=bom->ult=NULL;
   }else 
   if(bom->ult->num==numero)
   {
        bom->ult->j->bombas++;
        bom->ult->pos->BN=NULL;                                 
        bom->ult=bom->ult->ant;
        bom->ult->sig=NULL;
        aux2->pos->BN=NULL;
        free(aux2);         
   }                              
}
Bomba *iniciaBomba(Nodo *Pos)
{
        Bomba *nueva;
        nueva=(Bomba*)malloc(sizeof(Bomba));
        if(nueva)
        {
        nueva->ant=NULL;
        nueva->sig=NULL;
        nueva->pos=Pos;
        nueva->numEx=1;
        nueva->num=0;
        return (nueva);
        }
        return NULL;
}

void imaBomba(Imagen bomba[])
{
    char nomArch[10];
     
    for(int i=1;i<4;i++)
    {  
            sprintf(nomArch,"bom%d",i); 
            bomba[i].malla=Abre(nomArch);   
    } 
    int j=1;
    for(int i=4;i<8;i++)
    {
          sprintf(nomArch,"ex%d",j); 
          bomba[i].malla=Abre(nomArch); 
          j++;  
    }  
    bomba[8].malla=Abre("regalo1"); 
    bomba[9].malla=Abre("regalo2"); 
    bomba[10].malla=Abre("regalo3"); 
    bomba[11].malla=Abre("puerta"); 
}

Enemigo *CreaEnemigo(int Direccion,Nodo *Pos,int tipo)
{
        Enemigo *nuevo;
        nuevo=(Enemigo*)malloc(sizeof(Enemigo));
        nuevo->ant=NULL;
        nuevo->sig=NULL;
        nuevo->pos=Pos;
        nuevo->dir=Direccion;
        nuevo->num=0;
        nuevo->tipo=tipo;
        nuevo->mueve=0;
        nuevo->tiemp=0;
        return (nuevo);
}

void creaJugador(Jugador *JUG,Nodo *pos)
{
   char nomArch[5];
    for(int i=1;i<13;i++)
    {  
            sprintf(nomArch,"bm%d",i);  
         JUG->imaJ[i-1].malla=Abre(nomArch);    
    }
    JUG->imaJ[13].malla=Abre("muerto1"); 
    JUG->imaJ[14].malla=Abre("muerto2");   
    JUG->imaJ[15].malla=Abre("muerto3"); 
    JUG->imaJ[16].malla=Abre("muerto4"); 
    JUG->imaJ[17].malla=Abre("muerto5"); 
    JUG->imaJ[18].malla=Abre("muerto6"); 
    JUG->imaJ[19].malla=Abre("puerta");  
    JUG->imaJ[20].malla=Abre("menu");     
    JUG->pos=pos;
    JUG->dir=1;
    JUG->pasos=0;
    JUG->bombas=1;
    JUG->vidas=3;
    JUG->muerto=false;
    JUG->gana=false;
    JUG->imuerto=0;
    JUG->numExp=1;
    JUG->puntos=0;
    pos->posJN=JUG;
}

int gameover1(Jugador JUG)
{
    int tecla;
    int num=1;
    int y=200;
    
    do
    { 
       settextstyle(10,0,5);
       setfillstyle(SOLID_FILL,COLOR(0,162,232)); 
        bar(0,0,600,570);
        outtextxy(230,200,"Continuar");
        rectangle(229,200,250+212,200+40);
        outtextxy(230,250,"Salir");
        rectangle(229,250,250+88,250+40);
        PintaIcono(JUG.imaJ[5].malla,180,y,0);
         tecla=getch();
         switch(tecla)
         {
                      case 72:if(y!=200){y-=50;num-=1;}
                      break;
                      case 80:if(y!=250){y+=50;num+=1;}
                      break;
                      case 13:return num;
                      break;
         }
    }while(tecla!=27);     
}

int opcion(Jugador JUG)
{
    int tecla;
    int num=1;
    int y=100;
    
    do
    { 
       settextstyle(10,0,5);
       setfillstyle(SOLID_FILL,COLOR(0,162,232)); 
        bar(0,0,600,570);
        outtextxy(230,100,"Jugar");
        rectangle(229,100,250+115,100+40);
        outtextxy(230,150,"Records");
        rectangle(229,150,250+175,150+40);
        outtextxy(230,200,"Ayuda");
        rectangle(229,200,250+130,200+40);
        outtextxy(230,250,"Salir");
        rectangle(229,250,250+88,250+40);
        PintaIcono(JUG.imaJ[20].malla,180,y,0);
         tecla=getch();
         switch(tecla)
         {
                      case 72:if(y!=100){y-=50;num-=1;}
                      break;
                      case 80:if(y!=250){y+=50;num+=1;}
                      break;
                      case 13:return num;
                      break;
         }
    }while(tecla!=27);     
}
void portada()
{
     settextstyle(0,0,2);    
    readimagefile("pag1.jpg",0,0, 600 , 570 );
    outtextxy(30,300,"Univerdidad Autonoma de San Luis Potosi");
    outtextxy(140,320,"Facultad de Ingenieria");
    outtextxy(120,340,"Computacion e Informatica");
    outtextxy(100,380,"Estructura de Base de datos B");
    outtextxy(30,400,"Alumno:");
    outtextxy(30,420,"Profesor:");
    outtextxy(230,440,"00/00/00");
    outtextxy(20,540,"Preciona una tecla.....");
    
    getch(); 
}
void jugadorMuerto(Jugador *JUG,Nodo *prin)
{
    if(JUG->imuerto==0) 
    PintaIcono(JUG->imaJ[13].malla,JUG->pos->X,JUG->pos->Y,0);
    if(JUG->imuerto==50) 
    PintaIcono(JUG->imaJ[14].malla,JUG->pos->X,JUG->pos->Y,0);
    if(JUG->imuerto==100) 
    PintaIcono(JUG->imaJ[15].malla,JUG->pos->X,JUG->pos->Y,0);
    if(JUG->imuerto==150) 
    PintaIcono(JUG->imaJ[16].malla,JUG->pos->X,JUG->pos->Y,0);
    if(JUG->imuerto==200) 
    PintaIcono(JUG->imaJ[17].malla,JUG->pos->X,JUG->pos->Y,0);
    if(JUG->imuerto==250) 
    PintaIcono(JUG->imaJ[18].malla,JUG->pos->X,JUG->pos->Y,0);
    JUG->imuerto++;
    if(JUG->imuerto==300)
    {
        setfillstyle(SOLID_FILL,6);
        bar(JUG->pos->X,JUG->pos->Y,JUG->pos->X+40,JUG->pos->Y+40);
        JUG->imuerto=0;
        JUG->muerto=false;
        JUG->vidas-=1;
        JUG->pos=prin->sig->aba;
        
        
    }     
}

void cargaObstaculos(Imagen obs[])
{
     char nomArch[10];
     
     for(int i=1;i<11;i++)
    {  
            sprintf(nomArch,"pared%d",i); 
            obs[i].malla=Abre(nomArch);   
    }
    obs[11].malla=Abre("caja");
    obs[12].malla=Abre("piedra");    
}
void PintaIcono(Malla **m,int posx,int posy,int modo)
{ 
     int pix=2;
     
     if(modo==0)
     {
         for(int i=0;i<20;i++)
         {  
             for(int j=0;j<20;j++)
             {  
                     colores((*(m+i)+j)->color);
                     bar(posx+pix*i,posy+pix*j,posx+pix+pix*i,posy+pix+pix*j);             
             }
         } 
     return;
     }
     else
     {
        for(int i=0;i<20;i++)
        {  
         for(int j=0;j<20;j++)
         {  
                 colores((*(m+j)+i)->color);
                 bar(posx+pix*i,posy+pix*j,posx+pix+pix*i,posy+pix+pix*j);             
         }
        }  
     }
}
Malla **Abre(char nom[])
{
      FILE *arch;
      Malla **arg=NULL;
      int tamx,tamy;
      
      arch=fopen(nom,"rb");     
      
      if(arch!=NULL)
      {
          fread(&tamx,sizeof(int),1,arch);
          fread(&tamy,sizeof(int),1,arch);
          arg=CreaMalla(tamx,tamy);
      
          for(int i=0;i<tamx;i++)
         {
               for(int j=0;j<tamy;j++)
               {
                      fread( &(*(arg+i)+j)->color , sizeof(int), 1, arch);                                       
               }
         }
         fclose(arch);
     }
     else
     {
         setcolor(WHITE);
    	 outtextxy(455,15, "Error en el archivo");
     }
     return arg;
}

Malla **CreaMalla(int tamx,int tamy)
{
      Malla **a;
      
      a=(Malla **)malloc(sizeof(Malla *) * tamx);
      
      for(int i=0;i<tamx;i++)
      {
                      *(a+i)=(Malla *)malloc(sizeof(Malla) * tamy);               
      }

      for(int i=0;i<tamx;i++)
      {
                      for(int j=0;j<tamy;j++)
                      {
                                     
                                       (*(a+i)+j)->color=0;
                      }
      }
      
       return a;
}

void colores(int color)
{
     if(color<15)
     {
         setfillstyle(SOLID_FILL,color);        
     }
     else
     {
        if(color==15) 
        setfillstyle(SOLID_FILL,COLOR(181,230,29));
        if(color==16) 
        setfillstyle(SOLID_FILL,COLOR(0,185,0));
        if(color==17)
         setfillstyle(SOLID_FILL,COLOR(0,162,232));
        if(color==18)
        setfillstyle(SOLID_FILL,COLOR(94,27,184));
        if(color==19)
        setfillstyle(SOLID_FILL,COLOR(27,60,13));
        if(color==20)
        setfillstyle(SOLID_FILL,COLOR(242,123,83));
        if(color==21)
        setfillstyle(SOLID_FILL,COLOR(19,164,160));
        if(color==22)
        setfillstyle(SOLID_FILL,COLOR(170,55,13));
        if(color==23)
        setfillstyle(SOLID_FILL,COLOR(255,147,0));
     }
}

Nodo *CreaNodo(int i, int j)
{
      Nodo *nuevo;
      nuevo=(Nodo *)malloc(sizeof(Nodo));
      if(nuevo){
      nuevo->X=(j*40);
      nuevo->Y=50+(i*40);
      nuevo->ant=NULL;
      nuevo->sig=NULL;
      nuevo->arri=NULL;
      nuevo->aba=NULL;
      nuevo->obs=0;
      nuevo->SI=NULL;
      nuevo->SD=NULL;
      nuevo->II=NULL;
      nuevo->ID=NULL;
      nuevo->posJN=NULL;
      nuevo->BN=NULL;
      nuevo->EN=NULL;
      }
      return (nuevo);
}
Nodo *GeneraMalla()
{
      Nodo *nuevo; 
      Nodo *p;
      Nodo *auxArr=NULL;   
      Nodo *auxant=NULL;
      Nodo *II=NULL;     
      Nodo *ID=NULL;
      Nodo *auxd=NULL;
      Nodo *iniciofila;
      Nodo *iniciodiag=NULL;
      
      for(int i=0; i<13; i++)    
      {
       for(int j=0; j<15; j++)  
       {  
              nuevo=CreaNodo(i,j);             
              if(nuevo)
              {     
                 if(i==0 && j==0)
                 {
                     p=nuevo; 
                     auxd=nuevo;    
                 } 
                 if(j==0)
                 {
                     iniciofila=nuevo; 
                     iniciodiag=nuevo;
                     nuevo->SI=nuevo->II=NULL;   
                     nuevo->ant=auxant;
                     nuevo->SD=II;
                 }              
                 if(j==1 && i>0)
                 {
                     ID=auxd;  
                     auxd=ID->aba; 
                 }
                 nuevo->arri=auxArr;
                 if(j>0 && i>0)
                 {
                     nuevo->SD=II; 
                     nuevo->SI=ID; 
                 }
                 if(ID && j!=0)   
                 {
                     ID->ID=nuevo;
                     ID=ID->sig;
                 }
                 if(II)   
                 {
                     II->II=nuevo;
                     II=II->sig;            
                 }
                 if(auxant)
                 {    
                     auxant->sig=nuevo;
                     nuevo->ant=auxant;
                 }               
                 if(auxArr) 
                 {
                     auxArr->aba=nuevo;
                     auxArr=auxArr->sig;
                 }
                 auxant=nuevo;
             } 
           }         
           auxant=NULL;
           auxArr=iniciofila;
           II=iniciodiag->sig;             
       }
       return (p);
}
Nodo *CargaEscenario(Nodo *Principal,int nivel,Imagen obst[],LEnemigo *LE,Imagen enemigos[])
{
    Nodo *corre = Principal;
    Nodo *gia =Principal;
    Nodo *pos;
    int dato=0;
    int regalo=0;
    int Puerta=0;
    char cad[10];
    int num=0;
    FILE *archivo;
    
    
    sprintf(cad,"N%d.txt",nivel);
    archivo=fopen(cad,"r");
    rewind(archivo);
    while(gia!=NULL)
    {
        while(corre!=NULL)
        {
           fscanf(archivo,"%d",&dato);
           if(dato==88)  
            pos=corre;
           else
           if(dato==1)
              corre->obs=1;  
           else
           if(dato==2)
                corre->obs=2;
           else
           if(dato==3)
                corre->obs=3;        
           else
           if(dato==4)
                corre->obs=4; 
           else
           if(dato==5)
                corre->obs=5;
            else
           if(dato==6)
                corre->obs=6; 
           else
           if(dato==55)
                corre->obs=55;
            else
           if(dato==66)
                corre->obs=66; 
           else
           if(dato==7)
                corre->obs=7;
            else
           if(dato==8)
                corre->obs=8;
            else
           if(dato==9)
                corre->obs=9;
           else
           if(dato==10)
                corre->obs=10; 
           else  
           if(dato==11)
                corre->obs=11; 
           else
           if(dato==12)
           {
               InsertaEnemigo(LE,corre,0,enemigos);        
           }
           else
            if(dato==13)
           {
               InsertaEnemigo(LE,corre,2,enemigos);        
           }
           else
           if(dato==14)
           {
               InsertaEnemigo(LE,corre,4,enemigos);        
           }
           else
           if(dato==15)
           {
               InsertaEnemigo(LE,corre,6,enemigos);        
           }
           else
           if(rand()%7==0)
                 corre->obs=12;
           else
           if(rand()%15==0 && Puerta==0 && num>16)
           {
              Puerta=1; 
              corre->obs=13;              
           }
           else
           if(rand()%15==0 && regalo==0 && num>20)
           {
              regalo=1; 
              corre->obs=14;              
           }
           else
           if(dato==13)
           {
               InsertaEnemigo(LE,corre,2,enemigos);        
           }           
         corre = corre->sig;  
         num+=1;                                        
        }   
                               
        gia = gia->aba;
        corre = gia;              
    }   
    return pos;   
}
void EliminaEscenario(Nodo *Principal)
{
    Nodo *corre = Principal;
    Nodo *gia =Principal;
    while(gia!=NULL)
    {
        while(corre!=NULL)
        {
            corre->obs=0; 
            corre->EN=NULL;
            corre->posJN=NULL;
            corre = corre->sig;                               
        }                                 
        gia = gia->aba;
        corre = gia;              
    }     
}

Nodo *buscaPosicion(Nodo *Principal)
{
    Nodo *corre = Principal;
    Nodo *gia =Principal;
    int ra=0;
    int in=rand()%130;
    while(gia!=NULL)
    {
        while(corre!=NULL)
        {
            if(corre->obs==0 && ra>in)
            return corre;
            ra++;
            corre = corre->sig;                               
        }                                 
        gia = gia->aba;
        corre = gia;              
    } 
    return NULL;    
}

void dibuja(Nodo *principal,Imagen obs[],int nivel)
{
    Nodo *corre = principal;
    Nodo *gia =principal;
    setfillstyle(SOLID_FILL,6);
    bar(0,0,600,570);
    setfillstyle(SOLID_FILL,0);
    bar(10,10,590,40);
    outtextxy(12,10,"SC  00000000");
    while(gia!=NULL)
    {
        while(corre!=NULL)
        {
         if(corre->obs==1)
             PintaIcono(obs[1].malla,corre->X,corre->Y,0);     
         if(corre->obs==2)                                                               
             PintaIcono(obs[2].malla,corre->X,corre->Y,0);           
         if(corre->obs==3)                                                                 
             PintaIcono(obs[3].malla,corre->X,corre->Y,0);              
         if(corre->obs==4)                                                                 
            PintaIcono(obs[4].malla,corre->X,corre->Y,0);  
         if(corre->obs==5)                                                                 
            PintaIcono(obs[5].malla,corre->X,corre->Y,0);
         if(corre->obs==6)                                                                 
            PintaIcono(obs[5].malla,corre->X,corre->Y,0); 
         if(corre->obs==55)                                                                 
            PintaIcono(obs[5].malla,corre->X,corre->Y,1);
         if(corre->obs==66)                                                                 
            PintaIcono(obs[6].malla,corre->X,corre->Y,1); 
         if(corre->obs==7)                                                                 
            PintaIcono(obs[8].malla,corre->X,corre->Y,0);
         if(corre->obs==8)                                                                 
            PintaIcono(obs[7].malla,corre->X,corre->Y,0);                     
         if(corre->obs==9)                                                                 
            PintaIcono(obs[9].malla,corre->X,corre->Y,0);
         if(corre->obs==10)                                                                 
            PintaIcono(obs[10].malla,corre->X,corre->Y,0); 
         if(corre->obs==11)                                                                 
            PintaIcono(obs[11].malla,corre->X,corre->Y,0);
         if(corre->obs==12)                                                                 
            PintaIcono(obs[12].malla,corre->X,corre->Y,0);  
         if(corre->obs==13)                                                                 
            PintaIcono(obs[12].malla,corre->X,corre->Y,0);  
         if(corre->obs==14)                                                                 
            PintaIcono(obs[12].malla,corre->X,corre->Y,0);   
         corre = corre->sig;                                          
        }                                   
        gia = gia->aba;
        corre = gia;              
    }      
}

void eliminaLista(LEnemigo *le)
{
    int i=0;
    Enemigo *aux=le->prim;
    Enemigo *elimina=le->prim;
    while(aux!=NULL)
    {
        free(elimina);
        aux=aux->sig;
        elimina=aux;
    }
    le->prim=NULL;
    le->ult=NULL;
}

int cuentalistaE(LEnemigo le)
{
    int i=0;
    Enemigo *aux=le.prim;
    while(aux!=NULL)
    {
        i+=1;
        aux=aux->sig;
    }
    return i;
}

void ayuda()
{
    int iTecla;
    FILE *f;
    cleardevice();
    rectangle(0,0,600,570); 
     rectangle(10,10,600-10,570-10);
    char c[255];
    int x=50,y=50;
    settextstyle(4,0,3);
    f=fopen("Ayuda.txt","r");
    while(!feof(f))
    {
	 fgets(c,255,f);
	 c[strlen(c)-1]='\0';
	 outtextxy(x,y,c);
	 y+=textheight("H");
    }
    fclose(f);
    getch();
}

void imprimirRecords()
{
    FILE *pt;
    Record tanque[5];
    char cpuntos[20];
    int i;
    char op;
    cleardevice();
    rectangle(0,0,600,570); 
     rectangle(10,10,600-10,570-10);
    settextstyle(10,0,3);
    pt=fopen("Record","rb");
    do{
      if(pt==NULL) 
      {
      iniciaRecords();
      }
      else
      {
       fread(tanque,sizeof(Record),5,pt);
        for(i=0;i<5;i++)
         {
         itoa(tanque[i].puntos,cpuntos,10);
         outtextxy(50,100+(i+1)*30,tanque[i].nombre);
         outtextxy(500,100+(i+1)*30,cpuntos);
         }
       }
        op=getch();
    }while(op!=13);
    fclose(pt);
}
void iniciaRecords()
{
      FILE *pt;
      Record p[5];
       pt=fopen("Record","wb");
       for(int i=0;i<5;i++)
       {
       strcpy(p[i].nombre,"- - - - - - - - - -");
       p[i].puntos=0;
       }
       fwrite(p,sizeof(Record),5,pt);          
}
void insertarRecord(Record bomber)
{
     FILE *pt;
     Record p[5],aux[5];
     
     pt=fopen("Record","rb+");
     if(pt==NULL)
     {
       iniciaRecords();
     }
     else
     {
         fread(p,sizeof(Record),5,pt);
         fseek(pt,0,SEEK_SET);
         if(bomber.puntos > p[0].puntos)
         {
         strcpy(aux[0].nombre,bomber.nombre);
         aux[0].puntos=bomber.puntos;
           for(int i=0;i<4;i++)
           {
           strcpy(aux[i+1].nombre,p[i].nombre);
           aux[i+1].puntos=p[i].puntos;
           }
         }
         else
         {
             int i=0;
             do
             {
                 strcpy(aux[i].nombre,p[i].nombre);
                 aux[i].puntos=p[i].puntos;
                 i++;
                 if(i==5)
                 break;
             }while(p[i].puntos > bomber.puntos);
         if(i!=5)
         {
         strcpy(aux[i].nombre,bomber.nombre);
         aux[i].puntos=bomber.puntos;
         
           for(;i<4;i++)
           {
                        strcpy(aux[i+1].nombre,p[i].nombre);
                        aux[i+1].puntos=p[i].puntos;
           }
          }
         }
         fwrite(aux,sizeof(Record),5,pt);
     }
     fclose(pt);
}

void ingresaNombre(int x,int y, char cadena[5])
{
    cleardevice();
    outtextxy(100,200,"Nombre");
    char tecla;     
    int contador=0;
    cadena[0]='\0';
    do{
          tecla=getch();
          if(tecla==0)  
             tecla=getch();
            if(tecla==8 && contador > 0)  
    			{
    				 setcolor(BLACK);
    				 outtextxy(x,y,cadena); 
    				 cadena[--contador]='\0'; 
    				 setcolor(WHITE);
    				 outtextxy(x,y,cadena);
    			}
            else
    			{  
                  if(tecla!=13)
                  {  
    				 cadena[contador++]=tecla;
    				 cadena[contador]='\0';
    				 outtextxy(x,y,cadena);
                  }
                } 
                
      }while(tecla!=13 && contador!=6); 
      settextstyle(0, 0, 0);
}

void nivel(int nivel,int puntos,int vidas,char nombre[])
{
   char cad[5];
   int x=390;
   cleardevice();
   settextstyle(10,0,4);
    outtextxy(170,50,"SC  00000000"); 
    if(puntos>=10)
    x-=22;
    if(puntos>=100)
    x-=22;
    if(puntos>=1000)
    x-=22;
    if(puntos>=10000)
    x-=22;
    sprintf(cad,"%d",puntos);
    outtextxy(x,50,cad);  
    
    readimagefile("pag4.jpg",220,200,220+150,200+50);
    
    sprintf(cad,"Nivel - %d",nivel);
    outtextxy(230,300,cad);  
    
    sprintf(cad,"Vidas - %d",vidas);
    outtextxy(225,350,cad); 
    
    outtextxy(250,450,nombre);
    settextstyle(1,0,1);
    delay(1000);
}
