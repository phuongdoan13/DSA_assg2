#ifndef DIRECTED_GRAPH_H
#define DIRECTED_GRAPH_H

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<queue>
#include<stack>
#include<set>
#include<algorithm>
// include more libraries here if you need to

using namespace std; // the standard namespace are here just in case.

/*
	the vertex class
*/
template <typename T>
class vertex {

public:
    int id;
    T weight;
    //vector<int> in_v_id;
    vertex(int x, T y) {
        this -> id = x;
        this -> weight = y;
    }

    // add more functions here if you need to
};

/*
	the graph class
*/
template <typename T>
class directed_graph {

public:

    vector<vertex<T>> v_list;
    map<int, map<int, T>> adj_list;
    map<int, T> v_weight;



    directed_graph(); //A ructor for directed_graph. The graph should start empty.
    ~directed_graph(); //A destructor. Depending on how you do things, this may not be necessary.

    bool contains( int) ; //Returns true if the graph contains the given vertex_id, false otherwise.
    bool adjacent( int,  int) ; //Returns true if the first vertex is adjacent to the second, false otherwise.

    void add_vertex( vertex<T>); //Adds the passed in vertex to the graph (with no edges).
    void add_edge( int,  int,  T); //Adds a weighted edge from the first vertex to the second.

    void remove_vertex( int); //Removes the given vertex. Should also clear any incident edges.
    void remove_edge( int,  int); //Removes the edge between the two vertices, if it exists.

    size_t in_degree( int) ; //Returns number of edges coming in to a vertex.
    size_t out_degree( int) ; //Returns the number of edges leaving a vertex.
    size_t degree( int) ; //Returns the degree of the vertex (both in edges and out edges).

    size_t num_vertices() ; //Returns the total number of vertices in the graph.
    size_t num_edges() ; //Returns the total number of edges in the graph.

    vector<vertex<T>> get_vertices(); //Returns a vector containing all the vertices.
    vector<vertex<T>> get_neighbours( int); //Returns a vector containing all the vertices reachable from the given vertex. The vertex is not considered a neighbour of itself.
    vector<vertex<T>> get_second_order_neighbours( int); // Returns a vector containing all the second_order_neighbours (i.e., neighbours of neighbours) of the given vertex.
    // A vector cannot be considered a second_order_neighbour of itself.
    bool reachable( int,  int) ; //Returns true if the second vertex is reachable from the first (can you follow a path of out-edges to get from the first to the second?). Returns false otherwise.
    bool contain_cycles() ; // Return true if the graph contains cycles (there is a path from any vertices directly/indirectly to itself), false otherwise.

    vector<vertex<T>> depth_first( int); //Returns the vertices of the graph in the order they are visited in by a depth-first traversal starting at the given vertex.
    vector<vertex<T>> breadth_first( int); //Returns the vertices of the graph in the order they are visisted in by a breadth-first traversal starting at the given vertex.

    directed_graph<T> out_tree( int); //Returns a tree starting at the given vertex using the out-edges. This means every vertex in the tree is reachable from the root.

    vector<vertex<T>> pre_order_traversal( int, directed_graph<T>&); // returns the vertices in the visiting order of a pre-order traversal of the tree starting at the given vertex.
    vector<vertex<T>> in_order_traversal( int, directed_graph<T>&); // returns the vertices in the visiting order of an in-order traversal of the tree starting at the given vertex.
    vector<vertex<T>> post_order_traversal( int, directed_graph<T>&); // returns the vertices in ther visitig order of a post-order traversal of the tree starting at the given vertex.

    vector<vertex<T>> significance_sorting(); // Return a vector containing a sorted list of the vertices in descending order of their significance.
    int index_vlist( int) ; // find the index of the vertex in v_list by its id
    vertex<T>* v_vlist(int); //find the vertex in vlist by its id
    bool contain_cycles_util(int, vector<bool>&, vector<bool>&);
	T get_edge_weight(int, int);
    T get_total_edges_weight();
};

// Define all your methods down here (or move them up into the header, but be careful you don't double up). If you want to move this into another file, you can, but you should #include the file here.
// Although these are just the same names copied from above, you may find a few more clues in the full method headers.
// Note also that C++ is sensitive to the order you declare and define things in - you have to have it available before you use it.

template <typename T>
T directed_graph<T>::get_edge_weight(int u_id, int v_id){
	return adj_list.at(u_id).at(v_id);
}

