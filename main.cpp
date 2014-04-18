#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <queue>

#define SPACE ' '

using namespace std;

typedef int vertice;

void breaking_cycles( int ** graph, int n );
bool depth_first_search( int curr, int from, int**& graph, bool*& visited, int& highest, int*& edge, int n );
void print_graph( int ** graph, int n );

int main( int argc, char **argv ) {
  if( argc < 2 ) {
    cerr << "Usage: " << argv[1] << " [FILE]" << endl;
    exit( 0 );
  }

  ifstream ifs( argv[1], ifstream::in );
  if(! ifs.good() ) {
    cerr << "Unable to open input file: " << argv[1] << endl;
    exit( 0 );
  }

  string line;
  vector<int> first_row;
  getline( ifs, line );
  
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
  
  int n = first_row.size();

  //
  // Allocate 2-d matrix array
  //
  int ** matrix = new int*[n];
  for( int i = 0; i < n; ++i ) {
    matrix[i] = new int[n];
  }

  for( int i = 0; i < n; ++i ) {
    matrix[i][0] = first_row.at( i );
  }

  for( int i = 1; i < n; ++i ) {
    for( int j = 0; j < n; ++j ) {
      ifs >> line;
      int val = stoi( line );
      matrix[j][i] = val;
    }
  }

  for( int i = 0; i < n; ++i ) {
    for( int j = 0; j < n; ++j ) {
      cout << matrix[j][i] << " ";
    }
    cout << endl;
  }

  cout << "\nRunning breaking cycles algorithm...\n" << endl;
  breaking_cycles( matrix, n );
  
  //
  // Delete 2-d matrix array
  //
  for( int i = 0; i < n; ++i ) {
    delete [] matrix[i];
  }
  delete matrix;
  
  ifs.close();
  return 0;
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

  for( int i = 0; i <= (n/2); ++i ) {
    for( int j = i; j < n; ++j ) {
      if( graph[j][i] > 0 ) {
	cout << "(" << i + 1 << ", " << j + 1 << ")" << endl;
      }
    }
  }
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

  for( int i = 0; i < n; ++i ) {
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
