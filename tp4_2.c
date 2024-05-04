#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Tarea{
    int TareaID;//Numérico autoincremental comenzando en 1000
    char *Descripcion; //
    int Duracion; // entre 10 – 100
}Tarea;

typedef struct Nodo{
    Tarea T;
    struct Nodo *Siguiente;
}Nodo;

//#################### Funciones ####################

void limpiarBuffer();

void cargarTareas(Nodo ** tareasPendientes, int *id);

Nodo *crearListaVacia();

void insertarNodo(Nodo **start, Nodo *nuevoNodo);

Nodo * crearNodo(int *id);

void mostrarTarea(Tarea tarea);

void mostrarLista(Nodo *start, char *nombrLista);

Nodo * buscarTareaId(Nodo * start, int idBuscado);

Nodo * buscarTareaClave(Nodo * start, char *palabraClave);

void completarTarea(Nodo ** tareasPendientes,Nodo ** tareasRealizadas,int id);

void liberarMemoria(Nodo * pendientes,Nodo * realizadas);


int main(int argc, char const *argv[])
{
    int opc;
    int idBuscado, idCompletado;

    int *id = (int *) malloc (sizeof(int));
    *id = 1000;

    char*buff = (char *)malloc(100*sizeof(char));

    Nodo *tareasPendientes = crearListaVacia();
    Nodo *tareasRealizadas = crearListaVacia();
    Nodo *tareaEncontrada;
    
    do
    {
        puts("\n\n**************************************************");
        puts("**           1-cargar tareas pendiente          **");
        puts("**    2-listar tareas pendientes y realizadas   **");
        puts("** 3-transferir tareaa de pendiente a realizada **");
        puts("**           4-buscar tarea por ID              **");
        puts("**       5-buscar tarea por palabra clave       **");
        puts("**                    6-salir                   **");
        puts("**************************************************");
        printf("\nIngrese una opcion: ");
        scanf(" %d",&opc);
        limpiarBuffer();

        switch (opc){
        case 1:
            /* 1-cargar tareas pendiente */
            cargarTareas(&tareasPendientes, id);
            break;
        case 2:
            /* 2-listar tareas pendientes y realizadas */
            mostrarLista(tareasPendientes, "Tareas Pendientes");
            mostrarLista(tareasRealizadas, "Tareas Realizadas");
            break;
        case 3:
            /* 3-transferir tareaa de pendiente a realizada */
            if(tareasPendientes != NULL){
                printf("\n Ingrese el ID de la tarea que se completo: ");
                scanf(" %d",&idCompletado);
                limpiarBuffer();
                completarTarea(&tareasPendientes,&tareasRealizadas,idCompletado);
            }else{
                printf("\nNo hay tareas en la lista de pendientes");
            }
            break;
        case 4:
            /* 4-buscar tarea por ID */
            printf("\n Ingrese el ID que desea buscar: ");
            scanf(" %d",&idBuscado);
            limpiarBuffer();
            tareaEncontrada = buscarTareaId(tareasPendientes, idBuscado);
            if (tareaEncontrada != NULL){
                printf("\nTarea encontrada en la lista de Tareas Pendientes");
                mostrarTarea(tareaEncontrada->T);
            }else{
                tareaEncontrada = buscarTareaId(tareasRealizadas,idBuscado);
                if (tareaEncontrada != NULL){
                    printf("\nTarea encontrada en la lista de Tareas Realizadas");
                    mostrarTarea(tareaEncontrada->T);
                }else{
                    printf("\nNo se encontro ninguna tarea con ese ID\n");
                }
            }
            break;
        case 5:
            /* 5-buscar tarea por palabra clave */
            printf("\n Ingrese la palabra clave que desea buscar: ");
            
            fgets(buff,100,stdin);
            if((strlen(buff) > 0) && (buff[strlen(buff)-1] == '\n')) buff[strlen(buff)-1] = '\0';//elimina el salto de linea del final

            tareaEncontrada = buscarTareaClave(tareasPendientes, buff);
            if (tareaEncontrada != NULL){
                printf("\nTarea encontrada en la lista de Tareas Pendientes");
                mostrarTarea(tareaEncontrada->T);
            }else{
                tareaEncontrada = buscarTareaClave(tareasRealizadas, buff);
                if (tareaEncontrada != NULL){
                    printf("\nTarea encontrada en la lista de Tareas Realizadas");
                    mostrarTarea(tareaEncontrada->T);
                }else{
                    printf("\nNo se encontro ninguna tarea con esa descripcion\n");
                }
            }
            break;
        }
    } while (opc != 6);
    
    liberarMemoria(tareasPendientes,tareasRealizadas);
    free(buff);
    free(id);
    return 0;
}

