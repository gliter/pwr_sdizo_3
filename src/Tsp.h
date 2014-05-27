/*
 * Tsp.h
 *
 *  Created on: 24 maj 2014
 *      Author: Laxter
 */

#ifndef TSP_H_
#define TSP_H_

#include <stdlib.h>
#include <math.h>
#include <fstream>

using namespace std;

class Tsp {
public:
	//Macierz sasiedztwa
	unsigned **distMap;
	//Ilosc miast
	unsigned cities;
	Tsp() {
		init(0);
	}
	Tsp(unsigned cities) {
		init(cities);
	}
	/*
	 * Wspolna funkcja dla konstruktorow
	 * tworzaca i inicjalizujaca macierz i pola klasy
	 */
	void init(unsigned cities) {
		this->cities = cities;
		if(cities) {
			distMap = new unsigned*[cities];
			for(unsigned i = 0; i < cities; i ++) {
				distMap[i] = new unsigned[cities];
				for(unsigned j = 0; j < cities; j++)
					distMap[i][j] = 0;
			}
		}
		else
			distMap = 0;
	}
	virtual ~Tsp() {
		if(cities) {
			for(unsigned i = 0; i < cities; i++)
				delete[] distMap[i];
			delete[] distMap;
		}
		distMap = 0;
	}
	/*
	 * Funkcja do ustawienia dystansu
	 * from - numer miasta zrodlowego
	 * to - numer miasta docelowego
	 * dist - dystans
	 */
	void setDistance(unsigned from, unsigned to, unsigned dist) {
		distMap[from][to] = dist;
	}
	/*
	 * Funkcja zwracajaca odleglosc miedzy miastami
	 * from - numer miasta zrodlowego
	 * to - numer miasta docelowego
	 */
	unsigned getDistance(unsigned from, unsigned to) {
		return distMap[from][to];
	}
	/*
	 * Funkcja losujaca odleglosci miedzy miastami
	 */
	void randDistances() {
		for(unsigned i = 0; i < cities; i++)
			for(unsigned j = 0; j < cities; j++) {
				if(i != j) {
					setDistance(i, j, rand()%99 + 1);
				}
			}
	}
	/*
	 * Zaladuj odlegosci z pliku
	 * in - strumien wejsciowy
	 */
	void fromFile(ifstream &in) {
		unsigned cities;
		unsigned dist;
		in >> cities;
		init(cities);
		for(unsigned i = 0; i < cities ; i++)
			for(unsigned j = 0; j < cities; j++) {
				if(j == i) continue;
				in >> dist;
				setDistance(i, j, dist);
			}
	}
	/*
	 * Oblicz dystans dla podanej permutacji
	 * permutation - tablica z permutacja
	 */
	unsigned calcDist(unsigned *permutation) {
		unsigned distSum = 0;
		for(unsigned i = 0; i < cities - 1; i++)
			distSum += getDistance(permutation[i], permutation[i+1]);
		return distSum;
	}
	/*
	 * Generuje i sprawdza wszystkie permutacje (rekurencja)
	 * k - numer ostatniego indeksu miasta
	 * permutation - tablica z permutacja
	 * minPerm - tablica na permutacje wynikowa
	 * minDist - referencja do zmiennej przechowujacej najmniejsza sume odleglosci
	 */
	void perm(unsigned k, unsigned *permutation, unsigned *minPerm, unsigned &minDist) {
		if(k == 0) {
			unsigned dist = calcDist(permutation);
			if(dist < minDist) {
				minDist = dist;
				memcpy(minPerm, permutation, cities * 4);
			}
		}
		else
			for(unsigned i = 0; i <= k; i++) {
				unsigned temp = permutation[k];
				permutation[k] = permutation[i];
				permutation[i] = temp;

				perm(k - 1, permutation, minPerm, minDist);

				temp = permutation[k];
				permutation[k] = permutation[i];
				permutation[i] = temp;
			}
	}
	/*
	 * Wywolanie algorytmu przegladu zupelnego
	 * zwraca tablice z permutacja
	 */
	unsigned * checkAll() {
		unsigned *permutation = new unsigned[cities];
		for(unsigned i = 0; i < cities; i++)
			permutation[i] = i;
		unsigned minDist = 4294967295u;
		unsigned *minPerm = new unsigned[cities];

		perm(cities - 1, permutation, minPerm, minDist);

		delete[] permutation;

		return minPerm;
	}
	/*
	 * Funkcja drukujaca permutacje
	 * permutation - tablica zawierajaca permutacje miast
	 * out - strumien wyjsciowy
	 */
	void printPermutation(unsigned *permutation, ostream &out) {
		for(unsigned i = 0; i < cities; i++) {
			out << permutation[i] << " ";
		}
		out << endl << "Dystans: " << calcDist(permutation) << endl;
	}
	/*
	 * Funkcja resetujaca oznaczenie "odwiedzone" miasto
	 */
	void resetVisited() {
		for(unsigned i = 0; i < cities; i ++)
			setDistance(i, i, 0);
	}
	/*
	 * Algorytm zachlanny, wykorzystuje odleglosc z miasta A do A jako znacznik "odwiedzenia" miasta
	 * start - miasto poczatkowe
	 */
	unsigned * greedy(unsigned start) {
		unsigned *result = new unsigned[cities];
		result[0] = start;
		setDistance(start, start, 1); //Oznacz miasto jako odwiedzone

		for(unsigned i = 0; i < cities - 1; i++) {
			unsigned minDist = 4294967295u;
			unsigned minCity = 0;
			//Wyszukaj najblizszego nieodwiedzonego miasta
			for(unsigned j = 0; j < cities; j++) {
				unsigned dist = getDistance(result[i], j);
				if((getDistance(j, j) == 0)&&(dist < minDist)) {
					minDist = dist;
					minCity = j;
				}
			}
			setDistance(minCity, minCity, 1); //Oznacz miasto jako odwiedzone
			result[i+1] = minCity;
		}
		return result;
	}
	/*
	 * Wywolanie algorytmow zachlannych dla wszystkich miast jako poczatkowe
	 * i wybor najoptymalniejszego
	 */
	unsigned * greedyAll() {
		unsigned * result = 0;
		unsigned minDist = 4294967295u;
		for(unsigned i = 0; i < cities; i++) {
			unsigned * temp = greedy(i);
			unsigned tempDist = calcDist(temp);
			resetVisited();
			if(tempDist < minDist) {
				delete [] result;
				result = temp;
				minDist = tempDist;
			}
			else
				delete[] temp;
		}
		return result;
	}
	/*
	 * Funkcja pomocnicza dla algorytmy 2-opt, zamienia odcinek drogi <i, k>
	 * permutation - tablica z permutacja miast
	 * i - poczatek drogi do zamiany - wlacznie
	 * k - koniec drogi do zamiany - wlacznie
	 */
	unsigned * swap2opt(unsigned * permutation, unsigned i, unsigned k) {
		unsigned *result = new unsigned[cities];
		memcpy(result, permutation, i * 4);
		memcpy(result + k + 1, permutation + k + 1, (cities - k - 1) * 4);
		for(unsigned j = i; j <= k; j++) {
			result[j] = permutation[k - (j - i)];
		}
		return result;
	}
	/*
	 * Algorytm 2-opt
	 */
	unsigned * alg2opt() {
		unsigned *permutation = new unsigned[cities];
		for(unsigned i = 0; i < cities; i++)
			permutation[i] = i;
		unsigned curDist = calcDist(permutation);
		bool imp = false;
		do {
			imp = false;

			for(unsigned i = 0; i < cities - 1; i++) {
				for(unsigned k = i + 1; k < cities; k++) {
					unsigned *newRoute = swap2opt(permutation, i, k);
					unsigned newDist = calcDist(newRoute);
					if(newDist < curDist) {
						imp = true;
						delete[] permutation;
						permutation = newRoute;
						curDist = newDist;
					}
					else
						delete[] newRoute;
				}
			}

		}while(imp);
		return permutation;
	}
	friend ostream & operator<<(ostream &out, Tsp &tsp);
};
ostream & operator<<(ostream &out, Tsp &tsp) {
	out.width(2);
	out << right << "\\";
	for(unsigned i = 0; i < tsp.cities; i++) {
		out.width(4);
		out << right << i;
	}
	out << endl;
	for(unsigned i = 0; i < tsp.cities; i++) {
		out.width(2);
		out << right << i;
		for(unsigned j = 0; j < tsp.cities; j++) {
			out.width(4);
			out << right << tsp.distMap[i][j];
		}
		out << endl;
	}
	out << endl;
	return out;
}
#endif /* TSP_H_ */
