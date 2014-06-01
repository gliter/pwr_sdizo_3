
#include <iostream>
using namespace std;

#include <stdlib.h>
#include <math.h>
#include "Bag.h"
#include "Tsp.h"
#include "menu.h"

int main() {
	srand(0);
	menu();
	/*Bag b(27, 5000); //Rozsadny czas
	b.randItems();
	cout << b;
	int *result = b.dynamic();
	b.printSubSet(result, cout);
	delete[] result;

	unsigned long long maxSubSet = b.checkAll();
	b.printSubSet(maxSubSet, cout);

	int *result = b.aproximate();
	b.printSubSet(result, cout);
	delete[] result;

	cout << b.dynamic() << endl;
*/
	/*Tsp a(20);
	a.randDistances();
	cout << a;
	*/


	/*unsigned *minPerm;
	//minPerm = a.checkAll();
	//a.printPermutation(minPerm, cout);
	//delete[] minPerm;

	minPerm = a.greedy(5);
	a.printPermutation(minPerm, cout);
	delete[] minPerm;

	minPerm = a.alg2opt();
	a.printPermutation(minPerm, cout);
	delete[] minPerm;*/

	/*Tsp *tsps[10];
	for(unsigned i = 0; i < 10; i++) {
		tsps[i] = new Tsp(10);
		tsps[i]->randDistances();
	}
	for(unsigned i = 0; i < 10; i++) {
		unsigned *minPerm = tsps[i]->greedyAll();
		cout << "zachlanny" << endl;
		tsps[i]->printPermutation(minPerm, cout);
		delete[] minPerm;
		minPerm = tsps[i]->alg2opt();
		cout << "alg2opt" << endl;
		tsps[i]->printPermutation(minPerm, cout);
		//cout << minDist << endl;
		delete tsps[i];
	}*/

	//getchar();
	return 0;
}
