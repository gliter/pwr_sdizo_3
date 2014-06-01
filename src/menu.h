/*
 * menu.h
 *
 *  Created on: 25 maj 2014
 *      Author: Laxter
 */

#ifndef MENU_H_
#define MENU_H_

#include <iostream>
using namespace std;

#include <stdlib.h>
#include <math.h>
#include "Bag.h"
#include "Tsp.h"
#include "timeMes.h"

void menu();
void bagMenu();
void tspMenu();
void sim();

void menu() {
	int opt;
	do {

		cout << "Menu glowne" << endl;
		cout << "===============================" << endl;
		cout << "1. Problem plecakowy" << endl;
		cout << "2. Problem komiwojazera" << endl;
		cout << "3. Pomiary" << endl;
		cout << "0. Zakoncz" << endl;
		cout << "?>";
		cin >> opt;

		switch(opt) {

		case 1:
			bagMenu();
			break;
		case 2:
			tspMenu();
			break;
		case 3:
			sim();
			break;
		case 0:
			break;
		default:
			cout << "Nie poprawna opcja!" << endl;

		}
	}while(opt != 0);
}
void bagMenu() {
	Bag *bag = 0;

	int opt;

	do {
		cout << "Problem plecakowy" << endl;
		cout << "===============================" << endl;
		cout << "1. Utworz losowy plecak" << endl;
		cout << "2. Wczytaj plecak z pliku" << endl;
		cout << "3. Przeglad zupelny" << endl;
		cout << "4. Algorytm zachlanny" << endl;
		cout << "5. Programowanie dynamiczne" << endl;
		cout << "0. Wyjdz" << endl;
		cout << "?>";
		cin >> opt;

		switch(opt) {

		case 1:
		{
			unsigned size, elementCount;
			cout << "Rozmiar plecaka: ";
			cin >> size;
			cout << "Ilosc elementow: ";
			cin >> elementCount;
			delete bag;
			bag = new Bag(elementCount, size);
			bag->randItems();
			cout << *bag;
			break;
		}
		case 2:
		{
			delete bag;
			bag = new Bag();
			ifstream file("bag.txt", ios::in);
			bag->fromFile(file);
			file.close();
			cout << *bag;
			break;
		}
		case 3:
		{
			if(bag) {
				unsigned long long subSet = bag->checkAll();
				bag->printSubSet(subSet, cout);
			}
			break;
		}
		case 4:
		{
			if(bag) {
				int* subSet = bag->aproximate();
				bag->printSubSet(subSet, cout);
				delete[] subSet;
			}
			break;
		}
		case 5:
		{
			if(bag) {
				int* subSet = bag->dynamic();
				bag->printSubSet(subSet, cout);
				delete[] subSet;
			}
			break;
		}
		case 0:
		{
			break;
		}
		default:
			cout << "Nie poprawna opcja!" << endl;
		}


	}while(opt != 0);

}
void tspMenu() {
	int opt;

	Tsp *tsp = 0;

		do {
			cout << "Problem komiwojazera" << endl;
			cout << "===============================" << endl;
			cout << "1. Utworz losowy graf" << endl;
			cout << "2. Wczytaj graf z pliku" << endl;
			cout << "3. Przeglad zupelny" << endl;
			cout << "4. Algorytm zachlanny" << endl;
			cout << "5. Algorytm 2-opt" << endl;
			cout << "0. Wyjdz" << endl;
			cout << "?>";
			cin >> opt;

			switch(opt) {

			case 1:
			{
				unsigned size;
				cout << "Ilosc miast: ";
				cin >> size;
				delete tsp;
				tsp = new Tsp(size);
				tsp->randDistances();
				cout << *tsp;
				break;
			}
			case 2:
			{
				delete tsp;
				tsp = new Tsp();
				ifstream file("cities.txt", ios::in);
				tsp->fromFile(file);
				file.close();
				cout << *tsp;
				break;
			}
			case 3:
			{
				if(tsp) {
					unsigned* perm = tsp->checkAll();
					tsp->printPermutation(perm, cout);
					delete[] perm;
				}
				break;
			}
			case 4:
			{
				if(tsp) {
					unsigned* perm = tsp->greedyAll();
					tsp->printPermutation(perm, cout);
					delete[] perm;
				}
				break;
			}
			case 5:
			{
				if(tsp) {
					unsigned* perm = tsp->alg2opt();
					tsp->printPermutation(perm, cout);
					delete[] perm;
				}
				break;
			}
			case 0:
			{
				break;
			}
			default:
				cout << "Nie poprawna opcja!" << endl;
			}


		}while(opt != 0);


}
void sim() {
	const unsigned instances = 100;
	{
		cout << "Testowanie plecaka..." << endl;

		const unsigned paramNSize = 5;
		const unsigned paramN2Size = 10;
		const unsigned paramBSize = 3;


		//unsigned paramN[paramNSize] = {5, 10, 15, 20, 25};
		unsigned paramN[paramN2Size] = {15, 20, 23, 25, 27, 100, 200, 300, 400, 500};
		unsigned paramB[paramBSize] = {1000, 2500, 5000};

		ofstream CAFile("bag_checkAll.txt", ios::out);
		ofstream GFile("bag_greedy.txt", ios::out);
		ofstream DFile("bag_dynamic.txt", ios::out);

		CAFile << "B\tN\ttime" << endl;
		GFile << "B\tN\ttime" << endl;
		DFile << "B\tN\ttime" << endl;

		double start, end;

		Bag **bags;
		bags = new Bag*[instances];
		cout << "Algorytm zachlanny..." << endl;
		for(unsigned index_b = 0; index_b < paramBSize; index_b++) {
			unsigned b = paramB[index_b];
			for(unsigned index_n = 0; index_n < paramNSize; index_n++) {
				unsigned n = paramN[index_n];
				cout << (index_b * paramNSize + index_n) * 100 / (paramBSize * paramNSize) << "% ";
				//Tworzenie plecakow
				for(unsigned i = 0; i < instances; i++)
					bags[i] = new Bag(n, b);

				//Pomiar czasu dla zupelnego
				start = startTimer();
				for(unsigned i = 0; i < instances; i++) {
					bags[i]->checkAll();
				}
				end = startTimer();
				CAFile << fixed << b << "\t" << n << "\t" << (end - start)/instances << endl;

				//Zwolnienie plecakow
				for(unsigned i = 0; i < instances; i++)
					delete bags[i];

			}
		}
		cout << "100%" << endl;
		cout << "Algorytm zachlanny i programowanie dynaiczne..." << endl;
		for(unsigned index_b = 0; index_b < paramBSize; index_b++) {
			unsigned b = paramB[index_b];
			for(unsigned index_n = 0; index_n < paramN2Size; index_n++) {
				unsigned n = paramN[index_n];
				cout << (index_b * paramN2Size + index_n) * 100 / (paramBSize * paramN2Size) << "% ";
				//Tworzenie plecakow
				for(unsigned i = 0; i < instances; i++)
					bags[i] = new Bag(n, b);

				//Pomiar czasu dla zachlannego
				start = startTimer();
				for(unsigned i = 0; i < instances; i++) {
					int* subSet = bags[i]->aproximate();
					delete[] subSet;
				}
				end = startTimer();
				GFile << fixed << b << "\t" << n << "\t" << (end - start)/instances << endl;

				//Pomiar czasu dla programowania dynamicznego
				start = startTimer();
				for(unsigned i = 0; i < instances; i++) {
					int* subSet = bags[i]->dynamic();
					delete[] subSet;
				}
				end = startTimer();
				DFile << fixed << b << "\t" << n << "\t" << (end - start)/instances << endl;

				//Zwolnienie plecakow
				for(unsigned i = 0; i < instances; i++)
					delete bags[i];

			}
		}



		cout << "100%" << endl;
	}
	{
		cout << "Testowanie komiwojazera..." << endl;

		const unsigned paramNSize = 5;
		const unsigned paramN2Size = 10;

		unsigned paramN[paramN2Size] = {2, 4, 6, 8, 10, 50, 100, 150, 200, 250};

		ofstream CAFile("tsp_checkAll.txt", ios::out);
		ofstream GFile("tsp_greedy.txt", ios::out);
		ofstream DFile("tsp_alg2opt.txt", ios::out);

		CAFile << "N\ttime" << endl;
		GFile << "N\ttime" << endl;
		DFile << "N\ttime" << endl;

		double start, end;

		Tsp **tsps;
		tsps = new Tsp*[instances];

		cout << "Przeglad zupelny..." << endl;
		for(unsigned index_n = 0; index_n < paramNSize; index_n++) {
			cout << index_n * 100 / paramNSize << "% ";

			unsigned n = paramN[index_n];

			//Tworzenie map miast
			for(unsigned i = 0; i < instances; i++) {
				tsps[i] = new Tsp(n);
			}

			//Pomiar czasu dla zupelnego
			start = startTimer();
			for(unsigned i = 0; i < instances; i++) {
				unsigned *perm = tsps[i]->checkAll();
				delete[] perm;
			}
			end = startTimer();
			CAFile << fixed << n << "\t" << (end - start)/instances << endl;

			//Zwalnianie map miast
			for(unsigned i = 0; i < instances; i++) {
				delete tsps[i];
			}
		}
		cout << "100% " << endl;
		cout << "Algorytm zachlanny i 2-opt..." << endl;
		for(unsigned index_n = 0; index_n < paramN2Size; index_n++) {
			cout << index_n * 100 / paramN2Size << "% ";

			unsigned n = paramN[index_n];

			//Tworzenie map miast
			for(unsigned i = 0; i < instances; i++) {
				tsps[i] = new Tsp(n);
			}

			//Pomiar czasu dla zachlannego
			start = startTimer();
			for(unsigned i = 0; i < instances; i++) {
				unsigned *perm = tsps[i]->greedyAll();
				delete[] perm;
			}
			end = startTimer();
			GFile << fixed << n << "\t" << (end - start)/instances << endl;
			//Pomiar czasu dla alg 2-opt
			start = startTimer();
			for(unsigned i = 0; i < instances; i++) {
				unsigned *perm = tsps[i]->alg2opt();
				delete[] perm;
			}
			end = startTimer();
			DFile << fixed << n << "\t" << (end - start)/instances << endl;

			//Zwalnianie map miast
			for(unsigned i = 0; i < instances; i++) {
				delete tsps[i];
			}
		}
		cout << "100%" << endl;
	}

}



#endif /* MENU_H_ */
