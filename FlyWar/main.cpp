#include <stdio.h>
#include <iostream>
#include<time.h>

#include <graphics.h>
#define TamNodo 29
#define TamMayaX 15//17
#define TamMayaY 21//24


typedef struct nodo
{
      struct nodo *izq;  
      struct nodo *der;  
      struct nodo *arri;  
      struct nodo *aba;  
      int x1,y1,x2,y2;  
      struct bala *NodoB;
      struct enem *NodoE;
      struct enemf *NodoEF;
      struct ju *NodoJ;
      int arma;
}Nodo;

typedef struct
{
      int **malla;        
}dibujo;
typedef struct bala
{
        Nodo *PosB; 
        struct bala *ant,*sig;
        int itipo;
        int indice;
}Bala;

typedef struct{
        Bala *prim,*ult;
        dibujo iconoBal[10];
}ListBalas;

typedef struct enem
{
        Nodo *PosE;
        struct enem *ant,*sig;
        int tipo;
        int indice;
        int pasos;
        int numPasos;
}Enemigo;

typedef struct 
{
        Enemigo *prim,*ult;
        dibujo iconoEnem[10];
}ListEnemigos;

typedef struct enemf
{
    Nodo *PosEF;
    Nodo *PosArma1;
    Nodo *PosArma2;
    int dir;
    int pasos;
    int R;
    int imag;
    
    bool activo;
    dibujo iconoEnemF[3];    
}EnemigoFinal;

typedef struct
{
       char cNombre[10];
       int puntos;    
}Records;

typedef struct ju
{
      Nodo *PosJ; 
      int iNave;
      int iTipoB;
      int iNumB;
      int iVidas;
      int iNivel;
      int enemigosEliminados;
      bool muerto;
      Records R;
      dibujo iconoJug[10];           
}jug;

typedef struct
{
      int x;
      int y;        
}scroll;

void imprimeTiempo(int tiempo);
void ingresaNombre(char nombre[]);
void iniciaScroll(scroll p[]);
void dibujaScroll(scroll p[]);
Nodo *creaNodo(int x1,int y1);
Nodo *creaMaya();
void dibujaEsenario(Nodo *principal,jug J);
void ayuda(jug J,ListEnemigos LE);
int opcion(jug J,scroll p[]);
int menu(jug J,ListBalas LB,ListEnemigos LE,EnemigoFinal EF,Nodo *nodo,scroll p[]);
void mueveJugador(jug *J,int *tecla,ListBalas *LB,bool EnemFActivo);
void borraImagen(int x1,int y1,int x2,int y2);
void paginacion(int *pagina);
Nodo *BuscaNodoPosicion(Nodo *principal,int num);

void creaJugador(jug *J,Nodo *principal);
void insertarRecord(Records bomber);
void imprimirRecords();
int **abreimagen(char nombre[]);
void Dibujaicono(int **imagen,int x,int y,int tamx,int tamy,int pixel);


void imagenesBalas(ListBalas *balas);
bala *creaBala(Nodo *pos,int itipo);
void dispara(Nodo *pos,ListBalas *balas,int tipo);
void MueveListBalas(ListBalas *balas);
void MueveBala(bala *bal,dibujo iconoBal[]);
void cuentaList(ListBalas *lB);
void CondicionesEliminaBalasEnemigo(ListBalas *balas,ListEnemigos *enemigos,jug *J);
void EliminaBala(ListBalas *balas,int numero);


void imagenesEnemigos(ListEnemigos *enemigos);
Enemigo *creaEnemigo(Nodo *pos,int tipo);
void agregaEnemigo(ListEnemigos *enemigos,Nodo *pos,int tipo);
void MueveListEnemigos(ListEnemigos *enemigos);
void MueveEnemigo(Enemigo *enemigo,dibujo iconoEnem[]);
void cuentaList(ListEnemigos *LE);
void CondicionesEliminaEnemigoBalas(ListEnemigos *enemigos,ListBalas *balas,jug *J);
void EliminaEnemigo(ListEnemigos *enemigos,int numero);


void creaEnemigoFinal(EnemigoFinal *EF,Nodo *principal);
void mueveEnemigoFinal(EnemigoFinal *EF,ListEnemigos *LE,bool final);
void enemigosAleatorios(Nodo *nodo,ListEnemigos *LE,int nivel,jug *J,EnemigoFinal *EF,Nodo *principal);

void juego(jug J,ListBalas LB,ListEnemigos LE,EnemigoFinal EF,Nodo *nodo,scroll p[]);

typedef BOOL (*PlaySoundA_ptr) (char*, HMODULE, DWORD); 
PlaySoundA_ptr fn_PlaySound = 0;


void musica1(char cNombre[50]);

