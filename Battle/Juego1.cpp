#include <stdio.h>
#include <iostream>

#include <graphics.h>
#define TamNodo 40
#define TamMayaX 21
#define TamMayaY 17

typedef struct 
{
  int x;
  int y;
  int color;  
}Pixel;

typedef struct
{
  int tam;
  Pixel **MallaP;
}Imagen;

typedef struct bala
{
        struct nodo *pos;
        struct bala *ant,*sig;
        int Dir;
        int num;
        struct jug *j;
}Bala;

typedef struct{
        Bala *prim,*ult;
        Imagen imaBal[5];
}ListaBalas;

typedef struct balaE
{
        struct nodo *pos;
        struct balaE *ant,*sig;
        int Dir;
        int num;
        struct enem *e;
}BalaE;

typedef struct{
        BalaE *prim,*ult;
        Imagen imaBalE[5];
}ListaBalasE;

typedef struct enem
{
        struct nodo *pos;
        struct enem *ant,*sig;
        int num;
        int direccion;
        int pasos;
        int tipo;
        int balaE;
}Enemigo;

typedef struct{
        Enemigo *prim,*ult;
        Imagen imaEne[10];
}ListaEnemigos;

typedef struct nodo
{
      struct nodo *adelante;  
      struct nodo *atras;  
      struct nodo *arriba;  
      struct nodo *abajo; 
      
      int obstaculo;
      int x1,y1,x2,y2; 
      
      Bala *balaNodo;
      BalaE *balaNodoE;
      Enemigo *enemigoNodo;
      struct jug *jn;
}Nodo;

typedef struct jug
{     
      Imagen imaJu[5];
      char nomb[10];
      int Dir;
      int bala;
      int vida;
      int puntos;
      Nodo *PosJug;  
}Jugador;

typedef struct{
        char nombre[10];
        int puntos;
}Record;  

Nodo *creaNodo(int x1,int y1);
Nodo *creaMaya();
void dibuja(Nodo *principal,Imagen obst[]);
void Juego(Jugador J,ListaBalas LB,ListaEnemigos LE,ListaBalasE LBE,Nodo *posJug,Imagen animacion[],Imagen obst[],Nodo *principal,int nivel);
void borraImagen(int x1,int y1);
int leeimagen(Imagen *ima,char nombre[]);
void DibujaImagen(Imagen ima,int x, int y,int dir);
nodo *CargaEscenario(nodo *Principal,int nivel,Imagen obst[],ListaEnemigos *lE);
void cargaImagenesObstaculos(Imagen obst[],Imagen animacion[]);
void cargaImagenesJugador(Jugador *J);
void MueveJugador(Jugador *J,ListaBalas *LB,int tecla);
void explocion(Imagen animacion[],Nodo *pos);
void ayuda(Jugador J,Imagen obst[],ListaEnemigos lE);
char **lee_ayuda(int *cont);
void texto(int x,int y, char nombre[]);

void cargaImagenesBalas(ListaBalas *LB, ListaBalasE *LBE);
Bala *CreaBalaJ(int Direccion,Nodo *Pos);
Bala *DisparaJ(ListaBalas *balas,Nodo *pos,int iDireccion);
void MueveBalasJ(ListaBalas *balas,ListaBalasE *balasE,ListaEnemigos *enemigos,Imagen animacion[]);
int MueveBalaJ(Bala *bala,ListaBalas *balas);
void EliminaBalaJ(ListaBalas  *bal,int numero);

BalaE *CreaBalaE(int Direccion,Nodo *Pos);
BalaE *DisparaE(ListaBalasE *balasE,Nodo *pos,int iDireccion);
void MueveBalasE(ListaBalasE *balasE,Nodo *posJug,Imagen animacion[]);
int MueveBalaE(BalaE *bala,ListaBalasE *balas);
void EliminaBalaE(ListaBalasE  *balE,int numero);

void cargaImagenesEnemigo(ListaEnemigos *E);
Enemigo *CreaEnemigo(int Direccion,Nodo *Pos,int tipo);
void InsertaEnemigo(ListaEnemigos *lE,Nodo *pos,int iDireccion,int tipo);
void MueveEnemigos(ListaEnemigos *lE,ListaBalasE *balas);
void MueveEnemigo(Enemigo *E,ListaBalasE *LB,ListaEnemigos lE,int movimiento);
void buscaDireccion(Enemigo *E,ListaEnemigos lE);
void EliminaEnemigo(ListaEnemigos  *ene,int numero);
int cuentalistaEnemigos(ListaEnemigos lE);
bool perdiste=false;
bool ganaste =false;
void EliminaEscenario(nodo *Principal);
void graficos(Jugador *J,int numE,nodo *Principal,int *nivel,Imagen obst[],ListaEnemigos *lE);
void Nivel(int nivel);
void vidas_jugador(Jugador J);
int menu(Jugador J,Imagen obst[]);
void dibujaNombreJuego(Imagen obst[]);

void ImprimieRecord(Jugador J,Imagen obst[],ListaEnemigos lE,nodo *principal);
void IniciaRecords(Record tanque);
void InsertarRecord(Record tanque);

void creditos(Jugador J,Imagen obst[]);


int main( )
{
	initwindow( 1050, 700 , "Battle City" );
	int nivel=1;
	int tecla;
	Jugador J;
	ListaBalas lB;
	ListaBalasE lBE;
	ListaEnemigos lE;
	Imagen obst[10];
	Imagen animacion[10];
	srand( (unsigned)time( NULL ) );
	

    Nodo *principal=creaMaya();
    Nodo *posJug=NULL;
    cargaImagenesObstaculos(obst,animacion); 
    cargaImagenesJugador(&J);
    cargaImagenesEnemigo(&lE);
    cargaImagenesBalas(&lB,&lBE);

    do
    {
         tecla = menu(J,obst);
         cleardevice();
         switch(tecla)
         {
                      case 1:
                        texto(400,300,J.nomb);
                        Nivel(nivel);
                        EliminaEscenario(principal);
                        J.PosJug=posJug=CargaEscenario(principal,nivel,obst,&lE);
                        posJug->jn=&J;
                        setactivepage(1);
                        dibuja(principal,obst);
                        Juego(J,lB,lE,lBE,posJug,animacion,obst,principal,nivel); 
                        ImprimieRecord(J,obst,lE,principal);  
                      break;
                      case 2:
                           setactivepage(1);
                           delay(600);
                           ImprimieRecord(J,obst,lE,principal);
                      break;
                      case 3:
                           ayuda(J,obst,lE);
                      break;
                      case 4:
                           creditos(J,obst);
                      break;
         }
    }while(tecla!=27);
	closegraph( );
	return( 0 );
}

void ayuda(Jugador J,Imagen obst[],ListaEnemigos lE)
{
     int inicio =0,i,op=0,cont;
     int numLetras=10;
     char **A;
     
     cleardevice();
     settextstyle(2,0,8);
     outtextxy(800,120,"Ladrillo:");
     DibujaImagen(obst[0],900,100,0);
     DibujaImagen(obst[0],900+40,100,0);
     DibujaImagen(obst[0],900+40,100+40,0);
     DibujaImagen(obst[0],900,100+40,0);
     outtextxy(800,220,"Piedra:");
     DibujaImagen(obst[2],900,200,0);
     DibujaImagen(obst[2],900+40,200,0);
     DibujaImagen(obst[2],900+40,200+40,0);
     DibujaImagen(obst[2],900,200+40,0);
     outtextxy(800,320,"Pasto:");
     DibujaImagen(obst[3],900,300,0);
     DibujaImagen(obst[3],900+40,300,0);
     DibujaImagen(obst[3],900+40,300+40,0);
     DibujaImagen(obst[3],900,300+40,0);
     outtextxy(800,420,"Agua:");
     DibujaImagen(obst[4],900,400,0);
     DibujaImagen(obst[4],900+40,400,0);
     DibujaImagen(obst[4],900+40,400+40,0);
     DibujaImagen(obst[4],900,400+40,0);
     outtextxy(790,520,"Enemigo:");
     DibujaImagen(lE.imaEne[0],900,500+40,1);
     DibujaImagen(lE.imaEne[0],900+40,500,1);
     DibujaImagen(lE.imaEne[0],900+40,500+40,1);
     DibujaImagen(lE.imaEne[0],900,500,1);
     outtextxy(180,550+50,"Jugador:");
     DibujaImagen(J.imaJu[0],300,540+50,1);
     outtextxy(350,550+50,"Bandera:");
     DibujaImagen(obst[1],470,540+50,0);
     
     A = lee_ayuda(&cont);
     do{
         setfillstyle(SOLID_FILL,BLACK);
                   bar(100,100,700,550);
         for(i=0;i<numLetras;i++)
              outtextxy(100,100+i*40,A[i+inicio]);
         
         op = getch();
         switch(op)
         {
                     
                   case 80:
                        if(inicio<cont-numLetras)
                        inicio++;
                        break;
                   case 72:
                        if(inicio>0)
                        inicio--;
                        break;
         }
     }while(op!=13);
     cleardevice();
}

void Juego(Jugador J,ListaBalas LB,ListaEnemigos LE,ListaBalasE LBE,Nodo *posJug,Imagen animacion[],Imagen obst[],Nodo *principal,int nivel )
{
     int tecla;
     int numE;
     int pag=0;
     Record tanque;
     strcpy(tanque.nombre,J.nomb);
     DibujaImagen(J.imaJu[0],J.PosJug->x1,J.PosJug->y1,0);
     readimagefile("Fondo.jpg",840,0,1050,680);  
     do
     {  
    if(pag==0)
    {
     setactivepage(1);
     setvisualpage(1);
    } 
    pag=1;                                         
       MueveJugador(&J,&LB,tecla);
       MueveEnemigos(&LE,&LBE);
       MueveBalasE(&LBE,posJug,animacion);
       MueveBalasJ(&LB,&LBE,&LE,animacion); 
       numE=cuentalistaEnemigos(LE);
       graficos(&J,numE,principal,&nivel,obst,&LE);
       vidas_jugador(J);
       if(J.vida==0)
       perdiste=true;                                     
     }while(perdiste==false && ganaste ==false); 
     tanque.puntos=J.puntos;
     tanque.puntos+=nivel*10;
     InsertarRecord(tanque);
     cleardevice();
}


