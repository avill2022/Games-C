#include <graphics.h>

#include <time.h>
#define TAM 10
#define PROFX TAM/2
#define PROFY TAM/3 
#define M 20
#define N 1
#define L 20 
#include <stdio.h> 
#include <iostream> 

#include<windows.h>

typedef struct 
{
        int iColor;
}cubito;

typedef struct 
{
        int dir;
        int x,z;
}Jugador;

void pared1(int tipo,int lado);
void pared2(int tipo,int lado);
void pared3(int tipo,int lado);
void moverJugador(Jugador *J,cubito conte[M][N][L]);
void juega(cubito conte[M][N][L],Jugador J);
void vista_de_ariba(cubito conte[M][N][L],Jugador J, int x, int y);
void creaContenedor(cubito conte[M][N][L], int nivel);
void dibuja_contenedor(cubito conte[M][N][L],int x,int y);
void dibujaCubo(cubito cC,int iX,int iY);
void paginacion(int *pagina,cubito conte[M][N][L],Jugador J);
void mover_izquierda(cubito conte[M][N][L],Jugador *J);
void mover_derecha(cubito conte[M][N][L],Jugador *J);
void mover_arriba(cubito conte[M][N][L],Jugador *J);
void mover_abajo(cubito conte[M][N][L],Jugador *J);
void prespectivaJugador(cubito conte[M][N][L],Jugador J);

int main()
{
    initwindow( 950 , 430 , "asdf" );
    cubito c[M][N][L];
    Jugador J;
    J.dir=0;
    creaContenedor(c,1);  
    juega(c,J); 
   	return( 0 );
} 

void juega(cubito conte[M][N][L],Jugador J)
{  
   int pagina=0;
   do{
     paginacion(&pagina,conte,J); 
     moverJugador(&J,conte);
  }while(true);
}
void moverJugador(Jugador *J,cubito conte[M][N][L])
{
     if(kbhit())
     {
        int op=getch();
        switch(op)
        {
              case 75:
                   if(J->dir<3)
                   J->dir+=1;
                   else
                   J->dir=0;
                   printf("%d",J->dir);
                   break;   
              case 77:
                   if(J->dir>0)
                    J->dir-=1;
                    else
                    J->dir=3;
                   printf("%d",J->dir);
                   break; 
              case 72:  
                   printf("%d",J->dir);
                   if(J->dir == 0)
                   mover_abajo(conte,J);
                   else
                   if(J->dir == 1)
                   mover_izquierda(conte,J);
                   else
                   if(J->dir == 2)
                   mover_arriba(conte,J);
                   else
                   if(J->dir == 3)
                   mover_derecha(conte,J);  
                   break; 
              case 80://abajo
              
                   break;                      
        }          
     }
}
void paginacion(int *pagina,cubito conte[M][N][L],Jugador J)
{ 
    setactivepage(*pagina);  
    cleardevice();

    prespectivaJugador(conte,J);
    dibuja_contenedor(conte,748,330);
    rectangle(10,10,650,420);
    vista_de_ariba(conte,J,660,10);
    setvisualpage(*pagina);
    if(*pagina==0)
    *pagina=1;
    else
    *pagina=0;
}

