#include "AVLTree.h"

int main() {
	memtable t(3);
	t.put(20, 4);
	t.put(25, 4);
	t.put(15, 3);
    t.put(30, 1);
    t.put(5, 6);
    t.put(35, 7);
    t.put(67, 8);
    t.put(43, 3);
    t.put(21, 1);
    t.put(89, 5);
    t.put(38, 2);
    t.put(69, 1);
	return 0;
}