void ImprimieRecord(Jugador J,Imagen obst[],ListaEnemigos lE,nodo *principal)
{
    FILE *pt;
    Record tanque[5];
    char puntos[20];
    char tecla;  
    
    int pag=0;
    cleardevice();
    
    setcolor(15);
    settextstyle(2, 0, 10); 
    
    DibujaImagen(J.imaJu[3],840,100,1);
    DibujaImagen(obst[3],880,180,1);
    DibujaImagen(obst[2],840,180,1);
    DibujaImagen(obst[2],880,140,1);
    DibujaImagen(obst[2],920,140,1);
    DibujaImagen(obst[2],920,180,1);
    
    DibujaImagen(obst[3],880,180+40*5,1);
    DibujaImagen(obst[2],840,180+40*9,1);
    DibujaImagen(obst[2],880,140+40*8,1);
    DibujaImagen(obst[2],920,140+40*7,1);
    DibujaImagen(obst[2],920,180+40*6,1);
    
    DibujaImagen(obst[4],920,180+40*11,1);
    DibujaImagen(obst[4],920,180+40*13,1);
    DibujaImagen(obst[4],920+40*2,180+40*10,1);
    
    EliminaEscenario(principal);
    CargaEscenario(principal,6,obst,&lE);
    dibuja(principal,obst);
    EliminaEscenario(principal);
    
   
    
    
    pt=fopen("Record.bc","rb");

    do{
   if(pag==0)
    {
     setactivepage(1);
     setvisualpage(1);
    } 
    pag=1;
      if(pt==NULL) 
      {
           return;        
      }
      else
      {
       fread(tanque,sizeof(Record),5,pt);
        for(int i=0;i<5;i++)
         {
         itoa(tanque[i].puntos,puntos,10);
         if(tanque[i].puntos!=0){DibujaImagen(J.imaJu[0],700,100+(i+1)*50,1);}
         outtextxy(200,100+(i+1)*50,tanque[i].nombre);
         outtextxy(600,100+(i+1)*50,puntos);
         }
       }
        tecla=getch();
    }while(tecla!=13);
    fclose(pt);
    cleardevice();
}

void graficos(Jugador *J,int numE,Nodo *principal,int *nivel,Imagen obst[],ListaEnemigos *lE)
{
     for(int i=0;i<7;i++)
     {
         if(numE>i)
         { 
           readimagefile("Tanque.jpg",870,30+30*i,900,60+30*i); 
         }
         else
             readimagefile("tap.jpg",870,30+30*i,900,60+30*i); 
     }
     if(numE==0) 
     {
            cleardevice();
            *nivel+=1;
            
            if(*nivel==6)
            {
               ganaste=true;                         
            }
            else
            {  
             Nivel(*nivel);
             Nodo *posJug=NULL;
             EliminaEscenario(principal);
             
             J->PosJug=posJug=CargaEscenario(principal,*nivel,obst,lE);
             posJug->jn=J;
             J->Dir=0;
             DibujaImagen(J->imaJu[0],J->PosJug->x1,J->PosJug->y1,0);
             setactivepage(1);
             dibuja(principal,obst);
             readimagefile("Fondo.jpg",840,0,1100,680); 
            }
     }  
}

int cuentalistaEnemigos(ListaEnemigos lE)
{
    int i=0;
    Enemigo *aux=lE.prim;
    while(aux!=NULL)
    {
        i+=1;
        aux=aux->sig;
    }
    return i;
}

int menu(Jugador J,Imagen obst[])
{
    int tecla=0;
    int opcion=1;
    
    dibujaNombreJuego(obst);
    setcolor(15);
    settextstyle(2, 0, 10);
    do
    {
        outtextxy(380,400+30,"JUGAR");
        outtextxy(380,450+30,"RECORDS");
        outtextxy(380,500+30,"AYUDA");
        outtextxy(380,550+30,"CREDITOS");
        if(opcion==1){DibujaImagen(J.imaJu[1],300,400+30,1);borraImagen(300,450+30);borraImagen(300,500+30);borraImagen(300,550+30);}
        if(opcion==2){DibujaImagen(J.imaJu[1],300,450+30,1);borraImagen(300,400+30);borraImagen(300,550+30);borraImagen(300,500+30);}
        if(opcion==3){DibujaImagen(J.imaJu[1],300,500+30,1);borraImagen(300,450+30);borraImagen(300,400+30);borraImagen(300,550+30);}
        if(opcion==4){DibujaImagen(J.imaJu[1],300,550+30,1);borraImagen(300,450+30);borraImagen(300,400+30);borraImagen(300,500+30);}
         if(kbhit())
         {
            tecla = getch();
            switch(tecla)
            {
            case 72:
                 if(opcion!=1)
                 opcion-=1;
                 
            break;
            case 80:
                 if(opcion<4)
                 opcion+=1;
                 
            break;           
            } 
         }                   
    }while(tecla!=13); 
    return opcion; 
}


void Nivel(int nivel)
{
    char cnivel[5];
    
    
    settextstyle(3, 0, 5);
    outtextxy(400,300,"STAGE");
    itoa(nivel,cnivel,10);       
    outtextxy(580,300,cnivel);
    
    delay(3000);
    cleardevice(); 
}

void cargaImagenesEnemigo(ListaEnemigos *E)
{
     leeimagen(&E->imaEne[0],"tqe1.bet");
     leeimagen(&E->imaEne[1],"tqe2.bet");
     leeimagen(&E->imaEne[2],"tqe1p.bet");
     leeimagen(&E->imaEne[3],"tqe2p.bet");
     E->prim=NULL;
     E->ult=NULL;
}

Enemigo *CreaEnemigo(int Direccion,Nodo *Pos,int tipo)
{
        Enemigo *nuevo;
        nuevo=(Enemigo*)malloc(sizeof(Enemigo));
        nuevo->ant=NULL;
        nuevo->sig=NULL;
        nuevo->pos=Pos;
        nuevo->direccion=Direccion;
        nuevo->num=0;
        nuevo->balaE=1;
        nuevo->tipo=tipo;
        nuevo->pasos=0;
        return (nuevo);
}

void InsertaEnemigo(ListaEnemigos *lE,Nodo *pos,int iDireccion,int tipo)
{
      Enemigo *nuevoE;  
      Nodo *aux = pos;        
      nuevoE=CreaEnemigo(iDireccion,aux,tipo);
      if(nuevoE->pos->obstaculo!=4)
      {
         if(aux->obstaculo==5)
         DibujaImagen(lE->imaEne[2],nuevoE->pos->x1,nuevoE->pos->y1,0);
         else
         if(iDireccion==0){DibujaImagen(lE->imaEne[0],nuevoE->pos->x1,nuevoE->pos->y1,0);}
         if(iDireccion==1){DibujaImagen(lE->imaEne[1],nuevoE->pos->x1,nuevoE->pos->y1,1);}
         if(iDireccion==2){DibujaImagen(lE->imaEne[1],nuevoE->pos->x1,nuevoE->pos->y1,0);}
         if(iDireccion==3){DibujaImagen(lE->imaEne[0],nuevoE->pos->x1,nuevoE->pos->y1,1);}
      }
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
      aux->enemigoNodo=nuevoE;
      return;
      } 
}

void cargaImagenesBalas(ListaBalas *LB, ListaBalasE *LBE)
{
      leeimagen(&LB->imaBal[0],"bala1.bet");  
      leeimagen(&LB->imaBal[1],"bala2.bet");  
      leeimagen(&LB->imaBal[2],"bala3.bet");  
      leeimagen(&LB->imaBal[3],"agua.bet");
      leeimagen(&LB->imaBal[4],"pasto.bet");
      LB->prim=NULL;
      LB->ult=NULL;
      
      leeimagen(&LBE->imaBalE[0],"bala1.bet");  
      leeimagen(&LBE->imaBalE[1],"bala2.bet");  
      leeimagen(&LBE->imaBalE[2],"bala3.bet");  
      leeimagen(&LBE->imaBalE[3],"agua.bet");
      leeimagen(&LBE->imaBalE[4],"pasto.bet");
      LBE->prim=NULL;
      LBE->ult=NULL;
}
void cargaImagenesObstaculos(Imagen obst[],Imagen animacion[])
{
     leeimagen(&obst[0],"muro.bet");  
     leeimagen(&obst[1],"band.bet");  
     leeimagen(&obst[2],"pared.bet");  
     leeimagen(&obst[3],"pasto.bet");
     leeimagen(&obst[4],"agua.bet");
     
     leeimagen(&animacion[0],"exp1.bet");  
     leeimagen(&animacion[1],"exp2.bet");  
     leeimagen(&animacion[2],"exp3.bet");  
     leeimagen(&animacion[3],"exp4.bet");
}

void explocion(Imagen animacion[],Nodo *pos)
{
     int tiempo=0;
     do{
         if(tiempo==0)
         DibujaImagen(animacion[0],pos->x1,pos->y1,0);   
         if(tiempo==100)
         DibujaImagen(animacion[1],pos->x1,pos->y1,0);   
         if(tiempo==200)
         DibujaImagen(animacion[2],pos->x1,pos->y1,0);   
         if(tiempo==300)
         {
         DibujaImagen(animacion[3],pos->x1,pos->y1,0);  
         } 
         
         tiempo++;
     }while(tiempo<310);
     tiempo=0;
}
void cargaImagenesJugador(Jugador *J)
{
     leeimagen(&J->imaJu[0],"tq1.bet");
     leeimagen(&J->imaJu[1],"tq2.bet");
     leeimagen(&J->imaJu[2],"tq1p.bet");
     leeimagen(&J->imaJu[3],"tq2p.bet");
     J->Dir=0;
     J->bala=1;
     J->vida=3;
     J->puntos=0;
}

