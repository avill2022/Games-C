#include <graphics.h>
#include<windows.h>
#include <time.h>
#define TAM 20
#define PROFX TAM/2
#define PROFY TAM/3 
#define N 20 //Definicion del tamaño del contenedor
#define M 20 //Definicion del tamaño del contenedor
#include <stdio.h> //Librerias declaradas
#include <iostream> 



typedef struct //Estructura del cubo
{
        int iX;
        int iY;
        int iColor;
        int iExiste;
}cubito;

typedef struct 
{
    int x,y,z;
    //int tamanio;
}cuadro;
typedef struct
{
   int arriba;
   int abajo;
   int adelante;
   int atras;
   int izquierda;
   int derecha;        
}dir;
        
typedef struct{
        char nombre[10];
        int puntos;
}Record;  
            
void imprimirRecords();
void insertarRecord(Record snake);
void iniciaRecords(Record snake);
void felicitaciones();
void ingresaNombre(int x,int y, char cadena[5]);

void juega(cubito conte[M][N][M],dir direccion);
void crea_contenedor(cubito conte[M][N][M]); 
void dibuja_contenedor(cubito conte[M][N][M]); 
void cubo(cubito cC);
void paginacion(int *pagina,cubito conte[M][N][M],int tiempo,int nivel,int velocidad,int tamanioSnake,Record *snake,bool *gameOver);

void mueveJugador(cubito conte[M][N][M],cuadro Snake[],dir direccion,int tamanioSnake);
void inicializaJugador(cuadro Snake[],int *tamanioSnake);
void cambiaDir(int *tecla,dir *direccion,int *gira);
void pintaSnake(cubito conte[M][N][M],cuadro Snake[],int tamanioSnake);
void generaComida(cubito conte[M][N][M],cuadro *comida);
void condiciones(cubito conte[M][N][M],cuadro Snake[],int *tamanioSnake,cuadro *comida,int *velocidad,int *nivel,int *tiempoI,int tiempoA,int *puntosT);
void giraderecha(cubito conte[M][N][M]);
void giraderechaSnake(cubito conte[M][N][M],cuadro Snake[],cuadro *comida,int *gira,int tamanioSnake,dir *direccion);


typedef BOOL (*PlaySoundA_ptr) (char*, HMODULE, DWORD); 
PlaySoundA_ptr fn_PlaySound = 0;
   

