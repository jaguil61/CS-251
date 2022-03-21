/*graph.h*/

//
// << Jose M. Aguilar (jaguil61) >>
//
// Basic graph class using adjacency matrix representation.  Currently
// limited to a graph with at most 100 vertices.
//
// original author: Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
//

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <utility> // pair<>
#include <map> 

using namespace std;


template<typename VertexT, typename WeightT>
class graph
{
private:
    map<VertexT, vector<pair<VertexT, WeightT>>> GR; // the graph

    // _edgeExist
    //
    // Checks if an edge already exists and if so it returns true 
    // and the index location via a reference parameter.
    // Else if the edge does not exist then return false
    //
    bool _edgeExist(const vector<pair<VertexT, WeightT>>& vertex, VertexT to, int& index) const
    {
        for (size_t i = 0; i < vertex.size(); i++)
        {
            if (vertex[i].first == to)
            {
                index = i; // return index location via reference parameter

                return true; // the edge does exist
            }

            else // do nothing
                ; // keep going
        }

        return false; // the edge does not exist
    }


public:
    //
    // constructor:
    //
    // Constructs an empty graph 
    graph()
    {
        // C++ will handle the initializations
    }

    //
    //  NumVertices
    //
    // Returns the # of vertices currently in the graph.
    //
    int NumVertices() const
    {
        return this->GR.size();
    }

    //
    //  NumEdges
    //
    // Returns the # of edges currently in the graph.
    //
    int NumEdges() const
    {
        int count = 0;

        // loop through the graph and add up all the vector's sizes
        for (auto i : this->GR)
        {
            count += i.second.size();
        }

        return count;
    }

    //
    // addVertex
    //
    // Adds the vertex v to the graph, and if so
    // returns true.  If vertex already exists 
    // in the graph, then false is returned.
    //
    bool addVertex(VertexT v)
    {
        if (this->GR.count(v) != 0) // already in graph
            return false;

        else // not in graph so insert vertex
        {
            this->GR[v];

            /*
            if (GR.count(v) != 0) // found it the vertex in the map
            {
                auto i = GR.find(v);

                cout << "Added vertex " << i->first << endl;
                return true;
            }

            else
            {
                cout << "Did not add vertex " << v << endl;
                return false;
            }*/ // for debugging purposes

            return true;
        }   
    }

    //
    // addEdge
    //
    // Adds the edge (from, to, weight) to the graph, and returns
    // true.  If the vertices do not exist false is returned.
    //
    // NOTE: if the edge already exists, the existing edge weight
    // is overwritten with the new edge weight.
    //
    bool addEdge(VertexT from, VertexT to, WeightT weight)
    {
        if ((this->GR.count(from) == 0) || (this->GR.count(to) == 0)) // one of the vertices is not in the graph
            return false;

        int index = -1;

        bool success = _edgeExist(this->GR[from], to, index);

        if (success) // edge exists so update the value
        {
            //cout << "Updating Edge (" << from << ", " << to << ")" << " weight: " << GR[from][index].second << endl; // debugging purposes

            this->GR[from][index].second = weight;
            
            //cout << "Updated Edge (" << from << ", " << to << ")" << " to weight " << GR[from][index].second << endl; // debugging purposes

            return true;
        }

        else // edge does not exist so insert it
        {
            this->GR[from].push_back(make_pair(to, weight));

            //cout << "Added edge (" << from << ", " << to << ")" << " weight: " << weight << endl; // debugging purposes
            
            return true;
        }
    }

    
    //  getWeight
    //
    // Returns the weight associated with a given edge.  If 
    // the edge exists, the weight is returned via the reference
    // parameter and true is returned.  If the edge does not 
    // exist, the weight parameter is unchanged and false is
    // returned.
    //
    bool getWeight(VertexT from, VertexT to, WeightT& weight) const
    {
        if ((this->GR.count(from) == 0) || (this->GR.count(to) == 0)) // one of the vertices is not in the graph
            return false;

        int index = -1;
        
        auto vertexFrom = this->GR.find(from)->second;

        bool success = _edgeExist(vertexFrom, to, index); // use GR.find() so that the container (key) is not moidiefed "http://www.cplusplus.com/reference/map/map/find/"

        if (!success) // edge does not exist so return false
            return false;

        else // success == true
        {
            weight = vertexFrom[index].second;

            return true;
        }
    }

    //
    //  neighbors
    //
    // Returns a set containing the neighbors of v, i.e. all
    // vertices that can be reached from v along one edge.
    // Since a set is returned, the neighbors are returned in
    // sorted order; use foreach to iterate through the set.
    //
    set<VertexT> neighbors(VertexT v) const
    {
        set<VertexT>  S;

        if (this->GR.count(v) == 0) // v is not a vertex in the map
            return S;

        for (auto i : this->GR.find(v)->second) // insert all neighbors into the set
        {
            S.insert(i.first);
        }
       
        return S;
    }

    //
    //  getVertices
    //
    // Returns a vector containing all the vertices currently in
    // the graph.
    //
    vector<VertexT> getVertices() const
    {
        vector<VertexT> vertices;

        // loop through graph insert vertices into the vector
        for (auto i : this->GR)
        {
            vertices.push_back(i.first);
        }

        return vertices;
    }

    //  dump
    // 
    // Dumps the internal state of the graph for debugging purposes.
    //
    // Example:
    //    graph<string,int>  G;
    //    ...
    //    G.dump(cout);  // dump to console
    //
    void dump(ostream& output) const
    {
        output << "***************************************************" << endl;
        output << "********************* GRAPH ***********************" << endl;

        output << "**Num vertices: " << this->NumVertices() << endl;
        output << "**Num edges: " << this->NumEdges() << endl;

        output << endl;
        
        vector<VertexT> vertices = this->getVertices();

        for (auto v : vertices)
        {
            set<VertexT> neighbors = this->neighbors(v);

            output << v << ":";

            for (auto n : neighbors)
            {
                WeightT weight = WeightT{};

                if (this->getWeight(v, n, weight))
                {
                    output << " (" << v << "," << n << "," << weight << ")";
                }

                else
                {
                    output << " (" << v << "," << n << "," << "???" << ")";
                }
            }

            output << endl;
        }

        output << "**************************************************" << endl;
    }

};