void MueveJugador(Jugador *J,ListaBalas *LB,int tecla)
{

     if(kbhit())
         {
            tecla = getch(); 
         
             switch(tecla)
             {
                  case 72:
                       if(J->PosJug->arriba!=NULL && J->PosJug->arriba->enemigoNodo==NULL && J->Dir == 0)
                       {
                          if(J->PosJug->arriba->obstaculo==0 && J->PosJug->obstaculo==0)
                          {
                            borraImagen(J->PosJug->x1,J->PosJug->y1);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->arriba;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[0],J->PosJug->x1,J->PosJug->y1,0);
                          }
                          else
                          if(J->PosJug->arriba->obstaculo==4 && J->PosJug->obstaculo==0)
                          {
                            borraImagen(J->PosJug->x1,J->PosJug->y1);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->arriba;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[2],J->PosJug->x1,J->PosJug->y1,0);
                          }
                          else
                          if(J->PosJug->obstaculo==4 && J->PosJug->arriba->obstaculo==0)
                          {
                            DibujaImagen(J->imaJu[3],J->PosJug->x1,J->PosJug->y1,0);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->arriba;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[0],J->PosJug->x1,J->PosJug->y1,0);
                          }
                          else
                          if(J->PosJug->obstaculo==4 && J->PosJug->arriba->obstaculo!=1 && J->PosJug->arriba->obstaculo!=3)
                          {
                            DibujaImagen(J->imaJu[3],J->PosJug->x1,J->PosJug->y1,0);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->arriba;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[2],J->PosJug->x1,J->PosJug->y1,0);
                          }
                       }
                       else
                       {
                           J->Dir = 0;
                           if(J->PosJug->obstaculo==0)
                           DibujaImagen(J->imaJu[0],J->PosJug->x1,J->PosJug->y1,0);
                       }
                  break;
                  case 77:
                       if(J->PosJug->adelante!=NULL && J->PosJug->adelante->enemigoNodo==NULL && J->Dir == 1)
                       {  
                          if(J->PosJug->adelante->obstaculo==0 && J->PosJug->obstaculo==0)
                          {
                            borraImagen(J->PosJug->x1,J->PosJug->y1);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->adelante;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[1],J->PosJug->x1,J->PosJug->y1,1);
                          }
                          else
                          if(J->PosJug->adelante->obstaculo==4 && J->PosJug->obstaculo==0)
                          {
                            borraImagen(J->PosJug->x1,J->PosJug->y1);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->adelante;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[2],J->PosJug->x1,J->PosJug->y1,0);
                          }
                          else
                          if(J->PosJug->obstaculo==4 && J->PosJug->adelante->obstaculo==0)
                          {
                            DibujaImagen(J->imaJu[3],J->PosJug->x1,J->PosJug->y1,0);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->adelante;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[1],J->PosJug->x1,J->PosJug->y1,1);
                          }
                          else
                          if(J->PosJug->obstaculo==4 && J->PosJug->adelante->obstaculo!=1 && J->PosJug->adelante->obstaculo!=3)
                          {
                            DibujaImagen(J->imaJu[3],J->PosJug->x1,J->PosJug->y1,0);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->adelante;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[2],J->PosJug->x1,J->PosJug->y1,0);
                          }
                       }
                       else
                       {
                           J->Dir=1;
                           if(J->PosJug->obstaculo==0)
                           DibujaImagen(J->imaJu[1],J->PosJug->x1,J->PosJug->y1,1);
                       }
                  break;
                  case 80:
                       if(J->PosJug->abajo!=NULL && J->PosJug->abajo->enemigoNodo==NULL && J->Dir == 2)
                       {
                          if(J->PosJug->abajo->obstaculo==0 && J->PosJug->obstaculo==0)
                          {
                            borraImagen(J->PosJug->x1,J->PosJug->y1);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->abajo;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[1],J->PosJug->x1,J->PosJug->y1,0);
                          }
                          else
                          if(J->PosJug->abajo->obstaculo==4 && J->PosJug->obstaculo==0)
                          {
                            borraImagen(J->PosJug->x1,J->PosJug->y1);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->abajo;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[2],J->PosJug->x1,J->PosJug->y1,0);
                          }
                          else
                          if(J->PosJug->obstaculo==4 && J->PosJug->abajo->obstaculo==0)
                          {
                            DibujaImagen(J->imaJu[3],J->PosJug->x1,J->PosJug->y1,0);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->abajo;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[1],J->PosJug->x1,J->PosJug->y1,0);
                          }
                          else
                          if(J->PosJug->obstaculo==4 && J->PosJug->abajo->obstaculo!=1 && J->PosJug->abajo->obstaculo!=3)
                          {
                            DibujaImagen(J->imaJu[3],J->PosJug->x1,J->PosJug->y1,0);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->abajo;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[2],J->PosJug->x1,J->PosJug->y1,0);
                          }
                       }
                       else
                       {
                           J->Dir = 2;
                           if(J->PosJug->obstaculo==0)
                           DibujaImagen(J->imaJu[1],J->PosJug->x1,J->PosJug->y1,0);
                       }
                  break;
                  case 75:
                       if(J->PosJug->atras!=NULL && J->PosJug->atras->enemigoNodo==NULL && J->Dir == 3)
                       {
                          if(J->PosJug->atras->obstaculo==0 && J->PosJug->obstaculo==0)
                          {
                            borraImagen(J->PosJug->x1,J->PosJug->y1);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->atras;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[0],J->PosJug->x1,J->PosJug->y1,1);
                          }
                          else
                          if(J->PosJug->atras->obstaculo==4 && J->PosJug->obstaculo==0)
                          {
                            borraImagen(J->PosJug->x1,J->PosJug->y1);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->atras;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[2],J->PosJug->x1,J->PosJug->y1,0);
                          }
                          else
                          if(J->PosJug->obstaculo==4 && J->PosJug->atras->obstaculo==0)
                          {
                            DibujaImagen(J->imaJu[3],J->PosJug->x1,J->PosJug->y1,0);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->atras;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[0],J->PosJug->x1,J->PosJug->y1,1);
                          }
                          else
                          if(J->PosJug->obstaculo==4 && J->PosJug->atras->obstaculo!=1  && J->PosJug->atras->obstaculo!=3)
                          {
                            DibujaImagen(J->imaJu[3],J->PosJug->x1,J->PosJug->y1,0);
                            J->PosJug->jn=NULL;
                            J->PosJug=J->PosJug->atras;
                            J->PosJug->jn=J;
                            DibujaImagen(J->imaJu[2],J->PosJug->x1,J->PosJug->y1,0);
                          }
                       }
                       else
                       {
                           J->Dir = 3;
                           if(J->PosJug->obstaculo==0)
                           DibujaImagen(J->imaJu[0],J->PosJug->x1,J->PosJug->y1,1);
                       }
                  break;
                  case 'f':
                       if(J->bala==1)
                       {  
                           Bala *balaJ;
                           if(J->Dir == 0)
                           {
                              if(J->PosJug->arriba!=NULL && J->PosJug->arriba->obstaculo!=3)
                              {
                                  balaJ=DisparaJ(LB,J->PosJug->arriba,J->Dir);
                                  J->bala=0;
                                  balaJ->j=J;  
                              }
                           }
                           if(J->Dir == 1)
                           {
                              if(J->PosJug->adelante!=NULL && J->PosJug->adelante->obstaculo!=3)
                              {
                                  balaJ=DisparaJ(LB,J->PosJug->adelante,J->Dir);
                                  J->bala=0;
                                  balaJ->j=J;
                              }
                           }
                           if(J->Dir == 2)
                           {
                              if(J->PosJug->abajo!=NULL && J->PosJug->abajo->obstaculo!=3)
                              {
                                  balaJ=DisparaJ(LB,J->PosJug->abajo,J->Dir);
                                  J->bala=0;
                                  balaJ->j=J;
                              }
                           }
                           if(J->Dir == 3)
                           {
                              if(J->PosJug->atras!=NULL && J->PosJug->atras->obstaculo!=3)
                              {
                                  balaJ=DisparaJ(LB,J->PosJug->atras,J->Dir);
                                  J->bala=0;
                                  balaJ->j=J;
                              }
                           }
                       }
                  break;
             }           
         }              
}

Nodo *CargaEscenario(nodo *Principal,int nivel,Imagen obst[],ListaEnemigos *lE)
{
    Nodo *corre = Principal;
    Nodo *gia =Principal;
    Nodo *posJu;
    int dato=0;
    char cad[10];
    FILE *archivo;
    
    
    sprintf(cad,"Nivel%d.txt",nivel);
    archivo=fopen(cad,"r");
    rewind(archivo);
    while(gia!=NULL)
    {
        while(corre!=NULL)
        {
           fscanf(archivo,"%d",&dato);
           printf("%d ",dato);
           if(dato==1)
           {
            posJu=corre;    
           }
           if(dato==2)
           {
              corre->obstaculo=1;  
           }
           if(dato==3)
           {
                corre->obstaculo=2;        
           }
           if(dato==4)
           {
                corre->obstaculo=3;        
           }
           if(dato==5)
           {
                corre->obstaculo=4;  
           }
           if(dato==6)
           {
                corre->obstaculo=5;    
           }
           if(dato==7)
             InsertaEnemigo(lE,corre,0,1);
         corre = corre->adelante;                                          
        }   
        printf("\n");                                
        gia = gia->abajo;
        corre = gia;              
    }   
    return posJu;   
}