int main( )
{
	initwindow( 445 , 620 , "Fly Wars" );
    srand((unsigned) time(NULL));    
	jug J;
	EnemigoFinal EF;
    Nodo *principal=creaMaya();
    scroll p[150];
    iniciaScroll(p);
	ListBalas listB;
	imagenesBalas(&listB);
	ListEnemigos listE;
	imagenesEnemigos(&listE);
	creaEnemigoFinal(&EF,principal);
    creaJugador(&J,principal);
    menu(J,listB,listE,EF,principal,p);
    
	closegraph( );
    
	return( 0 );
}
int menu(jug J,ListBalas LB,ListEnemigos LE,EnemigoFinal EF,Nodo *nodo,scroll p[])
{
    int op = 0;
    do
    {
        op = opcion(J,p);
        switch(op)
        {
               case 1:
                    juego(J,LB,LE,EF,nodo,p);
                    break;
               case 2:
                    ayuda(J,LE);
                    break; 
               case 3:
                    imprimirRecords(); 
                    break;          
        }
             
    }while(op!=4); 
}
void mueveEnemigoFinal(EnemigoFinal *EF,ListEnemigos *LE)
{  
     if(EF->imag == 2)
     {
           Dibujaicono(EF->iconoEnemF[EF->imag].malla,EF->PosEF->x1-(29*7),EF->PosEF->y1-(29*5),33,16,14); 
           setfillstyle(SOLID_FILL,5); 
           bar(10,10,10+EF->R*(10),20);   
           EF->pasos+=1;
           if(EF->pasos == 6)
           {
                 EF->pasos = 0;      
           } 
           if(EF->pasos == 2)
           {
                 agregaEnemigo(LE,EF->PosArma1->der->der,2);
                 agregaEnemigo(LE,EF->PosArma2->izq->izq,2);
           }
           if(EF->pasos == 4)
           {
                agregaEnemigo(LE,EF->PosArma1->izq,3); 
                agregaEnemigo(LE,EF->PosArma2->der,3);     
           }
           if(EF->pasos == 3)
           {
                 agregaEnemigo(LE,EF->PosEF->aba->aba,4);      
           }    
     }
     else
     {
         if(EF->dir == 1)
         {
            if(EF->pasos<2)
            {
               EF->pasos+=1;
               EF->PosEF->NodoEF = NULL;
               EF->PosEF->der->NodoEF = NULL;
               EF->PosEF->izq->NodoEF = NULL;
               EF->PosEF=EF->PosEF->izq;
               EF->PosEF->NodoEF = EF;
               EF->PosEF->der->NodoEF = EF;
               EF->PosEF->izq->NodoEF = EF;
               EF->PosArma1 = EF->PosArma1->izq;
               EF->PosArma2 = EF->PosArma2->izq;
            }
            else
            {
               EF->pasos = -2;
               if(EF->imag == 0)
               {
                   agregaEnemigo(LE,EF->PosArma1,4);
                   agregaEnemigo(LE,EF->PosArma2,4);
               }
               if(EF->imag == 1)
               {
                   agregaEnemigo(LE,EF->PosArma1,1);
                   agregaEnemigo(LE,EF->PosArma2,1);
               }
               if(EF->imag == 2)
               {
                   agregaEnemigo(LE,EF->PosArma1,2);
                   agregaEnemigo(LE,EF->PosArma2,2);
               }
               EF->dir=0;
            }
         }
         if(EF->dir == 0)
         {
            if(EF->pasos<2)
            {
               EF->pasos+=1;
               EF->PosEF->NodoEF = NULL;
               EF->PosEF->der->NodoEF = NULL;
               EF->PosEF->izq->NodoEF = NULL;
               EF->PosEF=EF->PosEF->der;
               EF->PosEF->NodoEF = EF;
               EF->PosEF->der->NodoEF = EF;
               EF->PosEF->izq->NodoEF = EF;
               EF->PosArma1 = EF->PosArma1->der;
               EF->PosArma2 = EF->PosArma2->der;
               if(EF->imag == 1)
               {
                   if(EF->pasos==0)
                   agregaEnemigo(LE,EF->PosEF->aba,4);
               }
            }
            else
            {
                EF->pasos = -2;
                if(EF->imag == 0)
               {
                   agregaEnemigo(LE,EF->PosArma1,4);
                   agregaEnemigo(LE,EF->PosArma2,4);
               }
               if(EF->imag == 1)
               {
                   agregaEnemigo(LE,EF->PosArma1,1);
                   agregaEnemigo(LE,EF->PosArma2,1);
               }
                EF->dir=1;
            }
         }
        Dibujaicono(EF->iconoEnemF[EF->imag].malla,EF->PosEF->x1-(29*5),EF->PosEF->y1-(29*5),25,16,13); 
        setfillstyle(SOLID_FILL,14);
        bar(330,10,330+EF->R*(10),20); 
    }  
}
void juego(jug J,ListBalas LB,ListEnemigos LE,EnemigoFinal EF,Nodo *nodo,scroll p[])
{
     int tecla = 0;
     int pagina = 0;
     int tiempo = 0;
     time_t tIni,tActual=50;
     tIni=time(NULL);
     ingresaNombre(J.R.cNombre); 
     do
     {
         setactivepage(pagina);
         cleardevice();    
         delay(130);    
         tActual=time(NULL);  
         
         dibujaEsenario(nodo,J);
         dibujaScroll(p); 
         tiempo = tActual-tIni;
         
         if(J.muerto == true || J.iNivel == 4)
         {
             if(J.muerto == true) 
             {
                 outtextxy(100,100,"Perdiste"); 
             } 
             if(J.iNivel ==4) 
             {
                outtextxy(100,100,"Nivel completado");          
             }
             if(kbhit())
             {           
                tecla = getch();;
             }
             outtextxy(100,500,"Preciona una tecla.....");
                 
         }
         else
         {
             enemigosAleatorios(nodo,&LE,J.iNivel,&J,&EF,nodo);
             mueveJugador(&J,&tecla,&LB,EF.activo);
             MueveListEnemigos(&LE);
             MueveListBalas(&LB);
             imprimeTiempo(tiempo);
             CondicionesEliminaEnemigoBalas(&LE,&LB,&J);
             CondicionesEliminaBalasEnemigo(&LB,&LE,&J);
         }
         paginacion(&pagina);       
     }while(tecla!=27);   
     J.R.puntos = J.R.puntos+J.iVidas+J.iNivel-((tiempo)/60);
     insertarRecord(J.R);
     imprimirRecords(); 
}

