#include <stdio.h>
#include <iostream>

#include <graphics.h>

long **creaMalla(int tam);
void dibujaMalla(long **malla,int tam);
void coloreaMalla(long **malla,int tam,int x,int y,int color);

void paletaColores(int x,int y);
int selecColor(int x,int y);

void vistaPrevia(long **malla,int tam);

void menu(long **malla,int tam);

void nuevo(int *tam,int x,int y);
int datos(char cad [], int x, int y);
long **abreimagen(int indice,int *tam);
void guardar(long **maya,int tam);
int archivosActuales();



void paletadecolores();
void colores(int color);
int seleccionacolor(int x, int y);

int main( )
{
	initwindow( 760 , 610, "WinBGIm" );
	long **malla=NULL;
	
	malla=creaMalla(15);
	dibujaMalla(malla,15);
    paletaColores(700,10);
	

    menu(malla,15);
	while( !kbhit() ); 
	closegraph( );    
	return( 0 );
}

void menu(long **malla,int tam)
{
    int x,y;
    int color;
    do
	{
        
	  if(ismouseclick(WM_LBUTTONDOWN))
      {
        getmouseclick(WM_LBUTTONDOWN,x,y);
        clearmouseclick(WM_LBUTTONDOWN);
        //Selecciona color
        if(x>700 && y>10 && x<40+700 && y<570)
        {
            color=seleccionacolor(x,y); 
        }
        //Dibuja la imagen
        if(x>10 && y>10 && x<599 && y<599)
        {
             if(malla!=NULL)
             {
                 coloreaMalla(malla,tam,x,y,color);
                 vistaPrevia(malla,tam);
             }
        } 
        if(x>610 && y>200 && x<610+80 && y<200+30)
        {
              nuevo(&tam,610,200);
              //destrulle malla
              malla=creaMalla(tam); 
              cleardevice();
              dibujaMalla(malla,tam);
              vistaPrevia(malla,tam);
              paletaColores(700,10);              
        }
        if(x>610 && y>200+50 && x<610+80 && y<200+30+50)
        {
            guardar(malla,tam);                                  
        }
        if(x>610 && y>200+100 && x<610+80 && y<200+30+100)
        {
            int indice=0;
            char num[10];
            
            outtextxy(610+5,200+6+100,"Indice:");
            datos(num,610+50,200+6+100);
            indice=atoi(num);
            if (indice>55)
            indice=55;
            if(abreimagen(indice,&tam)!=NULL)
            malla = abreimagen(indice,&tam);  
            
            cleardevice();
            dibujaMalla(malla,tam);
            vistaPrevia(malla,tam);
            paletaColores(700,10);  
                                       
        }
       }
      }while(true) ;  
}
int archivosActuales()
{
   int i=1;;    
   FILE *archivo;
   char nombre[10] = "Nuevo"; 
   for(;;)
   {
       sprintf(nombre,"Naye%d",i); 
       strcat(nombre,".Na"); 
       archivo=fopen(nombre,"r");
       if(archivo==NULL)
       {
          return i;
       } 
           i+=1; 
   }
}
long **abreimagen(int indice,int *tam)
{
   long **malla;
   FILE *archivo;
   char nombre[20];
   
   
   sprintf(nombre,"Naye%d",indice); 
   strcat(nombre,".Na");
   archivo=fopen(nombre,"r");
   if(archivo==NULL)
   {
      return NULL;
   }
   else
   {
        do
        {
          fscanf(archivo, "%d ",&*tam);
          
          malla=(long**)malloc(sizeof(long*)*(*tam));
          for(int i=0;i<*tam;i++)
          {
            malla[i]=(long*)malloc(sizeof(long)*(*tam));
          }
          for(int i=0;i<*tam;i++)
          {
            for(int j=0;j<*tam;j++)
            {
              fscanf(archivo,"%d ",&malla[i][j]);
              printf("%d  ",malla[i][j]);
            }
            printf("\n\n");
          }
          return malla;
        }while(!feof(archivo)); 
        fclose(archivo);
   }
  
}
void guardar(long **maya,int tam)
{
    FILE *archivo;
    char nombre[10];
    
    
    sprintf(nombre,"Naye%d",archivosActuales()); 
    strcat(nombre,".Na");
    archivo=fopen(nombre,"w+");
    if(archivo==NULL)
    {
      return ;
    }
    else
    {
       fprintf(archivo, "%d ",tam);

      for(int i=0;i<tam;i++)
      {
        for(int j=0;j<tam;j++)
        {
         fprintf(archivo, "%d ",maya[i][j]);
        }
      }
        setfillstyle(SOLID_FILL,BLACK);
        bar(610+1,200+1+50,610+80,200+30+50);
        outtextxy(610+5,200+6+50,nombre);
        delay(800);
        bar(610+1,200+1+50,610+80,200+30+50);
        outtextxy(610+5,200+6+50,"Guardar");
    }
   fclose(archivo);
}
void nuevo(int *tam,int x,int y)
{
    char num[10];
    setfillstyle(SOLID_FILL,BLACK);
    bar(x+1,y+1,x+80,y+30);

    outtextxy(x+5,y+6,"TAM: ");
    datos(num,x+50,y+6);
    *tam=atoi(num);
    if (*tam>55)
    *tam=55;
    
    setfillstyle(SOLID_FILL,BLACK);
    bar(x,y,x+80,y+30);
    outtextxy(x,y,"Nuevo"); 
    
}
int datos(char cad [], int x, int y)
{ 
    char car;
    char texto [4]= "_";
    int pos = 0; 
    do 
    {
    outtextxy(x,y,texto);
    car = getch ();
     if (car == 0)
     {
       car = getch ();
     }
        switch (car)
        {
               case 13: strcpy(cad, texto);
               return 1;
               case 8: if(pos>0)
                    {
                    pos--;
                    setfillstyle(SOLID_FILL,BLACK);
                    bar(x,y,x+textwidth(texto),y+textheight(texto));
                    texto [pos]='\0';
                    }
               break;
               
               default: texto [pos]=car;
                        pos++;
                        texto [pos]='\0';
        }
    } while (car!=13 && car!= 27);
}
void vistaPrevia(long **malla,int tam)
{
     int pixel =70/tam;
     system("cls");
     rectangle(615-1,70-1,615+60,70+60);
     for(int i=0;i<tam;i++)
     {
         for(int j=0;j<tam;j++)
         {
           // setfillstyle(SOLID_FILL,malla[i][j]);
          
            colores(malla[i][j]); 
            printf("%d  ",malla[i][j]);
            bar(615+pixel*i,70+pixel*j,615+pixel+pixel*i,70+pixel+pixel*j);        
         }     
         printf("\n\n"); 
     }
      
}
void coloreaMalla(long **malla,int tam,int x,int y,int color)
{
   int tamCuadro = 590/tam;
   for(int i =0;i<tam;i++)
   {
       for(int j=0;j<tam;j++)
       {
            if(x>12+tamCuadro*i && y>12+tamCuadro*j && x<12+tamCuadro+tamCuadro*i && y<12+tamCuadro+tamCuadro*j)
            {
              malla[i][j]=color;
              // printf("%d\n",malla[i][j]);
              colores(color);
              floodfill(12+tamCuadro*i+1,12+tamCuadro*j+1,15); 
              return ;  
            }                   
       }        
   }
}
long **creaMalla(int tam)
{
   long **MALLA;
   MALLA = (long**)malloc(sizeof(long*)*tam);  
   for(int i = 0 ;i<tam;i++)
   {
        MALLA[i] = (long*)malloc(sizeof(long)*tam);        
   } 
   
   for(int i =0;i<tam;i++)
   {
       for(int j=0;j<tam;j++)
       {
            MALLA[i][j]=64;          
       }        
   }
   return MALLA;
}
void dibujaMalla(long **malla,int tam)
{
   rectangle(10,10,599,599);
   int tamCuadro = 590/tam;
   for(int i =0;i<tam;i++)
   {
       for(int j=0;j<tam;j++)
       {
            rectangle(12+tamCuadro*i,12+tamCuadro*j,12+tamCuadro+tamCuadro*i,12+tamCuadro+tamCuadro*j); 
            //setfillstyle(1,malla[i][j]);
            colores(malla[i][j]);
            floodfill(12+tamCuadro*i+1,12+tamCuadro*j+1,15);                      
       }        
   }
}
void paletaColores(int x,int y)
{
        rectangle(610,200,610+80,200+30);
        setfillstyle(SOLID_FILL,BLACK);
        bar(610+1,200+1,610+80,200+30);
        outtextxy(610+5,200+6,"Nuevo");
        
        rectangle(610,200+50,610+80,200+30+50);
        setfillstyle(SOLID_FILL,BLACK);
        bar(610+1,200+1+50,610+80,200+30+50);
        outtextxy(610+5,200+6+50,"Guardar");
        
        rectangle(610,200+100,610+80,200+30+100);
        setfillstyle(SOLID_FILL,BLACK);
        bar(610+1,200+1+100,610+80,200+30+100);
        outtextxy(610+5,200+6+100,"Abrir");
        
     paletadecolores();       
}
int selecColor(int x,int y)
{
     for(int i=0;i<14;i++)
     {
          if(x>5+700 && y>(10+40*i) && x<45+700 && y<(50+40*i))
          {
            return i;
          }
     } 
     return 0;        
}
void paletadecolores()
{
     rectangle(700,90,750,436);
	int r,g,b;
	r = 200;
	g = 0;
	b = 0;
	int c=0;
    for(int i=0;i<16;i++)
    {
      for(int j=0;j<115;j++)
      { 
        setfillstyle(SOLID_FILL,COLOR(r,g,b));
        if(g!=200 && r==200 && b==0)
        g+=10;
        if(g==200 && r!=0 && b==0)
        r-=10;
        if(g==200 && r==0 && b!=200)
        b+=10;
        if(g!=0 && r==0 && b==200)
        g-=10;
        if(g==0 && r!=200 && b==200)
        r+=10;
        if(g==0 && r ==200 && b!=0 )
        b-=10;    
        bar(701+3*i,91+3*j,701+3*i+4,90+3*j+4);
        setcolor(WHITE);
      }  
      
    }
}
int seleccionacolor(int x, int y)
{
    int color=0;
      for(int i=0;i<16;i++)
      {
              color=0;
         for(int j=0;j<115;j++)
         {
                 if(x>700 && x<750 && y>=91+3*j&& y<=90+3*j+4)
                 {
                    rectangle(700-1,470-1,750,520);
                    colores(color);
                     printf("%d\n",color);
                    bar(700,470,750,520);
                    setcolor(WHITE);
                    return color;                                    
                 }
           color++;
         }
      }     
}
void colores(int color)
{
     if(color==0)
     {
     setfillstyle(SOLID_FILL,0);
     }
     else
     {
    int r = 200;
	int g = 0;
	int b = 0;
	int c=0;
    for(int i=0;i<30;i++)
    {
      for(int j=0;j<115;j++)
      { 
        if(g!=200 && r==200 && b==0)
        g+=10;
        if(g==200 && r!=0 && b==0)
        r-=10;
        if(g==200 && r==0 && b!=200)
        b+=10;
        if(g!=0 && r==0 && b==200)
        g-=10;
        if(g==0 && r!=200 && b==200)
        r+=10;
        if(g==0 && r ==200 && b!=0 )
        b-=10;    
        c++;
        if(c==color)
        {
        setfillstyle(SOLID_FILL,COLOR(r,g,b));
        }
      }  
    }
}
}