template <typename T>
T directed_graph<T>::get_total_edges_weight(){
	T total_edges_weight = 0;
    for(auto & it : adj_list){
        for(auto & edge: it.second){
            total_edges_weight += edge.second;
        }
    }
    return total_edges_weight;
}

template <typename T>
int directed_graph<T>::index_vlist( int u_id)  {
    for(int i = 0; i < v_list.size(); i++){
        if(v_list[i].id == u_id) return i ;
    }
    return -1;
}

template <typename T>
vertex<T>* directed_graph<T>::v_vlist(int u_id)  {
    for(int i = 0; i < v_list.size(); i++){
        if(v_list.at(i).id == u_id) return &v_list.at(i) ;
    }
    return nullptr;
}

template <typename T>
directed_graph<T>::directed_graph() {}

template <typename T>
directed_graph<T>::~directed_graph() {}

template <typename T>
bool directed_graph<T>::contains( int u_id)  {
    return adj_list.find(u_id) != adj_list.end();

}

template <typename T>
bool directed_graph<T>::adjacent( int u_id,  int v_id)  {
    if(contains(u_id) && contains(v_id)){
        return adj_list.at(u_id).find(v_id) !=  adj_list.at(u_id).end();
    }
    return false;
}

template <typename T>
void directed_graph<T>::add_vertex( vertex<T> u) {
    if(!contains(u.id)){
        v_list.push_back(u);
        adj_list[u.id] =  map<int, T>();
        v_weight[u.id] =  u.weight;
    }
}

template <typename T>
void directed_graph<T>::add_edge( int u_id,  int v_id,  T weight) {
    if(contains(u_id) && contains(v_id)){
        if(adj_list[u_id].find(v_id) == adj_list[u_id].end()){
            adj_list[u_id][v_id] = weight;
            //v_list[index_vlist(v_id)].in_v_id.push_back(u_id);
        }
    }
}

template <typename T>
void directed_graph<T>::remove_vertex( int u_id) {
    for(int i = 0; i < v_list.size(); i++){
        if(v_list.at(i).id == u_id){
            v_list.erase(v_list.begin() +i);
            break;
        }
    }
    adj_list.erase(adj_list.find(u_id));
    v_weight.erase(v_weight.find(u_id));

    // delete the in_edge to the erased vertex
    for(auto & v : adj_list){ // for each source vertex in adj_list
        typename map<int, T>::iterator it = v.second.find(u_id);
        if(it != v.second.end()){
            v.second.erase(it);
        }
    }
}

template <typename T>
void directed_graph<T>::remove_edge( int u_id,  int v_id) {
    if(contains(u_id)){
        adj_list.at(u_id).erase(v_id);
    }
}

template <typename T>
size_t directed_graph<T>::in_degree( int u_id)  {
    int in_degree_u = 0;
    for(auto v : adj_list){
        if(v.first != u_id){
            for(auto adj_v : adj_list.at(v.first)){
                if(adj_v.first == u_id){
                    in_degree_u += 1;
                    break;
                }
            }
        }
    }
    return in_degree_u;
}

template <typename T>
size_t directed_graph<T>::out_degree( int u_id)  {

    return (adj_list.at(u_id)).size();
}

template <typename T>
size_t directed_graph<T>::degree( int u_id)  { return in_degree(u_id) + out_degree(u_id); }

template <typename T>
size_t directed_graph<T>::num_vertices()  { return v_list.size(); }

template <typename T>
size_t directed_graph<T>::num_edges()  {
    size_t no_edges = 0;
    for(auto & v : v_list){
        no_edges += out_degree(v.id);
    }
    return no_edges;
}

template <typename T>
vector<vertex<T>> directed_graph<T>::get_vertices() { return v_list; } //Done

template <typename T>
vector<vertex<T>> directed_graph<T>::get_neighbours( int u_id) {
    // O(n * m)
    // Using basic bfs
    vector<vertex<T>> visited_vertex;
    queue<int> q_idx_vlist;
    vector<bool> visited_idx_vlist (this -> num_vertices());

    int u_idx_vlist = index_vlist(u_id);
    q_idx_vlist.push(u_idx_vlist);
    visited_idx_vlist.at(u_idx_vlist) = true;


    while(!q_idx_vlist.empty()){
        int current_idx_vlist = q_idx_vlist.front();
        q_idx_vlist.pop();


        for(auto & adj_v : adj_list.at(v_list.at(current_idx_vlist).id)){
            int adj_idx_vlist = index_vlist(adj_v.first);
            if(!visited_idx_vlist[adj_idx_vlist]){
                visited_idx_vlist.at(adj_idx_vlist) = true;
                visited_vertex.push_back(v_list.at(adj_idx_vlist));
            }
        }
    }

    return visited_vertex;
}