void enemigosAleatorios(Nodo *nodo,ListEnemigos *LE,int nivel,jug *J,EnemigoFinal *EF,Nodo *principal)
{
     if(EF->activo == false)
     {
         if(nivel==1)
         {
              if(J->enemigosEliminados<10)
              {
                   if(rand()%2==1)
                   agregaEnemigo(LE,BuscaNodoPosicion(nodo,rand()%14+1),0);
                   else
                   if(rand()%4==2)
                   agregaEnemigo(LE,BuscaNodoPosicion(nodo,rand()%14+1),4);
              }
              else
              { 
                   EF->pasos = 0;  
                   EF->activo = true; 
                   EF->PosEF->NodoEF = EF;
                   J->enemigosEliminados = 0;
              }
         }
         if(nivel==2)
         {
              if(J->enemigosEliminados<10)
              {
                   if(rand()%2==1)
                       agregaEnemigo(LE,BuscaNodoPosicion(nodo,rand()%14+1),1);
                   else
                       agregaEnemigo(LE,BuscaNodoPosicion(nodo,rand()%14+1),2);
              }
              else
              { 
                  Nodo *a = BuscaNodoPosicion(nodo,rand()%10+200);
                  a->arma = 1;
                   EF->pasos = 0;  
                   EF->activo = true; 
                   EF->PosEF->NodoEF = EF;
                   J->enemigosEliminados = 0;
                   EF->PosEF = BuscaNodoPosicion(principal,82);
                   EF->PosArma1 = BuscaNodoPosicion(principal,82-4);
                   EF->PosArma2 = BuscaNodoPosicion(principal,82+4);
              }
         }
         if(nivel==3)
         {
              if(J->enemigosEliminados<10)
              {
                   if(rand()%2==1)
                       agregaEnemigo(LE,BuscaNodoPosicion(nodo,rand()%14+1),3);
                   else
                       agregaEnemigo(LE,BuscaNodoPosicion(nodo,rand()%14+1),2);
                   if(rand()%2==1)
                       agregaEnemigo(LE,BuscaNodoPosicion(nodo,rand()%14+1),1);
              }
              else
              {
                   EF->dir = 1;
                   EF->pasos = 0;  
                   EF->activo = true; 
                   EF->PosEF->NodoEF = EF;
                   EF->PosEF->izq->NodoEF = EF;
                   EF->PosEF->izq->izq->NodoEF = EF;
                   EF->PosEF->izq->izq->izq->NodoEF = EF;
                   EF->PosEF->der->NodoEF = EF;
                   EF->PosEF->der->der->NodoEF = EF;
                   EF->PosEF->der->der->der->NodoEF = EF;
                   J->enemigosEliminados = 0;
                   EF->PosEF = BuscaNodoPosicion(principal,82);
                   EF->PosArma1 = BuscaNodoPosicion(principal,82-4);
                   EF->PosArma2 = BuscaNodoPosicion(principal,82+4);
              }
         }
     }
     else
     {
         if(EF->R>0)
         {
             mueveEnemigoFinal(EF,LE); 
             J->enemigosEliminados = 0;            
         }
         else
         {
             EF->activo = false;
             EF->PosEF->NodoEF = NULL;
             EF->PosEF->izq->NodoEF = NULL;
             EF->PosEF->izq->izq->NodoEF = NULL;
             EF->PosEF->izq->izq->izq->NodoEF = NULL;
             EF->PosEF->der->NodoEF = NULL;
             EF->PosEF->der->der->NodoEF = NULL;
             EF->PosEF->der->der->der->NodoEF = NULL;
             J->enemigosEliminados = 0;    
             J->iNivel+=1;
             if(J->iNivel==3)
             EF->R = 43;
             else
             EF->R = 10;
             J->R.puntos+=10;
             EF->imag+=1;
             EF->pasos=0;
         }
     }
}
void mueveJugador(jug *J,int *tecla,ListBalas *LB,bool EnemFActivo)
{
     Dibujaicono(J->iconoJug[J->iNave].malla,J->PosJ->x1,J->PosJ->y1,15,15,2);       
     if(kbhit())
     {
        *tecla = getch(); 
         switch(*tecla)
         {
              case 'd':
                   if(J->PosJ->izq!=NULL)
                   {
                      borraImagen(J->PosJ->x1,J->PosJ->y1,0,0);
                      J->PosJ->NodoJ=NULL;
                      J->PosJ->izq->NodoJ = J;
                      J->PosJ=J->PosJ->izq;
                   }
              break;
              case 'a':
                   if(J->PosJ->der!=NULL)
                   {
                      borraImagen(J->PosJ->x1,J->PosJ->y1,0,0);
                      J->PosJ->NodoJ=NULL;
                      J->PosJ->der->NodoJ = J;
                      J->PosJ=J->PosJ->der;
                   }
              break;
              case 'w':
                   if(EnemFActivo==true)
                   {
                       if(J->PosJ->arri->arri->arri->arri->arri->arri->arri->arri->arri!=NULL)
                       {
                          borraImagen(J->PosJ->x1,J->PosJ->y1,0,0);
                          J->PosJ->NodoJ=NULL;
                          J->PosJ->arri->NodoJ = J;
                          J->PosJ=J->PosJ->arri;
                       }
                   }
                   else
                   if(J->PosJ->arri!=NULL)
                   {
                      borraImagen(J->PosJ->x1,J->PosJ->y1,0,0);
                      J->PosJ->NodoJ=NULL;
                      J->PosJ->arri->NodoJ = J;
                      J->PosJ=J->PosJ->arri;
                   }
                   
              break;
              case 's':
                   if(J->PosJ->aba->aba!=NULL)
                   {
                      borraImagen(J->PosJ->x1,J->PosJ->y1,0,0);
                      J->PosJ->NodoJ=NULL;
                      J->PosJ->aba->NodoJ = J;
                      J->PosJ=J->PosJ->aba;
                   }
              break;
              case 13:
                   outtextxy(100,100,"Pausa");
                   getch();
              break;
              case 'f':
                   //if(J->iNumB>-1)
                   //{
                       if(J->iTipoB == 0)
                       {
                         //  J->iNumB-=1;         
                           dispara(J->PosJ->arri,LB,J->iTipoB);
                       }
                       else
                       if(J->iTipoB == 1)
                       {
                          // J->iNumB-=1;  
                           dispara(J->PosJ->arri->izq,LB,J->iTipoB);
                           dispara(J->PosJ->arri->der,LB,J->iTipoB);         
                       }
                       else
                       if(J->iTipoB == 2)
                       {
                         //  J->iNumB-=1;  
                           dispara(J->PosJ->arri->izq,LB,J->iTipoB);
                           dispara(J->PosJ->arri,LB,J->iTipoB);
                           dispara(J->PosJ->arri->der,LB,J->iTipoB);         
                       }
                   //}
              break;
         }
         if(J->PosJ->arma !=0)
         {
            J->PosJ->arma =0;
            J->iTipoB+=1;            
         }
         Dibujaicono(J->iconoJug[J->iNave].malla,J->PosJ->x1,J->PosJ->y1,15,15,2);               
     }                                   
}