void prespectivaJugador(cubito conte[M][N][L],Jugador J)
{
     switch(J.dir)
     {
           case 1://Hisqu
                if(conte[J.z][0][J.x-1].iColor==8)
                      pared1(2,0);
                 else
                 if(conte[J.z][0][J.x-2].iColor==8)
                      pared2(2,0);
                 else
                 if(conte[J.z][0][J.x-3].iColor==8)
                      pared3(2,0);       
                //--------------------------------//  
                if(conte[J.z][0][J.x-1].iColor==0)
                {
                    if(conte[J.z+1][0][J.x-1].iColor==8)
                      pared2(0,0);
                      else
                      if(conte[J.z+1][0][J.x-1].iColor==0 && conte[J.z+1][0][J.x-2].iColor==8)
                      pared2(1,0);                              
                      if(conte[J.z-1][0][J.x-1].iColor==8)
                      pared2(0,1);
                      else
                      if(conte[J.z-1][0][J.x-1].iColor==0 && conte[J.z-1][0][J.x-2].iColor==8)
                      pared2(1,1);                                                             
                }  
                if(conte[J.z][0][J.x-1].iColor==0 && conte[J.z][0][J.x-2].iColor==0)
                {
                    if(conte[J.z+1][0][J.x-2].iColor==8)
                      pared3(0,0);
                      else
                      if(conte[J.z+1][0][J.x-2].iColor==0 && conte[J.z+1][0][J.x-3].iColor==8)
                      pared3(1,0);                              
                      if(conte[J.z-1][0][J.x-2].iColor==8)
                      pared3(0,1);
                      else
                      if(conte[J.z-1][0][J.x-2].iColor==0 && conte[J.z-1][0][J.x-3].iColor==8)
                      pared3(1,1);                                                             
                }    
                if(conte[J.z+1][0][J.x].iColor==8)
                      pared1(0,0);
                else
                if(conte[J.z+1][0][J.x].iColor==0 && conte[J.z+1][0][J.x-1].iColor==8)
                      pared1(1,0);                              
                if(conte[J.z-1][0][J.x].iColor==8)
                      pared1(0,1);
                else
                if(conte[J.z-1][0][J.x].iColor==0 && conte[J.z-1][0][J.x-1].iColor==8)
                      pared1(1,1);      
                break;
           case 3://Der
                 if(conte[J.z][0][J.x+1].iColor==8)
                      pared1(2,0);
                 else
                 if(conte[J.z][0][J.x+2].iColor==8)
                      pared2(2,0);
                 else
                 if(conte[J.z][0][J.x+3].iColor==8)
                      pared3(2,0);     
                  //--------------------------------//      
                 if(conte[J.z][0][J.x+1].iColor==0)  
                 {
                     if(conte[J.z+1][0][J.x+1].iColor==8)
                      pared2(0,1);
                      else
                      if(conte[J.z+1][0][J.x+1].iColor==0 && conte[J.z+1][0][J.x+2].iColor==8)
                      pared2(1,1);  
                      if(conte[J.z-1][0][J.x+1].iColor==8)
                      pared2(0,0);
                      else
                      if(conte[J.z-1][0][J.x+1].iColor==0 && conte[J.z-1][0][J.x+2].iColor==8)
                      pared2(1,0);                                 
                 }   
                 if(conte[J.z][0][J.x+1].iColor==0 && conte[J.z][0][J.x+2].iColor==0)  
                 {
                     if(conte[J.z+1][0][J.x+2].iColor==8)
                      pared3(0,1);
                      else
                      if(conte[J.z+1][0][J.x+2].iColor==0 && conte[J.z+1][0][J.x+3].iColor==8)
                      pared3(1,1);  
                      if(conte[J.z-1][0][J.x+2].iColor==8)
                      pared3(0,0);
                      else
                      if(conte[J.z-1][0][J.x+2].iColor==0 && conte[J.z-1][0][J.x+3].iColor==8)
                      pared3(1,0);                                 
                 }   
                if(conte[J.z+1][0][J.x].iColor==8)
                      pared1(0,1);
                  else
                if(conte[J.z+1][0][J.x].iColor==0 && conte[J.z+1][0][J.x+1].iColor==8)
                      pared1(1,1);  
                if(conte[J.z-1][0][J.x].iColor==8)
                      pared1(0,0);
                else
                if(conte[J.z-1][0][J.x].iColor==0 && conte[J.z-1][0][J.x+1].iColor==8)
                      pared1(1,0); 
                break;
           case 0://Aribba
                if(conte[J.z-1][0][J.x].iColor==8)
                      pared1(2,0);
                else
                if(conte[J.z-2][0][J.x].iColor==8)
                      pared2(2,0);
                else
                if(conte[J.z-3][0][J.x].iColor==8)
                      pared3(2,0);
                //--------------------------------//  
                if(conte[J.z-1][0][J.x].iColor==0)
                {
                    if(conte[J.z-1][0][J.x-1].iColor==8)
                      pared2(0,0);
                      else
                      if(conte[J.z-1][0][J.x-1].iColor==0 && conte[J.z-2][0][J.x-1].iColor==8)
                      pared2(1,0);
                      if(conte[J.z-1][0][J.x+1].iColor==8)
                      pared2(0,1);
                      else
                      if(conte[J.z-1][0][J.x+1].iColor==0 && conte[J.z-2][0][J.x+1].iColor==8)
                      pared2(1,1);                              
                                                  
                }  
                if(conte[J.z-1][0][J.x].iColor==0 && conte[J.z-2][0][J.x].iColor==0)
                {
                    if(conte[J.z-2][0][J.x-1].iColor==8)
                      pared3(0,0);
                      else
                      if(conte[J.z-2][0][J.x-1].iColor==0 && conte[J.z-3][0][J.x-1].iColor==8)
                      pared3(1,0);
                      if(conte[J.z-2][0][J.x+1].iColor==8)
                      pared3(0,1);
                      else
                      if(conte[J.z-2][0][J.x+1].iColor==0 && conte[J.z-3][0][J.x+1].iColor==8)
                      pared3(1,1);                              
                                                  
                } 
                if(conte[J.z][0][J.x-1].iColor==8)
                      pared1(0,0);
                else
                if(conte[J.z][0][J.x-1].iColor==0 && conte[J.z-1][0][J.x-1].iColor==8)
                      pared1(1,0);
                if(conte[J.z][0][J.x+1].iColor==8)
                      pared1(0,1);
                else
                if(conte[J.z][0][J.x+1].iColor==0 && conte[J.z-1][0][J.x+1].iColor==8)
                      pared1(1,1);  
                                       
                break;
           case 2://Abajo
                if(conte[J.z+1][0][J.x].iColor==8)
                      pared1(2,0);
                else
                if(conte[J.z+2][0][J.x].iColor==8)
                      pared2(2,0);
                else
                if(conte[J.z+3][0][J.x].iColor==8)
                      pared3(2,0);
                 //--------------------------------//  
                 if(conte[J.z+1][0][J.x].iColor==0)
                 {
                     if(conte[J.z+1][0][J.x+1].iColor==8)
                      pared2(0,0);
                      else
                      if(conte[J.z+1][0][J.x+1].iColor==0 && conte[J.z+2][0][J.x+1].iColor==8)
                      pared2(1,0);                   
                      
                      if(conte[J.z+1][0][J.x-1].iColor==8)
                      pared2(0,1);
                      else
                      if(conte[J.z+1][0][J.x-1].iColor==0 && conte[J.z+2][0][J.x-1].iColor==8)
                      pared2(1,1);                              
                 }  
                if(conte[J.z+1][0][J.x].iColor==0 && conte[J.z+2][0][J.x].iColor==0)
                 {
                     if(conte[J.z+2][0][J.x+1].iColor==8)
                      pared3(0,0);
                      else
                      if(conte[J.z+2][0][J.x+1].iColor==0 && conte[J.z+3][0][J.x+1].iColor==8)
                      pared3(1,0);                   
                      
                      if(conte[J.z+2][0][J.x-1].iColor==8)
                      pared3(0,1);
                      else
                      if(conte[J.z+2][0][J.x-1].iColor==0 && conte[J.z+3][0][J.x-1].iColor==8)
                      pared3(1,1);                              
                 }                                 
                if(conte[J.z][0][J.x+1].iColor==8)
                      pared1(0,0);
                else
                if(conte[J.z][0][J.x+1].iColor==0 && conte[J.z+1][0][J.x+1].iColor==8)
                      pared1(1,0);                   
                      
                if(conte[J.z][0][J.x-1].iColor==8)
                      pared1(0,1);
                else
                if(conte[J.z][0][J.x-1].iColor==0 && conte[J.z+1][0][J.x-1].iColor==8)
                      pared1(1,1);
                break;              
     }
}