void EliminaEscenario(nodo *Principal)
{
    Nodo *corre = Principal;
    Nodo *gia =Principal;
    while(gia!=NULL)
    {
        while(corre!=NULL)
        {
            corre->obstaculo=0; 
            printf("%d ",corre->obstaculo);
            corre = corre->adelante;                               
        }   
        printf("\n");                                
        gia = gia->abajo;
        corre = gia;              
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
          for(int j=0;j<TamMayaY;j++)//adelante*abajo-arriba*atras
          {
                  
               if(i == 0 && j == 0)
               {
                    principal = creaNodo(i*TamNodo,j*TamNodo);  
                            
                    corre1 = principal;
                    gia = principal; 
               }
               
               if(j==0 && i!=0)
               {
                       
                       aux = creaNodo(i*TamNodo,j*TamNodo);
                         
                       aux->atras=gia;
                       gia->adelante = aux;
                       corre2=gia;
                       gia = aux;
                       corre1=gia;          
               } 
               if(j!=0)
               {
                  nuevo = creaNodo(i*TamNodo,j*TamNodo);
                  corre1->abajo = nuevo;
                  nuevo->arriba = corre1;
                  corre1 = nuevo;
               }
               if(i!=0)
               {
               corre1->atras = corre2;
               corre2->adelante = corre1;
               corre2=corre2->abajo;          
               }    
          }               
     }   
     return principal;
}
Nodo *creaNodo(int x1,int y1)
{
 Nodo *nuevo;
 nuevo = (Nodo*)malloc(sizeof(Nodo));  
 nuevo->arriba = NULL;
 nuevo->abajo = NULL;
 nuevo->atras = NULL;
 nuevo->adelante = NULL;
 
 nuevo->x1=x1; 
 nuevo->y1=y1;
 nuevo->x2=x1+TamNodo;
 nuevo->y2=y1+TamNodo;
 nuevo->obstaculo = 0;
 
 nuevo->enemigoNodo=NULL;
 nuevo->balaNodo=NULL;
 nuevo->jn=NULL;
 return (nuevo); 
     
}
void dibuja(Nodo *principal,Imagen obst[])
{
    Nodo *corre = principal;
    Nodo *gia =principal;
    while(gia!=NULL)
    {
        while(corre!=NULL)
        {
         if(corre->obstaculo==1)
         {
             DibujaImagen(obst[0],corre->x1,corre->y1,0);           
         }
         if(corre->obstaculo==2)
         {                                                                  
             DibujaImagen(obst[1],corre->x1,corre->y1,0);                       
         }
         if(corre->obstaculo==3)
         {                                                                  
             DibujaImagen(obst[2],corre->x1,corre->y1,0);                       
         }
         if(corre->obstaculo==4)
         {                                                                  
             DibujaImagen(obst[3],corre->x1,corre->y1,0);                       
         }
         if(corre->obstaculo==5)
         {                                                                  
             DibujaImagen(obst[4],corre->x1,corre->y1,0);                       
         }
         corre = corre->adelante;                                          
        }                                   
        gia = gia->abajo;
        corre = gia;              
    }      
}

void borraImagen(int x1,int y1)
{
     setfillstyle(SOLID_FILL,BLACK);
     bar(x1,y1,x1+TamNodo,y1+TamNodo);    
}

int leeimagen(Imagen *ima,char nombre[])
{
   Imagen *aux=&*ima;
   FILE *archivo;
   archivo=fopen(nombre,"r");
   if(archivo==NULL)
   {
      outtextxy(700,640,"No existe el archivo ");
      return 1;
   }
   else
   {
        do
        {
          fscanf(archivo, "%d ",&aux->tam);
          
          aux->MallaP=(Pixel**)malloc(sizeof(Pixel*)*(aux->tam));
          for(int i=0;i<aux->tam;i++)
          {
            aux->MallaP[i]=(Pixel*)malloc(sizeof(Pixel)*(aux->tam));
          }
          for(int i=0;i<aux->tam;i++)
          {
            for(int j=0;j<aux->tam;j++)
            {
              aux->MallaP[i][j].x=i;
              aux->MallaP[i][j].y=j;
              fscanf(archivo,"%d ",&aux->MallaP[i][j].color);
            }
          }
        }while(!feof(archivo));
   }
   fclose(archivo);
}

void DibujaImagen(Imagen ima,int x, int y,int dir)
{
    int tama=2;
    for(int i=0;i<ima.tam;i++)
    {
       for(int j=0;j<ima.tam;j++)
       {

               if(dir==0)
               {
                   setfillstyle(SOLID_FILL,(*(ima.MallaP+i)+j)->color ); 
                   bar(x+(ima.MallaP[i][j].x*tama),y+(ima.MallaP[i][j].y*tama),x+tama+(ima.MallaP[i][j].x*tama),y+tama+(ima.MallaP[i][j].y*tama));   
               }   
               if(dir==1)
               {
                   setfillstyle(SOLID_FILL,(*(ima.MallaP+i)+j)->color ); 
                   bar(x+(ima.MallaP[j][i].x*tama),y+(ima.MallaP[j][i].y*tama),x+tama+(ima.MallaP[j][i].x*tama),y+tama+(ima.MallaP[j][i].y*tama));   
               }          
       }
    }
}

Bala *CreaBalaJ(int Direccion,Nodo *Pos)
{
        Bala *nuevo;
        nuevo=(Bala *)malloc(sizeof(Bala));
        nuevo->ant=NULL;
        nuevo->sig=NULL;
        nuevo->pos=Pos;
        nuevo->Dir=Direccion;
        nuevo->num=0;
        nuevo->j=NULL;
        return (nuevo);
}

BalaE *CreaBalaE(int Direccion,Nodo *Pos)
{
        BalaE *nuevo;
        nuevo=(BalaE*)malloc(sizeof(BalaE));
        nuevo->ant=NULL;
        nuevo->sig=NULL;
        nuevo->pos=Pos;
        nuevo->Dir=Direccion;
        nuevo->num=0;
        nuevo->e=NULL;
        return (nuevo);
}

Bala *DisparaJ(ListaBalas *balas,Nodo *pos,int iDireccion)
{
      Bala *nuevoE;  
      Nodo *aux = pos;        
      nuevoE=CreaBalaJ(iDireccion,aux);
      if(nuevoE->pos->obstaculo!=4)
      {
         if(aux->obstaculo==5)
         DibujaImagen(balas->imaBal[2],nuevoE->pos->x1,nuevoE->pos->y1,0);
         else
         if(iDireccion==0){DibujaImagen(balas->imaBal[0],nuevoE->pos->x1,nuevoE->pos->y1,0);}
         if(iDireccion==1){DibujaImagen(balas->imaBal[1],nuevoE->pos->x1,nuevoE->pos->y1,1);}
         if(iDireccion==2){DibujaImagen(balas->imaBal[1],nuevoE->pos->x1,nuevoE->pos->y1,0);}
         if(iDireccion==3){DibujaImagen(balas->imaBal[0],nuevoE->pos->x1,nuevoE->pos->y1,1);}
      }
      if(nuevoE)
      {
                if(balas->prim==NULL)
                {
                      balas->prim=nuevoE;
                      balas->ult=nuevoE;
                      balas->prim->num+=balas->prim->num;
                }
                else
                {
                      nuevoE->sig=balas->prim;
                      balas->prim->ant=nuevoE;
                      balas->prim=nuevoE;
                      balas->prim->num+=balas->prim->sig->num+1;
                       
                }
      aux->balaNodo=nuevoE;
      return  nuevoE;
      
      } 
}

BalaE *DisparaE(ListaBalasE *balasE,Nodo *pos,int iDireccion)
{
      BalaE *nuevoE;  
      Nodo *aux = pos;        
      nuevoE=CreaBalaE(iDireccion,aux);
      if(nuevoE->pos->obstaculo!=4)
      {
         if(aux->obstaculo==5)
         DibujaImagen(balasE->imaBalE[2],nuevoE->pos->x1,nuevoE->pos->y1,0);
         else
         if(iDireccion==0){DibujaImagen(balasE->imaBalE[0],nuevoE->pos->x1,nuevoE->pos->y1,0);}
         if(iDireccion==1){DibujaImagen(balasE->imaBalE[1],nuevoE->pos->x1,nuevoE->pos->y1,1);}
         if(iDireccion==2){DibujaImagen(balasE->imaBalE[1],nuevoE->pos->x1,nuevoE->pos->y1,0);}
         if(iDireccion==3){DibujaImagen(balasE->imaBalE[0],nuevoE->pos->x1,nuevoE->pos->y1,1);}
      }
      if(nuevoE)
      {
                if(balasE->prim==NULL)
                {
                      balasE->prim=nuevoE;
                      balasE->ult=nuevoE;
                      balasE->prim->num+=balasE->prim->num;
                }
                else
                {
                      nuevoE->sig=balasE->prim;
                      balasE->prim->ant=nuevoE;
                      balasE->prim=nuevoE;
                      balasE->prim->num+=balasE->prim->sig->num+1;
                       
                }
      aux->balaNodoE=nuevoE;
      return  nuevoE;
      
      } 
}

void MueveBalasJ(ListaBalas *balas,ListaBalasE *balasE,ListaEnemigos *enemigos,Imagen animacion[])
{     
   Bala *aux=balas->prim;
   while(aux)
   {
             if(aux->pos->obstaculo==2)
             {
                explocion(animacion,aux->pos);
                perdiste=true;                       
             }
             else
             if(aux->pos->enemigoNodo!=NULL)
             {
              explocion(animacion,aux->pos);
              aux->j->puntos+=10;
              EliminaEnemigo(enemigos,aux->pos->enemigoNodo->num);
              borraImagen(aux->pos->x1,aux->pos->y1);
              aux->pos->enemigoNodo=NULL;
              EliminaBalaJ(balas,aux->num);
             }
            /* else
             if(aux->pos->balaNodoE!=NULL)
             {
                  explocion(animacion,aux->pos);
                  aux->j->puntos+=1;
                  borraImagen(aux->pos->x1,aux->pos->y1);
                  aux->pos->enemigoNodo=NULL;
                  EliminaBalaE(balasE,aux->pos->balaNodoE->num);
                  aux->pos->balaNodoE=NULL;
                  EliminaBalaJ(balas,aux->num);                          
             }*/
             else
              if(MueveBalaJ(aux,balas)==1)
              {
                 //borraImagen(aux->pos->x1,aux->pos->y1);
                 EliminaBalaJ(balas,aux->num);
              }
      aux=aux->sig;       
   }
}


