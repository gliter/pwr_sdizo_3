/*
 * Bag.h
 *
 *  Created on: 24 maj 2014
 *      Author: Grzegorz Liter
 */

#ifndef BAG_H_
#define BAG_H_

#include <stdlib.h>
#include <math.h>
#include <fstream>

using namespace std;

class Bag {
public:

	//Ilosc elementow w plecaku
	unsigned elementCount;
	//Rozmiar plecaka
	unsigned size;

	//Tabela rozmiarow
	unsigned *sizes;
	//Tabela wag
	unsigned *weights;

	Bag() {
		init(0, 0);
	}
	Bag(unsigned elementCount, unsigned size) {
		init(elementCount, size);
	}
	/*
	 * Wspolna funkcja dla konstruktorow inicjalizujaca pola klasy
	 * i tworzoca tablice do przechowywania przedmiotow
	 * elementCount - ilosc elementow
	 * size - rozmiar plecaka
	 */
	void init(unsigned elementCount, unsigned size) {
		this->size = size;
		this->elementCount = elementCount;

		if(size) {
			this->sizes = new unsigned[elementCount];
			this->weights = new unsigned[elementCount];
		}
		else {
			this->sizes = 0;
			this->weights = 0;
		}

	}
	virtual ~Bag() {
		delete[] sizes;
		delete[] weights;
		sizes = 0;
		weights = 0;
	}
	/*
	 * Generowanie losowych przedmiotow w plecaku
	 * Suma rozmiarow przedmiotow wylosowanych bedzie wieksza niz
	 * 1.25 * rozmiar plecaka (w okolicach 1.3 * rozmiar plecaka)
	 */
	void randItems() {
		unsigned sizeSum = 0;
		for(unsigned i = 0; i < elementCount; i++) {
			sizes[i] = rand()%98+1;
			sizeSum += sizes[i];
			weights[i] = rand()%98+1;
		}
		while(sizeSum < size + size / 10 * 3) {
			for(unsigned i = 0; i < elementCount; i++) {
				unsigned size = rand()%30;
				sizes[i] += size;
				sizeSum += size;
			}
		}
	}
	/*
	 * Wczytywanie plecaka z pliku
	 * in - strumien pliku
	 */
	void fromFile(ifstream &in) {
		delete[] sizes;
		delete[] weights;
		in >> size;
		in >> elementCount;
		init(elementCount, size);
		for(unsigned i = 0; i < elementCount; i++) {
			in >> sizes[i];
			in >> weights[i];
		}
	}
	friend ostream & operator<<(ostream &out, Bag &b);
	/*
	 * Zwraca sume wag podzbioru przedmiotow utworzonego na podstawie liczby setNum
	 * kolejne bity tej liczby beda traktowane jako prawda/falsz dla kolejnych przedmiotow
	 *
	 * Funkcja zwraca 0 w przypadku przekroczenia pojemnosci plecaka
	 *
	 * W zwiazku z 64 bitami w zmiennej typu long long, mozna testowac plecak
	 * tylko z maksymalnie 64 przedmiotami
	 */
	unsigned getSumWeight(unsigned long long setNum) {
		unsigned sizeSum = 0, weightSum = 0;
		for(unsigned i = 0; i < elementCount; i++) {
			if(setNum % 2) {
				sizeSum += sizes[i];
				weightSum += weights[i];
			}
			setNum /= 2; //Z jakiegos powodu w srodowisku nie dziala mi przesuniecie bitowe
		}
		if(sizeSum <= size)
			return weightSum;
		return 0;
	}
	/*
	 * Drukuje podzbior utworzony na podstawie liczby setNum j/w
	 */
	void printSubSet(unsigned long long setNum, ostream &out) {
		unsigned sizeSum = 0, weightSum = 0;
		out << "s/w: ";
		for(unsigned i = 0; i < elementCount; i++) {
			if(setNum % 2) {
				sizeSum += sizes[i];
				weightSum += weights[i];
				out << i << ":" << sizes[i] << "/" << weights[i] << " ";
			}
			setNum /= 2; //Z jakiegos powodu w srodowisku nie dziala mi przesuniecie bitowe
		}
		out << endl << "Pojemnosc: " << size << " Suma rozmiarow: " << sizeSum << " Suma wag: " << weightSum << endl;
	}
	/*
	 * Drukuje podzbior utworzony na podstawie tablicy z elementami wchodzacymi w sklad rozwiazania
	 */
	void printSubSet(int *result, ostream &out) {
		unsigned sizeSum = 0, weightSum = 0;
		out << "s/w: ";
		for(unsigned i = 0 ; i < elementCount; i++) {
			if(result[i] < 0)
				break;
			out << result[i] << ":" << sizes[result[i]] << "/" << weights[result[i]] << " ";
			sizeSum += sizes[result[i]];
			weightSum += weights[result[i]];
		}
		out << endl << "Pojemnosc: " << size << " Suma rozmiarow: " << sizeSum << " Suma wag: " << weightSum << endl;
	}
	/*
	 * Przeglad zupelny
	 * zwraca setNum
	 */
	unsigned long long checkAll() {
		unsigned long long setCount = 1;
		for(unsigned int i = 0; i < elementCount; i++) {
			setCount *= 2;
		}
		unsigned maxSubSet = 0;
		unsigned maxWeight = 0;
		for(unsigned long long i = 0; i <= setCount; i++) {
			unsigned sumWeight = getSumWeight(i);
			if(sumWeight > maxWeight) {
				maxWeight = sumWeight;
				maxSubSet = i;
			}
		}
		return maxSubSet;
	}
	/*
	 * Algorytm aproksymacyjny zachlanny
	 * zwraca tablice elementow ktore weszly w sklad rozwiazania
	 */
	int* aproximate() {
		//Sortowanie
		unsigned *sorted = new unsigned[elementCount];
		for(unsigned i = 0; i < elementCount; i++)
			sorted[i] = i;
		for(unsigned i = 0; i < elementCount - 1; i++)
			for(unsigned j = 0; j < elementCount - 1 - i; j++) {
				double element_h = ((double)weights[sorted[j]])/sizes[sorted[j]];
				double next_h = ((double)weights[sorted[j+1]])/sizes[sorted[j+1]];
				if(element_h < next_h) {
					unsigned temp = sorted[j];
					sorted[j] = sorted[j+1];
					sorted[j+1] = temp;
				}
			}
		//for(unsigned i = 0; i < elementCount; i++)
		//			cout << sorted[i] << ":" << ((double)weights[sorted[i]])/sizes[sorted[i]] << " ";

		int *result = new int[elementCount];
		for(unsigned i = 0; i < elementCount; i++)
			result[i] = -1;

		unsigned resultIndex = 0;
		unsigned sizeSum = 0;
		for(unsigned i = 0; i < elementCount; i++) {
			if(sizeSum + sizes[sorted[i]] < size) {
				result[resultIndex] = sorted[i];
				resultIndex++;
				sizeSum += sizes[sorted[i]];
			}
		}
		delete[] sorted;

		return result;
	}
	/*
	 * Algorytm rozwi¹zania dynamicznego
	 * zwraca tablicê elementów które wesz³y w sk³ad rozwi¹zania
	 */
	int* dynamic() {
		unsigned **a = new unsigned*[elementCount + 1];
		for(unsigned i = 0; i < elementCount + 1; i++) {
			a[i] = new unsigned[size+ 1];
			for(unsigned j = 0; j < size + 1; j++)
				a[i][j] = 0;
		}

		for(unsigned i = 1; i < elementCount + 1; i++)
			for(unsigned j = 0; j < size + 1; j++) {
				if(sizes[i-1] > j) {
					a[i][j] = a[i-1][j];
				}
				else {
					if( a[i-1][j] > a[i-1][j - sizes[i-1]] + weights[i-1] )
						a[i][j] = a[i-1][j];
					else
						a[i][j] = a[i-1][j - sizes[i-1]] + weights[i-1];
				}
			}
		//unsigned result = a[elementCount][size];
		/*
		//================================================
		ostream &out = cout;
		out.width(2);
			out << right << "\\";
			for(unsigned i = 0; i < size+1; i++) {
				out.width(4);
				out << right << i;
			}
			out << endl;
			for(unsigned i = 0; i < elementCount+1; i++) {
				out.width(2);
				out << right << i;
				for(unsigned j = 0; j < size+1; j++) {
					out.width(4);
					out << right << a[i][j];
				}
				out << endl;
			}
			out << endl;
		//================================================
		*/
		int *result = new int[elementCount];
			for(unsigned i = 0; i < elementCount; i++)
				result[i] = -1;


		unsigned resultIndex = 0;
		unsigned sizeIndex = size;
		for(int i = elementCount; i > 0; i--) {
			if(a[i][sizeIndex] != a[i-1][sizeIndex]) {
				result[resultIndex] = i-1;
				resultIndex++;
				sizeIndex -= sizes[i-1];
			}
		}

		for(unsigned i = 0; i < elementCount; i++)
			delete[] a[i];
		delete[] a;
		a = 0;


		return result;
	}

};

ostream & operator<<(ostream &out, Bag &b) {
	unsigned sizeSum = 0, weightSum = 0;
	out << "s/w: ";
	for(unsigned i = 0; i < b.elementCount; i++) {
		out << i << ":" << b.sizes[i] << "/" << b.weights[i] << " ";
		sizeSum += b.sizes[i];
		weightSum += b.weights[i];
	}
	out << endl;
	out << "Pojemnosc: " << b.size << " Suma rozmiarow: " << sizeSum << " Suma wag: " << weightSum << endl;
	return out;
}

#endif /* BAG_H_ */
