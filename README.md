eecs560_lab11
=============

Minimum Spanning Tree

To build:
"make"

To run:
"./lab11 [FILE]"

[FILE] is the name of a file with a cost matrix for a graph.
The program will run both the break-cycles algorithm
and kruskal's algorithm and output the edges in the minimum spanning
tree as well as the total weight of the tree.  The input must
hold a valid cost matrix for a non-directional graph (symmetric and
square).  The program does not do any real error checking for either
of these conditions.  Edge weights can NOT be negative.
