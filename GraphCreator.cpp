#include <iostream>
#include <limits>
#include <cstdlib>
#include <string.h>

/* A Graph Creator (positive weighted edges, directed graphs)
 * It can search for the shortest path using Dijkstra's Algorithm 
 * (max of 20 vertices in table)
 * Uses an adjacency table for the graph and is printable
 * Author: Ryan Le
 * Date: 1/31/2021
 */


using namespace std;

#define SIZE 21

struct vertex {
  char* name;
  vertex(char* newVertexName) {
    name = newVertexName;
  }
};

struct Vertex_data {
  int Value;
  int Path;
  bool Visit;
  vertex* Vertex_item;

  Vertex_data(vertex* newVertex) {
    Value = std::rand();
    Path = 0;
    Visit = false;
    Vertex_item = newVertex;
  }
  Vertex_data() {
    Value = std::rand();
    Path = 0;
    Visit = false;
    Vertex_item = NULL;
  }
};

// Define functions.
void addVertex(vertex** Vlist);
void addEdge(vertex** Vlist, int** adjTable);
void print(vertex** Vlist, int** adjTable);
int  findIndex(char* name, vertex** Vlist);
void shortestPath(vertex** Vlist, int** adjTable);
void Dijkstra(Vertex_data** itemList, int** adjTable, int endIndex);
void delVertex(vertex** Vlist, int** adjTable);
void delEdge(vertex** Vlist, int** adjTable);

int main() {

  //Initialize vertex list and adjacency table
  vertex* Vlist[SIZE];
  for (int i=1; i<SIZE; i++) {
    Vlist[i] = NULL;
  }
 
  // init adjacency table
  int* adjTable[SIZE];
  for(int i=1; i<SIZE; i++) {
    adjTable[i] = new int[SIZE];
    for(int j=1; j<SIZE; j++) {
      adjTable[i][j] = -1;
    }
  }


  cout << "Graph Creator ... " << endl;
  char action[20];
  bool exit = 0;
  while (exit == 0) {
    cout << endl;
    cout << "Executable Actions: ADD VERTEX, ADD EDGE, DEL VERTEX, DEL EDGE, PATH, PRINT, QUIT" << endl;
    cin.get(action,50);
    cin.get();

    if (strcmp(action, "ADD VERTEX")==0 || strcmp(action, "add vertex")==0) {
      addVertex(Vlist);
    }
    else if (strcmp(action, "ADD EDGE")==0 || strcmp(action, "add edge")==0) {
      addEdge(Vlist, adjTable);
    }
    else if (strcmp(action, "PRINT")==0 || strcmp(action,"print")==0) {
   print(Vlist, adjTable);
    }
    else if (strcmp(action, "PATH")==0 || strcmp(action,"path")==0) {
       shortestPath(Vlist, adjTable);
    }
    else if (strcmp(action, "DEL VERTEX")==0 || strcmp(action, "del vertex")==0) {
       delVertex(Vlist, adjTable);
    }
    else if (strcmp(action, "DEL EDGE")==0 || strcmp(action, "del edge")==0) {
       delEdge(Vlist, adjTable);
    }
    else if (strcmp(action, "QUIT") == 0 || strcmp(action,"quit")==0) {
      exit = 1;
    }
    //If typed something else, try again
    else {
      cout << "Please try again." << endl;
    }
  }
 
  return 0;
}
 
void addVertex(vertex** Vlist) {
  // add vertex name onto Vertex list
  char* input = new char[20];
  cout << "Enter vertex name to add: ";
  cin.get(input, 20);
  cin.get();

  int index = 1;
  while(index < SIZE && (Vlist[index]!=NULL)) {
    index++;
  }
 
  if(index < SIZE) {
    Vlist[index] = new vertex(input);
  }
  else {
    cout << "Vertex list is exceed 20: " << index << endl;
    return;
  }
 
}

// Add edge to vertex for directed graph between 2 vertice.
void addEdge(vertex** Vlist, int** adjTable) {

  cout << "Add Vertex edge. Please enter 1st vertex name: ";
  char* firstVertex = new char[20];
  cin.get(firstVertex, 20);
  cin.get();

  // check if vertex exists
  if( findIndex(firstVertex, Vlist) == 0) {
    cout << "Vertex name is not existed: " << firstVertex << endl;
    return;
  }

  cout << "Add Vertex edge. Please enter 2nd vertex name: ";
  char* secVertex = new char[20];
  cin.get(secVertex, 20);
  cin.get();
  if(findIndex(secVertex, Vlist) == 0) {
    cout << "Vertex name is not existed: " << secVertex << endl;
    return;
  }
 
  // weight for the edge.
  cout << "Enter the weight edge: ";
  int weight;
  cin >> weight;
  cin.get();
  int index1 = findIndex(firstVertex, Vlist);
  int index2 = findIndex(secVertex, Vlist);
  adjTable[index1][index2] = weight;

}

