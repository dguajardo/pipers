#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX 9999

int flagfirst = 0;
int flagrouteTXT = 0;
char matrixRouters[MAX];
char route[MAX];

/* Determina si es router dado una ruta de caracteres retornando un entero (0,1)*/
int is_router(char *A)
{
    int i, j;
    for(i = 0; i < strlen(A); i++){
        if (A[i] == '\\' && A[i+1] == '\\'){
            for(j = 0; j < strlen(A); j++){
                if(A[j] == ' ')
                    return 0;
            }
        }
    }
    return 1;
}
/* Determina si es dirección dado una ruta de caracteres retornando un entero (0,1)*/
int is_address(char *A)
{
    int i;
    for(i = 0; i < strlen(A); i++){
        if (A[i] == ' ' && (!(is_router(A))))
            return 1;
    }
    return 0;
}

void search_directory(char *route)
{
    int i;
    int piper[2];
    pipe(piper);
    /*pipes */
    pid_t pid;
    /*pipe id*/
    char *newName; 
    /*String con el nombre de la folder encontrada temporalmente*/
    char *extension; 
    /*String que tomara el valor de la extension de un archivo*/
    char newRoute[MAX]; 
    /*String con la route de las carpetas encontradas*/
    char contenText[MAX]; 
    /*String con el contenido de un archivo .txt*/
    char bufferRoute[MAX]; 
    /*String donde se escribira la route encontrada por los hijos para ser comunicada al padre*/
    struct stat characts; 
    /*Estructura stat que contiene metadata contenida en un directorio*/
    struct dirent *pointerDirect; 
    /*Puntero para la entrada a un directorio*/
    
    FILE *entrance; 
    /*Puntero a un archivo*/
    DIR *folder; 
    /*Puntero al directorio*/
    

    while (1)
    {
        /*printf("abro directorio\n");
        printf("inicio busqueda en route: %s\n", route);*/

        if (folder != NULL)
            folder = NULL;
		
            folder = opendir(route); 
            /* Abro directorio folder en el path que correspondes */
		        rewinddir(folder);
		        /*printf("puntero folder: %x\n", folder);*/

        if (folder == NULL) /*Si el directorio  no existe*/
        { /*printf("No se pudo abrir el directorio");*/
            return;
        }
            /*printf("Abri y estoy dentro del directorio\n");
            printf("voy a buscar dentro del directorio\n\n");*/
            int router_value = is_router(contenText);
            int address_value = is_address(contenText);

        while ((pointerDirect = readdir(folder)) != NULL)
        {
            newName = pointerDirect->d_name;
            /*printf("nombretemp: %s\n", newName);*/
            strcpy(newRoute, route);
            strcat(newRoute, newName);
            stat(newRoute, &characts); //Se inicializan los valores de la estructura con la metadata

            if(S_ISREG(characts.st_mode))
            { /*Si lo que encontro es un archivo, entonces*/
                extension = strrchr(newRoute, '.');
                if(strcmp(extension, ".txt") == 0)
                {   /*Si la extension es .txt, entonces*/
                    /*printf("%s\n", pointerDirect->d_name); */
                    entrance = fopen(newRoute, "r");
                    fgets(contenText, MAX, entrance);
                    printf("%s", contenText);

                    if(flagfirst == 0 || pid == 0)
                    {
                        flagfirst = 1; 
                        /*Se aumenta el flag para saber que ya pasó*/
                        int router_value = is_router(contenText);
                        int address_value = is_address(contenText);

                        if(router_value == 0)
                        { /* Si es router haz lo siguiente*/
                            strcpy(route, contenText);
                            /*printf("\n route: %s", route);*/
                            for(i = 0; i < strlen(contenText); i++)
                                matrixRouters[i] = contenText[i];
                                pid = fork();
                                /*crea un hijo*/
                            break;
                            
                        }
                        else if( address_value == 1)
                        {
                            flagrouteTXT = 1;
                            closedir(folder); 
                            /*Cierra el directorio que se abrió*/
                        }
                    }
                        if(address_value == 1)
                        {
                            printf("\nLa direccion es: %s",route);
                              /*Si es dirección, imprimela por pantalla*/
                        if (pid == 0)
                        {   /*hijo*/
                            close(piper[0]);
                            write(piper[1], route, strlen(newRoute));
                            close(piper[1]);
                        }
                        
                        if (pid > 0)
                        {   /*padre*/
                            close(piper[0]);
                            read(piper[0], route, MAX);
                            close(piper[1]);
                        }
                            flagrouteTXT = 1;
                            strcpy(contenText, route);
                            printf("%s", contenText);
                            /*closedir(folder); //Cierra el directorio*/
                        }
                }
            }  
	    }
    }
    fclose(entrance);
    exit(0);
}

int main(int argc, char* argv[])
{
    strcpy(route, argv[1]); 
    /*Se asigna al puntero la ruta a buscar*/
    search_directory(route);
    return 0;
}