void creaEnemigoFinal(EnemigoFinal *EF,Nodo *principal)
{
     EF->PosEF = BuscaNodoPosicion(principal,82);//26
     EF->PosArma1 = BuscaNodoPosicion(principal,82-4);//136
     EF->PosArma2 = BuscaNodoPosicion(principal,82+4);
     EF->iconoEnemF[0].malla=abreimagen("EF0.bet");
     EF->iconoEnemF[1].malla=abreimagen("EF1.bet");
     EF->iconoEnemF[2].malla=abreimagen("EF2.bet");
     EF->dir = 1;
     EF->pasos = 0;  
     EF->activo = false;
     EF->imag = 0;
     EF->R = 10; 
}

void creaJugador(jug *J,Nodo *principal)
{
     J->PosJ=BuscaNodoPosicion(principal,232);
     J->PosJ->NodoJ = J;
     J->iconoJug[0].malla=abreimagen("Jugador1.bet");
     J->iconoJug[1].malla=abreimagen("Jugador2.bet");
     J->iconoJug[2].malla=abreimagen("Jugador3.bet");
     J->iconoJug[3].malla=abreimagen("Vidas.bet");
     J->iconoJug[4].malla=abreimagen("Enem.bet");
     J->iconoJug[5].malla=abreimagen("arma.bet");
     J->iNave = 0;
     J->iTipoB = 0;
     J->iVidas = 3;
     J->iNivel = 1;
     J->R.puntos = 0;
     J->iNumB = 3;
     J->enemigosEliminados = 0;
     J->muerto = false;
}

void imagenesBalas(ListBalas *balas)
{
      balas->prim = NULL;
      balas->ult =NULL;
      balas->iconoBal[0].malla = abreimagen("Bala1.bet");
      balas->iconoBal[1].malla = abreimagen("Bala2.bet");
      balas->iconoBal[2].malla = abreimagen("Bala3.bet");
}

Bala *creaBala(Nodo *pos,int itipo)
{
     bala *nuevo;
     nuevo = (bala*)malloc(sizeof(bala));
     nuevo->ant = NULL;
     nuevo->sig = NULL;
     nuevo->itipo = itipo;
     nuevo->indice = 0;
     nuevo->PosB = pos;
     return (nuevo);
}

void dispara(Nodo *pos,ListBalas *balas,int tipo)
{
     bala *nB;
     nB = creaBala(pos,tipo);
     if(nB)
     {
        if(balas->prim == NULL)
        {
                nB->indice = 1;
                balas->prim = nB;
                balas->ult = nB;         
        }  
        else
        {
                nB->sig = balas->prim;
                balas->prim->ant = nB;
                balas->prim = nB;
                balas->prim->indice = balas->prim->sig->indice+1;  
        }  
        pos->NodoB = nB;
     } 
}

void MueveListBalas(ListBalas *balas)
{
     bala *auz;
     auz = balas->prim;
     while(auz)
     {
         MueveBala(auz,balas->iconoBal);
          auz = auz->sig;     
     }    
}

