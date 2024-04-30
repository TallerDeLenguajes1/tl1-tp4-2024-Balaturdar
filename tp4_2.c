#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

typedef struct Tarea{
    int TareaID;//Numérico autoincremental comenzando en 1000
    char *Descripcion; //
    int Duracion; // entre 10 – 100
}Tarea;

typedef struct Nodo{
    Tarea T;
    struct Nodo *Siguiente;
}Nodo;

//############################## Funciones ##############################

Nodo *crearListaVacia(){
    return NULL;
};

Nodo * crearNodo(int *id){
    Nodo *Nnodo = (Nodo*)malloc(sizeof(Nodo));
    char *buff = (char*)malloc(100*sizeof(char));
    Nnodo->T.TareaID = *id;
    (*id)++;
    puts("** ingrese la descripcion de la tarea **");
    fflush(stdin);
    gets(buff);
    Nnodo->T.Descripcion = (char*) malloc ((strlen(buff)+1) * sizeof(char));
    strcpy(Nnodo->T.Descripcion, buff);
    do{
        printf("\n**Ingrese la duracion de la tarea (entre 10 y 100)**\n");
        fflush(stdin);
        scanf("%d",&Nnodo->T.Duracion);
        if (Nnodo->T.Duracion < 10 || Nnodo->T.Duracion > 100){
            printf("\n**la duracion no puede ser menor a 10 ni mayor a 100**\n");
        }
    } while(Nnodo->T.Duracion < 10 || Nnodo->T.Duracion > 100);
    Nnodo->Siguiente = NULL;

    free(buff);
    return Nnodo;
};

//*start = &tareapendiente
//puntero -> *start -> "tarea" 

void InsertarNodo(Nodo **start, Nodo *NuevoNodo){
    NuevoNodo->Siguiente = *start;
    *start = NuevoNodo;
};
 
void cargarTareas(Nodo **tareasPendientes,int *id){
    char *opc = (char *)malloc(sizeof(char));
    do{
        InsertarNodo(tareasPendientes, crearNodo(id));
        puts("¿desea ingresar otra tarea? s/n \n");
        fflush(stdin);
        *opc = getchar();
    } while(*opc == 's' || *opc == 'S');
    free(opc);
};

void mostrarTarea(Tarea tarea){
    printf("\n\n** ID: %d **",tarea.TareaID);
    printf("\n** Descripcion: %s **",tarea.Descripcion);
    printf("\n** Duracion: %d **",tarea.Duracion);
};

void mostrarLista(Nodo *start, char *nombreLista){
    printf("\n\n**** %s ****",nombreLista);
    if(start == NULL){
        printf("\n\n** La lista no tiene elementos **\n\n");
        return;
    }
    do{
        mostrarTarea(start->T);
        start = start->Siguiente;
    } while (start != NULL);
    printf("\n\n--------------------------------------------------------------------------------\n");
};

Nodo * buscarTareaId (Nodo * start, int IdBuscado)
{
    while(start && start->T.TareaID != IdBuscado){
        start = start->Siguiente;
    }
    return start;
};

Nodo * buscarTareaClave(Nodo *start,char *palabraClave){
    while(start && (strstr(start->T.Descripcion,palabraClave) == NULL)){
        start = start->Siguiente;
    }
    return start;
};

//pendientes tiene que ser doble para que el puntero de main no apunte al viejo nodo
void completarTarea(Nodo **pendientes, Nodo **realizadas,int id){
    Nodo *aux = *pendientes;
    Nodo *anterior=NULL;

    //[id]->[]->[]->[]->[]->[]->NULL

    while(aux && (aux)->T.TareaID != id){
        anterior = (aux);
        aux = (aux)->Siguiente;
    }
    if (aux == NULL){
        printf("\n\n** No se encontro la tarea **\n");
        return;
    }

    if (anterior != NULL){
        anterior->Siguiente = (aux)->Siguiente;
    }else
    {
        *pendientes = aux->Siguiente;
    }
        
    InsertarNodo(realizadas, (aux));
    printf("\n** Se agrego la tarea a la lista de realizados **\n");
};

//[]->[]->[id]->[]->[]->[]
//              []->[]->[]


void liberarMemoria(Nodo *Pendientes, Nodo *realizadas){
    
    Nodo *aux = Pendientes;
    for (int i = 0; aux != NULL; i++)
    {
        Pendientes = aux;
        aux = Pendientes->Siguiente;
        free(Pendientes->T.Descripcion);
        free(Pendientes);
    }
    aux = realizadas;
    for (int i = 0; aux != NULL; i++)
    {
        realizadas = aux;
        aux = realizadas->Siguiente;
        free(realizadas->T.Descripcion);
        free(realizadas);
    }
    
};

//############################## PRINCIPAL ##############################


