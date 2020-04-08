// PerceptronFirstTry.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <math.h>

using namespace std;

class NeuralNetwork {
	private: 
		// Веса синапсов первого нейрона скрытого слоя
		double* W0 = new double[2];
		// Веса синапсов второго нейрона скрытого слоя
		double* W1 = new double[2];
		// Веса синапсов нейрона выходного слоя
		double* W2 = new double[2];

		void learn(int x0, int x1, double t) {
			double z0;
			double z1;
			double y;

			// Вычисление выходов нейронов
			z0 = 1 / (1 + exp(-(x0 * W0[0] + x1 * W0[1])));
			z1 = 1 / (1 + exp(-(x0 * W1[0] + x1 * W1[1])));
			y = 1 / (1 + exp(-(z0 * W2[0] + z1 * W2[1])));

			/* Обратное распространение ошибки */

			// Обучение выходного слоя
			double sigma = y * (1 - y) * (t - y);
			W2[0] = W2[0] + 0.001 * sigma * z0;
			W2[1] = W2[1] + 0.001 * sigma * z1;

			// Обучение скрытого слоя
			double sigma0 = z0 * (1 - z0) * (sigma * W2[0]);
			double sigma1 = z1 * (1 - z1) * (sigma * W2[1]);

			// Первый нейрон скрытого слоя
			W0[0] = W0[0] + 0.001 * sigma0 * x0;
			W0[1] = W0[1] + 0.001 * sigma0 * x1;

			// Второй нейрон скрытого слоя
			W1[0] = W1[0] + 0.001 * sigma1 * x0;
			W1[1] = W1[1] + 0.001 * sigma1 * x1;
		}


	// Инициализация весов
	public:
		void initWeights() {
			for (int i = 0; i < 2; ++i) {
				W0[i] = ((double)rand() / (RAND_MAX));
				W1[i] = ((double)rand() / (RAND_MAX));
				W2[i] = ((double)rand() / (RAND_MAX));
			}
		}

		// Сохранения весов в файле
		void print() {
			cout << "w00: " << W0[0] << ' ' << "w01: " << W0[1] << endl;
			cout << "w10: " << W1[0] << ' ' << "w11: " << W1[1] << endl;
			cout << "w20: " << W2[0] << ' ' << "w22: " << W2[1] << endl;
		}

		// Решение (прямой проход)
		double solve(int x0, int x1) {

			double z0;
			double z1;
			double y;

			// Вычисление выхода первого нейрона скрытого слоя
			z0 = 1 / (1 + exp(-(x0 * W0[0] + x1 * W0[1])));
			// Вычисление выхода второго нейрона скрытого слоя
			z1 = 1 / (1 + exp(-(x0 * W1[0] + x1 * W1[1])));
			// Вычисление выхода нейрона выходного слоя (собсвенно результат)
			y = 1 / (1 + exp(-(z0 * W2[0] + z1 * W2[1])));

			return y;
		}

		void batch() {
			for (int i = 0; i < 10000000; ++i) {
				int x0 = rand() % 2;
				int x1 = rand() % 2;

				learn(x0, x1, (x0 + x1) % 2);
			}
		}
		
		NeuralNetwork() { }
};

int main()
{
	cout << "Hi!" << endl;
	/*
	int x1;
	cout << "x1 - ";
	cin >> x1;
	

	int x2;
	cout << "x2 - ";
	cin >> x2;*/
	
	cout << endl;

	NeuralNetwork* network = new NeuralNetwork();

	network->initWeights();
	cout << "----------------------------------" << endl;
	network->print();
	cout <<"----------------------------------"<< endl;

	network->batch();
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	network->print();
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

	cout << "00: " << network->solve(0, 0) << endl;
	cout << "01: " << network->solve(0, 1) << endl;
	cout << "10: " << network->solve(1, 0) << endl;
	cout << "11: " << network->solve(1, 1) << endl;

	int a;
	cin >> a;

    return 0;
}

