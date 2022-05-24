#include<stdio.h>
#include<graphics.h>
#include<string.h>
#include<time.h>
#define WIDTH 610
#define HEIGHT 465

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

typedef struct nodo
{
      struct nodo *adelante;
      struct nodo *atras;
      struct nodo *arriba;
      struct nodo *abajo;
      struct fru *fruta;
      struct jug *jugador;
      struct ene *enemigo;
      int x1,y1,x2,y2;
}Nodo;

typedef struct fru
{
    struct fru *ant;
    struct fru *sig;
    int tipo;
    Nodo *pos;
}Fruta;

typedef struct l
{
    Fruta *prim;
    Fruta *ult;
    Imagen imagen[5];
}Frutas;

typedef struct ene
{
    Imagen imagen[4];
    int anima;
    int direccion;
    Nodo *pos;
}Enemigo;

typedef struct jug
{
      Imagen imagen[5];
      int anima;
      int puntos;
      int vidas;
      Nodo *pos;
}Jugador;

typedef struct
{
   char nombre[50];
   int puntos;
}Trecord;

void DibujaImagen(Imagen ima,int x, int y,int dir);
int leeimagen(Imagen *ima,char nombre[]);
void iniGraficos();
Nodo *creaNodo(int x1,int y1,int TamNodo);
Nodo *creaMaya();
void iniciaImagenesJugador(Jugador *j);
void creaJugador(Jugador *j,Nodo *prim);
void mueveJugador(Jugador *j);
Nodo *p = NULL;
Nodo *u = NULL;//VARIABLES GLOBALES PARA BUSCAR LAS POSICIONES DE EL PAJARO
Nodo *BuscaPosicionesE(Nodo *cab,int pos);
Nodo *BuscaPosicionesF(Nodo *cab,int pos);
Nodo *BuscaPosicionesJ(Nodo *cab,int pos);
void creaFrutas(Frutas *f);
Fruta *creaFruta(Nodo *pos,int tipo);
//ELIMINA UNA FRUTA DE LA LISTA
void eliminaFruta(Frutas *frutas,Fruta *fruta);
//AGREGA UNA FRUTA A LA LISTA
void dejaCaerFruta(Frutas *frutas,Nodo *cab);
void mueveFruta(Fruta *f,Imagen ima);
void mueveFrutas(Frutas *frutas);
void creaEnem(Enemigo *enem,Nodo *pos);
void mueveEnemigo(Enemigo *enem);
void condiciones(Frutas *frutas,Jugador *j,int tipoFruta);
void dibuja(Nodo *prim,Imagen fruta,int puntos,int tiempo,int vidas);
void juego(Enemigo enem,Jugador jug,Frutas F);
void inicializarecord();
void imprimirrecord();
void actualizarecord(char jug[],int pts);
int intextxy(int x,int y,char scadena[],int itamcad);
int Menu();
void LeeAyuda();

