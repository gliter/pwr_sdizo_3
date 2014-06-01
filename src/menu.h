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

}



#endif /* MENU_H_ */
