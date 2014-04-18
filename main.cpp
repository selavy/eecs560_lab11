#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <queue>
#include <queue>

#include "DisjointSet.hpp"

#define SPACE ' '

using namespace std;

typedef int vertice;

void breaking_cycles( int ** graph, int n );
bool depth_first_search( int curr, int from, int**& graph, bool*& visited, int& highest, int*& edge, int n );
void print_graph( int ** graph, int n );
void kruskal( int ** graph, int n );

int main( int argc, char **argv ) {
  if( argc < 2 ) {
    cerr << "Usage: " << argv[0] << " [FILE]" << endl;
    exit( 0 );
  }

  ifstream ifs;
  try {
    ifs.open( argv[1], ifstream::in );
    if(! ifs.good() ) {
      cerr << "Unable to open input file: " << argv[1] << endl;
      exit( 0 );
    }
  } catch( ... ) {
    cerr << "Error in opening input file: " << argv[1] << endl;
    exit( 0 );
  }

  string line;
  vector<int> first_row;

  try {
    getline( ifs, line );
  } catch( ... ) {
    cerr << "Error: malformed input file: " << argv[1] << endl;
    exit( 0 );
  }
  
  int n;
  try {
    size_t prev = 0;
    size_t pos = line.find_first_of(SPACE);
    while( pos != string::npos ) {
      int val = stoi( line.substr( prev, pos ) );
      first_row.push_back( val );
      prev = pos;
      pos = line.find_first_of( SPACE, pos+1 );
    }
    int val = stoi( line.substr( prev, line.size() ) );
    first_row.push_back( val );
    
    n = first_row.size();
  } catch( ... ) {
    cerr << "Error: unable to determine the dimensions of cost matrix." << endl;
    exit( 0 );
  }

  //
  // Allocate 2-d matrix array
  //
  int ** matrix = NULL;
  int ** matrix2 = NULL;
  try {
    matrix = new int*[n];
    matrix2 = new int*[n];
    for( int i = 0; i < n; ++i ) {
      matrix[i] = new int[n];
      matrix2[i] = new int[n];
    }
  } catch( ... ) {
    cerr << "Error: not enough memory available" << endl;
    //
    // Don't worry about de-allocating memory because OS will
    // reclaim it anyways
    //
    exit( 0 );
  }

  try {
    for( int i = 0; i < n; ++i ) {
      matrix[i][0] = first_row.at( i );
      matrix2[i][0] = matrix[i][0];
    }
    
    for( int i = 1; i < n; ++i ) {
      for( int j = 0; j < n; ++j ) {
	ifs >> line;
	int val = stoi( line );
	matrix[j][i] = val;
	matrix2[j][i] = val;
      }
    }
  } catch( ... ) {
    cerr << "Error: unrecoverable" << endl;
    goto error;
  }
    
  try {
    cout << "Cost Matrix: " << endl;
    print_graph( matrix, n );
  } catch( ... ) {
    cerr << "Error: print_graph()" << endl;
    goto error;
  }
    
  try {
    cout << "\nRunning breaking cycles algorithm...\n" << endl;
    breaking_cycles( matrix, n );
  } catch( ... ) {
    cerr << "Error: breaking-cycles algorithm" << endl;
    goto error;
  }
    
  try {
    cout << "\nRunning kruskal's algorithm...\n" << endl;
    kruskal( matrix2, n );
  } catch( ... ) {
    cerr << "Error: kruskal's algorithm" << endl;
    goto error;
  }

 error:
  //
  // Delete 2-d matrix array
  //
  for( int i = 0; i < n; ++i ) {
    delete [] matrix[i];
    delete [] matrix2[i];
  }
  delete matrix;
  delete matrix2;
  
  ifs.close();
  return 0;
}

struct Edge {
  int to;
  int from;
  int weight;

  Edge( int a, int b, int c ) : to( a ), from( b ), weight( c ) {}
};

