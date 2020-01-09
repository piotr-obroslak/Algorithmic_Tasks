# playing around with AVL tree

Implementation of a generic key->value AVL tree just to get familiar with the concept in practice. No recurssion is used in the insert procedure because, I use a binding between a node and its parent & upward traversal instead because:
  1. recursion is costly
  2. the is a plenty of example implementations of the AVL tree insert in pseudo code so copying/re-doig them was not challenging enough

The test app produces a tree with signed integer keys and string values. It works in one of the tree modes:
  1. ./test - generates a tree with random size and random values
  2. ./test followed-by-a-number - generates a tree with specified size and random values (e.g. ./test 20 will generate a random tree with 20 nodes)
  3. ./test followed-by-key-value-pairs - generates a tree with the values specified (e.g. ./test -7 abc 2 def 6 ghij)
The output is stored in dot format (under the filename test.dot) so that it can be further visualized.