void mover_arriba(cubito conte[M][N][L],Jugador *J)
{
  for(int iVarz=0;iVarz<M;iVarz++)
     {
         for(int iVary=0;iVary<N;iVary++)
         {
             for(int iVarx=0;iVarx<L;iVarx++)
             {  
                if(conte[iVarz][iVary][iVarx].iColor==5 && conte[iVarz+1][iVary][iVarx].iColor==0) 
                {
                   conte[iVarz][iVary][iVarx].iColor=0;
                   conte[iVarz+1][iVary][iVarx].iColor=5; 
                   J->z=iVarz+1;
                   J->x=iVarx;
                   return;                                        
                }
             }
         }
     }     
}
void mover_abajo(cubito conte[M][N][L],Jugador *J)
{
  for(int iVarz=0;iVarz<M;iVarz++)
     {
         for(int iVary=0;iVary<N;iVary++)
         {
             for(int iVarx=0;iVarx<L;iVarx++)
             {  
                if(conte[iVarz][iVary][iVarx].iColor==5 && conte[iVarz-1][iVary][iVarx].iColor==0) 
                {
                   conte[iVarz][iVary][iVarx].iColor=0;
                   conte[iVarz-1][iVary][iVarx].iColor=5; 
                   J->z=iVarz-1;
                   J->x=iVarx;
                   return;                                       
                }
             }
         }
     }     
}
void mover_izquierda(cubito conte[M][N][L],Jugador *J)
{
  for(int iVarz=0;iVarz<M;iVarz++)
     {
         for(int iVary=0;iVary<N;iVary++)
         {
             for(int iVarx=0;iVarx<L;iVarx++)
             {  
                if(conte[iVarz][iVary][iVarx].iColor==5 && conte[iVarz][iVary][iVarx-1].iColor==0) 
                {
                   conte[iVarz][iVary][iVarx].iColor=0;
                   conte[iVarz][iVary][iVarx-1].iColor=5;  
                   J->z=iVarz;
                   J->x=iVarx-1;                 
                   return;                                      
                }
             }
         }
     }     
}
void mover_derecha(cubito conte[M][N][L],Jugador *J)
{
  for(int iVarz=0;iVarz<M;iVarz++)
     {
         for(int iVary=0;iVary<N;iVary++)
         {
             for(int iVarx=0;iVarx<L;iVarx++)
             {  
                if(conte[iVarz][iVary][iVarx].iColor==5 && conte[iVarz][iVary][iVarx+1].iColor==0) 
                {
                   conte[iVarz][iVary][iVarx].iColor=0;
                   conte[iVarz][iVary][iVarx+1].iColor=5; 
                   J->z=iVarz;
                   J->x=iVarx+1;  
                   return;                                    
                }
             }
         }
     }     
}