struct CompareEdges {
  bool operator()( const Edge& a, const Edge& b ) const {
    return a.weight > b.weight;
  }
};

void kruskal( int ** graph, int n ) {
  DisjointSet djset( n );
  priority_queue<Edge, vector<Edge>, CompareEdges> heap;

  int ** new_graph = new int*[n];
  for( int i = 0; i < n; ++i ) {
    new_graph[i] = new int[n];
  }

  for( int i = 0; i < n; ++i ) {
    for( int j = 0; j < n; ++j ) {
      new_graph[i][j] = 0;
    }
  }

  //
  // Add all edges to heap
  //
  for( int i = 0; i < n; ++i ) {
    for( int j = (i+1); j < n; ++j ) {
      if( graph[i][j] > 0 ) {
	heap.emplace( i, j, graph[i][j] );
      }
    }
  }

  int edgesAccepted = 0;
  const int edgesNeeded = n - 1;
  
  while(( !heap.empty() ) && ( edgesAccepted < edgesNeeded ) ) {
    Edge edge = heap.top();
    heap.pop();
    const int uSet = djset.Find( edge.to );
    const int vSet = djset.Find( edge.from );
    if( uSet != vSet ) {
      djset.Union( uSet, vSet );
      new_graph[edge.to][edge.from] = graph[edge.to][edge.from];
      new_graph[edge.from][edge.to] = graph[edge.from][edge.to];
      ++edgesAccepted;
    }

  }

  int totalWeight = 0;
  for( int i = 0; i < n; ++i ) {
    for( int j = (i+1); j < n; ++j ) {
      if( new_graph[i][j] > 0 ) {
	cout << "(" << i + 1 << ", " << j + 1 << ")" << endl;
	totalWeight += new_graph[i][j];
      }
    }
  }
  cout << "Total Weight = " << totalWeight << endl;

  for( int i = 0; i < n; ++i ) {
    delete [] new_graph[i];
  }
  delete [] new_graph;
}

void breaking_cycles( int ** graph, int n ) {
  bool * visited = new bool[n];
  memset( visited, 0, sizeof( bool ) * n );


  int * edge = new int[2];
  edge[0] = 0; edge[1] = 0;
  int highest = -1;
  while( depth_first_search( 0, 0, graph, visited, highest, edge, n ) ) {
    graph[edge[0]][edge[1]] = 0;
    graph[edge[1]][edge[0]] = 0;
    edge[0] = 0; edge[1] = 0; 
    highest = -1;
    memset( visited, 0, sizeof( bool ) * n );
  }

  delete [] edge;
  delete [] visited;

  int totalWeight = 0;
  for( int i = 0; i < n; ++i ) {
    for( int j = (i+1); j < n; ++j ) {
      if( graph[i][j] > 0 ) {
	cout << "(" << i + 1 << ", " << j + 1 << ")" << endl;
	totalWeight += graph[i][j];
      }
    }
  }
  cout << "Total Weight = " << totalWeight << endl;
}

void print_graph( int ** graph, int n ) {
  for( int i = 0; i < n; ++i ) {
    for( int j = 0; j < n; ++j ) {
      cout << graph[j][i] << " ";
    }
   cout << endl;
  }
}

//
// return true if a cycle was found
//
bool depth_first_search( int curr, int from, int**& graph, bool*& visited, int& highest, int*& edge, int n ) {
  if( visited[curr] ) {
    return true;
  }

  //
  // mark the vertice as visited
  //
  visited[curr] = true;
  bool retVal = false;

  for( int i = 0; (i < n) && (retVal == false); ++i ) {
    if( i == from ) {
      continue;
    } else if( graph[i][curr] > 0 ) {
      if( graph[i][curr] > highest ) {
	highest = graph[i][curr];
	edge[0] = curr;
	edge[1] = i;
      }

      //
      // connected to vertice i, so explore that
      //
      retVal |= depth_first_search( i, curr, graph, visited, highest, edge, n );
    }
  }

  return retVal;
}
