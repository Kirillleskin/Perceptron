// PerceptronFirstTry.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <windows.h>
#include <string>

using namespace std;

#define learnIterations 1000000

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

		double y = 0;
		double _y = 0;
		double sigma;

		// Обучение скрытого слоя
		double* _sigma;

		void learn(double x0, double x1, double t, int iteration) {
			_y = 0;
			
			// Веса синапсов нейрона выходного слоя
			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				Z[i] = fActivation(x0 * W[i][0] + x1 * W[i][1]);
			}

			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				_y += Z[i] * resultS[i];
			}

			y = fActivation(_y);


			/* Обратное распространение ошибки */

			// Обучение выходного слоя
			sigma = y * (1 - y) * (t - y)	;

			_sigmaErrors[iteration] = sigma;

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
		double* _sigmaErrors; // ошибки на каждой итерации

		// коэффициенты объемов барита
		double barit[5] = { 0.1, 0.2, 0.3, 0.4, 0.5 };
		// соответствующее объему барита давление 
		int baritPressure[5] = { 15, 21, 42, 450, 818 };
		// процент содержания парафина
		double parafinPercentage[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
		// коэффициенты изменения барита
		double factor[9] = { 1, 0.208, 0.177, 0.173, 0.168, 0.164, 0.16, 0.155, 0.151 };
		
		void initWeights() {
			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				for (int j = 0; j < 2; j++) {
					W[i][j] = ((double)rand() / (RAND_MAX));
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
				Z[i] = fActivation(x0 * W[i][0] + x1 * W[i][1]);
			}
			for (int i = 0; i < hiddenLayerNCount; i++)
			{
				_y += Z[i] * resultS[i];
			}
			y = fActivation(_y);
			return y;
		}

		void batch() {
			for (int i = 0; i < learnIterations; i++) 
			{
				int _barit = rand() % 5;
				int _parafin = rand() % 9;

				learn(barit[_barit], (double)parafinPercentage[_parafin] / 4, baritPressure[_barit] * factor[_parafin] / 900, i);
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
			_sigmaErrors = new double[learnIterations];
		}
};

int main()
{
	system("color 0a");
	cout << "Processing...\n";
	
	// шаг по оси Ох
	double pIncrease = 1;	
	for (int barit = 3; barit <= 3; barit++)
	{
			
		for (int layer = 5; layer <= 5; layer += 1)
		{
			// отклонение вычислений
			/*double idealParam = 1;
			double jkdeviation = 0; 
			string jkdeviations = "";
			double deviation = DBL_MAX;
*/
			NeuralNetwork* network = new NeuralNetwork(layer);

			ofstream _file;
			_file.open("layers/layer" + (to_string(layer)) + ".txt");

			if (_file.is_open())
			{
				for (double parafin = 0; parafin <= 8; parafin += pIncrease)
				{
					_file << " " << parafin << " ";
				}

				_file << "\nЭталон";

				for (double parafin = 0; parafin <= 8; parafin += 1)
				{
					_file << " " << network->baritPressure[barit] * network->factor[(int)parafin];
				}
				
				for (double _delta = 0.52; _delta <= 0.52; _delta += 0.01)
				{
					network->delta = _delta;
					network->initWeights();

					network->batch();

					_file << "\n" << _delta << " ";

					//jkdeviation = 0;
					for (double parafin = 0; parafin <= 8; parafin += pIncrease)
					{
						double solve = network->solve(network->barit[barit], parafin / 8) * 900;
						_file << solve << " ";
						//jkdeviation += abs((network->baritPressure[barit] * network->factor[(int)parafin]) - solve);
					}
					//jkdeviations += to_string(jkdeviation) + ' ';

					/*_file << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
					_file << jkdeviation;
					_file << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

					if (deviation > jkdeviation)
					{
						deviation = jkdeviation;
						idealParam = _delta;
					}*/

					std::ofstream os("sigma.txt");
					if (!os.is_open())
						return false;
					for (int i = 0; i < learnIterations; i++)
					{
						os << to_string(network->_sigmaErrors[i]) + "\n";
					}
					os.close();
				}

				/*_file << "\n";
				_file << "\n";
				_file << "deviation: " << deviation;
				_file << "\n";
				_file << "idealParam: " << idealParam;
				_file << "\n";
				_file << "jkdeviations: " << jkdeviations;*/
			}
			_file.close();
			delete network;
		}
	}

	cout << "done!";

    return 0;
}

