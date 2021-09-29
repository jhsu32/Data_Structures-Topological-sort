#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <list>
#include <queue>
#include <sstream>
#include <fstream>

using namespace std;

//edits to this are likely not needed
template <class T>
struct Vertex {
  T label; // unique int for a vertex
  vector<T> adj_list;
  int indegree; // Part 2: number of nodes pointing in
  int top_num; // Part 2: topological sorting number
  Vertex(T l) : label(l) {top_num = 0;} //Part 1
  // You may include default constructor optionally.
};

// optional, but probably helpful
template <class T>
ostream& operator<<(ostream &o, Vertex<T> v){
  o << v.label << " : ";
  for(int i = 0; i < v.adj_list.size(); i++){
        o << v.adj_list[i] << " ";
  }
  //o << endl;
  return o;
};



// std::priority_queue may take takes three template parameters: <Type, ContainerType, Functor>
// syntax for custom compare functor
// Refer the instructions PDF last section for more information

template<class T>
class VertexCompare
{
public:
  bool operator()(Vertex<T> v1, Vertex<T> v2){
    if(v1.top_num > v2.top_num){
      return true;
    }
    return false;
  }
};

template <class T>
class Graph {
private:
  //c++ stl hash table

  unordered_map<T, Vertex<T>> node_set;
  //Use other data fields if needed
public:
  Graph() {};  //No modification needed to default constructor. Can be used optionally if needed.
  ~Graph() {}; //No modification needed to destructor. Can be used optionally if needed.

  // build a graph - refer the instructions PDF for more information.
  void buildGraph(istream &input){
    string inputstream = "";
    T token;
    bool exist = false;
    while(!input.eof()){
        getline(input, inputstream);   //getline from the input file
        if(input.fail() || inputstream.length() == 0){
          //input.clear();
          continue;
        }
        std::stringstream ss(inputstream);
        T label;   
        ss >> label;
        Vertex<T> new_vertex(label);   //create a new vertex
        while(ss >> token){    //read in the adjacent vertices
          new_vertex.adj_list.push_back(token);
        }

        for(std::pair<T, Vertex<T>> element : node_set){  //check if the vertex already exist in the node_set
          if(element.second.label == label){
            exist = true;
          }
        }
        if(exist){       //continue if already exist in the map
          continue;
          exist = false;
        }else{  
          new_vertex.indegree = 0;         //append to the map if does not exist in the map
          node_set.emplace(label, new_vertex);
        }
    }

  }

  // display the graph into o - refer the instructions PDF for more information.
  void displayGraph(ostream& o){
    for(auto element : node_set){
      o << element.second << endl;
    }
  }

  //return the vertex at label, else throw any exception  - refer the instructions PDF for more information.
  Vertex<T> at(T label){
    try{
      return node_set.at(label);
    }
    catch(std::out_of_range& ){
      throw std::runtime_error("The vertex does not exist in the graph.");
    }

  }

  //return the graph size (number of verticies)
  int size(){
    return node_set.size();
  }

  // topological sort
  // return true if successful, false on failure (cycle)
  bool topological_sort(){
      queue<Vertex<T>> q;
      int counter = 0;

      for(auto element : node_set){
        if(element.second.indegree == 0){
          q.push(element.second);
        }
      }

      while(!q.empty()){
        Vertex<T> v = q.front();
        q.pop();
        node_set.at(v.label).top_num = ++counter;

        for(int i = 0; i < v.adj_list.size(); i++){
          if(--node_set.at(v.adj_list[i]).indegree == 0){
            q.push(node_set.at(v.adj_list[i]));
          }
        }
      }

      if( counter != node_set.size()){//found a cycle in the graph
        return false;
      }
      return true;
  }; // Part 2

  // find indegree
  void compute_indegree(){
    for(auto& [key, value]: node_set){
      value.indegree = 0;
    }
    for(auto& [key, value]: node_set){
      for(int i = 0; i < value.adj_list.size(); i++){
        //cout << value.adj_list[i] << endl;
        node_set.at(value.adj_list[i]).indegree++;
      }
      //cout << endl;
    }
  }; // Part 2


  // print topological sort into o
  //  if addNewline is true insert newline into stream
  void print_top_sort(ostream& o, bool addNewline=true){
    priority_queue<Vertex<T>, vector <Vertex <T>>, VertexCompare <T>> pq; 
    for(auto& [key, value] : node_set){  //c++17 style 
      pq.push(value);
    }
    while(!pq.empty()){
      o << pq.top().label << " ";
      pq.pop();
    }
    if(addNewline){
      o << std::endl;
    }else{
      o << flush;
    }
  }; // Part 2
};

#endif