void MueveBala(bala *bal,dibujo iconoBal[])
{
     if(bal->PosB->arri!=NULL)
     { 
          bal->PosB->NodoB = NULL;
          bal->PosB->arri->NodoB = bal;
          bal->PosB = bal->PosB->arri;
          Dibujaicono(iconoBal[bal->itipo].malla,bal->PosB->x1,bal->PosB->y1,15,15,2);  
     }
}
void CondicionesEliminaBalasEnemigo(ListBalas *balas,ListEnemigos *enemigos,jug *J)
{
    Bala *aux;
    aux = balas->prim;
    while(aux)
    {
       if(aux->PosB->NodoE!=NULL)
       {
            if(aux->PosB->NodoE->tipo!=1)
            {
                if(aux->PosB->NodoE->tipo==0)
                J->R.puntos +=1;
                if(aux->PosB->NodoE->tipo==2)
                J->R.puntos +=2;
                if(aux->PosB->NodoE->tipo==3)
                J->R.puntos +=3;
                EliminaEnemigo(enemigos,aux->PosB->NodoE->indice); 
                J->iNumB+=1;                 
                if(aux->PosB->NodoE->tipo!=4)
                J->enemigosEliminados +=1;

            }
            EliminaBala(balas,aux->indice);     
            musica1("Sound1.wav");     
            
       }else   
       if(aux->PosB->NodoEF!=NULL)
       {
            aux->PosB->NodoEF->R-=1;
            if(aux->PosB->NodoEF->R == 0)
            {
                 aux->PosB->NodoEF->activo = false;
                 //aux->PosB->NodoEF->PosEF->izq->NodoEF = NULL;
                 //aux->PosB->NodoEF->PosEF->izq->izq->NodoEF = NULL;
                 //aux->PosB->NodoEF->PosEF->der->NodoEF = NULL;
                 //aux->PosB->NodoEF->PosEF->der->der->NodoEF = NULL;
                 //aux->PosB->NodoEF = NULL;
            }
            EliminaBala(balas,aux->indice);
            J->iNumB+=1; 
            J->R.puntos +=1;
            musica1("Sound1.wav"); 
       }else
       if(aux->PosB->arri==NULL) 
       { 
            EliminaBala(balas,aux->indice);
            J->iNumB+=1; 
       }  
       aux = aux->sig;       
    }      
}
void EliminaBala(ListBalas *balas,int numero)
{ 
   bala *ult=balas->ult;  
   nodo *Noso=NULL;
   bala *prim=balas->prim;
   
   if(balas->prim==balas->ult)
   {
        balas->prim->PosB->NodoB = NULL;
        balas->prim->PosB = NULL;
        free(balas->prim);
        balas->prim=balas->ult=NULL;
   }else 
   if(balas->ult->indice==numero)
   {                                 
        balas->ult=balas->ult->ant;
        balas->ult->sig=NULL;
        ult->ant = NULL;
        ult->PosB->NodoB=NULL;
        free(ult);         
   }else 
   if(balas->prim->indice==numero)
   {       
        balas->prim=balas->prim->sig;
        balas->prim->ant=NULL;
        prim->sig = NULL;
        prim->PosB->NodoB=NULL;
        free(prim);           
   }else
   {
        while(prim != NULL && prim->indice != numero)
        {
            prim=prim->sig;
        }
        if(prim->indice == numero)
        {  
            prim->sig->ant=prim->ant;
            prim->ant->sig=prim->sig;
            prim->sig=NULL;
            prim->ant=NULL;
            prim->PosB->NodoB=NULL;
            free(prim);
        }
   }                                
}

void imagenesEnemigos(ListEnemigos *enemigos)
{
     enemigos->prim = NULL;
     enemigos->ult = NULL;
     enemigos->iconoEnem[0].malla = abreimagen("Enemigo1.bet");
     enemigos->iconoEnem[1].malla = abreimagen("Enemigo2.bet");
     enemigos->iconoEnem[2].malla = abreimagen("Enemigo3.bet");
     enemigos->iconoEnem[3].malla = abreimagen("Enemigo4.bet");
     enemigos->iconoEnem[4].malla = abreimagen("Enemigo5.bet");
}

Enemigo *creaEnemigo(Nodo *pos,int tipo)
{
        Enemigo *nuevoE;
        nuevoE = (Enemigo*)malloc(sizeof(Enemigo));
        nuevoE->ant = NULL;
        nuevoE->sig = NULL;
        nuevoE->tipo = tipo;
        nuevoE->indice = 0;
        nuevoE->pasos = 0;
        nuevoE->numPasos = rand()%2+3;
        nuevoE->PosE = pos;
        return (nuevoE);
}