template <typename T>
vector<vertex<T>> directed_graph<T>::get_second_order_neighbours( int u_id) {

    vector<vertex<T>> second_order;
    vector<bool> visited_idx_vlist (this -> num_vertices());
    if(contains(u_id)){
        visited_idx_vlist.at(index_vlist(u_id)) = true; //mark the u vertex at visited
        for(auto adj_u: adj_list.at(u_id)){ // for each neighbour of u
            for(auto adj_adj_u: adj_list.at(adj_u.first)){ // for each neighbour of neighbours of u
                if(!visited_idx_vlist.at(index_vlist(adj_adj_u.first))){
                    visited_idx_vlist.at(index_vlist(adj_adj_u.first)) = true;
                    second_order.push_back(*v_vlist(adj_adj_u.first));
                }
            }
        }
    }
    return second_order;
}

template <typename T>
bool directed_graph<T>::reachable( int u_id,  int v_id)  {
    queue<int> q_idx_vlist;
    vector<bool> visited_idx_vlist (this -> num_vertices());

    int u_idx_vlist = index_vlist(u_id);
    q_idx_vlist.push(u_idx_vlist);
    visited_idx_vlist.at(u_idx_vlist) = true;

    while(!q_idx_vlist.empty()){
        int current_idx_vlist = q_idx_vlist.front();
        q_idx_vlist.pop();

        if(v_list.at(current_idx_vlist).id == v_id){return true;}

        for(auto & adj_v : adj_list.at(v_list.at(current_idx_vlist).id)){
            int adj_idx_vlist = index_vlist(adj_v.first);
            if(!visited_idx_vlist[adj_idx_vlist]){
                q_idx_vlist.push(adj_idx_vlist);
                visited_idx_vlist.at(adj_idx_vlist) = true;
            }
        }
    }

    return false;
}

template <typename T>
bool directed_graph<T>::contain_cycles_util(int idx_vlist, vector<bool>& visited_vector, vector<bool>& recursive_vector) {
    visited_vector.at(idx_vlist) = true;
    recursive_vector.at(idx_vlist) = true;
    typename map<int, T>::iterator it;
    for(it = adj_list.at(v_list.at(idx_vlist).id).begin(); it != adj_list.at(v_list.at(idx_vlist).id).end(); it++){
        int adj_idx_vlist = index_vlist(it -> first);
        if(!visited_vector[adj_idx_vlist] && contain_cycles_util(adj_idx_vlist, visited_vector, recursive_vector)) return true;
        else if(recursive_vector[adj_idx_vlist]) return true;
    }

    recursive_vector.at(idx_vlist) = false;
    return false;
}

template <typename T>
bool directed_graph<T>::contain_cycles()  {
    vector<bool> visited_idx_vlist (this -> num_vertices());
    vector<bool> recursive_idx_vlist  (this -> num_vertices());
    for(int i = 0; i < v_list.size(); i++){
        if(contain_cycles_util(i, visited_idx_vlist, recursive_idx_vlist)) return true;
    }
    return false;

}

template <typename T>
vector<vertex<T>> directed_graph<T>::depth_first( int u_id) {
    vector<vertex<T>> visited_vertex;
    stack<int> s_idx_vlist;
    vector<bool> visited_idx_vlist (this -> num_vertices()); // the position of this vector corresponds to the position of the vertex in v_list

    int u_idx_vlist = index_vlist(u_id);
    s_idx_vlist.push(u_idx_vlist);

    while(!s_idx_vlist.empty()){
        int current_idx_vlist = s_idx_vlist.top();
        s_idx_vlist.pop();
        visited_vertex.push_back(v_list.at(current_idx_vlist));
        visited_idx_vlist.at(current_idx_vlist) = true;

        for(auto & adj_v : adj_list.at(v_list.at(current_idx_vlist).id)){
            int adj_idx_vlist = index_vlist(adj_v.first);
            if(!visited_idx_vlist[adj_idx_vlist]){
                s_idx_vlist.push(adj_idx_vlist);
            }
        }
    }

    return visited_vertex;
}

