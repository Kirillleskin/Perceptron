// PerceptronFirstTry.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <math.h>

using namespace std;

#define hiddenLayerNCount 9

double fActivation(double x) {
	return 1 / (1 + exp(-(x)));
}

class NeuralNetwork {
	private: 
		// Веса синапсов нейронов скрытого слоя		
		double** W = new double*[hiddenLayerNCount];
		// Веса синапсов нейрона выходного слоя
		double* resultS = new double[hiddenLayerNCount];

		double* Z = new double[hiddenLayerNCount];

		double y;
		double _y = 0;
		double sigma;

		// Обучение скрытого слоя
		double* _sigma = new double[hiddenLayerNCount];

		void learn(int x0, int x1, double t) {
			// Веса синапсов нейрона выходного слоя
			
			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				Z[i] = fActivation(x0 * W[i][0] + x1 * W[i][1]); // 1 / (1 + exp(-(x0 * W[i][0] + x1 * W[i][1])));
			}

			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				_y += Z[i] * resultS[i];
			}

			y = fActivation(_y); // 1 / (1 + exp(-(_y)));


			/* Обратное распространение ошибки */

			// Обучение выходного слоя
			sigma = y * (1 - y) * (t - y);
			
			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				resultS[i] = resultS[i] + 0.01 * sigma * Z[i];
			}

			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				_sigma[i] = Z[i] * (1 - Z[i]) * (sigma * resultS[i]);
			}

			// нейроны скрытого слоя
			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				W[i][0] = W[i][0] + 0.01 * _sigma[i] * x0;
				W[i][1] = W[i][1] + 0.01 * _sigma[i] * x1;
			}
		}

	// Инициализация весов
	public:
		// learning data
		// коэффициенты барита (3.14) (пока просто барит, потом это napolnitel)
		double barit[5] = { 0.1, 0.2, 0.3, 0.4, 0.5 };
		// давление по бариту из 3.14
		int baritPressure[5] = { 15, 21, 42, 450, 818 };

		// проценты из 3.16
		int parafinPercentage[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
		// коэффициенты изменения барита по графику 3.16 [соответствует по индексам % парафина(выше)]
		double factor[9] = { 1, 0.208, 0.177, 0.173, 0.168, 0.164, 0.16, 0.155, 0.151 };

		// не используется, просто из графика
		int pressure[10] = { 0, 100, 200, 300, 400, 500, 600, 700, 800, 900 };

		void initWeights() {
			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				for (int j = 0; j < 2; j++) {
					W[i][j] = ((double)rand() / (RAND_MAX)); // может быть и 0
				}
				resultS[i] = ((double)rand() / (RAND_MAX));
			}
		}

		// Веса скрытого слоя 
		void print() {
			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				cout << "W[" << i << "][0-1]: " << W[i][0] << " " << W[i][1] << endl;
			}
		}

		// Решение (прямой проход)
		double solve(int x0, int x1) {

			double z0;
			double z1;
			double* Z = new double[hiddenLayerNCount];

			double y;
			double _y = 0;

			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				Z[i] = fActivation(x0 * W[i][0] + x1 * W[i][1]); // 1 / (1 + exp(-(x0 * W[i][0] + x1 * W[i][1])));
			}

			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				_y += Z[i] * resultS[i];
			}

			y = fActivation(_y); // 1 / (1 + exp(-(_y)));

			return y;
		}

		void batch() {
			for (int i = 0; i < 100000000; ++i) {
				int _barit = 3; // barit 0.4
				int _parafin = rand() % 9; // 0-8

				learn(barit[_barit], parafinPercentage[_parafin]/8, (baritPressure[_barit]/818) * factor[_parafin]);
			}
		}
		
		NeuralNetwork() {
			for (int i = 0; i < hiddenLayerNCount; ++i) {
				W[i] = new double[2];
			}
		}
};

int main()
{
	cout << "Hi!" << endl;
	
	cout << endl;

	NeuralNetwork* network = new NeuralNetwork();

	network->initWeights();
	cout << "Before learning weights: " << endl;
	network->print();
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

	network->batch();
	cout << "After learning weights: " << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	network->print();
	cout << "------------------------------------" << endl;

	cout << "0.4 barit pressure: " << endl;
	
	for (int i = 0; i < 9; i++)
	{
		cout << network->baritPressure[3] * network->factor[i] << " ";
	}
	cout << endl << endl;

	// по идее тут по всем barit[0.1-0.6] но пока хотя бы 0.4
	for (int i = 0; i < 1; i++)
	{
		cout << "barit:  " << network->barit[3] << endl;
		for (int j = 0; j < 9; j++)
		{
			cout << j << "%: " << network->solve(network->barit[3], j) << endl; // barit 0.4
		}
		cout << "-----------------------------------" << endl;
	}

	int a;
	cin >> a;

    return 0;
}