void juega(cubito conte[M][N][M],dir direccion)
{   
   int tecla;
   int npag=0;
   int nivel=1;
   int velocidad=200;
   int tamanioSnake=0;
   int puntTiem=0;
   int gira=0;
   int tiempoI=time(NULL);
   int tiempoA=time(NULL);
   bool gameOver=false;
   Record snake;
   cuadro Snake[10];
   cuadro comida;

   inicializaJugador(Snake,&tamanioSnake);
   generaComida(conte,&comida);
   do{
       if(gira==1)
       {        
        giraderechaSnake(conte,Snake,&comida,&gira,tamanioSnake,&direccion); 
        giraderecha(conte);     
       }                      
       tiempoA=time(NULL); 
       paginacion(&npag,conte,tiempoA-tiempoI,nivel,velocidad,tamanioSnake,&snake,&gameOver);    
       cambiaDir(&tecla,&direccion,&gira);
       mueveJugador(conte,Snake,direccion,tamanioSnake); 
       condiciones(conte,Snake,&tamanioSnake,&comida,&velocidad,&nivel,&tiempoI,tiempoA,&puntTiem);    
       pintaSnake(conte,Snake,tamanioSnake);
       delay(velocidad);     
  }while(gameOver==false);
  if(nivel==10)
  felicitaciones();
  ingresaNombre(300,200,snake.nombre);
  snake.puntos=(nivel*60-puntTiem);
  insertarRecord(snake);
  imprimirRecords();
}
void giraderechaSnake(cubito conte[M][N][M],cuadro Snake[],cuadro *comida,int *gira,int tamanioSnake,dir *direccion)
{
     int z;
     conte[comida->x][comida->y][comida->z].iExiste=0;  
     conte[comida->x][comida->y][comida->z].iColor=0; 
     z=comida->z;
     comida->z=M-1-comida->x;
     comida->x=z;
     conte[comida->x][comida->y][comida->z].iExiste=1;   
     conte[comida->x][comida->y][comida->z].iColor=1;   
   

     if(tamanioSnake==0)
     { 
         conte[Snake[0].x][Snake[0].y][Snake[0].z].iColor=0; 
         conte[Snake[0].x][Snake[0].y][Snake[0].z].iExiste=0; 
           
         z=Snake[0].z;    
         Snake[0].z=M-1-Snake[0].x;
         Snake[0].x=z; 
         conte[Snake[0].x][Snake[0].y][Snake[0].z].iColor=2;  
         conte[Snake[0].x][Snake[0].y][Snake[0].z].iExiste=1;                     
     }
     else
     {
         for(int i=0;i<tamanioSnake+1;i++)
         {
             z=Snake[i].z;    
             Snake[i].z=M-1-Snake[i].x;
             Snake[i].x=z;    
         }
     }
     
      
      if(direccion->adelante==1)
      {
      direccion->izquierda=1;
      direccion->derecha=0;
      direccion->arriba=0;
      direccion->abajo=0;
      direccion->adelante=0;
      direccion->atras=0;
      }
      else
      if(direccion->izquierda==1)
      {
      direccion->izquierda=0;
      direccion->derecha=0;
      direccion->arriba=0;
      direccion->abajo=0;
      direccion->adelante=0;
      direccion->atras=1;
      }
      else
      if(direccion->atras==1)
      {
      direccion->izquierda=0;
      direccion->derecha=1;
      direccion->arriba=0;
      direccion->abajo=0;
      direccion->adelante=0;
      direccion->atras=0;
      }
      else
      if(direccion->derecha==1)
      {
      direccion->izquierda=0;
      direccion->derecha=0;
      direccion->arriba=0;
      direccion->abajo=0;
      direccion->adelante=1;
      direccion->atras=0;
      }   
       
       
       
         
       *gira=0;    
}
void paginacion(int *pagina,cubito conte[M][N][M],int tiempo,int nivel,int velocidad,int tamanioSnake,Record *snake,bool *gameOver)
{
    char aux[5]; 
    setactivepage(*pagina);  
    cleardevice();
    itoa(tiempo,aux,10); 
    if(tiempo==60)
    *gameOver=true;
    outtextxy(700,10,"Tiempo:");
    outtextxy(770,10,aux);
    itoa(nivel,aux,10); 
    if(nivel==10)
    *gameOver=true;
    outtextxy(700,70,"Nivel:");
    outtextxy(770,70,aux);
    itoa(velocidad,aux,10); 
    outtextxy(700,100,"Velocidad:");
    outtextxy(780,100,aux);
    itoa(tamanioSnake,aux,10); 
    outtextxy(700,130,"tamaño Snake:");
    outtextxy(810,130,aux);
    
    dibuja_contenedor(conte);
    setvisualpage(*pagina);
    if(*pagina==0)
    *pagina=1;
    else
    *pagina=0;
}
void condiciones(cubito conte[M][N][M],cuadro Snake[],int *tamanioSnake,cuadro *comida,int *velocidad,int *nivel,int *tiempoI,int tiempoA,int *puntosT)
{
    if(Snake[0].x==comida->x && Snake[0].y==comida->y && Snake[0].z==comida->z)
    {
         conte[comida->x][comida->y][comida->z].iExiste=0;   
         generaComida(conte,comida); 
         *tamanioSnake+=1;   
         *puntosT+=tiempoA-*tiempoI;
         *tiempoI=time(NULL);
         if(*tamanioSnake==10)
         {
            inicializaJugador(Snake,tamanioSnake);
            *velocidad-=20;
            *nivel+=1;
            
         }          
    }    
}
int main()
{
    initwindow( 900 , 550 , "asdf" );//Inicia graficos
    cubito c[M][N][M];
    crea_contenedor(c);
    dir dire;
    dire.izquierda=0;
    dire.derecha=0;
    dire.arriba=0;
    dire.abajo=0;
    dire.adelante=0;
    dire.atras=0;
    juega(c,dire);
    
  
   	return( 0 );
} 
void felicitaciones()
{
     
     
}
void generaComida(cubito conte[M][N][M],cuadro *comida)
{
     comida->x=rand()%19;
     comida->y=rand()%19;
     comida->z=rand()%19;
     conte[comida->x][comida->y][comida->z].iExiste=1;   
     conte[comida->x][comida->y][comida->z].iColor=3; 
}
void pintaSnake(cubito conte[M][N][M],cuadro Snake[],int tamanioSnake)
{
   for(int i=0;i<tamanioSnake+1;i++)
    {
     if(i==0)
     {
           conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=1;   
           conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=2; 
     }
     else
     {
         conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=1;   
         conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=5; 
     }        
            
    }      
}