void creaContenedor(cubito conte[M][N][L], int nivel)
{
   FILE *f;
   char cadena[20];
   sprintf(cadena,"Nivel%d.txt",nivel);
   f=fopen(cadena,"r");
   
   for(int iVary=0;iVary<M;iVary++)
   {
      for(int iVarz=0;iVarz<N;iVarz++)
      {
          for(int iVarx=0;iVarx<L;iVarx++)
          {       
              fscanf(f,"%d",&conte[iVarz][iVary][iVarx].iColor);
              
          }
      }
   }    
}

void dibuja_contenedor(cubito conte[M][N][L],int x,int y)
{
     for(int iVarz=0;iVarz<M;iVarz++)
     {
         for(int iVary=0;iVary<N;iVary++)
         {
             for(int iVarx=0;iVarx<L;iVarx++)
             {  
                if(conte[iVarz][iVary][iVarx].iColor!=0)   
                   dibujaCubo(conte[iVarz][iVary][iVarx],x+(TAM*iVarx),y-(TAM*iVary));
             }
         }
         x-=PROFX;
         y+=PROFY;
     }
}

void vista_de_ariba(cubito conte[M][N][L],Jugador J, int x, int y)  
{
     for(int iVary=0;iVary<M;iVary++)
     {
         for(int iVarx=0;iVarx<L;iVarx++)
         {  
            if(conte[0][iVary][iVarx].iColor!=0)
            {
               setfillstyle(1,conte[0][iVary][iVarx].iColor);
               bar(x+iVarx*TAM,y+iVary*TAM,x+TAM+iVarx*TAM,y+TAM+iVary*TAM);
               rectangle(x+iVarx*TAM,y+iVary*TAM,x+TAM+iVarx*TAM,y+TAM+iVary*TAM);
               if(conte[0][iVary][iVarx].iColor==5)
               switch(J.dir)
               {
                   case 0:
                        line(x+iVarx*TAM+TAM/2,y+iVary*TAM+TAM/2,x+TAM+iVarx*TAM-TAM/2,y+TAM+iVary*TAM-TAM);
                        break;
                   case 1:
                        line(x+iVarx*TAM,y+iVary*TAM+TAM/2,x+TAM+iVarx*TAM-TAM/2,y+TAM+iVary*TAM-TAM/2);
                        break;
                   case 2:
                        line(x+iVarx*TAM+TAM/2,y+iVary*TAM+TAM/2,x+TAM+iVarx*TAM-TAM/2,y+TAM+iVary*TAM);
                        break;
                   case 3:
                        line(x+iVarx*TAM+TAM/2,y+iVary*TAM+TAM/2,x+TAM+iVarx*TAM,y+TAM+iVary*TAM-TAM/2);
                        break;           
               }                              
            }
         }
     }                   
}