void MueveBalasE(ListaBalasE *balasE,Nodo *posJug,Imagen animacion[])
{     
   BalaE *aux=balasE->prim;
   while(aux)
   {
             if(aux->pos->obstaculo==2)
             {
                explocion(animacion,aux->pos);
                perdiste=true;                       
             }
             else
             if(aux->pos->jn!=NULL)
             {
              explocion(animacion,aux->pos);
              borraImagen(aux->pos->x1,aux->pos->y1);
              aux->pos->jn->vida--;
              posJug->jn=aux->pos->jn;
              aux->pos->jn->PosJug=posJug;
              DibujaImagen(aux->pos->jn->imaJu[0],posJug->x1,posJug->y1,0);
              aux->pos->jn=NULL;
              EliminaBalaE(balasE,aux->num);
             }
             else
              if(MueveBalaE(aux,balasE)==1)
              {
                 //borraImagen(aux->pos->x1,aux->pos->y1);
                 EliminaBalaE(balasE,aux->num);
              }
      aux=aux->sig;       
   }
}

int MueveBalaE(BalaE *bala,ListaBalasE *balas)
{    
     if(bala->Dir==0)
     {
         if(bala->pos->arriba!=NULL && bala->pos->arriba->obstaculo!=3 && bala->pos->obstaculo!=1)
         {
               if(bala->pos->obstaculo!=4 && bala->pos->obstaculo!=5)  
               borraImagen(bala->pos->x1,bala->pos->y1);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBalE[3],bala->pos->x1,bala->pos->y1,0);
               if(bala->pos->obstaculo==1)
               bala->pos->obstaculo=0;
               bala->pos->balaNodoE=NULL; 
               bala->pos=bala->pos->arriba; 
               bala->pos->balaNodoE=bala;
               if(bala->pos->obstaculo!=4)  
               DibujaImagen(balas->imaBalE[0],bala->pos->x1,bala->pos->y1,0);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBalE[2],bala->pos->x1,bala->pos->y1,0);
         }
         else
         {
             //if(bala->pos->arriba!=NULL && bala->pos->arriba->balaNodo!=NULL)
             //EliminaBala(balas,bala->pos->arriba->balaNodo->num);
             bala->pos->balaNodoE=NULL; 
             if(bala->pos->obstaculo==1)
             bala->pos->obstaculo=0;
             if(bala->pos->obstaculo!=5 && bala->pos->obstaculo!=4)
             borraImagen(bala->pos->x1,bala->pos->y1);
             if(bala->pos->obstaculo==5)
             DibujaImagen(balas->imaBalE[3],bala->pos->x1,bala->pos->y1,0);
             if(bala->pos->obstaculo==4)
             DibujaImagen(balas->imaBalE[4],bala->pos->x1,bala->pos->y1,0);
             return 1;
         }
     }
     if(bala->Dir==1)
     {
         if(bala->pos->adelante!=NULL && bala->pos->adelante->balaNodo==NULL && bala->pos->adelante->obstaculo!=3 && bala->pos->obstaculo!=1)
         {  
               if(bala->pos->obstaculo!=4 && bala->pos->obstaculo!=5)  
               borraImagen(bala->pos->x1,bala->pos->y1);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBalE[3],bala->pos->x1,bala->pos->y1,0);
               if(bala->pos->obstaculo==1)
               bala->pos->obstaculo=0;
               bala->pos->balaNodoE=NULL; 
               bala->pos=bala->pos->adelante; 
               bala->pos->balaNodoE=bala;
               if(bala->pos->obstaculo!=4)  
               DibujaImagen(balas->imaBalE[1],bala->pos->x1,bala->pos->y1,1);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBalE[2],bala->pos->x1,bala->pos->y1,0);
         }
         else
         {
             //if(bala->pos->adelante!=NULL && bala->pos->adelante->balaNodo!=NULL)
             //EliminaBala(balas,bala->pos->adelante->balaNodo->num);
             bala->pos->balaNodoE=NULL;
             if(bala->pos->obstaculo==1)
             bala->pos->obstaculo=0;
             if(bala->pos->obstaculo!=5 && bala->pos->obstaculo!=4)
             borraImagen(bala->pos->x1,bala->pos->y1);
             if(bala->pos->obstaculo==5)
             DibujaImagen(balas->imaBalE[3],bala->pos->x1,bala->pos->y1,0);
             if(bala->pos->obstaculo==4)
             DibujaImagen(balas->imaBalE[4],bala->pos->x1,bala->pos->y1,0);
             return 1;
         }
     }
     if(bala->Dir==2)
     {
         if(bala->pos->abajo!=NULL && bala->pos->abajo->balaNodo==NULL && bala->pos->abajo->obstaculo!=3 && bala->pos->obstaculo!=1)
         { 
               if(bala->pos->obstaculo!=4 && bala->pos->obstaculo!=5)  
               borraImagen(bala->pos->x1,bala->pos->y1);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBalE[3],bala->pos->x1,bala->pos->y1,0);
               if(bala->pos->obstaculo==1)
               bala->pos->obstaculo=0;
               bala->pos->balaNodoE=NULL; 
               bala->pos=bala->pos->abajo; 
               bala->pos->balaNodoE=bala;
               if(bala->pos->obstaculo!=4)  
               DibujaImagen(balas->imaBalE[1],bala->pos->x1,bala->pos->y1,0);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBalE[2],bala->pos->x1,bala->pos->y1,0);
         }
         else
         {
             //if(bala->pos->abajo!=NULL && bala->pos->abajo->balaNodo!=NULL)
             //EliminaBala(balas,bala->pos->abajo->balaNodo->num);
             bala->pos->balaNodoE=NULL;
             if(bala->pos->obstaculo==1)
             bala->pos->obstaculo=0;
             if(bala->pos->obstaculo!=5 && bala->pos->obstaculo!=4)
             borraImagen(bala->pos->x1,bala->pos->y1);
             if(bala->pos->obstaculo==5)
             DibujaImagen(balas->imaBalE[3],bala->pos->x1,bala->pos->y1,0);
             if(bala->pos->obstaculo==4)
             DibujaImagen(balas->imaBalE[4],bala->pos->x1,bala->pos->y1,0);
             return 1;
         }
     }
     if(bala->Dir==3)
     {
         if(bala->pos->atras!=NULL && bala->pos->atras->balaNodo==NULL && bala->pos->atras->obstaculo!=3 && bala->pos->obstaculo!=1)
         { 
               if(bala->pos->obstaculo!=4 && bala->pos->obstaculo!=5)  
               borraImagen(bala->pos->x1,bala->pos->y1);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBalE[3],bala->pos->x1,bala->pos->y1,0);
               if(bala->pos->obstaculo==1)
               bala->pos->obstaculo=0;
               bala->pos->balaNodoE=NULL; 
               bala->pos=bala->pos->atras; 
               bala->pos->balaNodoE=bala;
               if(bala->pos->obstaculo!=4)  
               DibujaImagen(balas->imaBalE[0],bala->pos->x1,bala->pos->y1,1);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBalE[2],bala->pos->x1,bala->pos->y1,0);
         }
         else
         {
             //if(bala->pos->atras!=NULL && bala->pos->atras->balaNodo!=NULL)
             //EliminaBala(balas,bala->pos->atras->balaNodo->num);
             bala->pos->balaNodoE=NULL;
             if(bala->pos->obstaculo==1)
             bala->pos->obstaculo=0;
             if(bala->pos->obstaculo!=5 && bala->pos->obstaculo!=4)
             borraImagen(bala->pos->x1,bala->pos->y1);
             if(bala->pos->obstaculo==5)
             DibujaImagen(balas->imaBalE[3],bala->pos->x1,bala->pos->y1,0);
             if(bala->pos->obstaculo==4)
             DibujaImagen(balas->imaBalE[4],bala->pos->x1,bala->pos->y1,0);
             return 1;
         }
     }
}