template <typename T>
vector<vertex<T>> directed_graph<T>::breadth_first( int u_id) {
    vector<vertex<T>> visited_vertex;
    queue<int> q_idx_vlist;
    vector<bool> visited_idx_vlist (this -> num_vertices()); // the positions of this vector corresponds to the position of the vertex in v_list

    int u_idx_vlist = index_vlist(u_id);
    q_idx_vlist.push(u_idx_vlist);
    visited_idx_vlist.at(u_idx_vlist) = true; // mark this vertex as visited
    visited_vertex.push_back(v_list.at(u_idx_vlist));

    while(!q_idx_vlist.empty()){
        int current_idx_vlist = q_idx_vlist.front();
        q_idx_vlist.pop();

        for(auto & adj_v : adj_list.at(v_list.at(current_idx_vlist).id)){
            int adj_idx_vlist = index_vlist(adj_v.first);
            if(!visited_idx_vlist[adj_idx_vlist]){
                q_idx_vlist.push(adj_idx_vlist);
                visited_idx_vlist.at(adj_idx_vlist) = true;
                visited_vertex.push_back(v_list.at(adj_idx_vlist));
            }
        }
    }

    return visited_vertex;
}

template <typename T>
directed_graph<T> directed_graph<T>::out_tree( int u_id) {
    // using basic bfs
    directed_graph<T> tree = directed_graph<T>();

    queue<int> q_idx_vlist;
    vector<bool> visited_idx_vlist (this -> num_vertices()); // the position of this vector corresponds to the position of the vertices in v_list

    int u_idx_vlist = index_vlist(u_id);
    q_idx_vlist.push(u_idx_vlist);
    visited_idx_vlist.at(u_idx_vlist) = true;

    while(!q_idx_vlist.empty()){
        int current_idx_vlist = q_idx_vlist.front();
        q_idx_vlist.pop();
        tree.add_vertex(v_list.at(current_idx_vlist));

        for(auto & adj_v : adj_list.at((v_list.at(current_idx_vlist)).id)){
            int adj_idx_vlist = index_vlist(adj_v.first);
            if(!visited_idx_vlist.at(adj_idx_vlist)){
                q_idx_vlist.push(adj_idx_vlist);
                visited_idx_vlist.at(adj_idx_vlist) = true;
                tree.add_vertex(v_list.at(adj_idx_vlist)); // add vertex
                tree.add_edge(v_list.at(current_idx_vlist).id, v_list.at(adj_idx_vlist).id, adj_v.second); // add edge
            }
        }
    }
    return tree;
}

template <typename T>
vector<vertex<T>> directed_graph<T>::pre_order_traversal(int u_id, directed_graph<T>& mst) {
    // using basic dfs
    vector<vertex<T>> visited_vertex;
    stack<int> s_idx_vlist;
    vector<bool> visited_idx_vlist (mst.num_vertices());

    int u_idx_vlist = mst.index_vlist(u_id);
    s_idx_vlist.push(u_idx_vlist);

    while(!s_idx_vlist.empty()){
        int current_idx_vlist = s_idx_vlist.top();
        s_idx_vlist.pop();
        visited_vertex.push_back(mst.v_list.at(current_idx_vlist));
        visited_idx_vlist.at(current_idx_vlist) = true;

        for(auto & adj_v : mst.adj_list.at(mst.v_list.at(current_idx_vlist).id)){
            int adj_idx_vlist = mst.index_vlist(adj_v.first);
            if(!visited_idx_vlist.at(adj_idx_vlist)){
                s_idx_vlist.push(adj_idx_vlist);
            }
        }
    }

    return visited_vertex;
}


