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



	// Инициализация весов
public:
	double* input; // входные параметры
	int inputLength; // длинна входных параметров
	double delta = 1; // скорость обучения
	int hiddenLayerLength; // количество нейронов скрытого слоя
	double* _sigmaErrors; // ошибки на каждой итерации
	//int flag = 100000;

	void initWeights() {
		for (int i = 0; i < hiddenLayerLength; i++)
		{
			for (int j = 0; j < inputLength; j++) {
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
		for (int i = 0; i < hiddenLayerLength; i++)
		{
			for (int j = 0; j < inputLength; j++)
			{
				cout << "W[" << i << "][" << j << "]: " << W[i][j] << " ";
			}
			cout << endl;
		}
	}

	void printZ() {
		cout << "------------------------------------" << endl;
		cout << "Z: " << endl;
		printZz();
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	}

	// Веса скрытого слоя 
	void printZz() {
		for (int i = 0; i < hiddenLayerLength; i++)
		{
				cout << "Z[" << i << "]: " << Z[i] << " ";
			cout << endl;
		}
	}

	// Решение (прямой проход)
	double solve() {
		double res = 0;
		double z = 0;
		double _z = 0;
		for (int i = 0; i < hiddenLayerLength; i++)
		{
			_z = 0;
			for (int j = 0; j < inputLength; j++)
			{
				_z += input[j] * W[i][j];
			}
			Z[i] = fActivation(_z);
			res += Z[i] * resultS[i];
		}
		return fActivation(res);
	}

	void learn(double result, int iteration) {
		// Веса синапсов нейрона выходного слоя
		y = solve();

		/* Обратное распространение ошибки */
		// Обучение выходного слоя
		sigma = y * (1 - y) * (result - y);

		_sigmaErrors[iteration] = sigma;


		/*if (input[1] == 1 && input[0] == 0 && iteration % flag && iteration > flag) {
			double d = y;
			flag += 100000;
		}*/

		for (int i = 0; i < hiddenLayerLength; i++)
		{
			resultS[i] = resultS[i] + delta * sigma * Z[i];
			_sigma[i] = Z[i] * (1 - Z[i]) * (sigma * resultS[i]);
			
			// нейроны скрытого слоя
			for (int j = 0; j < inputLength; j++)
			{
				W[i][j] = W[i][j] + delta * _sigma[i] * input[j];
			}
		}
	}

	NeuralNetwork(int _inputLength, int layerCount) {
		input = new double[_inputLength];
		inputLength = _inputLength;

		// currently only one hidden layer
		hiddenLayerLength = layerCount;

		W = new double*[hiddenLayerLength];
		for (int i = 0; i < hiddenLayerLength; i++) {
			W[i] = new double[_inputLength];
		}

		resultS = new double[hiddenLayerLength];
		Z = new double[hiddenLayerLength];
		_sigma = new double[hiddenLayerLength];
		_sigmaErrors = new double[learnIterations];
	}

	~NeuralNetwork() {
		//delete input;
		delete W;

		delete resultS;
		delete Z;
		delete _sigma;
		delete _sigmaErrors;
	}
};

int main()
{
	system("color 0a");
	cout << "Processing...\n";

	//// коэффициенты объемов барита
	//double barit[5] = { 0.1, 0.2, 0.3, 0.4, 0.5 };
	//// соответствующее объему барита давление 
	//int baritPressure[5] = { 15, 21, 42, 450, 818 };
	//// процент содержания парафина
	//double parafinPercentage[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	//// коэффициенты изменения барита
	//double factor[9] = { 1, 0.208, 0.177, 0.173, 0.168, 0.164, 0.16, 0.155, 0.151 };


	// коэффициенты объемов барита
	double barit[5] = { 0.1, 0.2, 0.3, 0.4, 0.5 };
	// соответствующее объему барита давление 
	int baritPressure[5] = { 15, 21, 42, 450, 818 };
	// процент содержания парафина
	double parafinPercentage[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	// коэффициенты изменения барита
	double factor[9] = { 1, 0.208, 0.177, 0.173, 0.168, 0.164, 0.16, 0.155, 0.151 };

	//double inputs[4][2] = { { 0, 0 },{ 0, 1 },{ 1, 0 },{ 1, 1 } };
	//double expected_output[4] = { 0, 1, 1, 0 };

	// шаг по оси Ох
	double pIncrease = 1;

	// входной параметр
	/*for (int _barit = 4; _barit <= 4; _barit++)
	{*/
	// количество внутренних слоев
	for (int layer = 5; layer <= 5; layer += 1)
	{
		// отклонение вычислений
		/*double idealParam = 1;
		double jkdeviation = 0;
		string jkdeviations = "";
		double deviation = DBL_MAX;
		*/


		ofstream _file;
		_file.open("layers/layer" + (to_string(layer)) + ".txt");

		if (_file.is_open())
		{
			for (double _parafin = 0; _parafin <= 8; _parafin += pIncrease)
			{
				_file << " " << _parafin << " ";
			}

			_file << "\nЭталон";

			for (double _parafin = 0; _parafin <= 8; _parafin += 1)
			{
				_file << " " << baritPressure[3] * factor[(int)_parafin];
			}

			/*_file << "\nЭталон";

			for (double _parafin = 0; _parafin < 4; _parafin += 1)
			{
				_file << " " << expected_output[(int)_parafin];
			}*/


			// цикл определения скорости обучения (константа)
			for (double _delta = 0; _delta <= 1; _delta += 0.01)
			{
				NeuralNetwork* network = new NeuralNetwork(2, layer);
				network->delta = _delta;
				network->initWeights();

				double* input = new double[2];

				// обучение нейросети
				for (int i = 0; i < learnIterations; i++)
				{
					// формирование входных параметров

					int _barit = 3;// rand() % 5;//3;
					int _parafin = rand() % 9;
					input[0] = barit[_barit];
					input[1] = (double)(parafinPercentage[_parafin] / 8);

					/*int tst = rand() % 4;*/

					network->input = input;


					network->learn(baritPressure[3] * factor[_parafin] / 900, i);
					//network->learn(expected_output[tst], i);
				}
				//network->printWeights();
				//network->printZ();
				
				_file << "\n" << _delta << " ";


				//jkdeviation = 0;
				for (int parafin = 0; parafin <= 8; parafin += pIncrease)
				{
					//network->input = inputs[parafin];
					//_file << "\nЭталон: " << network->input[0] << " " << network->input[1] << " ";

					network->input[0] = barit[3];
					network->input[1] = (double)(parafinPercentage[parafin] / 8);
					//double solve = network->solve();
					double solve = network->solve() * 900;
					_file << solve << " ";
					/*if (abs(solve - expected_output[parafin]) <= 0.2) {
						_file << "Topchik ";
					}
					else {
						_file << "Nope";
					}*/
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

				//std::ofstream os("sigma.txt");
				//if (!os.is_open())
				//	return false;
				//for (int i = 0; i < learnIterations; i++)
				//{
				//	os << to_string(network->_sigmaErrors[i]) + "\n";
				//}
				//os.close();
				delete network;
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
	}
	//}

	cout << "done!";

	return 0;
}

