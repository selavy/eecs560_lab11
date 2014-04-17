#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <queue>

#define SPACE ' '

using namespace std;

typedef int vertice;

void breaking_cycles( int ** graph, int n );

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

  cout << "Running breaking cycles algorithm...\n" << endl;
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
  queue<vertice> Q;
  int highest_weight = 0;
  vertice edge[2];
  int * visited = new int[n];
  for( int i = 0; i < n; ++i ) {
      visited[i] = false;
  }

  int from = 0;
  const int start = 0;
  Q.push( start );

  cout << "1"; 

  while(! Q.empty() ) {
    vertice v = Q.front();
    Q.pop();

    if( graph[from][v] > highest_weight ) {
      highest_weight = graph[from][v];
      edge[0] = from; edge[1] = v;
    }

    if( visited[v] ) {
      //
      // cycle
      //
      cout << " to " << v + 1 << " from " << from + 1;
      cout << "\nfound cycle at " << v + 1 << endl;
      graph[edge[0]][edge[1]] = 0;
      graph[edge[1]][edge[0]] = 0;
      highest_weight = 0;
      cout << "edge = " << edge[0] + 1 << " to " << edge[1] + 1 << endl;
      while(! Q.empty() ) Q.pop();
      for( int i = 0; i < n; ++i ) visited[i] = false;
      Q.push( start );
      from = 0;
    } else {
      visited[v] = true;
      cout << " to " << v + 1 << " from " << from + 1 << ", " << v + 1;
      //
      // move function
      //
      for( int i = 0; i < n; ++i ) {
	if( i == from ) continue;
	if( graph[v][i] > 0 ) {
	  Q.push( i );
	}
      }

      from = v;
    }
  }
  cout << endl << endl;

  for( int i = 0; i < n; ++i ) {
    for( int j = 0; j < n; ++j ) {
      cout << graph[j][i] << " ";
    }
    cout << endl;
  }
}

