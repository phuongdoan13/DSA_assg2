/*
* A main function for you to build and run your
* own tests with.
* This file is not part of the marking, so you
* can do anything you want here.
*/
#include <iostream>

#include "directed_graph_algorithms.cpp"

int main() {
    vertex<int> vA = vertex<int>(0, 800);
 	vertex<int> vB = vertex<int>(10, 300);
	vertex<int> vC = vertex<int>(20, 400);
	vertex<int> vD = vertex<int>(30, 710);
	vertex<int> vE = vertex<int>(40, 221);
	vertex<int> v5 = vertex<int>(50, 500);
	vertex<int> v6 = vertex<int>(60, 600);
	vertex<int> v7 = vertex<int>(70, 700);
	
	directed_graph<int> g = directed_graph<int>();
	
	g.add_vertex(vA);
	g.add_vertex(vB);
	g.add_vertex(vC);
	g.add_vertex(vD);
	g.add_vertex(vE);
	
	g.add_edge(0, 10, 600);
	g.add_edge(0, 20, 900);
	g.add_edge(20, 30, 4000);
	g.add_edge(10, 40, 3000);
	g.add_edge(30, 20, 700);
	g.add_edge(30, 40, 500);
	g.add_edge(30, 0, 1);

	
	//g.add_edge(30, 10, 999);
	vector<int> a = dijkstra_prev(g, 1);
	for(int i = 0; i < a.size() ; i ++){
		cout << a[i] << endl;
	}

	//cout <<g.v_weight[20];
	cout << low_cost_delivery(g, 10) << endl;
	
	
}
