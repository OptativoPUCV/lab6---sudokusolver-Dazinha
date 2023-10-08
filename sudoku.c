#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

/*
2.Cree la función **int is_valid(Node * n)**, para validar si un estado/nodo es válido (cumple con las restricciones del problema). Debe validar que:

- No se repitan números en las filas
- No se repitan números en las columnas
- No se repitan números en las submatrices de 3x3

Si el estado es válido la función retorna 1, si no lo es retorna 0.

> Para marcar los números que vayan apareciendo en una fila/columna/submatriz puede usar un arreglo de enteros de largo 10 inicializado con 0s. Cada vez que aparezca un número i, verifique que la casilla i del arreglo sea igual a 0, luego márquela con un '1'. Si la casilla es '1' quiere decir que el número ya estaba marcado por lo que la fla/columna/submatriz no es válida.
*/


int is_valid(Node* n){
  
  int used[10] = {0}; 

    // Verificar filas y columnas
  for (int i = 0; i < 9; i++) {
    memset(used, 0, sizeof(used)); // Reiniciar el arreglo used
    for (int j = 0; j < 9; j++) {
      int num = n->sudo[i][j];
      if (num != 0) {
        if (used[num] == 1) {
          return 0; // Número repetido en la fila
        }
        used[num] = 1;
      }
    }

    memset(used, 0, sizeof(used)); // Reiniciar el arreglo used
    for (int j = 0; j < 9; j++) {
      int num = n->sudo[j][i];
      if (num != 0) {
        if (used[num] == 1) {
          return 0; // Número repetido en la columna
        }
        used[num] = 1;
      }
    }
  }

  // Verificar submatrices de 3x3
  for (int row = 0; row < 9; row += 3) {
    for (int col = 0; col < 9; col += 3) {
      memset(used, 0, sizeof(used)); // Reiniciar el arreglo used
      for (int i = row; i < row + 3; i++) {
        for (int j = col; j < col + 3; j++) {
          int num = n->sudo[i][j];
          if (num != 0) {
            if (used[num] == 1) {
              return 0; // Número repetido en la submatriz
            }
            used[num] = 1;
          }
        }
      }
    }
  }
  
  return 1; 
  
}


/*
1.Cree una función que a partir de un nodo genere una **lista con los nodos adyacentes**:

Recuerde que los nodos adyacentes son generados aplicando las acciones al estado actual.

> - Para el caso del ejemplo, la función debería retornar una lista con **9 nodos**. Cada uno de ellos idéntico al nodo original pero cambiando el valor de la primera casilla vacía, es decir: sudo[0][2], por 1, 2, ..., 9.
> - Utilice la función Node* copy(Node* n) para copiar nodos.
*/


List* get_adj_nodes(Node* n){
  
  List* list=createList();
  /*
    obtenga los nodos adyacentes a n
    y agréguelos a la lista
  */

  for (int i = 0 ; i < 9 ; i++)
    {
      for (int j = 0 ; j < 9 ; j++)
        {
          if (n -> sudo[i][j] == 0) //si tengo un 0
          {
            
            for (int k = 1; k <= 9 ; k++)
              {
                
                Node *ady_node = copy(n);
                ady_node -> sudo[i][j] = k; //cambio el 0
                pushBack(list, ady_node);
               /*
                if (is_valid(ady_node) == 1)
                {
                  pushBack(list, ady_node);
                }
                else
                {
                  free(ady_node); 
                }
                */
              
              }

          
          }
        }
    }

  
    return list;
}

/*
3.Modifique la función *get_adj_nodes* para que sólo los nodos válidos sean retornados (use la función *is_valid*).

4.Implemente la función **int is_final(Node * n)**. Esta función retorna 1 si el nodo corresponde a un nodo final (es decir, todos los valores de la matriz son distintos a 0) y 0 en caso contrario.
*/

int is_final(Node* n){

  for (int i = 0 ; i < 9 ; i++)
    {
      for (int j = 0 ; j < 9 ; j++)
        {
          if (n -> sudo[i][j] == 0) //si son distintos a 0
          {
            return 0;
          }
        }
    }
  
    return 1;
}


/*
5.Implemente la función Node* DFS(Node* n, int* cont). Esta función realiza una búsqueda en profundidad a partir del nodo n. El algoritmo es el siguiente:

1. Cree un stack S (pila) e inserte el nodo.
2. Mientras el stack S no se encuentre vacío:

   a) Saque y elimine el primer nodo de S.
   
   b) Verifique si corresponde a un estado final, si es así retorne el nodo.
   
   c) Obtenga la lista de nodos adyacentes al nodo.
   
   d) Agregue los nodos de la lista (uno por uno) al stack S.
   
   e) Libere la memoria usada por el nodo.
   
3. Si terminó de recorre el grafo sin encontrar una solución, retorne NULL.
Almacene en la variable cont, la cantidad de iteraciones que realiza el algoritmo.
*/

Node* DFS(Node* initial, int* cont){
  //Cree un stack S (pila) e inserte el nodo.
  Stack* pila = createStack();
  push(pila, initial);

  //Mientras el stack S NO se encuentre vacío:
  while (!is_empty(pila)) 
  { 
    (*cont)++;
    //Saque y elimine el primer nodo de S.
    Node* current = top(pila); 
    pop(pila);

//Verifique si corresponde a un estado final, si es así retorne el nodo.
    if (is_final(current)) 
    {
      return current; 
    }

    //Obtenga la lista de nodos adyacentes al nodo
    List* adj_nodes = get_adj_nodes(current);

    //Agregue los nodos de la lista (uno por uno) al stack S.
    void* adj_node = first(adj_nodes);
    
    while (adj_node != NULL) 
    {
      Node* vecino_node = (Node*)adj_node;
      push(pila, vecino_node);
      adj_node = next(adj_nodes);

    }
    //Libere la memoria usada por el nodo.
    clean(adj_nodes);
    
  }

  return NULL;
  
}


/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}
  */