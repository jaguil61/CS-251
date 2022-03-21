/*main.cpp*/

//
// << Jose M. Aguilar (jaguil61) >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #07: open street maps, graphs, and Dijkstra's alg
// 
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:  
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <iomanip>  /*setprecision*/
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"
#include "graph.h"
#include "util.h"

using namespace std;
using namespace tinyxml2;



//
//
// main
//
int main()
{
    map<long long, Coordinates>  Nodes;     // maps a Node ID to it's coordinates (lat, lon)
    vector<FootwayInfo>          Footways;  // info about each footway, in no particular order
    vector<BuildingInfo>         Buildings; // info about each building, in no particular order
    XMLDocument                  xmldoc;
    graph<long long, double> G; // vertices are nodes, weights are distance

    cout << "** Navigating UIC open street map **" << endl;
    cout << endl;
    cout << std::setprecision(8);

    string def_filename = "map.osm";
    string filename;

    cout << "Enter map filename> ";
    getline(cin, filename);

    if (filename == "")
    {
        filename = def_filename;
    }

    //
    // Load XML-based map file 
    //
    if (!LoadOpenStreetMap(filename, xmldoc))
    {
        cout << "**Error: unable to load open street map." << endl;
        cout << endl;
        return 0;
    }

    //
    // Read the nodes, which are the various known positions on the map:
    //
    int nodeCount = ReadMapNodes(xmldoc, Nodes);

    //
    // Read the footways, which are the walking paths:
    //
    int footwayCount = ReadFootways(xmldoc, Footways);

    //
    // Read the university buildings:
    //
    int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);

    // Adds nodes as vertices
    for (auto i : Nodes)
        G.addVertex(i.first);
    
    // Add edges
    for (auto i : Footways) // loop through vector of footways
    {
        // loop through footway nodes vector
        for (size_t k = 1; k < i.Nodes.size(); k++) // start index 1 so that we never use an invalid index
        {
            // find lat/lon for nodes
            double fromLat = Nodes[i.Nodes[k - 1]].Lat;
            double fromLon = Nodes[i.Nodes[k - 1]].Lon;
            double toLat = Nodes[i.Nodes[k]].Lat;
            double toLon = Nodes[i.Nodes[k]].Lon;
           
            // calculate distance 
            double dist = distBetween2Points(fromLat, fromLon, toLat, toLon);

            // add edges in both directions (NODE Id's and distance (weight))
            G.addEdge(i.Nodes[k - 1], i.Nodes[k], dist);
            G.addEdge(i.Nodes[k], i.Nodes[k-1], dist);
        }
    }


    //
    // Stats
    //
    assert(nodeCount == Nodes.size());
    assert(footwayCount == Footways.size());
    assert(buildingCount == Buildings.size());

    cout << endl;
    cout << "# of nodes: " << Nodes.size() << endl;
    cout << "# of footways: " << Footways.size() << endl;
    cout << "# of buildings: " << Buildings.size() << endl;
   

    //
    // build the graph, output stats:
    cout << "# of vertices: " << G.NumVertices() << endl;
    cout << "# of edges: " << G.NumEdges() << endl;
    cout << endl;

    //G.dump(cout); // for debugging purposes

    //
    // Navigation from building to building
    //
    string startBuilding, destBuilding;

    cout << "Enter start (partial name or abbreviation), or #> ";
    getline(cin, startBuilding);

    while (startBuilding != "#")
    {
        cout << "Enter destination (partial name or abbreviation)> ";
        getline(cin, destBuilding);

        // lookup buildings
        string startFull, destFull;
        Coordinates startCoords, destCoords;

        bool startSuccess = findBuilding(startBuilding, Buildings, startFull, startCoords);
        bool destSuccess = findBuilding(destBuilding, Buildings, destFull, destCoords);

        if (!startSuccess) // start building not found
        {
            cout << "Start building not found" << endl;

            cout << endl;
            cout << "Enter start (partial name or abbreviation), or #> ";
            getline(cin, startBuilding);

            continue;
        }

        else if (!destSuccess) // destination building not found
        {
            cout << "Destination building not found" << endl;

            cout << endl;
            cout << "Enter start (partial name or abbreviation), or #> ";
            getline(cin, startBuilding);

            continue;
        }

        else // start success was true and destSuccess was true
        {
            //cout << "Start building found" << endl; // for debugging purposes
            cout << "Starting Point:" << endl;
            cout << " " << startFull << endl;
            cout << " (" << startCoords.Lat << ", " << startCoords.Lon << ")" << endl;

            //cout << "Destination building found" << endl; // for debugging purposes
            cout << "Destination Point:" << endl;
            cout << " " << destFull << endl;
            cout << " (" << destCoords.Lat << ", " << destCoords.Lon << ")" << endl << endl;
        }

        // find nearest start and dest nodes
        //cout << "Finding nearest start and dest nodes." << endl; // for debugging purposes

        Coordinates startFootN, destFootN;

        findNode(startCoords, Footways, Nodes, startFootN);
        findNode(destCoords, Footways, Nodes, destFootN);

        cout << "Nearest start node:" << endl;
        cout << " " << startFootN.ID << endl;
        cout << " (" << startFootN.Lat << ", " << startFootN.Lon << ")" << endl;

        cout << "Nearest destination node:" << endl;
        cout << " " << destFootN.ID << endl;
        cout << " (" << destFootN.Lat << ", " << destFootN.Lon << ")" << endl << endl;

        // run Dijkstra's alg
        vector<long long> visited;
        map<long long, long long> predecessors;  // (vertex, predecessor to vertex)
        map<long long, double> distances;

        visited = Dijkstra(G, startFootN.ID, distances, predecessors);

        cout << "Navigating with Dijkstra..." << endl;

        // output distance and path to destination:
        if (distances[destFootN.ID] == INF)
        {
            cout << "Sorry, destination unreachable" << endl;
        }

        else
        {
            cout << "Distance to dest: " << distances[destFootN.ID] << " miles" << endl;
            printPath(predecessors, startFootN.ID, destFootN.ID);
        }


        //
        // another navigation?
        //
        cout << endl;
        cout << "Enter start (partial name or abbreviation), or #> ";
        getline(cin, startBuilding);
    }

    //
    // done:
    //
    cout << "** Done **" << endl;

    //system("pause");
    return 0;
}
