#include "DisjointSet.hpp"
#include <iostream>
using namespace std;

DisjointSet::DisjointSet( int size ) {
  //
  // Initially all elements are in their own set
  //
  for( int i = 0; i < size; ++i ) {
    set_.push_back( -1 );
  }
}

DisjointSet::~DisjointSet() {
}

//
// Union-by-rank
//
void DisjointSet::Union( int rootA, int rootB ) {
  if( rootA == rootB ) return;
  if( set_.at( rootB ) < set_.at( rootA ) ) { // rootB is deeper
    set_.at( rootA ) = rootB;
  } else {
    if( set_.at( rootA ) == set_.at( rootB ) ) {
      --(set_.at( rootA ));                     // update height if same
    }
    
    set_.at( rootB ) = rootA;                  // make rootA new root
  }
}

//
// Find with path compression
//
int DisjointSet::Find( int x ) {
  if( set_.at( x ) < 0 ) {
    return x;
  } else {
    return( set_.at( x ) = Find( set_.at( x ) ) );
  }
}

int DisjointSet::NumberOfSets() const {
  unsigned int counter = 0;

  for( const auto& it : set_ ) {
    if( it < 0 ) {
      ++counter;
    }
  }
  return counter;
}

void DisjointSet::print() {
  for( unsigned int i = 0; i < set_.size(); ++i ) {
    cout << i << "\t";
  }
  cout << endl;
  
  for( auto it : set_ ) {
    cout << it << "\t";
  }
  cout << endl;
}
