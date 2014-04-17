#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>

#define SPACE ' '

using namespace std;

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
