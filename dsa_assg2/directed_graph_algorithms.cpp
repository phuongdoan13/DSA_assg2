#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <array>
#include <list>
#include <forward_list>
#include <deque>
#include <map>
#include <cstddef>
#include <string>
#include <utility>
#include <algorithm>
#include <limits>
#include <optional>
#include <exception>
#include <stdexcept>
#define NIL -1
#define INF 9999
#include "directed_graph.hpp"

using namespace std;

/*
 * Computes the shortest distance from u to v in graph g.
 * The shortest path corresponds to a sequence of vertices starting from u and ends at v,
 * which has the smallest total weight of edges among all possible paths from u to v.
 */
template <typename T>
vector<int> dijkstra_prev(directed_graph<T> g, int source_idx_vlist){
	// initialise vectors whose indices correspond with the indices of g.v_list
	vector<int> prev (g.num_vertices(), NIL); // the predecessor node's v_list_index
	vector<T> dist (g.num_vertices(), INF); // the shortest distance from the source node
	
	//initialise the source node distrance from itself
	dist.at(source_idx_vlist) = 0;
	
	// create vector that contains the idx_vlist of the unvisited vertices and their current shortest distrance from the source.
	// unvisited<<idx_vlist, dist.at(idx_vlist)>>
	// this vector is meant to maintain the ascending order of shortest distance by lambda function cmp
	vector<pair<int, T>> unvisited;
	for(int i = 0; i < g.num_vertices(); i++){
		if(i != source_idx_vlist)
			unvisited.push_back(make_pair(i, INF));
		else
			unvisited.push_back(make_pair(i, 0)); // the source
	}
	
	auto cmp = [](pair<int, T> left, pair<int, T> right){return left.second < right.second;};
	
	sort(unvisited.begin(), unvisited.end(), cmp);
	//cout << endl;
	//for(auto & it: unvisited){
	//	cout << it.first << " " << it.second << endl;
	//}
	// Dijkstra Algorithm
	while(!unvisited.empty()){
		int curr_idx_vlist = unvisited.begin() -> first;
		int curr_id = g.v_list.at(curr_idx_vlist).id;
		unvisited.erase(unvisited.begin());
		
		for(auto & adj_v : g.adj_list.at(curr_id)){
			int adj_idx_vlist = g.index_vlist(adj_v.first);
			int adj_id = adj_v.first;
			
			T temp_dist = dist.at(curr_idx_vlist) + g.get_edge_weight(curr_id, adj_id);
			
			if(temp_dist < dist.at(adj_idx_vlist)){
				prev.at(adj_idx_vlist) = curr_idx_vlist; // change the predecessor;
				
				dist.at(adj_idx_vlist) = temp_dist; // change the shortest distance in vector dist;
				for(auto & p : unvisited)			// change the shortest distance in vector unvisited;
					if(p.first == adj_idx_vlist){
						p.second = temp_dist;
						break;
					}
			}
			
		}
		
		sort(unvisited.begin(), unvisited.end(), cmp);
		//cout << endl;
		//for(auto & it: unvisited){
		//	cout << it.first << " " << it.second << endl;
		//}
	}
	return prev;
}

template <typename T>
vector<vertex<T>> shortest_path(directed_graph<T> g, int u_id, int v_id) {
	vector<vertex<T>> spath;
	
	// get the g.v_list index of the source and destination node
	int source_idx_vlist = g.index_vlist(u_id);
	int dest_idx_vlist = g.index_vlist(v_id);

	vector<int> prev = dijkstra_prev(g, source_idx_vlist);
	
	// trace back to get the path
	int curr_idx_vlist = dest_idx_vlist;
	while(prev.at(curr_idx_vlist) != NIL){
		spath.insert(spath.begin(), g.v_list.at(curr_idx_vlist));
		curr_idx_vlist =prev.at(curr_idx_vlist);
	}
	spath.insert(spath.begin(), g.v_list.at(source_idx_vlist));
	
	return spath;

}

/*
 * Computes the strongly connected components of the graph.
 * A strongly connected component is a subset of the vertices
 * such that for every pair u, v of vertices in the subset,
 * v is reachable from u and u is reachable from v.
 */

template <typename T>
void SCCUtil(int idx_vlist, vector<int>& disc, vector<int>& low, stack<int>& S, vector<bool>& is_S_member, vector<vector<vertex<T>>>& scc, directed_graph<T>& g, int& time){
	
	// Initialize discovery time and low value
	disc.at(idx_vlist) = low.at(idx_vlist) = ++time;
	S.push(idx_vlist);
	is_S_member.at(idx_vlist) = true;
	
	// Go through all vertices adjacent to this
	typename map<int, T>::iterator it;
	for (it = g.adj_list.at(g.v_list.at(idx_vlist).id).begin(); it != g.adj_list.at(g.v_list.at(idx_vlist).id).end(); ++it){
		int adj_idx_vlist = g.index_vlist(it -> first);  // v is current adjacent of 'u'
	
		// If adj vertex is not visited yet, then recur for it
		if (disc.at(adj_idx_vlist) == -1){
			SCCUtil(adj_idx_vlist, disc, low, S, is_S_member,scc,g, time);
	
			low.at(idx_vlist)  = min(low.at(idx_vlist), low.at(adj_idx_vlist));
		}
	
		// Update low value of 'u' only of 'v' is still in stack
		// (i.e. it's a back edge, not cross edge).
		else if (is_S_member.at(adj_idx_vlist) == true)
			low.at(idx_vlist)  = min(low.at(idx_vlist), disc.at(adj_idx_vlist));
	}
	
	// head node found
	int idx_vlist_from_stack = 0;  // To store stack extracted vertices
	if (low.at(idx_vlist) == disc.at(idx_vlist)){
		vector<vertex<T>> component;
		while (S.top() != idx_vlist){
			idx_vlist_from_stack = (int) S.top();
			//cout << g.v_list.at(idx_vlist_from_stack).id << " ";
			component.push_back(g.v_list.at(idx_vlist_from_stack));
			is_S_member.at(idx_vlist_from_stack) = false;
			S.pop();
		}
		
		idx_vlist_from_stack = (int) S.top();
		component.push_back(g.v_list.at(idx_vlist_from_stack));
		is_S_member.at(idx_vlist_from_stack) = false;
		S.pop();
		scc.push_back(component);
	}
}