void agregaEnemigo(ListEnemigos *enemigos,Nodo *pos,int tipo)
{
     Enemigo *nuevo;
     nuevo = creaEnemigo(pos,tipo);
     if(nuevo)
     {
         if(enemigos->prim == NULL)
         {
              nuevo->indice = 1;
              enemigos->prim = nuevo;
              enemigos->ult = nuevo;                  
         }  
         else
         {
              nuevo->sig = enemigos->prim;
              enemigos->prim->ant = nuevo;
              enemigos->prim = nuevo;
              enemigos->prim->indice = enemigos->prim->sig->indice+1;   
         }       
         pos->NodoE = nuevo;    
     }
}
void MueveListEnemigos(ListEnemigos *enemigos)
{
    Enemigo *aux;
    aux = enemigos->prim;
    while(aux)
    {
       MueveEnemigo(aux,enemigos->iconoEnem);          
       aux = aux->sig;       
    }     
}
void MueveEnemigo(Enemigo *enemigo,dibujo iconoEnem[])
{
     if(enemigo->PosE->aba!=NULL)
     {
          if(enemigo->pasos == enemigo->numPasos)
          {
               enemigo->pasos = 0;
               if(rand()%2 == 1)
               {
                    enemigo->PosE->NodoE = NULL;  
                    enemigo->PosE->izq->NodoE = enemigo; 
                    enemigo->PosE = enemigo->PosE->izq;
                    Dibujaicono(iconoEnem[enemigo->tipo].malla,enemigo->PosE->x1,enemigo->PosE->y1,15,15,2);         
               } 
               else
               {
                    enemigo->PosE->NodoE = NULL;  
                    enemigo->PosE->der->NodoE = enemigo; 
                    enemigo->PosE = enemigo->PosE->der;
                    Dibujaicono(iconoEnem[enemigo->tipo].malla,enemigo->PosE->x1,enemigo->PosE->y1,15,15,2);    
               }            
          }
          else
          {
              if(enemigo->tipo==3)
              enemigo->pasos+=1;
              enemigo->PosE->NodoE = NULL;  
              enemigo->PosE->aba->NodoE = enemigo; 
              enemigo->PosE = enemigo->PosE->aba;
              Dibujaicono(iconoEnem[enemigo->tipo].malla,enemigo->PosE->x1,enemigo->PosE->y1,15,15,2);   
          }                  
     }
}
void CondicionesEliminaEnemigoBalas(ListEnemigos *enemigos,ListBalas *balas,jug *J)
{
    Enemigo *aux;
    aux = enemigos->prim;
    while(aux)
    {
       if(aux->PosE->NodoJ!=NULL)
       {
            EliminaEnemigo(enemigos,aux->indice); 
            J->iVidas -=1;
            musica1("Sound1.wav"); 
            if(J->iVidas == 0)
            {
                 musica1("Muerto.wav"); 
                 J->muerto = true;
            }
       }else  
       if(aux->PosE->NodoB!=NULL)
       {
            EliminaBala(balas,aux->PosE->NodoB->indice);
            musica1("Sound1.wav"); 
            if(aux->tipo!=1)
            {
                if(aux->tipo==0)
                J->R.puntos +=1;
                if(aux->tipo==2)
                J->R.puntos +=2;
                if(aux->tipo==3)
                J->R.puntos +=3;
                EliminaEnemigo(enemigos,aux->indice);
                J->iNumB+=1;
                if(aux->tipo!=4) 
                J->enemigosEliminados +=1; 
                        
            }
       }else
        if(aux->PosE->arri!=NULL && aux->PosE->arri->NodoB!=NULL)
       {
            EliminaBala(balas,aux->PosE->arri->NodoB->indice);
            musica1("Sound1.wav"); 
            if(aux->tipo!=1)
            {
                if(aux->tipo==0)
                J->R.puntos +=1;
                if(aux->tipo==2)
                J->R.puntos +=2;
                if(aux->tipo==3)
                J->R.puntos +=3; 
                EliminaEnemigo(enemigos,aux->indice);
                J->enemigosEliminados +=1;
                J->iNumB+=1; 
            }
       }else
       if(aux->PosE->aba==NULL)  
       {
            EliminaEnemigo(enemigos,aux->indice); 
       }
       aux = aux->sig;       
    }      
}
void EliminaEnemigo(ListEnemigos *enemigos,int numero)
{
   Enemigo *ult=enemigos->ult;  
   nodo *Noso=NULL;
   Enemigo *prim=enemigos->prim;
   
   if(enemigos->prim==enemigos->ult)
   {
        enemigos->prim->PosE->NodoE = NULL;
        enemigos->prim->PosE = NULL;
        free(enemigos->prim);
        enemigos->prim=enemigos->ult=NULL;
   }else 
   if(enemigos->ult->indice==numero)
   {                                 
        enemigos->ult=enemigos->ult->ant;
        enemigos->ult->sig=NULL;
        ult->ant = NULL;
        ult->PosE->NodoE=NULL;
        free(ult);         
   }else 
   if(enemigos->prim->indice==numero)
   {       
        enemigos->prim=enemigos->prim->sig;
        enemigos->prim->ant=NULL;
        prim->sig = NULL;
        prim->PosE->NodoE=NULL;
        free(prim);           
   }else
   {
        while(prim != NULL && prim->indice != numero)
        {
            prim=prim->sig;
        }
        if(prim->indice == numero)
        {  
            prim->sig->ant=prim->ant;
            prim->ant->sig=prim->sig;
            prim->sig=NULL;
            prim->ant=NULL;
            prim->PosE->NodoE=NULL;
            free(prim);
        }
   }  
}
int **abreimagen(char nombre[])
{
   FILE *archivo;
   int tamx,tamy;
   int **malla;
   archivo=fopen(nombre,"r");
   
   if(archivo==NULL)
   {
      return NULL;
   }
   else
   {
        do
        {
          fscanf(archivo, "%d ",&tamx);
          fscanf(archivo, "%d ",&tamy);
          
          malla=(int**)malloc(sizeof(int*)*(tamx));
          for(int i=0;i<tamx;i++)
          {
            malla[i]=(int*)malloc(sizeof(int)*(tamy));
          }
          for(int i=0;i<tamx;i++)
          {
            for(int j=0;j<tamy;j++)
            {
              fscanf(archivo,"%d ",&malla[i][j]);
            }
          }
          return malla;
        }while(!feof(archivo)); 
        fclose(archivo);
   }
}
void borraImagen(int x1,int y1,int x2,int y2)
{
     setfillstyle(SOLID_FILL,9);
     bar(x1,y1,x1+TamNodo+x2,y1+TamNodo+y2);  
}
void Dibujaicono(int **imagen,int x,int y,int tamx,int tamy,int pixel)
{
    int pixelx,pixely;

    pixelx=pixel;
    pixely=pixel;
    for (int i=0;i<tamx;i++)
    {
      for(int j=0;j<tamy;j++)
      {  
         setfillstyle(SOLID_FILL,imagen[i][j]);
         bar(x+pixelx*i,y+pixely*j,x+pixelx+pixelx*i,y+pixely+pixely*j);
      }
    }   
}
Nodo *creaMaya()
{
     Nodo *corre1=NULL;
     Nodo *corre2=NULL;
     Nodo *principal=NULL;
     Nodo *gia=NULL;
     Nodo *aux=NULL;
     Nodo *nuevo=NULL;
     
     for(int i=0;i<TamMayaX;i++)
     {
          for(int j=0;j<TamMayaY;j++)
          { 
               if(i == 0 && j == 0)
               {
                    principal = creaNodo(i*TamNodo+5,j*TamNodo+5);   
                    corre1 = principal;
                    gia = principal; 
               }
               if(j==0 && i!=0)
               {
                       aux = creaNodo(i*TamNodo+5,j*TamNodo+5);
                       aux->der=gia;
                       gia->izq = aux;
                       corre2=gia;
                       gia = aux;
                       corre1=gia;     
               } 
               if(j!=0)
               {
                  nuevo = creaNodo(i*TamNodo+5,j*TamNodo+5);
                  corre1->aba = nuevo;
                  nuevo->arri = corre1;
                  corre1 = nuevo;
               }
               if(i!=0)
               {
                  corre1->der = corre2;
                  corre2->izq = corre1;
                  corre2=corre2->aba;          
               }    
          }               
     }   
     corre1 = principal;
     gia = principal; 
     for(int i=0;i<TamMayaY;i++)
     {          
          for(int j=0;j<TamMayaX;j++)
          {     
                if(j == 14)
                {
                     gia->der = corre1;
                     corre1->izq = gia;  
                }       
                corre1 = corre1->izq;                               
          }  
          gia = gia->aba;
          corre1 = gia;    
     }
     return principal;
}
Nodo *creaNodo(int x1,int y1)
{
     Nodo *nuevo;
     nuevo = (Nodo*)malloc(sizeof(Nodo));  
     nuevo->arri= NULL;
     nuevo->aba = NULL;
     nuevo->der = NULL;
     nuevo->izq = NULL;
     nuevo->NodoE = NULL;
     nuevo->NodoB = NULL;
     nuevo->NodoEF = NULL;
     nuevo->NodoJ = NULL;
     nuevo->x1=x1; 
     nuevo->y1=y1;
     nuevo->x2=x1+TamNodo;
     nuevo->y2=y1+TamNodo;
     nuevo->arma = 0;
     return (nuevo);     
}