int main()
{
    iniGraficos();
    srand((unsigned) time(NULL));
    Menu();
    closegraph();
}
int Menu()
{
    char cual;
    int a, b,Color;
    setcolor(WHITE);
    cleardevice();
    do
    {          setcolor(BLUE);
                 settextstyle (10,0,7);
                 outtextxy(10,10,"Frutas");
                 settextstyle (10,0,1);
                 outtextxy(10,100,"Presiona una la tecla de las siguientes opciones");
                 settextstyle(10,0,3);
                 rectangle (220,200,350,240); //Rectangulo de jugar
                 outtextxy(225,205,"1. Jugar");
                 rectangle (220,200+50,350,240+50);  // Rectangulo de record
                 outtextxy(225,200+55,"2. Records");
                 rectangle (220,200+100,350,240+100); // Rectangulo de ayuda
                 outtextxy(225,200+105,"3. Ayuda");
                 rectangle (220,200+150,350,240+150);; //Rectangulo de salida
                 outtextxy(225,200+155,"4. Salida");
                 fflush (stdin);
                       cual=getch();
    cleardevice();
    switch (cual)
    {
           cleardevice();
           case '1':    p = creaMaya();
                        Enemigo P;
                        Jugador J;
                        Frutas F;

                        creaFrutas(&F);
                        creaEnem(&P,p);
                        creaJugador(&J,p);
                        juego(P,J,F);
                     break;
           case '2': imprimirrecord();
                     break;
           case '3': LeeAyuda();
                     break;
                     }
    }while (cual != '4');
}
void condiciones(Frutas *frutas,Jugador *j,int tipoFruta)
{
    Fruta *fruta = frutas->prim;
    while(fruta)
    {
        if(fruta->pos->abajo==NULL)
        {
            fruta->pos->fruta = NULL;
            eliminaFruta(frutas,fruta);
            return;
        }else
        if(fruta->pos->abajo->jugador!=NULL)
        {
            if(tipoFruta==fruta->tipo)
                j->puntos +=1;
            else
                j->vidas -=1;
            fruta->pos->fruta = NULL;
            eliminaFruta(frutas,fruta);
            return;
        }else
        if(fruta->pos->enemigo != NULL)
        {
            fruta->pos->fruta = NULL;
            eliminaFruta(frutas,fruta);
            return;
        }
        fruta = fruta->sig;
    }
}
void dibuja(Nodo *prim,Imagen fruta,int puntos,int tiempo,int vidas)
{
    setfillstyle(SOLID_FILL,9);
    settextstyle(8, 0, 2);
    bar(0,0,WIDTH, HEIGHT);
    setfillstyle(SOLID_FILL,7);
    bar(0,0,WIDTH, 32);
    rectangle(0,0,WIDTH, 32);
    rectangle(0+270,0,31+270,31);
    DibujaImagen(fruta,1+270,1,0);

    char SEGUNDOS[3],MINUTOS[3];
    sprintf(MINUTOS,"%.2d:",tiempo/60);
    sprintf(SEGUNDOS,"%.2d",tiempo%60);
    outtextxy(WIDTH-107,5,MINUTOS);

    char PUNTOS[2];
    sprintf(PUNTOS,"%d",puntos);
    outtextxy(107,5,PUNTOS);
    outtextxy(20,5,"Puntos:");

    char VIDAS[2];
    sprintf(VIDAS,"%d",vidas);
    outtextxy(107+300,5,VIDAS);
    outtextxy(20+300,5,"Vidas:");

    Nodo *aux1=prim;
    Nodo *aux2=prim;
    int i =0;
    while(aux1)
    {
        i++;aux1 = aux1->abajo;
        if(i == 14)
            break;
    }
    aux2 = aux1;
    while(aux2)
    {
        if(aux2->jugador != NULL)
            DibujaImagen(aux2->jugador->imagen[aux2->jugador->anima],aux2->x1,aux2->y1,0);
        aux2 = aux2->adelante;
    }
}
void juego(Enemigo enem,Jugador jug,Frutas F)
{
    int tipoFruta = rand()%4;
    int pagina = 0;
    int cont = 0;
    int tiempo = 0;
    time_t tIni,tActual=50;
    tIni=time(NULL);
    do{
        setactivepage(pagina);
        cleardevice();
        tActual=time(NULL);
        delay(130);
        cont += 1;
        if(cont == 200)
        {
            tipoFruta = rand()%4;
            cont =0;
        }
        tActual=time(NULL);
        tiempo = tActual-tIni;
        dibuja(p,F.imagen[tipoFruta],jug.puntos,tiempo,jug.vidas);
        mueveEnemigo(&enem);
        mueveFrutas(&F);
        condiciones(&F,&jug,tipoFruta);
        mueveJugador(&jug);
        if(rand()%4==1)
        {
            dejaCaerFruta(&F,p);
        }
        setvisualpage(pagina);
        if(pagina==0)
        pagina=1;
        else
        pagina=0;

    }while(jug.vidas != 0);
    Trecord t;
    cleardevice();
    intextxy(10,10,t.nombre,5);
    actualizarecord(t.nombre,jug.puntos);
    cleardevice();
    imprimirrecord();
    getch();
}
void iniGraficos()
{
    initwindow(WIDTH, HEIGHT, "Frutas");
    cleardevice();
}
Nodo *creaMaya()
{
     Nodo *corre1=NULL;
     Nodo *corre2=NULL;
     Nodo *principal=NULL;
     Nodo *gia=NULL;
     Nodo *aux=NULL;
     Nodo *nuevo=NULL;
     int TamNodo = 30;
     for(int i=0;i<20;i++)
     {
          for(int j=0;j<16;j++)
          {
               if(i == 0 && j == 0)
               {
                    principal = creaNodo(i*TamNodo,j*TamNodo,TamNodo);

                    corre1 = principal;
                    gia = principal;
               }

               if(j==0 && i!=0)
               {
                       aux = creaNodo(i*TamNodo,j*TamNodo,TamNodo);
                       if(i==19)
                        u = aux;
                       aux->atras=gia;
                       gia->adelante = aux;
                       corre2=gia;
                       gia = aux;
                       corre1=gia;
               }
               if(j!=0)
               {
                  nuevo = creaNodo(i*TamNodo,j*TamNodo,TamNodo);
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
Nodo *creaNodo(int x1,int y1,int TamNodo)
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

    nuevo->enemigo = NULL;
    nuevo->jugador = NULL;
    nuevo->fruta = NULL;
    return (nuevo);
}
void creaJugador(Jugador *j,Nodo *prim)
{
    j->pos = BuscaPosicionesJ(prim,289);
    j->anima = 0;
    j->puntos = 0;
    j->vidas = 5;
    prim->jugador = j;
    leeimagen(&j->imagen[0],"C1.bet");
    leeimagen(&j->imagen[1],"C2.bet");
    leeimagen(&j->imagen[2],"C3.bet");
    leeimagen(&j->imagen[3],"C4.bet");
}
void mueveJugador(Jugador *j)
{
    if(kbhit())
    {
        int cp = getch();
        switch(cp)
        {
            case 77:
                if(j->pos->adelante->adelante!=NULL)
                {
                    j->pos->jugador = NULL;
                    j->pos = j->pos->adelante;
                    j->pos->jugador = j;
                    DibujaImagen(j->imagen[j->anima],j->pos->x1,j->pos->y1,0);
                }
            break;
            case 75:
                if(j->pos->atras->atras!=NULL)
                {
                    j->pos->jugador = NULL;
                    j->pos = j->pos->atras;
                    j->pos->jugador = j;
                    DibujaImagen(j->imagen[j->anima],j->pos->x1,j->pos->y1,0);
                }
            break;
        }
    }
    if(j->puntos>5 && j->puntos<9)
        j->anima = 1;
    if(j->puntos>10&& j->puntos<14)
        j->anima = 2;
    if(j->puntos>15)
        j->anima = 3;
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
    if(dir == 0)
    for(int i=0;i<ima.tam;i++)
    {
       for(int j=0;j<ima.tam;j++)
       {
                   setfillstyle(SOLID_FILL,(*(ima.MallaP+i)+j)->color );
                   bar(x+(ima.MallaP[i][j].x*tama),y+(ima.MallaP[i][j].y*tama),x+tama+(ima.MallaP[i][j].x*tama),y+tama+(ima.MallaP[i][j].y*tama));
       }
    }
    int jj,ii;
    jj = ii =0;
    if(dir == 1)
    for(int i=ima.tam-1;i>0;i--)
    {
        jj =0;
        for(int j=0;j<ima.tam;j++)
        {
            setfillstyle(SOLID_FILL,(*(ima.MallaP+i)+j)->color );
            bar(x+(ii*tama),y+(jj*tama),x+tama+(ii*tama),y+tama+(jj*tama));
            jj++;
        }
        ii++;
    }
}
Nodo *BuscaPosicionesE(Nodo *cab,int pos)
{
    Nodo *aux = cab;
    int posAc = 0;
    while(aux)
    {
        if(posAc == pos)
            return aux;
        aux = aux->abajo;
        posAc += 1;
    }
    return NULL;
}
Nodo *BuscaPosicionesF(Nodo *cab,int pos)
{
    Nodo *aux = cab;
    int posAc = 0;
    while(aux)
    {
        if(posAc == pos)
            return aux;
        aux = aux->adelante;
        posAc += 1;
    }
    return NULL;
}
Nodo *BuscaPosicionesJ(Nodo *cab,int pos)
{
    Nodo *aux1 = cab;
    Nodo *aux2 = cab;
    int posAc = 0;
    while(aux1)
    {
        while(aux2)
        {
            if(posAc == pos)
                return aux2;
            aux2 = aux2->adelante;
            posAc += 1;
        }
        aux1 = aux1->abajo;
        aux2 = aux1;
    }
    return NULL;
}

void creaFrutas(Frutas *f)
{
    f->prim = NULL;
    f->ult = NULL;
    leeimagen(&f->imagen[0],"F1.bet");
    leeimagen(&f->imagen[1],"F2.bet");
    leeimagen(&f->imagen[2],"F3.bet");
    leeimagen(&f->imagen[3],"F4.bet");
    leeimagen(&f->imagen[4],"F1.bet");
}

Fruta *creaFruta(Nodo *pos,int tipo)
{
    Fruta *nueva = (Fruta*)malloc(sizeof(Fruta));
    nueva->pos = pos;
    nueva->tipo = tipo;
    nueva->ant = NULL;
    nueva->sig = NULL;
    pos->fruta = nueva;
    return nueva;
}
//ELIMINA UNA FRUTA DE LA LISTA
void eliminaFruta(Frutas *frutas,Fruta *fruta)
{
   Fruta *ult=frutas->ult;
   Fruta *prim=frutas->prim;

   if(frutas->prim==frutas->ult)
   {
        frutas->prim->pos->fruta = NULL;
        frutas->prim->pos = NULL;
        free(frutas->prim);
        frutas->prim=frutas->ult=NULL;
   }else
   if(frutas->ult==fruta)
   {
        frutas->ult=frutas->ult->ant;
        frutas->ult->sig=NULL;
        ult->ant = NULL;
        ult->pos->fruta=NULL;
        free(ult);
   }else
   if(frutas->prim==fruta)
   {
        frutas->prim=frutas->prim->sig;
        frutas->prim->ant=NULL;
        prim->sig = NULL;
        prim->pos->fruta=NULL;
        free(prim);
   }else
   {
        while(prim != NULL && prim != fruta)
        {
            prim=prim->sig;
        }
        if(prim == fruta)
        {
            prim->sig->ant=prim->ant;
            prim->ant->sig=prim->sig;
            prim->sig=NULL;
            prim->ant=NULL;
            prim->pos->fruta=NULL;
            free(prim);
        }
   }
}
//AGREGA UNA FRUTA A LA LISTA
void dejaCaerFruta(Frutas *frutas,Nodo *cab)
{
    Fruta *fruta = creaFruta(BuscaPosicionesF(cab,rand()%18+1),rand()%4);
    if(frutas->prim == NULL && frutas->ult == NULL)
    {
        frutas->prim = fruta;
        frutas->ult = fruta;
    }else
    {
        frutas->prim->ant = fruta;
        fruta->sig = frutas->prim;
        frutas->prim = fruta;
    }
}
void mueveFruta(Fruta *f,Imagen ima)
{
    if(f->pos->abajo != NULL)
    {
        f->pos->fruta = NULL;
        f->pos = f->pos->abajo;
        f->pos->fruta = f;
        DibujaImagen(ima,f->pos->x1,f->pos->y1,0);
    }
}
void mueveFrutas(Frutas *frutas)
{
    Fruta *f = frutas->prim;
    while(f)
    {
        mueveFruta(f,frutas->imagen[f->tipo]);
        f = f->sig;
    }
}
void creaEnem(Enemigo *enem,Nodo *pos)
{
    enem->pos = BuscaPosicionesE(pos,rand()%10+2);
    enem->anima = 0;
    enem->direccion = 0;
    pos->enemigo = enem;
    leeimagen(&enem->imagen[0],"P1.bet");
    leeimagen(&enem->imagen[1],"P2.bet");
    leeimagen(&enem->imagen[2],"P3.bet");
    leeimagen(&enem->imagen[3],"P4.bet");
}
void mueveEnemigo(Enemigo *enem)
{
    switch(enem->direccion)
    {
        case 0:
            if(enem->pos->adelante!=NULL)
            {
                enem->pos->enemigo = NULL;
                enem->pos = enem->pos->adelante;
                enem->pos->enemigo = enem;
                DibujaImagen(enem->imagen[enem->anima],enem->pos->x1,enem->pos->y1,0);
            }else
            {
                enem->pos->enemigo = NULL;
                enem->pos = BuscaPosicionesE(u,rand()%10+2);
                enem->direccion = 1;
            }
        break;
        case 1:
            if(enem->pos->atras!=NULL)
            {
                enem->pos->enemigo = NULL;
                enem->pos = enem->pos->atras;
                enem->pos->enemigo = enem;
                DibujaImagen(enem->imagen[enem->anima],enem->pos->x1,enem->pos->y1,1);
            }else
            {
                enem->pos->enemigo = NULL;
                enem->pos = BuscaPosicionesE(p,rand()%10+2);
                enem->direccion = 0;
            }
        break;
    }
    if(enem->anima == 3)
        enem->anima = 0;
    else
        enem->anima += 1;
}
void inicializarecord()
{
     FILE *fp;
     Trecord R[5]={{"----",0},{"----",0},{"----",0},{"----",0},{"----",0}};
     fp=fopen("records.dat","rb");
     if(fp!=NULL)
     {
        fclose(fp);
        return;
     }
     fp=fopen("records.dat","wb");
     fwrite(R,sizeof(Trecord),5,fp);
     fclose(fp);
}
void imprimirrecord()
{
     FILE *fp;
     Trecord R;
     char cad[20];
     cleardevice();
        settextstyle(8, 0, 2);
     fp=fopen("records.dat","rb");

     outtextxy(200,100,"nombre jugador  puntuacion");

     for(int i=0;i<5;i++)
     {
          fread(&R,sizeof(Trecord),1,fp);
          outtextxy(200,150+30*i,R.nombre);

          sprintf(cad,"%d",R.puntos);
          outtextxy(500,150+30*i,cad);

     }
     fclose(fp);
     getch();
     cleardevice();
}
void actualizarecord(char jug[],int pts)
{
     Trecord R[5];
     int i;
     FILE *fp;
     fp=fopen("records.dat","rb+");
     if(fp!=NULL)
     {
        fread(R,sizeof(Trecord),5,fp);

         for(i=0;i<5&&R[i].puntos>pts;i++);

         if(i==5)
         {
                 fclose(fp);
                 return;
         }

         for(int j=4;j>i;j--)
         {
            R[j].puntos=R[j-1].puntos;
            strcpy(R[j].nombre,R[j-1].nombre);
         }

         R[i].puntos=pts;
         strcpy(R[i].nombre,jug);

         rewind(fp);
         fwrite(R,sizeof(Trecord),5,fp);
         fclose(fp);

         outtextxy(298,500,"¡ Nuevo Record !");
     }
     else
     {
         inicializarecord();
     }
}
int intextxy(int x,int y,char scadena[],int itamcad)
{
    char ccar;
    int ipos=0;
    scadena[0]='\0';
    do
    {
        outtextxy(60,100,"nombre del jugador");
        outtextxy(x,y,scadena);
        ccar=getch();
        cleardevice();
        switch(ccar)
        {
                   case 27:
                        return(0);

                   case 8:
                        if(ipos>0)
                        {
                        ipos--;
                        scadena[ipos]='\0';
                        }
                        break;

                   default:
                           if(ipos<itamcad)
                           {
                              scadena[ipos]=ccar;
                              ipos++;
                              scadena[ipos]='\0';

                           }



                           }
        }while(ccar!=13);


scadena[ipos-1]='\0';
return(1);
}
void LeeAyuda()
{
    FILE *f;
    char op;
    char c[255];
    int x=10, y=10;
    setcolor(YELLOW);
    cleardevice();
    settextstyle(8, 0, 2);
    f=fopen("Ayuda.txt","r");

        while (!feof(f))
        {
              fgets(c,255,f);
              c[strlen(c)-1]='\0';
              outtextxy(x,y,c);
              y+=textheight("H");
        }
          fclose(f);
             settextstyle (10,0,1);
    getch();
    cleardevice();
}