void dibujaCubo(cubito cC,int iX,int iY)
{
     int arr[8];
     setcolor(WHITE);
     setfillstyle(1,cC.iColor);

     arr[0]=iX; 
     arr[1]=iY; 
     arr[2]=iX+TAM; 
     arr[3]=iY;
     arr[4]=iX+TAM; 
     arr[5]=iY+TAM; 
     arr[6]=iX;
     arr[7]=iY+TAM; 
     
     fillpoly(4,arr);
     
     arr[0]=iX+TAM;
     arr[1]=iY;
     arr[2]=iX+TAM+PROFX;
     arr[3]=iY-PROFY;
     arr[4]=iX+TAM+PROFX;
     arr[5]=iY+TAM-PROFY;
     arr[6]=iX+TAM;
     arr[7]=iY+TAM;   
      
     fillpoly(4,arr);
     
     arr[0]=iX;
     arr[1]=iY;
     arr[2]=iX+PROFX;
     arr[3]=iY-PROFY;
     arr[4]=iX+TAM+PROFX;
     arr[5]=iY-PROFY;
     arr[6]=iX+TAM;
     arr[7]=iY;
     fillpoly(4,arr);   
}

void pared1(int tipo,int lado)
{ 
     int arr[8];
     setcolor(WHITE);
     setfillstyle(1,8);
     if(lado==0)
     {
         if(tipo==0)
         {
             arr[0]=10;
             arr[1]=10;   
             arr[2]=130;
             arr[3]=87; 
             arr[4]=130;
             arr[5]=340; 
             arr[6]=10; 
             arr[7]=420; 
         }else
         if(tipo==1)
         {
              arr[0]=10; 
              arr[1]=87; 
              arr[2]=130; 
              arr[3]=87;    
              arr[4]=130; 
              arr[5]=340;     
              arr[6]=10;  
              arr[7]=340; 
         }
     }
     if(lado==1)
     {
         if(tipo==0)
         {
             arr[0]=530;
             arr[1]=87;  
             arr[2]=650; 
             arr[3]=10;  
             arr[4]=650; 
             arr[5]=420;              
             arr[6]=530; 
             arr[7]=340; 
         }else
         if(tipo==1)
         {
             arr[0]=530;
             arr[1]=87;  
             arr[2]=650; 
             arr[3]=87;  
             arr[4]=650; 
             arr[5]=340;              
             arr[6]=530; 
             arr[7]=340; 
         }  
     }
     if(tipo==2)
     {
          arr[0]=130;
          arr[1]=87; 
          arr[2]=530; 
          arr[3]=87;  
          arr[4]=530; 
          arr[5]=340;    
          arr[6]=130;  
          arr[7]=340;      
     }   
     fillpoly(4,arr);  
}