void mueveJugador(cubito conte[M][N][M],cuadro Snake[],dir direccion,int tamanioSnake)
{
    for(int i=tamanioSnake;i>=0;i--)
   {
        if(i==0)
        {
           if(direccion.adelante==1)
           {
              if(Snake[i].x<19)
              {
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0; 
               Snake[i].x+=1;              
              }
              else
              { 
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0;              
              Snake[i].x=0;
              }
          }
          else
          if(direccion.atras==1)
           {
              if(Snake[i].x>0)
              {
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0; 
               Snake[i].x-=1;              
              }
              else
              { 
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0;              
              Snake[i].x=19;
              }
          }
          if(direccion.izquierda==1)
           {
              if(Snake[i].z<19)
              {
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0; 
               Snake[i].z+=1;              
              }
              else
              { 
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0;              
              Snake[i].z=0;
              }
          }
          else
          if(direccion.derecha==1)
           {
              if(Snake[i].z>0)
              {
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0; 
               Snake[i].z-=1;              
              }
              else
              { 
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0;              
              Snake[i].z=19;
              }
          } 
          else
          if(direccion.arriba==1)
           {
              if(Snake[i].y<19)
              {
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0; 
               Snake[i].y+=1;              
              }
              else
              { 
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0;              
              Snake[i].y=0;
              }
          }
          else
          if(direccion.abajo==1)
           {
              if(Snake[i].y>0)
              {
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0; 
               Snake[i].y-=1;              
              }
              else
              { 
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
              conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0;              
              Snake[i].y=19;
              }
          }
          
        }
        else
        {
         conte[Snake[i].x][Snake[i].y][Snake[i].z].iExiste=0;   
         conte[Snake[i].x][Snake[i].y][Snake[i].z].iColor=0; 
           Snake[i].x=Snake[i-1].x; 
           Snake[i].y=Snake[i-1].y;   
           Snake[i].z=Snake[i-1].z;      
        } 
   }                                 
}
void cambiaDir(int *tecla,dir *direccion,int *gira)
{
        if(kbhit())
        {
        *tecla=getch();
        switch(*tecla)
        {
             case 75:
                  if(direccion->izquierda==0)
                  {
                  direccion->izquierda=0;
                  direccion->derecha=1;
                  direccion->arriba=0;
                  direccion->abajo=0;
                  direccion->adelante=0;
                  direccion->atras=0;
                  }
             break;   
             case 77:
                  if(direccion->derecha==0)
                  {
                  direccion->izquierda=1;
                  direccion->derecha=0;
                  direccion->arriba=0;
                  direccion->abajo=0;
                  direccion->adelante=0;
                  direccion->atras=0;
                  }
             break;    
             case 80:
                  if(direccion->atras==0)
                  {
                  direccion->izquierda=0;
                  direccion->derecha=0;
                  direccion->arriba=0;
                  direccion->abajo=0;
                  direccion->adelante=1;
                  direccion->atras=0;
                  }
             break;    
             case 72:
                  if(direccion->adelante==0)
                  {
                  direccion->izquierda=0;
                  direccion->derecha=0;
                  direccion->arriba=0;
                  direccion->abajo=0;
                  direccion->adelante=0;
                  direccion->atras=1;
                  }
             break; 
             case 'a':
                  if(direccion->abajo==0)
                  {
                  direccion->izquierda=0;
                  direccion->derecha=0;
                  direccion->arriba=1;
                  direccion->abajo=0;
                  direccion->adelante=0;
                  direccion->atras=0;
                  }
             break; 
             case 's':
                  if(direccion->arriba==0)
                  {
                  direccion->izquierda=0;
                  direccion->derecha=0;
                  direccion->arriba=0;
                  direccion->abajo=1;
                  direccion->adelante=0;
                  direccion->atras=0;
                  }
             break;  
             case 'd':
                   *gira=1;
             break;             
        }
   }
}
void inicializaJugador(cuadro Snake[],int *tamanioSnake)
{
     for(int i=0;i<10;i++)
     {
         if(i==0)
         {
            Snake[i].x=10;
            Snake[i].y=10;
            Snake[i].z=10; 
            *tamanioSnake=0;   
         }
        Snake[i].x=0;
        Snake[i].y=0;
        Snake[i].z=0;
     }     
}
void crea_contenedor(cubito conte[M][N][M])
{
      int iVarx,iVary,iVarz;
          iVarx=iVary=iVarz=0;
      int iCoordx,iCoordy;
          iCoordx=200;
          iCoordy=400;
      
      for(iVarz=0;iVarz<M;iVarz++){
      for(iVary=0;iVary<N;iVary++){
      for(iVarx=0;iVarx<M;iVarx++){
      conte[iVarz][iVary][iVarx].iX=iCoordx+(TAM*iVarx);
      conte[iVarz][iVary][iVarx].iY=iCoordy-(TAM*iVary);
      conte[iVarz][iVary][iVarx].iColor=0;
      conte[iVarz][iVary][iVarx].iExiste=0;
      }
      }
iCoordx-=PROFX;
iCoordy+=PROFY;
}
}
void dibuja_contenedor(cubito conte[M][N][M])
{
     int iVarx,iVary,iVarz;
     iVarx=iVary=iVarz=0; 
     line(210,14,610,14); 
     line(210,14*30-5,610,14*30-5);
     line(210,14,210,14*30-5); 
     line(610,14,610,14*30-5);
 
     line(210,14,10,135);
     line(610,14,410,135);
     line(210,14*30-5,10,135+400);
     line(610,14*30-5,410,135+400);
     
     line(10,135,410,135); 
     line(10,135+400,410,135+400); 
     line(10,135,10,135+400); 
     line(410,135,410,135+400); 
     
     for(iVarz=0;iVarz<M;iVarz++){
     for(iVary=0;iVary<M;iVary++){
     for(iVarx=0;iVarx<M;iVarx++){
                                  
     if(conte[iVarz][iVary][iVarx].iExiste==1)
     cubo(conte[iVarz][iVary][iVarx]);
     }
}
}
}