int MueveBalaJ(Bala *bala,ListaBalas *balas)
{    
     if(bala->Dir==0)
     {
         if(bala->pos->arriba!=NULL && bala->pos->arriba->obstaculo!=3 && bala->pos->obstaculo!=1)
         {
               if(bala->pos->obstaculo!=4 && bala->pos->obstaculo!=5)  
               borraImagen(bala->pos->x1,bala->pos->y1);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBal[3],bala->pos->x1,bala->pos->y1,0);
               if(bala->pos->obstaculo==1)
               bala->pos->obstaculo=0;
               bala->pos->balaNodo=NULL; 
               bala->pos=bala->pos->arriba; 
               bala->pos->balaNodo=bala;
               if(bala->pos->obstaculo!=4)  
               DibujaImagen(balas->imaBal[0],bala->pos->x1,bala->pos->y1,0);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBal[2],bala->pos->x1,bala->pos->y1,0);
         }
         else
         {
             bala->pos->balaNodo=NULL; 
             if(bala->pos->obstaculo==1)
             bala->pos->obstaculo=0;
             if(bala->pos->obstaculo!=5 && bala->pos->obstaculo!=4)
             borraImagen(bala->pos->x1,bala->pos->y1);
             if(bala->pos->obstaculo==5)
             DibujaImagen(balas->imaBal[3],bala->pos->x1,bala->pos->y1,0);
             if(bala->pos->obstaculo==4)
             DibujaImagen(balas->imaBal[4],bala->pos->x1,bala->pos->y1,0);
             return 1;
         }
     }
     if(bala->Dir==1)
     {
         if(bala->pos->adelante!=NULL && bala->pos->adelante->obstaculo!=3 && bala->pos->obstaculo!=1)
         {  
               if(bala->pos->obstaculo!=4 && bala->pos->obstaculo!=5)  
               borraImagen(bala->pos->x1,bala->pos->y1);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBal[3],bala->pos->x1,bala->pos->y1,0);
               if(bala->pos->obstaculo==1)
               bala->pos->obstaculo=0;
               bala->pos->balaNodo=NULL; 
               bala->pos=bala->pos->adelante; 
               bala->pos->balaNodo=bala;
               if(bala->pos->obstaculo!=4)  
               DibujaImagen(balas->imaBal[1],bala->pos->x1,bala->pos->y1,1);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBal[2],bala->pos->x1,bala->pos->y1,0);
         }
         else
         {
             bala->pos->balaNodo=NULL;
             if(bala->pos->obstaculo==1)
             bala->pos->obstaculo=0;
             if(bala->pos->obstaculo!=5 && bala->pos->obstaculo!=4)
             borraImagen(bala->pos->x1,bala->pos->y1);
             if(bala->pos->obstaculo==5)
             DibujaImagen(balas->imaBal[3],bala->pos->x1,bala->pos->y1,0);
             if(bala->pos->obstaculo==4)
             DibujaImagen(balas->imaBal[4],bala->pos->x1,bala->pos->y1,0);
             return 1;
         }
     }
     if(bala->Dir==2)
     {
         if(bala->pos->abajo!=NULL && bala->pos->abajo->obstaculo!=3 && bala->pos->obstaculo!=1)
         { 
               if(bala->pos->obstaculo!=4 && bala->pos->obstaculo!=5)  
               borraImagen(bala->pos->x1,bala->pos->y1);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBal[3],bala->pos->x1,bala->pos->y1,0);
               if(bala->pos->obstaculo==1)
               bala->pos->obstaculo=0;
               bala->pos->balaNodo=NULL; 
               bala->pos=bala->pos->abajo; 
               bala->pos->balaNodo=bala;
               if(bala->pos->obstaculo!=4)  
               DibujaImagen(balas->imaBal[1],bala->pos->x1,bala->pos->y1,0);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBal[2],bala->pos->x1,bala->pos->y1,0);
         }
         else
         {
             bala->pos->balaNodo=NULL;
             if(bala->pos->obstaculo==1)
             bala->pos->obstaculo=0;
             if(bala->pos->obstaculo!=5 && bala->pos->obstaculo!=4)
             borraImagen(bala->pos->x1,bala->pos->y1);
             if(bala->pos->obstaculo==5)
             DibujaImagen(balas->imaBal[3],bala->pos->x1,bala->pos->y1,0);
             if(bala->pos->obstaculo==4)
             DibujaImagen(balas->imaBal[4],bala->pos->x1,bala->pos->y1,0);
             return 1;
         }
     }
     if(bala->Dir==3)
     {
         if(bala->pos->atras!=NULL && bala->pos->atras->obstaculo!=3 && bala->pos->obstaculo!=1)
         { 
               if(bala->pos->obstaculo!=4 && bala->pos->obstaculo!=5)  
               borraImagen(bala->pos->x1,bala->pos->y1);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBal[3],bala->pos->x1,bala->pos->y1,0);
               if(bala->pos->obstaculo==1)
               bala->pos->obstaculo=0;
               bala->pos->balaNodo=NULL; 
               bala->pos=bala->pos->atras; 
               bala->pos->balaNodo=bala;
               if(bala->pos->obstaculo!=4)  
               DibujaImagen(balas->imaBal[0],bala->pos->x1,bala->pos->y1,1);
               if(bala->pos->obstaculo==5)
               DibujaImagen(balas->imaBal[2],bala->pos->x1,bala->pos->y1,0);
         }
         else
         {
             bala->pos->balaNodo=NULL;
             if(bala->pos->obstaculo==1)
             bala->pos->obstaculo=0;
             if(bala->pos->obstaculo!=5 && bala->pos->obstaculo!=4)
             borraImagen(bala->pos->x1,bala->pos->y1);
             if(bala->pos->obstaculo==5)
             DibujaImagen(balas->imaBal[3],bala->pos->x1,bala->pos->y1,0);
             if(bala->pos->obstaculo==4)
             DibujaImagen(balas->imaBal[4],bala->pos->x1,bala->pos->y1,0);
             return 1;
         }
     }
}


void EliminaBalaJ(ListaBalas  *bal,int numero)
{ 
   Bala *aux2=bal->ult;  
   Bala *aux=bal->prim;
   
   if(bal->prim==bal->ult)
   {
        bal->prim->pos->balaNodo==NULL;
        
        if(bal->prim->j!=NULL)
        {
          bal->prim->j->bala=1;
          bal->prim->j=NULL;
        }
        free(bal->prim);
        bal->prim=bal->ult=NULL;
   }else 
   if(bal->ult->num==numero)
   {                                 
        bal->ult=bal->ult->ant;
        bal->ult->sig=NULL;
        aux2->pos->balaNodo=NULL;
        if(aux2->j!=NULL)
        {
          aux2->j->bala=1;
          aux2->j=NULL;
        }
        free(aux2);         
   }else 
   if(bal->prim->num==numero)
   {       
        bal->prim=bal->prim->sig;
        bal->prim->ant=NULL;
        aux->pos->balaNodo=NULL;
        if(aux->j!=NULL)
        {
          aux->j->bala=1;
          aux->j=NULL;
        }
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
                aux->pos->balaNodo=NULL;
                if(aux->j!=NULL)
                {
                  aux->j->bala=1;
                  aux->j=NULL;
                } 
                free(aux);
            }
   }                                
}

void MueveEnemigos(ListaEnemigos *lE,ListaBalasE *balas)
{     
   Enemigo *aux=lE->prim;
   
   while(aux)
   { 
              MueveEnemigo(aux,balas,*lE,rand()%4);
               
      aux=aux->sig;       
   }
}