Nodo *BuscaNodoPosicion(Nodo *principal,int num)
{
    Nodo *corre = principal;
    Nodo *gia =principal;
    int k = 0;

     for(int i=0;i<TamMayaY;i++)
     {          
          for(int j=0;j<TamMayaX;j++)
          {            
                corre = corre->izq; 
                
                k+=1;
                if(k==num)
                return corre;                                
          }  
          gia = gia->aba;
          corre = gia;    
     }
    
    return NULL;    
}

void iniciaScroll(scroll p[])
{
    for(int cont=1;cont<=100;cont++)
    {
     p[cont].x=rand()%500+2;
     p[cont].y=rand()%695;
    }     
}

void dibujaScroll(scroll p[])
{ 
     for(int cont=1;cont<=100;cont++)
     {
         putpixel(p[cont].x,p[cont].y,1);
         setfillstyle(SOLID_FILL,15);
         bar(p[cont].x,p[cont].y,p[cont].x+5,p[cont].y+5);
         p[cont].y+=10;
         if(p[cont].y>695)
         {
               p[cont].x=rand()%485+5;
               p[cont].y=10;  
         }
         setfillstyle(SOLID_FILL,15);
         bar(p[cont].x,p[cont].y,p[cont].x+5,p[cont].y+5);
    } 
}

void dibujaEsenario(Nodo *principal,jug J)
{
    char c[10];
    
    Nodo *corre = principal;
    Nodo *gia = principal;
    setfillstyle(SOLID_FILL,9);
    bar(6,6, 440,615); 
    settextstyle(8, 0, 2);        
    setfillstyle(1,0);
    itoa(J.R.puntos,c,10); 
    outtextxy(10,590,"Puntos");
    outtextxy(100,590,c);
    Dibujaicono(J.iconoJug[4].malla,200,590,11,11,2); 
    itoa(J.enemigosEliminados,c,10); 
    outtextxy(225,590,c);
    Dibujaicono(J.iconoJug[3].malla,380,590,11,11,2); 
    itoa(J.iVidas,c,10); 
    outtextxy(405,590,c);
    for(int i=0;i<TamMayaY;i++)
     {          
          for(int j=0;j<TamMayaX;j++)
          { 
                if(corre->arma == 1)
                {
                    Dibujaicono(J.iconoJug[5].malla,corre->x1,corre->y1,15,15,2);
                }            
                corre = corre->izq;                                
          }  
          gia = gia->aba;
          corre = gia;    
     } 
}

void paginacion(int *pagina)
{
     setvisualpage(*pagina);
     if(*pagina==0)
     *pagina=1;
     else
     *pagina=0;     
}