template <typename T>
vector<vector<vertex<T>>> strongly_connected_components(directed_graph<T>& g) {
	vector<vector<vertex<T>>> scc;
	
	vector<int> disc (g.num_vertices(), -1);
	vector<int> low  (g.num_vertices(), -1);
	stack<int> S;
	vector<bool> is_S_member (g.num_vertices(), false);
	int time = 0;
	
	for(int idx_vlist = 0; idx_vlist < g.num_vertices(); idx_vlist++){
		if(disc[idx_vlist] == NIL)
			SCCUtil(idx_vlist, disc, low, S, is_S_member, scc, g, time);
	}
  return scc;

}


/*
 * Computes a topological ordering of the vertices.
 * For every vertex u in the order, and any of its
 * neighbours v, v appears later in the order than u.
 * You will be given a DAG as the argument.
 */
template <typename T>
void topological_sort_util(int idx_vlist, directed_graph<T>& g, stack<int>& s, vector<bool>& visited){
	
	// Mark the node at this index in v_list as visited
	visited.at(idx_vlist) = true;
	
	int curr_id = g.v_list.at(idx_vlist).id; // current id
	
	typename map<int, T>::iterator it;
	for(it  = g.adj_list.at(curr_id).begin(); it != g.adj_list.at(curr_id).end(); ++it){
		int adj_idx_vlist = g.index_vlist(it -> first);
		if (!visited.at(adj_idx_vlist))
			topological_sort_util(adj_idx_vlist, g, s, visited);
	}
	
	s.push(idx_vlist);
	
}

template <typename T>
vector<vertex<T>> topological_sort(directed_graph<T>& g) {
	vector<vertex<T>> visited_vertex;
	
	stack<int> s_idx_vlist; // stack that contains the INDEX of the g.v_list
	vector<bool> visited_idx_vlist (g.num_vertices());
	
	for(int idx_vlist = 0; idx_vlist < g.num_vertices(); ++idx_vlist){
		if(!visited_idx_vlist.at(idx_vlist))
			topological_sort_util(idx_vlist, g, s_idx_vlist, visited_idx_vlist);
	}
	
	while (!s_idx_vlist.empty())
    {
		
		visited_vertex.push_back(g.v_list.at(s_idx_vlist.top()));
        s_idx_vlist.pop();
    }
	return visited_vertex;
}


/*
 * Computes the lowest cost-per-person for delivery over the graph.
 * u is the source vertex, which send deliveries to all other vertices.
 * vertices denote cities; vertex weights denote cities' population;
 * edge weights denote the fixed delivery cost between cities, which is irrelevant to
 * the amount of goods being delivered.
 */
template <typename T>
T low_cost_delivery(directed_graph<T> g, int u_id) {
	int u_idx_vlist = g.index_vlist(u_id); // get the index of u in vlist
	
	vector<int> prev = dijkstra_prev(g, u_idx_vlist); // do dijkstra on the graph and get the predecessor list
	
	directed_graph<T> span_tree;
	span_tree.add_vertex(g.v_list.at(u_idx_vlist)); // add the source node to the spanning treee
	
	T total_nodes_weight = 0;
	
	for(int idx_vlist = 0; idx_vlist < prev.size(); idx_vlist++){
		if(prev.at(idx_vlist) != NIL){
			vertex<T> vertex_at_idx_vlist = g.v_list.at(idx_vlist);
			span_tree.add_vertex(vertex_at_idx_vlist);
			total_nodes_weight += g.v_weight.at(vertex_at_idx_vlist.id);
		}
	}

	if(total_nodes_weight == 0)
		return 0;

	// trace back to get the path
	for(int idx_vlist = 0; idx_vlist < prev.size(); idx_vlist++){
		if(prev.at(idx_vlist) != NIL){ //check if this node is the source node or unreachable nodes from the source
			int current_node_id = g.v_list.at(idx_vlist).id;
			int parent_node_id = g.v_list.at(prev.at(idx_vlist)).id;
			span_tree.add_edge(parent_node_id, current_node_id, g.get_edge_weight(parent_node_id, current_node_id));
			
		}
	}
	
	T total_paths_weight = span_tree.get_total_edges_weight();

	return total_paths_weight/total_nodes_weight;

}