void giraderecha(cubito conte[M][N][M])
{
int i,j,k;
cubito conte2[M][N][M];
for(i=0;i<M;i++)
for(j=0;j<N;j++)
for(k=0;k<M;k++)
{
conte2[k][j][M-1-i].iExiste= conte[i][j][k].iExiste; //Giro para la Izquierda
conte2[k][j][M-1-i].iColor= conte[i][j][k].iColor; 
}
for(i=0;i<M;i++)
for(j=0;j<N;j++)
for(k=0;k<M;k++)
{
conte[i][j][k].iExiste=conte2[i][j][k].iExiste;
conte[i][j][k].iColor=conte2[i][j][k].iColor;
}}

void cubo(cubito cC)
{
     int arr[8];
     setcolor(WHITE);
     setfillstyle(1,cC.iColor);
     
     //cara frontal
     arr[0]=cC.iX; //coordenada #1 en x
     arr[1]=cC.iY; //coordenada #1 en y
     
     arr[2]=cC.iX+TAM; //coordenada  #2 en x
     arr[3]=cC.iY; //coordenada #2 en y
     
     arr[4]=cC.iX+TAM; //coordenada #3 en x
     arr[5]=cC.iY+TAM; //coordenada #3 en y
    
     arr[6]=cC.iX;  //coordenada #4 en x
     arr[7]=cC.iY+TAM; //coordenada #4 en y
     
     //Parametros
     //numero de puntos=4,multiples puntos en un arreglo=arr
     
     fillpoly(4,arr);
     
     //cara lateral
     arr[0]=cC.iX+TAM;
     arr[1]=cC.iY;
     arr[2]=cC.iX+TAM+PROFX;
     arr[3]=cC.iY-PROFY;
     arr[4]=cC.iX+TAM+PROFX;
     arr[5]=cC.iY+TAM-PROFY;
     arr[6]=cC.iX+TAM;
     arr[7]=cC.iY+TAM;
     
     //parametros
     //numero de puntos=4,multiples puntos en un arreglo=arr
     
     fillpoly(4,arr);
     //cara superior
     arr[0]=cC.iX;
     arr[1]=cC.iY;
     arr[2]=cC.iX+PROFX;
     arr[3]=cC.iY-PROFY;
     arr[4]=cC.iX+TAM+PROFX;
     arr[5]=cC.iY-PROFY;
     arr[6]=cC.iX+TAM;
     arr[7]=cC.iY;
     
     //parametros 
     //numero de puntos=4,multiples puntos en un arreglo=arr
     
     fillpoly(4,arr);
}