template <typename T>
vector<vertex<T>> directed_graph<T>::in_order_traversal( int u_id, directed_graph<T>& mst) {
    // Reference: https://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
    //	1) Create an empty stack S.
    //	2) Initialize current node as root
    //	3) Push the current node to S and set current = current->first_child until current is NULL
    //	4) If current is NULL and stack is not empty then
    //		 a) Pop the top item from stack.
    //		 b) push the popped item, set current = popped_item->second_child
    //		 c) Go to step 3.
    //	5) If current is NULL and stack is empty then we are done.

    /*
    To tutor: I'm pretty sure that my logic for both in_order and post_order are correct (have a look at my test case). But I got "exited with code 1" for in_order so couldn't check if in_order and post_order were correct

    */
    vector<vertex<T>> visited_vertex ;
    stack<vertex<T>*> s;
    vertex<T>* curr = mst.v_vlist(u_id);

    while(curr != nullptr || !s.empty()){
        //go to the first child
        while(curr != nullptr){
            s.push(curr); //every time go through a vertex, add it to the stack
            if(mst.adj_list.at(curr->id).empty()){
                curr = nullptr; // if there is no child, stop this while loop. Since we are using map, one vertex must have left child before having right child.
            }else{
                typename map<int, T>::iterator left_it = mst.adj_list.at(curr->id).begin();
                curr = v_vlist(left_it -> first); // go the the left child
            }
        } // end left while
        // As this point, curr is null;

        curr = s.top();
        s.pop();
        visited_vertex.push_back(*curr);

        // go to the second child
		if(mst.adj_list.at(curr->id).size() <= 1){
            curr = nullptr;
        }else{
            typename map<int, T>::iterator right_it = ++mst.adj_list.at(curr->id).begin();
            curr = v_vlist(right_it -> first);
        }
    }
    return visited_vertex;

}

template <typename T>
vector<vertex<T>> directed_graph<T>::post_order_traversal( int u_id, directed_graph<T>& mst) {
    // Reference: https://www.geeksforgeeks.org/iterative-postorder-traversal/
    //	1. Push root to first stack.
    //	2. Loop while first stack is not empty
    //	   2.1 Pop a node from first stack and push it to second stack
    //	   2.2 Push left and right children of the popped node to first stack
    //	3. contents of second stack are sort according to post_order_traversal
    /*
        To tutor: I'm pretty sure that my logic for both in_order and post_order are correct (have a look at my test case). But I got "exited with code 1" for in_order so couldn't check if in_order and post_order were correct

    */
    vector<vertex<T>> visited_vertex;
    if(!mst.contains(u_id)) return visited_vertex;

    stack<vertex<T>*> s1, s2;
    vertex<T>* v = mst.v_vlist(u_id);
    s1.push(v); //	1. Push root to first stack.
    while(!s1.empty()){
        v = s1.top();
        s1.pop();
        s2.push(v); // 2.1 Pop a node from first stack and push it to second stack
        int num_child = mst.adj_list.at(v->id).size();
        // Since there is no such thing called left node and right node, we have to check the number of child.
        if(num_child == 1){
            typename map<int, T>::iterator it = mst.adj_list.at(v->id).begin();
            s1.push(v_vlist(it -> first)); // 2.2 Push left and right children of the popped node to first stack

        }else if(num_child == 2){
            typename map<int, T>::iterator it = mst.adj_list.at(v->id).begin();
            s1.push(v_vlist(it -> first));
            it++;
            s1.push(v_vlist(it -> first)); // 2.2 Push left and right children of the popped node to first stack
        }

    }

    while (!s2.empty()) {
        v = s2.top();
        s2.pop();
        visited_vertex.push_back(*v); // 3. contents of second stack are sort according to post_order_traversal
    }
    return visited_vertex;
}

template <typename T>
vector<vertex<T>> directed_graph<T>::significance_sorting() { // sort by vertex weight
    /*version 1
    vector<pair<int, T>> idx_vlist_and_weight; //contains <index of vertex in the v_list, weight of vertex>
    vector<vertex<T>> weight_descending_result;

    for(int i = 0; i < v_list.size(); i++){
        pair<int, T> p (i, v_list.at(i).weight);
        idx_vlist_and_weight.push_back(p);
    }

    auto cmp = [](pair<int, T> left, pair<int, T> right){return left.second > right.second;}; //sort ascending
    sort(idx_vlist_and_weight.begin(), idx_vlist_and_weight.end(), cmp);

    for(int i = 0; i < idx_vlist_and_weight.size(); i++){
        int idx_in_pair = idx_vlist_and_weight.at(i).first;
        weight_descending_result.push_back(v_list[idx_in_pair]);
    } // sort descending

    return weight_descending_result;
    */
    // version 2
    vector<vertex<T>>  weight_descending_result = v_list;
    auto cmp = [](vertex<T> left, vertex<T> right){return left.weight > right.weight;}; //lambda function that sorts two vertex
    sort(weight_descending_result.begin(), weight_descending_result.end(), cmp); // bubble sort
    return weight_descending_result;
}

#endif