void pared2(int tipo,int lado)
{ 
     int arr[8];
     setcolor(WHITE);
     setfillstyle(1,8);
     if(lado==0)
     {
         if(tipo==0)
         {
             arr[0]=130;
             arr[1]=87;   
             arr[2]=230;
             arr[3]=151; 
             arr[4]=230;
             arr[5]=275; 
             arr[6]=130; 
             arr[7]=340; 
         }else
         if(tipo==1)
         {
             arr[0]=130;
             arr[1]=151;   
             arr[2]=230;
             arr[3]=151; 
             arr[4]=230;
             arr[5]=275; 
             arr[6]=130; 
             arr[7]=275; 
         }
     }
     if(lado==1)
     {
         if(tipo==0)
         {
             arr[0]=430;
             arr[1]=151;  
             arr[2]=530; 
             arr[3]=87;  
             arr[4]=530; 
             arr[5]=340;              
             arr[6]=430; 
             arr[7]=275; 
         }else
         if(tipo==1)
         {
             arr[0]=430;
             arr[1]=151;  
             arr[2]=530; 
             arr[3]=151;  
             arr[4]=530; 
             arr[5]=275;              
             arr[6]=430; 
             arr[7]=275; 
         }  
     }
     if(tipo==2)
     {
          arr[0]=230;
          arr[1]=151; 
          arr[2]=430; 
          arr[3]=151;  
          arr[4]=430; 
          arr[5]=275;    
          arr[6]=230;  
          arr[7]=275;      
     }   
     fillpoly(4,arr);  
}
void pared3(int tipo,int lado)
{ 
     int arr[8];
     setcolor(WHITE);
     setfillstyle(1,8);
     if(lado==0)
     {
         if(tipo==0)
         {
             arr[0]=230;
             arr[1]=151;   
             arr[2]=290;
             arr[3]=185; 
             arr[4]=290;
             arr[5]=235; 
             arr[6]=230; 
             arr[7]=275; 
         }else
         if(tipo==1)
         {
             arr[0]=230;
             arr[1]=185;   
             arr[2]=290;
             arr[3]=185; 
             arr[4]=290;
             arr[5]=235; 
             arr[6]=230; 
             arr[7]=235; 
         }
     }
     if(lado==1)
     {
         if(tipo==0)
         {
             arr[0]=370;
             arr[1]=185;  
             arr[2]=430; 
             arr[3]=151;  
             arr[4]=430; 
             arr[5]=275;              
             arr[6]=370; 
             arr[7]=235; 
         }else
         if(tipo==1)
         {
             arr[0]=370;
             arr[1]=185;  
             arr[2]=430; 
             arr[3]=185;  
             arr[4]=430; 
             arr[5]=235;              
             arr[6]=370; 
             arr[7]=235; 
         }  
     }
     if(tipo==2)
     {
          arr[0]=290;
          arr[1]=185; 
          arr[2]=370; 
          arr[3]=185;  
          arr[4]=370; 
          arr[5]=235;    
          arr[6]=290;  
          arr[7]=235;      
     }   
     fillpoly(4,arr);  
}