Nodo * crearListaVacia(){
    return NULL;
};

void insertarNodo(Nodo **start, Nodo *nuevoNodo){
    nuevoNodo->Siguiente = *start;
    *start = nuevoNodo;
};

void limpiarBuffer(){
    char c;
    while((c = getchar()) != '\n' && c != EOF);
}

Nodo * crearNodo(int *id){
    Nodo *nuevoNodo = (Nodo *)malloc(sizeof(Nodo));
    char * buff = (char*) calloc(100,sizeof(char));
    int *duracion = (int *) calloc(1,sizeof(int));
    
    nuevoNodo->T.TareaID = *id;
    (*id)++;

    printf("\n**Ingrese la descripcion de la tarea**\n");
    fgets(buff,100,stdin);

    if((strlen(buff) > 0) && (buff[strlen(buff)-1] == '\n')) buff[strlen(buff)-1] = '\0';//elimina el salto de linea del final

    nuevoNodo->T.Descripcion = (char*)malloc((strlen(buff)+1)*sizeof(char));
    strcpy(nuevoNodo->T.Descripcion, buff);
    free(buff);
    do{
        printf("\n**ingrese la duracion de la tarea (entre 10 y 100)**\n");
        scanf(" %d", duracion);
        if(*duracion < 10 || *duracion > 100){
            printf("\n**La duracion no puede ser menor a 10 ni mayor a 100 **\n");
        }
    } while (*duracion < 10 || *duracion > 100);
    nuevoNodo->T.Duracion = *duracion;
    free(duracion);
    nuevoNodo->Siguiente = NULL;
    limpiarBuffer();//reemplazo de fflush(stdin) en linux
    return nuevoNodo;
};

void cargarTareas(Nodo ** tareasPendientes, int *id){
    char *opc = (char *)malloc(sizeof(char));
    do{
        insertarNodo(tareasPendientes,crearNodo(id));
        printf("\n** ¿Desea ingresar otra tarea? **\n");
        scanf(" %c", opc);
        limpiarBuffer();
    } while (*opc == 's' || *opc == 'S');
    free(opc);
};

void mostrarTarea(Tarea tarea){
    printf("\n\n** ID: %d **",tarea.TareaID);
    printf("\n** Descripcion: %s **",tarea.Descripcion);
    printf("\n** Duracion: %d **",tarea.Duracion);
}


void mostrarLista(Nodo *start, char *nombreLista){
    printf("\n\n**** %s ****",nombreLista);
    if (start == NULL){
        printf("\n\n** La lista no tiene elementos **\n\n");
        return;
    }
    do{
        mostrarTarea(start->T);
        start = start->Siguiente;
    } while (start != NULL);
    printf("\n\n--------------------------------------------------------------------------------\n\n");
}

Nodo *buscarTareaId(Nodo *start, int idBuscado){
    while (start && start->T.TareaID != idBuscado){
        start = start->Siguiente;
    }
    return start;
}

Nodo *buscarTareaClave(Nodo *start, char *palabraClave){
    while (start && (strstr(start->T.Descripcion,palabraClave) == NULL)){
        start = start->Siguiente;
    }
    return start;
}

void completarTarea(Nodo **tareasPendientes, Nodo **tareasRealizadas, int id){
    Nodo *aux = *tareasPendientes;
    Nodo *anterior = NULL;

    while (aux && aux->T.TareaID != id){
        anterior = aux;
        aux = aux->Siguiente;
    }
    if(aux == NULL){
        printf("\n\n**No se encontro la tarea** \n");
        return;
    }
    if (anterior != NULL){
        anterior->Siguiente = aux->Siguiente;
    }else{
        *tareasPendientes = aux->Siguiente;
    }
    insertarNodo(tareasRealizadas, aux);
    printf("\n**Se agrego la tarea a la lista de realizados**\n");
}

void liberarMemoria(Nodo *pendientes, Nodo *realizadas){
    Nodo *aux = pendientes;
    while (aux != NULL){
        pendientes = aux;
        aux = pendientes->Siguiente;
        free(pendientes->T.Descripcion);
        free(pendientes);
    }
    aux = realizadas;
    while (aux !=NULL){
        realizadas = aux;
        aux = realizadas->Siguiente;
        free(realizadas->T.Descripcion);
        free(realizadas);
    }
}