int main(int argc, char const *argv[]){
    srand(time(NULL));

    int *id = (int *)malloc(sizeof(int));
    *id = 1000;

    Nodo *tareasPendientes = crearListaVacia();
    Nodo *tareasRealizadas = crearListaVacia();
    Nodo *tareaEncontrada;
    //menu(tareasPendientes, tareasRealizadas, id);

    char opc;
    char *buff = (char *)malloc(100*sizeof(char));
    int idBuscado,idcompletado;

    cargarTareas(&tareasPendientes, id);
    puts("**************************************************");
    mostrarLista(tareasPendientes, "Tareas Pendientes");
    mostrarLista(tareasRealizadas, "Tareas Realizadas");
    puts("**************************************************");
   /* 3-transferir tareaa de pendiente a realizada */
    if(tareasPendientes != NULL){
        printf("\n Ingrese el ID de la tarea que se completo: ");
        fflush(stdin);
        scanf("%d",&idcompletado);
        completarTarea(&tareasPendientes,&tareasRealizadas,idcompletado);
    }else{
        printf("\nNo hay tareas en la lista de pendientes");
    }
    puts("**************************************************");
    mostrarLista(tareasPendientes, "Tareas Pendientes");
    mostrarLista(tareasRealizadas, "Tareas Realizadas");
    puts("**************************************************");
    if(tareasPendientes != NULL){
        printf("\n Ingrese el ID de la tarea que se completo: ");
        fflush(stdin);
        scanf("%d",&idcompletado);
        completarTarea(&tareasPendientes,&tareasRealizadas,idcompletado);
    }else{
        printf("\nNo hay tareas en la lista de pendientes");
    }
    puts("**************************************************");
    mostrarLista(tareasPendientes, "Tareas Pendientes");
    mostrarLista(tareasRealizadas, "Tareas Realizadas");
    puts("**************************************************");
       printf("\n Ingrese el ID que desea buscar: ");
    fflush(stdin);
    scanf("%d",&idBuscado);
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
    /* 5-buscar tarea por palabra clave */
    printf("\n Ingrese la palabra clave que desea buscar: ");
    fflush(stdin);
    gets(buff);
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




//    do
//    {
//
//        puts("**************************************************");
//        puts("**           1-cargar tareas pendiente          **");
//        puts("**    2-listar tareas pendientes y realizadas   **");
//        puts("** 3-transferir tareaa de pendiente a realizada **");
//        puts("**           4-buscar tarea por ID              **");
//        puts("**       5-buscar tarea por palabra clave       **");
//        puts("**                    6-salir                   **");
//        puts("**************************************************");
//        fflush(stdin);
//        printf("\nIngrese una opcion: ");
//        scanf("%d",&opc);
//
//        switch (opc){
//        case 1:
//            /* 1-cargar tareas pendiente */
//            cargarTareas(&tareasPendientes, id);
//            break;
//        case 2:
//            /* 2-listar tareas pendientes y realizadas */
//            mostrarLista(tareasPendientes, "Tareas Pendientes");
//            mostrarLista(tareasRealizadas, "Tareas Realizadas");
//            break;
//        case 3:
//            /* 3-transferir tareaa de pendiente a realizada */
//            if(tareasPendientes != NULL){
//                printf("\n Ingrese el ID de la tarea que se completo: ");
//                fflush(stdin);
//                scanf("%d",&idcompletado);
//                completarTarea(&tareasPendientes,&tareasRealizadas,idcompletado);
//            }else{
//                printf("\nNo hay tareas en la lista de pendientes");
//            }
//            break;
//        case 4:
//            /* 4-buscar tarea por ID */
//            printf("\n Ingrese el ID que desea buscar: ");
//            fflush(stdin);
//            scanf("%d",&idBuscado);
//            tareaEncontrada = buscarTareaId(tareasPendientes, idBuscado);
//            if (tareaEncontrada != NULL){
//                printf("\nTarea encontrada en la lista de Tareas Pendientes");
//                mostrarTarea(tareaEncontrada->T);
//            }else{
//                tareaEncontrada = buscarTareaId(tareasRealizadas,idBuscado);
//                if (tareaEncontrada != NULL){
//                    printf("\nTarea encontrada en la lista de Tareas Realizadas");
//                    mostrarTarea(tareaEncontrada->T);
//                }else{
//                    printf("\nNo se encontro ninguna tarea con ese ID\n");
//                }
//            }
//            break;
//        case 5:
//            /* 5-buscar tarea por palabra clave */
//            printf("\n Ingrese la palabra clave que desea buscar: ");
//            fflush(stdin);
//            gets(buff);
//            tareaEncontrada = buscarTareaClave(tareasPendientes, buff);
//            if (tareaEncontrada != NULL){
//                printf("\nTarea encontrada en la lista de Tareas Pendientes");
//                mostrarTarea(tareaEncontrada->T);
//            }else{
//                tareaEncontrada = buscarTareaClave(tareasRealizadas, buff);
//                if (tareaEncontrada != NULL){
//                    printf("\nTarea encontrada en la lista de Tareas Realizadas");
//                    mostrarTarea(tareaEncontrada->T);
//                }else{
//                    printf("\nNo se encontro ninguna tarea con esa descripcion\n");
//                }
//            }
//            break;
//        }
//        //if (opc != 6)
//        //{
//        //    printf("\n\n**Desea realizar otra operacion?(s/n)**\n");
//        //    fflush(stdin);
//        //    opc=getchar();
//        //}
//        
//    } while (opc != 6);
    free(buff);
    liberarMemoria(tareasPendientes,tareasRealizadas);
    free(id);
    return 0;
}
