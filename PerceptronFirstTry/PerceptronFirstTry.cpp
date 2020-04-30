// PerceptronFirstTry.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <windows.h>
#include <string>

using namespace std;

//#define hiddenLayerNCount 9
//#define delta 0.01
#define learnIterations 600

double fActivation(double x) {
	//return tanh(x); // гиперболический тангенс
	//return (2 / (1 + exp(-2 * x))) - 1; // тангенс
	return 1 / (1 + exp(-(x))); // сигмоида
}

class NeuralNetwork {
	private: 
		// Веса синапсов нейронов скрытого слоя		
		double** W;
		// Веса синапсов нейрона выходного слоя
		double* resultS;

		double* Z;

		double y;
		double _y = 0;
		double sigma;

		// Обучение скрытого слоя
		double* _sigma;

		void learn(double x0, double x1, double t, int iteration) {
			_y = 0;
			
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
			sigma = y * (1 - y) * (t - y)	; //sigmoid
			//sigma = (1 - y * y) * (t - y);//tangens

			_sigmaTest[iteration] = sigma;

			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				resultS[i] = resultS[i] + delta * sigma * Z[i];
			}

			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				_sigma[i] = Z[i] * (1 - Z[i]) * (sigma * resultS[i]);
			}

			// нейроны скрытого слоя
			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				W[i][0] = W[i][0] + delta * _sigma[i] * x0;
				W[i][1] = W[i][1] + delta * _sigma[i] * x1;
			}
		}

	// Инициализация весов
	public:
		double delta = 1; // скорость обучения
		int hiddenLayerNCount; // количество нейронов скрытого слоя
		double* _sigmaTest; // ошибки на каждой итерации

		// learning data
		// коэффициенты барита (3.14) (пока просто барит, потом это napolnitel)
		double barit[5] = { 0.1, 0.2, 0.3, 0.4, 0.5 };
		// давление по бариту из 3.14
		int baritPressure[5] = { 15, 21, 42, 450, 818 };
		
		// проценты из 3.16
		int parafinPercentage[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
		// коэффициенты изменения барита по графику 3.16 [соответствует по индексам % парафина(выше)]
		double factor[9] = { 1, 0.208, 0.177, 0.173, 0.168, 0.164, 0.16, 0.155, 0.151 };

		// показатели для барита = 0.4 и %[0-8]
		// не используется, просто из графика
		double pressure[10] = { 450, 93.6, 79.65, 77.85, 75.6, 73.8, 72, 69.75, 67.95 };

		void initWeights() {
			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				for (int j = 0; j < 2; j++) {
					W[i][j] = ((double)rand() / (RAND_MAX)); // может быть и 0
				}
				resultS[i] = ((double)rand() / (RAND_MAX));
			}
		}

		void printWeights() {
			cout << "------------------------------------" << endl;
			cout << "Weights: " << endl;
			print();
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		}

		// Веса скрытого слоя 
		void print() {
			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				cout << "W[" << i << "][0-1]: " << W[i][0] << " " << W[i][1] << endl;
			}
		}

		// Решение (прямой проход)
		double solve(double x0, double x1) {

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
			for (int i = 0; i < learnIterations; i++) 
			{
				int _barit = 3;// rand() % 5; // barit 0.1-0.5
				int _parafin = rand() % 9; // 0-8

				learn(barit[_barit], (double)parafinPercentage[_parafin] / 8, baritPressure[_barit] * factor[_parafin] / 900, i);
			}
		}
		
		NeuralNetwork(int k) {
			hiddenLayerNCount = k;

			W = new double*[hiddenLayerNCount];
			for (int i = 0; i < hiddenLayerNCount; i++) {
				W[i] = new double[2];
			}

			resultS = new double[hiddenLayerNCount];
			Z = new double[hiddenLayerNCount];
			_sigma = new double[hiddenLayerNCount];
			_sigmaTest = new double[learnIterations];
		}
};

void printLine(int x, ofstream _file) {

}

int main()
{
	system("color 0a");
	cout << "Processing...\n";
/*
	NeuralNetwork* network = new NeuralNetwork();

	network->initWeights();*/
/*
	for (double o = 1; o >= 0.01; o -= 0.01)
	{
		_file << "delta:  " << o << "%\n";
		_file << "------------------------------------ - \n";
	}*/
	//network->batch();

	ofstream _file;
	_file.open("data.txt");
	double jIncrease = 1;

	if (_file.is_open())
	{
		_file << "step:  " << jIncrease << "%\n";
		
		for (int i = 3; i < 4; i++)
		{
			/*_file << "barit:  " << network->barit[i] << "\n";
			_file << "=====================================\n";
			_file << "ideal stats:\n";
			for (double j = 0; j <= 8; j += jIncrease)
			{
				_file << network->baritPressure[i] * network->factor[(int)j] << " ";
			}
			_file << "\n=====================================\n";*/

			double idealParam = 1;
			double deviation = DBL_MAX;
			double jkdeviation = 0;
			string jkdeviations = "";
			//network->delta = 0.52;
			
			for (int k = 3; k <= 3; k += 1)
			{
				NeuralNetwork* network = new NeuralNetwork(k);
				_file << "barit:  " << network->barit[i] << "\n";
				_file << "=====================================\n";
				_file << "ideal stats:\n";
				for (double j = 0; j <= 8; j += jIncrease)
				{
				_file << network->baritPressure[i] * network->factor[(int)j] << " ";
				}
				_file << "\n=====================================\n";

				_file << "layer:  " << k << "\n";
				_file << "------------------------------------- \n";

				network->delta = 0.52;
				network->initWeights();

				network->batch();

				for (double j = 0; j <= 8; j += jIncrease)
				{
					_file << network->solve(network->barit[i], j / 8) << " ";
				}
				_file << "\n-------------------------------------\n";
				
				jkdeviation = 0;
				for (double j = 0; j <= 8; j += jIncrease)
				{
					double solve = network->solve(network->barit[i], j / 8) * 900;
					_file << solve << " ";
					jkdeviation += abs((network->baritPressure[i] * network->factor[(int)j]) - solve);
				}
				jkdeviations += to_string(jkdeviation) + ' ';

				_file << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
				_file << "jkdeviation: " << jkdeviation;
				_file << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

				if (deviation > jkdeviation)
				{
					deviation = jkdeviation;
					idealParam = k;
				}

				std::ofstream os("sigma.txt");
				if (!os.is_open())
					return false;
				for (int i = 0; i < learnIterations; i++)
				{
					os << to_string(network->_sigmaTest[i]) + "\n";
				}
				os.close();

				delete network;
			}

			_file << "\n";
			_file << "\n";
			_file << "deviation: " << deviation;
			_file << "\n";
			_file << "idealParam: " << idealParam;
			_file << "\n";
			_file << "jkdeviations: " << jkdeviations;


			/*for (double j = 0; j <= 8; j += jIncrease)
			{
				_file << network->solve(network->barit[i], j / 8) << " ";
			}
			_file << "\n-------------------------------------\n";

			for (double j = 0; j <= 8; j += jIncrease)
			{
				_file << network->solve(network->barit[i], j / 8) * 900 << " ";
			}

			_file << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";*/
		}
		

		_file.close();
	}

	cout << "done!";
	//int i;
	//cin >> i;
    return 0;
}