void MueveEnemigo(Enemigo *E,ListaBalasE *LB,ListaEnemigos lE,int movimiento)
{
     if(E->balaE==1)
      {
        if(rand()%80==1)
        {
           balaE *balaE=NULL;
           if(E->direccion == 0)
           {
              if(E->pos->arriba!=NULL && E->pos->arriba->obstaculo!=3)
              {
                  balaE=DisparaE(LB,E->pos->arriba,E->direccion);
                  E->balaE=0;
                  balaE->e=E;  
              }
           }
           if(E->direccion == 1)
           {
              if(E->pos->adelante!=NULL && E->pos->adelante->obstaculo!=3)
              {
                  balaE=DisparaE(LB,E->pos->adelante,E->direccion);
                  E->balaE=0;
                  balaE->e=E;
              }
           }
           if(E->direccion == 2)
           {
              if(E->pos->abajo!=NULL && E->pos->abajo->obstaculo!=3)
              {
                  balaE=DisparaE(LB,E->pos->abajo,E->direccion);
                  E->balaE=0;
                  balaE->e=E;
              }
           }
           if(E->direccion == 3)
           {
              if(E->pos->atras!=NULL && E->pos->atras->obstaculo!=3)
              {
                  balaE=DisparaE(LB,E->pos->atras,E->direccion);
                  E->balaE=0;
                  balaE->e=E;
              }
           }
       }
   }
   
   
    if(movimiento==0)
    {
        if(E->pasos==4+rand()%2)
        {
            E->pasos=0;
            buscaDireccion(E,lE);
        }
        else
        {
            E->pasos++;
            switch(E->direccion)
            {
                   case 0:
                      if(E->pos->arriba!=NULL && E->pos->arriba->jn==NULL && E->pos->arriba->enemigoNodo==NULL && E->pos->arriba->obstaculo!=1 && E->pos->arriba->obstaculo!=2 && E->pos->arriba->obstaculo!=3)
                      {
                      if(E->pos->arriba->obstaculo==0 && E->pos->obstaculo==0)
                      {
                        borraImagen(E->pos->x1,E->pos->y1);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->arriba;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[0],E->pos->x1,E->pos->y1,0);
                      }
                      else
                      if(E->pos->arriba->obstaculo==4 && E->pos->obstaculo==0)
                      {
                        borraImagen(E->pos->x1,E->pos->y1);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->arriba;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[2],E->pos->x1,E->pos->y1,0);
                      }
                      else
                      if(E->pos->obstaculo==4 && E->pos->arriba->obstaculo==0)
                      {
                        
                        DibujaImagen(lE.imaEne[3],E->pos->x1,E->pos->y1,0);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->arriba;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[0],E->pos->x1,E->pos->y1,0);
                      }
                      else
                      if(E->pos->obstaculo==4 && E->pos->arriba->obstaculo!=1 && E->pos->arriba->obstaculo!=3)
                      {
                        DibujaImagen(lE.imaEne[3],E->pos->x1,E->pos->y1,0);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->arriba;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[2],E->pos->x1,E->pos->y1,0);
                      }
                   }
                   else
                   {
                       buscaDireccion(E,lE);
                   }
                   break;   
                   case 1:
                   if(E->pos->adelante!=NULL && E->pos->adelante->jn==NULL && E->pos->adelante->enemigoNodo==NULL && E->pos->adelante->obstaculo!=1 && E->pos->adelante->obstaculo!=2 && E->pos->adelante->obstaculo!=3)
                   {  
                      if(E->pos->adelante->obstaculo==0 && E->pos->obstaculo==0)
                      {
                        borraImagen(E->pos->x1,E->pos->y1);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->adelante;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,1);
                      }
                      else
                      if(E->pos->adelante->obstaculo==4 && E->pos->obstaculo==0)
                      {
                        borraImagen(E->pos->x1,E->pos->y1);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->adelante;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[2],E->pos->x1,E->pos->y1,0);
                      }
                      else
                      if(E->pos->obstaculo==4 && E->pos->adelante->obstaculo==0)
                      {
                        DibujaImagen(lE.imaEne[3],E->pos->x1,E->pos->y1,0);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->adelante;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,1);
                      }
                      else
                      if(E->pos->obstaculo==4 && E->pos->adelante->obstaculo!=1 && E->pos->adelante->obstaculo!=3)
                      {
                        DibujaImagen(lE.imaEne[3],E->pos->x1,E->pos->y1,0);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->adelante;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[2],E->pos->x1,E->pos->y1,0);
                      }
                   }
                   else
                   {
                        buscaDireccion(E,lE);
                   }
                   break;
                   case 2:
                   if(E->pos->abajo!=NULL && E->pos->abajo->jn==NULL && E->pos->abajo->enemigoNodo==NULL &&  E->pos->abajo->obstaculo!=1 && E->pos->abajo->obstaculo!=2 && E->pos->abajo->obstaculo!=3)
                   {
                      if(E->pos->abajo->obstaculo==0 && E->pos->obstaculo==0)
                      {
                        borraImagen(E->pos->x1,E->pos->y1);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->abajo;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,0);
                      }
                      else
                      if(E->pos->abajo->obstaculo==4 && E->pos->obstaculo==0)
                      {
                        borraImagen(E->pos->x1,E->pos->y1);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->abajo;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[2],E->pos->x1,E->pos->y1,0);
                      }
                      else
                      if(E->pos->obstaculo==4 && E->pos->abajo->obstaculo==0)
                      {
                        DibujaImagen(lE.imaEne[3],E->pos->x1,E->pos->y1,0);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->abajo;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,0);
                      }
                      else
                      if(E->pos->obstaculo==4 && E->pos->abajo->obstaculo!=1 && E->pos->abajo->obstaculo!=3)
                      {
                        DibujaImagen(lE.imaEne[3],E->pos->x1,E->pos->y1,0);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->abajo;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[2],E->pos->x1,E->pos->y1,0);
                      }
                   }
                   else
                   {
                       buscaDireccion(E,lE);
                   }
                   break; 
                   case 3:
                   if(E->pos->atras!=NULL && E->pos->atras->jn==NULL  && E->pos->atras->enemigoNodo==NULL && E->pos->atras->obstaculo!=1 && E->pos->atras->obstaculo!=2 && E->pos->atras->obstaculo!=3)
                   {
                      if(E->pos->atras->obstaculo==0 && E->pos->obstaculo==0)
                      {
                        borraImagen(E->pos->x1,E->pos->y1);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->atras;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[0],E->pos->x1,E->pos->y1,1);
                      }
                      else
                      if(E->pos->atras->obstaculo==4 && E->pos->obstaculo==0)
                      {
                        borraImagen(E->pos->x1,E->pos->y1);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->atras;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[2],E->pos->x1,E->pos->y1,0);
                      }
                      else
                      if(E->pos->obstaculo==4 && E->pos->atras->obstaculo==0)
                      {
                        DibujaImagen(lE.imaEne[3],E->pos->x1,E->pos->y1,0);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->atras;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[0],E->pos->x1,E->pos->y1,1);
                      }
                      else
                      if(E->pos->obstaculo==4 && E->pos->atras->obstaculo!=1  && E->pos->atras->obstaculo!=3)
                      {
                        DibujaImagen(lE.imaEne[3],E->pos->x1,E->pos->y1,0);
                        E->pos->enemigoNodo=NULL;
                        E->pos=E->pos->atras;
                        E->pos->enemigoNodo=E;
                        DibujaImagen(lE.imaEne[2],E->pos->x1,E->pos->y1,0);
                      }
                   }
                   else
                   {
                        buscaDireccion(E,lE);
                   }
                   break;                
            }
        }
    }
}

void buscaDireccion(Enemigo *E,ListaEnemigos lE)
{
   switch(rand()%4)
   {
          case 0:
               if(E->pos->arriba!=NULL && E->pos->arriba->jn==NULL  && E->pos->arriba->enemigoNodo==NULL && E->pos->arriba->obstaculo!=1 && E->pos->arriba->obstaculo!=2 && E->pos->arriba->obstaculo!=3)
               {
                   E->direccion=0;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[0],E->pos->x1,E->pos->y1,0);
               }
               else
               if(E->pos->adelante!=NULL && E->pos->adelante->jn==NULL && E->pos->adelante->enemigoNodo==NULL && E->pos->adelante->obstaculo!=1 && E->pos->adelante->obstaculo!=2 && E->pos->adelante->obstaculo!=3)
               {
                   E->direccion=1;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,1);
               }
               else
               if(E->pos->abajo!=NULL && E->pos->abajo->jn==NULL && E->pos->abajo->enemigoNodo==NULL &&  E->pos->abajo->obstaculo!=1 && E->pos->abajo->obstaculo!=2 && E->pos->abajo->obstaculo!=3)
               {
                   E->direccion=2;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,1);
               }
               else
               if(E->pos->atras!=NULL && E->pos->atras->jn==NULL && E->pos->atras->enemigoNodo==NULL && E->pos->atras->obstaculo!=1 && E->pos->atras->obstaculo!=2 && E->pos->atras->obstaculo!=3)
               {
                   E->direccion=3;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,0);
               }
          break;
          case 1:
               if(E->pos->atras!=NULL && E->pos->atras->jn==NULL && E->pos->atras->enemigoNodo==NULL && E->pos->atras->obstaculo!=1 && E->pos->atras->obstaculo!=2 && E->pos->atras->obstaculo!=3)
               {
                   E->direccion=3;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,0);
               }else
               if(E->pos->abajo!=NULL && E->pos->abajo->jn==NULL && E->pos->abajo->enemigoNodo==NULL &&  E->pos->abajo->obstaculo!=1 && E->pos->abajo->obstaculo!=2 && E->pos->abajo->obstaculo!=3)
               {
                   E->direccion=2;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,1);
               }else
               if(E->pos->adelante!=NULL && E->pos->adelante->jn==NULL && E->pos->adelante->enemigoNodo==NULL && E->pos->adelante->obstaculo!=1 && E->pos->adelante->obstaculo!=2 && E->pos->adelante->obstaculo!=3)
               {
                   E->direccion=1;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,1);
               }else
               if(E->pos->arriba!=NULL && E->pos->arriba->jn==NULL && E->pos->arriba->enemigoNodo==NULL && E->pos->arriba->obstaculo!=1 && E->pos->arriba->obstaculo!=2 && E->pos->arriba->obstaculo!=3)
               {
                   E->direccion=0;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[0],E->pos->x1,E->pos->y1,0);
               }  
          break;
          case 2: if(E->pos->adelante!=NULL && E->pos->adelante->jn==NULL && E->pos->adelante->enemigoNodo==NULL && E->pos->adelante->obstaculo!=1 && E->pos->adelante->obstaculo!=2 && E->pos->adelante->obstaculo!=3)
               {
                   E->direccion=1;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,1);
               }
               else
               if(E->pos->arriba!=NULL && E->pos->arriba->jn==NULL && E->pos->arriba->enemigoNodo==NULL && E->pos->arriba->obstaculo!=1 && E->pos->arriba->obstaculo!=2 && E->pos->arriba->obstaculo!=3)
               {
                   E->direccion=0;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[0],E->pos->x1,E->pos->y1,0);
               }
               else
               if(E->pos->atras!=NULL && E->pos->atras->jn==NULL && E->pos->atras->enemigoNodo==NULL && E->pos->atras->obstaculo!=1 && E->pos->atras->obstaculo!=2 && E->pos->atras->obstaculo!=3)
               {
                   E->direccion=3;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,0);
               }
              else
               if(E->pos->abajo!=NULL && E->pos->abajo->jn==NULL && E->pos->abajo->enemigoNodo==NULL &&  E->pos->abajo->obstaculo!=1 && E->pos->abajo->obstaculo!=2 && E->pos->abajo->obstaculo!=3)
               {
                   E->direccion=2;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,1);
               }
               
               
          break;
          case 3: if(E->pos->abajo!=NULL && E->pos->abajo->jn==NULL && E->pos->abajo->enemigoNodo==NULL &&  E->pos->abajo->obstaculo!=1 && E->pos->abajo->obstaculo!=2 && E->pos->abajo->obstaculo!=3)
               {
                   E->direccion=2;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,1);
               }
               else
               if(E->pos->arriba!=NULL && E->pos->arriba->jn==NULL && E->pos->arriba->enemigoNodo==NULL && E->pos->arriba->obstaculo!=1 && E->pos->arriba->obstaculo!=2 && E->pos->arriba->obstaculo!=3)
               {
                   E->direccion=0;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[0],E->pos->x1,E->pos->y1,0);
               }else
              
               if(E->pos->atras!=NULL && E->pos->atras->jn==NULL && E->pos->atras->enemigoNodo==NULL && E->pos->atras->obstaculo!=1 && E->pos->atras->obstaculo!=2 && E->pos->atras->obstaculo!=3)
               {
                   E->direccion=3;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,0);
               }
               else
               if(E->pos->adelante!=NULL && E->pos->adelante->jn==NULL && E->pos->adelante->enemigoNodo==NULL && E->pos->adelante->obstaculo!=1 && E->pos->adelante->obstaculo!=2 && E->pos->adelante->obstaculo!=3)
               {
                   E->direccion=1;
                   if(E->pos->obstaculo==0)
                   DibujaImagen(lE.imaEne[1],E->pos->x1,E->pos->y1,1);
               }
               
          break; 
   }
   
}