void imprimeTiempo(int tiempo)
{
    char cad[3],cad1[3];
    sprintf(cad1,"%.2d:",tiempo/60);  
    sprintf(cad,"%.2d",tiempo%60); 
    outtextxy(10,10,cad1);
    outtextxy(47,10,cad);  

}
void ingresaNombre(char nombre[])
{
    char tecla;     
    int contador=0;
       
    cleardevice();
    setfillstyle(SOLID_FILL,9);
    bar(6,6, 440,615);
    outtextxy(70,550,"Nombre");
    nombre[0]='\0';
    do{
          tecla=getch();
          if(tecla==0)  
             tecla=getch();
            if(tecla==8 && contador > 0)  
    			{
    				 setcolor(BLACK);
    				 outtextxy(170,550,nombre); 
    				 nombre[--contador]='\0'; 
    				 setcolor(WHITE);
    				 outtextxy(170,550,nombre);
    			}
            else
    			{  
                  if(tecla!=13)
                  {  
    				 nombre[contador++]=tecla;
    				 nombre[contador]='\0';
    				 outtextxy(170,550,nombre);
                  }
                } 
                
      }while(tecla!=13 && contador!=6); 
      settextstyle(0, 0, 0);
}
void insertarRecord(Records bomber)
{
     FILE *pt;
     Records p[5],aux[5];
     
     pt=fopen("Record.rbe","rb+");
     if(pt==NULL)
     {
          outtextxy(100,100,"No hay records"); 
          FILE *pt;
          Records p[5];
          pt=fopen("Record.rbe","wb");
          for(int i=0;i<5;i++)
           {
           strcpy(p[i].cNombre,"-");
           p[i].puntos=0;
           }
          fwrite(p,sizeof(Records),5,pt);
          getch();
     }
     else
     {
         fread(p,sizeof(Records),5,pt);
         fseek(pt,0,SEEK_SET);
         if(bomber.puntos > p[0].puntos)
         {
         strcpy(aux[0].cNombre,bomber.cNombre);
         aux[0].puntos=bomber.puntos;
           for(int i=0;i<4;i++)
           {
           strcpy(aux[i+1].cNombre,p[i].cNombre);
           aux[i+1].puntos=p[i].puntos;
           }
         }
         else
         {
             int i=0;
             do
             {
                 strcpy(aux[i].cNombre,p[i].cNombre);
                 aux[i].puntos=p[i].puntos;
                 i++;
                 if(i==5)
                 break;
             }while(p[i].puntos > bomber.puntos);
         if(i!=5)
         {
         strcpy(aux[i].cNombre,bomber.cNombre);
         aux[i].puntos=bomber.puntos;
         
           for(;i<4;i++)
           {
                        strcpy(aux[i+1].cNombre,p[i].cNombre);
                        aux[i+1].puntos=p[i].puntos;
           }
          }
         }
         fwrite(aux,sizeof(Records),5,pt);
     }
     fclose(pt);
}
void imprimirRecords()
{
    FILE *pt;
    Records avion[5];
    char cpuntos[20];
    int i;
    char op;
    
    
    cleardevice();
    bar(6,6, 440,615);
    setfillstyle(SOLID_FILL,9);
    settextstyle(8,0,2);
    pt=fopen("Record.rbe","rb");
    do{
      if(pt==NULL) 
      {
          outtextxy(50,600,"No hay records");
          FILE *pt;
          Records p[5];
          pt=fopen("Record.rbe","wb");
          for(int i=0;i<5;i++)
          {
             strcpy(p[i].cNombre,"-");
             p[i].puntos=0;
          }
          fwrite(p,sizeof(Records),5,pt);
          getch();
      }
      else
      {
       fread(avion,sizeof(Records),5,pt);
        for(i=0;i<5;i++)
         {
            itoa(avion[i].puntos,cpuntos,10);
            outtextxy(50,100+(i+1)*30,avion[i].cNombre);
            outtextxy(300,100+(i+1)*30,cpuntos);
         }
       }
        op=getch();
    }while(op!=13);
    fclose(pt);
}
void ayuda(jug J,ListEnemigos LE)
{
    int iTecla;
    FILE *f;
    char c[255];
    int x=10,y=10;
    cleardevice();
    bar(6,6, 440,615);
    settextstyle(10, 0, 19); 
    Dibujaicono(J.iconoJug[0].malla,100,100,15,15,2); 
    Dibujaicono(J.iconoJug[5].malla,100,150,15,15,2);  
    Dibujaicono(LE.iconoEnem[0].malla,50,270,15,15,2);      
    Dibujaicono(LE.iconoEnem[1].malla,50,570,15,15,2);    
    Dibujaicono(LE.iconoEnem[2].malla,50,340,15,15,2);     
    Dibujaicono(LE.iconoEnem[3].malla,50,420,15,15,2);  
     Dibujaicono(LE.iconoEnem[4].malla,50,500,15,15,2);     
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
int opcion(jug J,scroll p[])
{
    int tecla;
    int num=1;
    int pagina = 0;
    int x=30;
    int l1 = 450;
    int l2 = 480;
    setfillstyle(SOLID_FILL,9);
   
    settextstyle(8, 0, 2);        
    do
    { 
        setactivepage(pagina);    
        cleardevice();
        bar(6,6, 440,615);
        setfillstyle(SOLID_FILL,0);  
        bar(-1,10, 616,45); 
        bar(-1,70, 616,100); 
        outtextxy(l1,10,"Universidad Autonoma de San Luis Potosi - Facultad de Ingenieria - ");
        outtextxy(l2,70,"Area de Computacion e  Informatica");
        outtextxy(175,120,"Fly Wars");
        outtextxy(60,170,"----------------------------");
        outtextxy(60,200,"----------------------------");
        l1-=5;
        l2-=5;
        if(l1 == -900)
        l1=450;
        dibujaScroll(p); 
        if(l2 == -480)
        l2 = 480;
        setfillstyle(SOLID_FILL,9);
        delay(50);
        setfillstyle(SOLID_FILL,0);
        bar(-1,425,615 ,450); 
        outtextxy(25,425,"Jugar");
        outtextxy(125,425,"Ayuda");
        outtextxy(225,425,"Records");
        outtextxy(335,425,"Salir");
        rectangle(-1,425,615 ,450);        
        Dibujaicono(J.iconoJug[0].malla,x,450,15,15,2);
         paginacion(&pagina); 
         if(kbhit())
         { 
         tecla=getch();
         switch(tecla)
         {
                      case KEY_LEFT:if(x!=30){x-=100;num-=1;}
                      break;
                      case KEY_RIGHT:if(x!=330){x+=100;num+=1;}
                      break;
                      case 13:return num;
                      break;
         }
         }
    }while(tecla!=27);     
}
void musica1(char cNombre[50])
{
  HMODULE Lib = LoadLibrary("winmm.dll");  
  if (Lib)
  {
    fn_PlaySound =(PlaySoundA_ptr)GetProcAddress(Lib, "PlaySoundA"); 
    if (fn_PlaySound)
    fn_PlaySound(cNombre, NULL, SND_FILENAME | SND_ASYNC);  
    else
    outtextxy(150,200,"Error No se encontro la función PlaySoundA"); 
   
    FreeLibrary(Lib); 
  }
}
