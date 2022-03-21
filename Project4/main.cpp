//Project 4
//Jose M. Aguilar (jaguil61)

#include <iostream>
#include "bstt.h"

using namespace std;

int main()
{
	/*bstt<int, int> tree;
	
	tree.insert(123, 45);
	tree.insert(125, 78);
	tree.insert(5, 7);
	tree.insert(67, 89);
	tree.insert(127, 5);
	tree.insert(122, 95);
	tree.insert(121, 486);
	tree.insert(60, 46);

	//cout << "Size: " << tree.size() << endl;
	
	cout << "**tree**" << endl;
	tree.dump(cout);
	cout << endl;
	//tree.clear();

	//tree.dump(cout);

	//cout << "Size: " << tree.size() << endl;

	bstt <int, int> tree2(tree);

	tree.clear();

	cout << "**tree**" << endl;
	tree.dump(cout);
	cout << endl;

	cout << "**tree2**" << endl;
	tree2.dump(cout);
	cout << endl;
	
	tree2.insert(78, 0);

	cout << "**tree**" << endl;
	tree.dump(cout);
	cout << endl;

	cout << "**tree2**" << endl;
	tree2.dump(cout);
	cout << endl;

	bstt <int, int> tree3;

	tree3 = tree2;

	tree2.clear();

	cout << "**tree2**" << endl;
	tree2.dump(cout);
	cout << endl;

	cout << "**tree3**" << endl;
	tree3.dump(cout);
	cout << endl;

	bstt<int, int> tree4;

	tree4.insert(123, 45);
	tree4.insert(125, 78);
	tree4.insert(5, 7);
	tree4.insert(67, 89);
	
	cout << "**tree4**" << endl;
	tree4.dump(cout);
	cout << endl;

	tree4 = tree3;

	tree3.clear();

	cout << "**tree3**" << endl;
	tree3.dump(cout);
	cout << endl;

	cout << "**tree4**" << endl;
	tree4.dump(cout);
	cout << endl;

	cout << "Value of 123: " << tree4[123] << endl;
	cout << "Value of 125: " << tree4[125] << endl;
	cout << "Value of 900: " << tree4[900] << endl;
	cout << "Value of 121: " << tree4[121] << endl;
	cout << "Value of 127: " << tree4[127] << endl;
	
	bstt<int, int> tree5;

	tree5.insert(200, 19);
	tree5.insert(72, 771);
	tree5.insert(73, 273);
	tree5.insert(636, 378);
	tree5.insert(12, 193);
	tree5.insert(722, 7271);
	tree5.insert(731, 273);
	tree5.insert(6326, 3378);
	tree5.insert(124, 593);
	tree5.insert(79, 5271);
	tree5.insert(736, 773);
	tree5.insert(6316, 2378);

	cout << "**tree5**" << endl;
	tree5.dump(cout);
	cout << endl;

	bstt<int, int> tree6(tree5);

	tree5.clear();

	cout << "**tree6**" << endl;
	tree6.dump(cout);
	cout << endl;

	cout << "**tree6**" << endl;
	tree6.dump(cout);
	cout << endl;

	cout << "**tree6**" << endl;
	tree6.dump(cout);
	cout << endl;
	*/
	bstt<int, int> tree7;

	tree7.insert(200, 19);
	tree7.insert(72, 771);
	tree7.insert(75, 273);
	tree7.insert(636, 378);
	tree7.insert(12, 193);
	tree7.insert(722, 7271);
	tree7.insert(731, 273);
	tree7.insert(6326, 3378);
	tree7.insert(124, 593);
	tree7.insert(73, 5271);
	tree7.insert(70, 278);
	tree7.insert(50, 28);
	tree7.insert(736, 773);
	tree7.insert(6316, 2378);

	cout << "**tree7**" << endl;
	tree7.dump(cout);
	cout << endl;

	tree7.begin();

	int key = 0;

	while (tree7.next(key))
		cout << key << endl;

	cout << endl <<"**Closing Program**" << endl;
	system("pause");
	return 0;
}