void EliminaBalaE(ListaBalasE  *balE,int numero)
{ 
   BalaE *aux2=balE->ult;  
   BalaE *aux=balE->prim;
   
   if(balE->prim==balE->ult)
   {
        balE->prim->pos->balaNodoE==NULL;
        
        if(balE->prim->e!=NULL)
        {
          balE->prim->e->balaE=1;
          balE->prim->e=NULL;
        }
        free(balE->prim);
        balE->prim=balE->ult=NULL;
   }else 
   if(balE->ult->num==numero)
   {                                 
        balE->ult=balE->ult->ant;
        balE->ult->sig=NULL;
        aux2->pos->balaNodoE=NULL;
        if(aux2->e!=NULL)
        {
          aux2->e->balaE=1;
          aux2->e=NULL;
        }
        free(aux2);         
   }else 
   if(balE->prim->num==numero)
   {       
        balE->prim=balE->prim->sig;
        balE->prim->ant=NULL;
        aux->pos->balaNodoE=NULL;
        if(aux->e!=NULL)
        {
          aux->e->balaE=1;
          aux->e=NULL;
        }
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
                aux->pos->balaNodoE=NULL;
                if(aux->e!=NULL)
                {
                  aux->e->balaE=1;
                  aux->e=NULL;
                } 
                free(aux);
            }
   }                                
}

void EliminaEnemigo(ListaEnemigos  *ene,int numero)
{ 
   Enemigo *aux2=ene->ult;  
   Enemigo *aux=ene->prim;
   
   if(ene->prim==ene->ult)
   {
        ene->prim->pos->enemigoNodo==NULL;
        free(ene->prim);
        ene->prim=ene->ult=NULL;
   }
   else 
   if(ene->ult->num==numero)
   {                                 
        ene->ult=ene->ult->ant;
        ene->ult->sig=NULL;
        aux2->pos->enemigoNodo=NULL;
        free(aux2);         
   }
   else 
   if(ene->prim->num==numero)
   {       
        ene->prim=ene->prim->sig;
        ene->prim->ant=NULL;
        aux->pos->enemigoNodo=NULL;
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
                aux->pos->enemigoNodo=NULL;
                free(aux);
            }
   }                                
}


char **lee_ayuda(int *cont)
{
     char **A;
     int x,i;
     char cad[80];
     *cont = 0;
     FILE *fp;
     fp = fopen("ayuda.txt","r");
     
     fgets(cad,30,fp);
     while(!feof(fp))
     {
                     (*cont)++;
                     fgets(cad,30,fp);
     }
     A = (char**)malloc(sizeof(char*)*(*cont));
     rewind(fp);
     
     for(i=0;i<*cont;i++)
     {
                         fgets(cad,80,fp);
                         x = strlen(cad);
                         *(A+i) = (char*)malloc(sizeof(char)*x+1);
                         strcpy(*(A+i),cad);
                         *(*(A+i)+(x-1))='\0';                        
     }
     fclose(fp);
     return(A);
}

void texto(int x,int y, char nombre[])
{
        cleardevice();
        outtextxy(x-100,y,"Nombre");
        char tecla;     
        int contador=0;
        nombre[0]='\0';
        do{
      tecla=getch();
      if(tecla==0)  
         tecla=getch();
        if(tecla==8 && contador > 0) 
			{
				 setcolor(BLACK);
				 outtextxy(x+100,y,nombre); 
				 nombre[--contador]='\0'; 
				 setcolor(WHITE);
				 outtextxy(x+100,y,nombre);
			}
        else
			{  
              if(tecla!=13)
              {  
				nombre[contador++]=tecla;
				 nombre[contador]='\0';
				 outtextxy(x+100,y,nombre);
              }
            } 
            
  }while(tecla!=13 && contador!=6); 
  cleardevice();
}


void IniciaRecords(Record tanque)
{
      FILE *pt;
      Record p[5];
       pt=fopen("Record.bc","wb");
       strcpy(p[0].nombre,tanque.nombre);
       p[0].puntos=tanque.puntos;
       for(int i=1;i<5;i++)
       {
       strcpy(p[i].nombre,"##################");
       p[i].puntos=0;
       }
       fwrite(p,sizeof(Record),5,pt);          
}

void dibujaNombreJuego(Imagen obst[])
{
    DibujaImagen(obst[0],100,50,0);
    DibujaImagen(obst[0],100,90,0);
    DibujaImagen(obst[0],100,90+40*1,0);
    DibujaImagen(obst[0],100,90+40*2,0);
    DibujaImagen(obst[0],100,90+40*3,0);
    DibujaImagen(obst[0],140,90+40*3,0);
    DibujaImagen(obst[0],100+40*2,90+40*2,0);
    DibujaImagen(obst[0],140,90+40*1,0);
    DibujaImagen(obst[0],100+40*2,90,0);
    DibujaImagen(obst[0],140,50,0);
    DibujaImagen(obst[0],100+40*4,90+40*1,0);
    DibujaImagen(obst[0],100+40*4,90+40*2,0);
    DibujaImagen(obst[0],100+40*4,90+40*3,0);
    DibujaImagen(obst[0],100+40*5,90+40*1,0);
    DibujaImagen(obst[0],100+40*4,90,0);
    DibujaImagen(obst[0],100+40*5,50,0);
    DibujaImagen(obst[0],100+40*6,90+40*1,0);
    DibujaImagen(obst[0],100+40*6,90+40*2,0);
    DibujaImagen(obst[0],100+40*6,90+40*3,0);
    DibujaImagen(obst[0],100+40*6,90,0);
    DibujaImagen(obst[0],180+40*6,50,0);
    DibujaImagen(obst[0],180+40*7,50,0);
    DibujaImagen(obst[0],180+40*8,50,0);
    DibujaImagen(obst[0],180+40*7,90,0);
    DibujaImagen(obst[0],180+40*7,90+40*1,0);
    DibujaImagen(obst[0],180+40*7,90+40*2,0);
    DibujaImagen(obst[0],180+40*7,90+40*3,0);
    DibujaImagen(obst[0],180+40*10,50,0);
    DibujaImagen(obst[0],180+40*11,50,0);
    DibujaImagen(obst[0],180+40*12,50,0);
    DibujaImagen(obst[0],180+40*11,90,0);
    DibujaImagen(obst[0],180+40*11,90+40*1,0);
    DibujaImagen(obst[0],180+40*11,90+40*2,0);
    DibujaImagen(obst[0],180+40*11,90+40*3,0);
    DibujaImagen(obst[0],180+40*14,50,0);
    DibujaImagen(obst[0],180+40*14,90,0);
    DibujaImagen(obst[0],180+40*14,90+40*1,0);
    DibujaImagen(obst[0],180+40*14,90+40*2,0);
    DibujaImagen(obst[0],180+40*14,90+40*3,0);
    DibujaImagen(obst[0],180+40*15,90+40*3,0);   
    DibujaImagen(obst[0],180+40*17,50,0);
    DibujaImagen(obst[0],180+40*18,50,0);
    DibujaImagen(obst[0],180+40*17,90,0);
    DibujaImagen(obst[0],180+40*17,90+40*1,0);
    DibujaImagen(obst[0],180+40*18,90+40*1,0);
    DibujaImagen(obst[0],180+40*17,90+40*2,0);
    DibujaImagen(obst[0],180+40*17,90+40*3,0);
    DibujaImagen(obst[0],180+40*18,90+40*3,0);
    settextstyle(4, 0, 7);
    setcolor(4);
    outtextxy(300,300,"CITY");
}

void InsertarRecord(Record tanque)
{
     FILE *pt;
     Record p[5],aux[5];
     
     pt=fopen("Record.bc","rb+");
     if(pt==NULL)
     {
       IniciaRecords(tanque);
     }
     else
     {
         fread(p,sizeof(Record),5,pt);
         fseek(pt,0,SEEK_SET);
         if(tanque.puntos > p[0].puntos)
         {
         strcpy(aux[0].nombre,tanque.nombre);
         aux[0].puntos=tanque.puntos;
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
             }while(p[i].puntos > tanque.puntos);
         if(i!=5)
         {
         strcpy(aux[i].nombre,tanque.nombre);
         aux[i].puntos=tanque.puntos;
         
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

void vidas_jugador(Jugador J)
{
    if(J.vida==1) 
    readimagefile("1.jpg",920,408,920+30,408+30); 
    if(J.vida==2) 
    readimagefile("2.jpg",920,408,920+30,408+30); 
    if(J.vida==3) 
    readimagefile("3.jpg",920,408,920+30,408+30);         
}

void creditos(Jugador J,Imagen obst[])
{     
     int tecla;
     
     
     setcolor(15);
     settextstyle(4, 0, 3);
     outtextxy(200,50+70*1,"Universidad Autonoma de San Luis Potosi");
     outtextxy(290,50+70*2,"Computacion e Informatica");
     outtextxy(390,50+70*3,"Profesor");
     outtextxy(400,50+70*4,"Alumno");
     outtextxy(390,50+70*5,"Battle City");
     outtextxy(390,50+70*6,"00/00/2015");
     
     for(int i=0;i<26;i++)
     {
             if(kbhit()){tecla=13;i=28;}
         DibujaImagen(obst[0],5+40*i,0,0);
     }
     for(int i=0;i<15;i++)
     {
             if(kbhit()){tecla=13;i=28;}
         DibujaImagen(obst[0],5+40*25,40+40*i,0);
     }
     for(int i=0;i<15;i++)
     {
             if(kbhit()){tecla=13;i=28;}
         DibujaImagen(obst[0],5,40+40*i,0); 
     }
     for(int i=0;i<26;i++)
     {
             if(kbhit()){tecla=13;i=28;}
         DibujaImagen(obst[0],5+40*i,630,0);  
     }
     
      
     if(tecla!=13);
     do
     {
               tecla=getch();                         
     }while(tecla!=13);
     cleardevice();
}