// print for info and debug
void print(vertex** Vlist, int** adjTable) {
  cout << "Vertex List: " << endl;
  for(int i = 1; i < SIZE; i++) {
    if(Vlist[i] != NULL) {
      cout << i << " " << Vlist[i]->name << endl;
    }
  }
  cout << endl;
  cout << "Adjacency Table: " << endl;
  for(int i = 1; i < SIZE; i++) {
    for (int j = 1; j < SIZE; j++) {
      if(adjTable[i][j] != -1) {
    cout << "["<< i <<"]" << "[" << j << "] ";
    cout << "--> weight: " << adjTable[i][j] << endl;
      }
    }
  }
}


int findIndex(char* name, vertex** Vlist) {
  //Gets the index of a vertex based on the name by go through vertex list
  for (int i = 1; i < SIZE; i++) {
    if(Vlist[i] != NULL && strcmp(Vlist[i]->name, name) == 0) {
      return i;
    }
  }
  return 0;
}

//Shortestpath function calls Dijkstra algorithm func.
void shortestPath(vertex** Vlist, int** adjTable)  {
  //We get the user input about where to start and end in the next few lines
  cout << "Enter start vertex name: ";
  char input[20];
  cin.get(input, 20);
  cin.get();

  int startIndex = findIndex(input, Vlist);
  // cout << "Dijk startIndex " << startIndex << endl;
  if(startIndex == 0) {
    cout << "Start vertex name is not existed: " << input << endl;
    return;
  }


  cout << "Enter end vertex name: ";
  cin.get(input, 20);
  cin.get();
  int endIndex = findIndex(input, Vlist);
  //cout << "Dijk endIndex " << endIndex << endl;
  if(endIndex == 0) {
    cout << "End vertex name is not existed: " << input << endl;
    return;
  }

  //create a new Vertext_data itemList type and initialize it
  Vertex_data* itemList[SIZE];
  for(int i = 1; i < SIZE; i++) {
    itemList[i] = new Vertex_data(Vlist[i]);
  }
  itemList[startIndex]->Value = 0;
  itemList[startIndex]->Path = startIndex;

  //Call Dijkstra and print the path
  Dijkstra(itemList, adjTable, endIndex);

  int path = 0;
  while (itemList[endIndex]->Path != 0) {
    path *= 100;
    path += itemList[endIndex]->Path%100;
    itemList[endIndex]->Path /= 100;
  }
  itemList[endIndex]->Path = path;
  cout << "Length for shortest path:  " << itemList[endIndex]->Value << endl;
  cout << "Paths: " << endl;;
  while(itemList[endIndex]->Path != 0) {
    cout << Vlist[itemList[endIndex]->Path%100]->name << endl;
    itemList[endIndex]->Path = itemList[endIndex]->Path/100;
  }

}

//Dijkstra's Shortest Path First algorithm
void Dijkstra(Vertex_data** itemList, int** adjTable, int endIndex) {
 
  int smallValue =  std::rand();
  int smallIndex = 0;

  //Find the itemList the smallest value.  
  for(int i = 1; i < SIZE; i++) {
    if(itemList[i]->Vertex_item != NULL && itemList[i]->Value < smallValue && itemList[i]->Visit == false) {
      smallValue = itemList[i]->Value;
      smallIndex = i;
    }
  }

  //Done if this is the endIndex
  if(endIndex == smallIndex) {
    return;
  }
  //set visited to true
  itemList[smallIndex]->Visit = true;

  //loop through the graph table and update vertex value if sum of itemList value and adjTable is less vertex's
  for(int i = 1; i < SIZE; i++) {
    if(adjTable[smallIndex][i] != -1) {
      if((itemList[smallIndex]->Value + adjTable[smallIndex][i]) < itemList[i]->Value) {
    itemList[i]->Value = itemList[smallIndex]->Value + adjTable[smallIndex][i];
    itemList[i]->Path = itemList[smallIndex]->Path * 100 + i;
      }
    }
  }
 
 // recursively call until endIndex == smallIndex
 Dijkstra(itemList, adjTable, endIndex);
}


// Vertex deletion
void delVertex(vertex** Vlist, int** adjTable) {
  cout << "Enter vertex name for deletion:  ";
  char name[20];
  cin.get(name, 20);
  cin.get();  
  int index = findIndex(name, Vlist);
  cout << "index " << index << endl;
  if(index == 0) {
    cout << "Vertex name is not found: " << name << endl;
    return;
  }

  //reset vertext list and its adjacency table
  Vlist[index] = NULL;
  for(int i = 1; i < SIZE; i++) {
    adjTable[index][i] = -1;
    adjTable[i][index] = -1;
  }

}

// Edge deletion
void delEdge(vertex** Vlist, int** adjTable) {
  cout << "Enter 1st Vertex name to delete its edge: ";
  char first[20];
  cin.get(first, 20);
  cin.get();
  int Index_1 = findIndex(first, Vlist);
  if (Index_1 == 0) {
    cout << "Vertex name is not found" << first << endl;
    return;
  }  
  cout << "Enter 2nd Vertex name to delete its edge: ";
  char second[20];
  cin.get(second, 20);
  cin.get();
  int Index_2 = findIndex(second, Vlist);
  if (Index_2 == 0) {
    cout << "Vertex name is not found" << second << endl;
    return;
  }
 
  adjTable[Index_1][Index_2] = -1;
}

