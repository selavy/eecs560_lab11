#ifndef _DISJOINTSET_
#define _DISJOINTSET_

#include <vector>
#include <stdexcept>

class DisjointSet {
public:
  explicit DisjointSet( int size );
  virtual ~DisjointSet();
  void Union( int rootA, int rootB );
  int Find( int value );
  int NumberOfSets() const;

  void print();
private:
  std::vector<int> set_;
};

#endif