void imprimirRecords()
{
    FILE *pt;
    Record tanque[5];
    char cpuntos[20];
    int i;
    char op;
    
    pt=fopen("Record.bc","rb");
    cleardevice();


    do{
      if(pt==NULL) 
      {
      
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
    }while(op!=27);
    fclose(pt);
}
void iniciaRecords(Record snake)
{
      FILE *pt;
      Record p[5];
       pt=fopen("Record.bc","wb");
       strcpy(p[0].nombre,snake.nombre);
       p[0].puntos=snake.puntos;
       for(int i=1;i<5;i++)
       {
       strcpy(p[i].nombre,"- - - - - - - - - -");
       p[i].puntos=0;
       }
       fwrite(p,sizeof(Record),5,pt);          
}
void insertarRecord(Record snake)
{
     FILE *pt;
     Record p[5],aux[5];
     
     pt=fopen("Record.bc","rb+");
     if(pt==NULL)
     {
       iniciaRecords(snake);
     }
     else
     {
         fread(p,sizeof(Record),5,pt);
         fseek(pt,0,SEEK_SET);
         if(snake.puntos > p[0].puntos)
         {
         strcpy(aux[0].nombre,snake.nombre);
         aux[0].puntos=snake.puntos;
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
             }while(p[i].puntos > snake.puntos);
         if(i!=5)
         {
         strcpy(aux[i].nombre,snake.nombre);
         aux[i].puntos=snake.puntos;
         
           for(;i<4;i++)
           {
                        strcpy(aux[i+1].nombre,p[i].nombre);
                        aux[i+1].puntos=p[i].puntos;
           }
          }
         }
         fwrite(aux,sizeof(snake),5,pt);
     }
     fclose(pt);
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
