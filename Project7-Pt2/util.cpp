/*util.cpp*/

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
#include <utility> // pair<>
#include <queue>
#include <set>
#include <stack>

#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"
#include "graph.h"
#include "util.h"

using namespace std;
using namespace tinyxml2;

//
// prioratize (functor)
// 
// sorts data based on value or by key if values are the same
//
class prioritize
{
public:
    bool operator()(const pair<long long, double>& p1, const pair <long long, double>& p2) const
    {
        if (p1.second != p2.second)
            return p1.second > p2.second; // smallest is first

        else // p1 and p2 have the same value so sort by key
            return p1.first > p2.first;
    }
};

// findDes()
//
// locates a building based on abbreviation or full name. 
// Returns full name, coordinates (lat/long), 
// and ID via reference parameter 
//
bool findBuilding(string userInput, vector<BuildingInfo>& Buildings, string& buildingFull, Coordinates& buildCoords)
{
    // search by abbrevation first
    for (auto i : Buildings)
    {
        if (userInput == i.Abbrev)
        {
            buildingFull = i.Fullname;
            buildCoords = i.Coords;

            return true;
        }

        else // do nothing
            ; // keep going
    }

    // search for a partial match 
    for (auto i : Buildings)
    {
        size_t locate = i.Fullname.find(userInput); // used to check if there is a partial match

        // search for full name exact match or partial match
        if ((userInput == i.Fullname) || (locate != string::npos)) // used from http://www.cplusplus.com/reference/string/string/find/
        {
            buildingFull = i.Fullname;
            buildCoords = i.Coords;

            return true;
        }

        else // do nothing
            ; // keep going
    }

    return false; // destination not found
}

// findNode()
//
// locates the closest footway node based on the coordinates passed to it.
// returns coordinates of the nearest footway node via reference parameter
//
void findNode(Coordinates buildingCoords, vector<FootwayInfo>& Footways, map<long long, Coordinates>& Nodes, Coordinates& footwayCoords)
{
    //initial coordinates (first footway node in the first footway node vector)
    footwayCoords.ID = Footways[0].Nodes[0];
    footwayCoords.Lat = Nodes[Footways[0].Nodes[0]].Lat;
    footwayCoords.Lon = Nodes[Footways[0].Nodes[0]].Lon;

    double initDist = distBetween2Points(buildingCoords.Lat, buildingCoords.Lon, Nodes[Footways[0].Nodes[0]].Lat, Nodes[Footways[0].Nodes[0]].Lon); // initial distance to compare too

    for (auto i : Footways) // loop through vector of footways
    {
        // loop through footway nodes vector
        for (size_t k = 0; k < i.Nodes.size(); k++)
        {
            // find lat/lon for footway nodes
            double footLat = Nodes[i.Nodes[k]].Lat;
            double footLon = Nodes[i.Nodes[k]].Lon;

            // calculate distance 
            double calcDist = distBetween2Points(buildingCoords.Lat, buildingCoords.Lon, footLat, footLon);

            if (calcDist < initDist) // found a closer footway node
            {
                initDist = calcDist;

                // assign new coordinates
                footwayCoords.ID = i.Nodes[k];
                footwayCoords.Lat = footLat;
                footwayCoords.Lon = footLon;
            }

            else // do nothing
                ; // leave the footway node alone
        }
    }

    return;
}

//
// Dijkstra:
//
// Performs Dijkstra's shortest weighted path algorithm from
// the given start vertex.  Returns a vector of vertices in
// the order they were visited, along with a map of 
// pairs where the long long is a vertex V and the double is the 
// distance from the start vertex to V; if no such path exists,
// the distance is INF (defined in util.h).
//
// map<long long, long long>& predecessors (vertex, predecessor to vertex)
//
vector<long long> Dijkstra(graph<long long, double>& G, long long startV, map<long long, double>& distances, map<long long, long long>& predecessors)
{
    vector<long long>  visited;
    vector<long long> vertices = G.getVertices(); // used to store all the vertices in the graph
    set<long long> visitedSet; // used to search if a vertex has been visited
    priority_queue<
        pair<long long, double>,         // (key, value) pair
        vector<pair<long long, double>>, // store pairs in a vector
        prioritize> unvisitedQueue;// prioritize function

    //
    // NOTE: the value of INF is predefined in "util.h", so use
    // this value to initialize the distances.
    //

    // for each vertex currentV in graph {
    for (auto i : vertices)
    {
        // currentV->distance = INF
        distances[i] = INF;

        // currentV->predV = 0;
        predecessors[i] = 0;

        // unvisitedQueue.push(currentV, INF)
        unvisitedQueue.push(make_pair(i, INF));
    }

    // startV has a distance of 0 from itself
    // startV->distance = 0
    distances[startV] = 0;

    // unvisitedQueue.push(startV,0)
    unvisitedQueue.push(make_pair(startV, 0));

    // while (unvisitedQueue is not empty) {
    while (!unvisitedQueue.empty())
    {
        // Visit vertex with mininum distance from startV
        // currentV = PopMin unvisitedQueue
        pair<long long, double> top = unvisitedQueue.top();
        long long currentV = top.first;
        //cout << "currentV = " << currentV << endl; // for debugging purposes

        unvisitedQueue.pop();

        // if (currentV->distance == INF)
        if (distances[currentV] == INF)
            break;


        // else if (currentV has been Visited)
        else if (visitedSet.count(currentV) != 0)
            continue;


        // else
        else
        {
            // Visit currentV
            visited.push_back(currentV);
            visitedSet.insert(currentV);
        }

        // set<string> neighbors = G.neighbors(currentV) 
        set<long long> adjV = G.neighbors(currentV); // adjacent vertices

        // for each vertex adjV adjacent to currentV {
        for (auto i = adjV.rbegin(); i != adjV.rend(); ++i)
        {
            // edgeWeight = weight of edge from currentV to adjV (Assume true will be returned from getWeight() function)
            double edgeWeight = 0;

            G.getWeight(currentV, *i, edgeWeight);

            // alternativePathDistance = currentV->distance + edgeWeight
            double altPathDis = distances[currentV] + edgeWeight;

            // If shorter path from startV to adjV is found,
            // update adjV's distance and predecessor
            // if (alternativePathDistance < adjV⇢distance) {
            if (altPathDis < distances[*i])
            {
                // adjV->distance = alternativePathDistance
                distances[*i] = altPathDis;

                //adj->predV = currentV 
                predecessors[*i] = currentV;

                // unvisitedQueue.push(adjV, alternativePathDistance);
                unvisitedQueue.push(make_pair(*i, altPathDis));
            }

            else // altPathDis > distances[i]
                ; // do nothing
        }
    }

    return visited;
}

//
// printPath()
//
// Prints the path it takes to get from a start node 
// to a destination node based on the predecessors 
// map given by Djkstra's alg.
//
void printPath(map<long long, long long>& predecessors, long long startNodeID, long long destNodeID)
{
    stack<long long> path;
    long long curID = destNodeID;
    
    path.push(curID);

    // go through the predecessors and add them to the path stack
    while (curID != startNodeID)
    {
        path.push(predecessors[curID]);

        curID = predecessors[curID];
    }
    
    if (path.size() == 1)
        cout << "Path: " << startNodeID;
    
    else
        cout << "Path: " << startNodeID << "->";

    // pop path stack and print out the contents
    while (path.top() != destNodeID)
    {
        path.pop();

        if (path.top() == destNodeID)
            cout << destNodeID;

        else
            cout << path.top() << "->";
    }

    cout << endl;

    return;
}