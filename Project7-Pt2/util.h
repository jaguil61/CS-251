/*util.h*/

#pragma once

//
// << Jose M. Aguilar (jaguil61) >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #07: open street maps, graphs, and Dijkstra's alg
// 

#include <iostream>
#include <iomanip>  /*setprecision*/
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <limits>

#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"
#include "graph.h"

using namespace std;
using namespace tinyxml2;

const double INF = numeric_limits<double>::max();
bool findBuilding(string userInput, vector<BuildingInfo>& Buildings, string& buildingFull, Coordinates& buildCoords);
void findNode(Coordinates buildingCoords, vector<FootwayInfo>& Footways, map<long long, Coordinates>& Nodes, Coordinates& footwayCoords);
vector<long long> Dijkstra(graph<long long, double>& G, long long startV, map<long long, double>& distances, map<long long, long long> & predecessors);
void printPath(map<long long, long long>& predecessors, long long startNodeID, long long destNodeID);
