// PerceptronFirstTry.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <windows.h>
#include <string>
#include <vector>
#include <time.h>
#include <algorithm>

using namespace std;

// параметры обучения
#define learnIterations_base 500000
#define learnIterations_max  10000000
#define learnIterations_step 500000

#define learn_rate_base 0.5 
#define learn_rate_max 0.5
#define learn_rate_step 0.01 

// нейроны скрытого слоя
#define hidden 4
#define hidden_max 4

int input_length; // количество входных нейронов
vector<double> preset_learn_input; // вектор предустановленных данных

double input_normalization; // нормализация входных данных
double result_normalization; // нормализация результата

//// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// вычисление функции xor

vector<vector<double>> expertData = {
	{ 0, 0 },
	{ 0, 1 },
	{ 1, 0 },
	{ 1, 1 }
};

vector<double> results = { 0, 1, 1, 0 };

vector<vector<double>> inputData = {
	{ 0, 0 },
	{ 0, 1 },
	{ 1, 0 },
	{ 1, 1 }
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct input {
	vector<double> inputs;
	double result;
	vector<double> base;
	double baseResult;
};

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
	vector<double> input; // входные параметры
	int inputLength; // длинна входных параметров
	int learnIterations; // количество итераций

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

	NeuralNetwork(int _inputLength, int layerCount, int iterations) {
		input.clear();
		inputLength = _inputLength;
		learnIterations = iterations;
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
		delete W;
		delete resultS;
		delete Z;
		delete _sigma;
		delete _sigmaErrors;
	}
};


// корректный результат эксперта
double getResult(vector<double> inputs) {
	auto it = find(expertData.begin(), expertData.end(), inputs);
	int index = it - expertData.begin();
	return it != expertData.end() ? results[index] : -1;
}

// Эталонный результат
string printBaseline() {
	string str = "Эталон: ";
	
	for (int j = 0; j < expertData.size(); j += 1)
	{
		str += " " + to_string(getResult(expertData[j])).substr(0, 9);
	}

	return str;
}

// Подготовка массива входных данных и результата
input *prepareInput(vector<double> _x) {
	vector<double> inputs;

	// если входные значения не заданы, то подбирается случайный элемент экспертных данных для обучения
	if (!_x.size()) {
		int index = rand() % expertData.size();
		for (int i = 0; i < expertData[index].size(); i++) {
			_x.push_back(expertData[index][i]);
		}
	}

	for (int i = 0; i < _x.size(); i++) {
		inputs.push_back(_x[i] / input_normalization); // нормализация данных
	}

	double baseResult = getResult(_x); // корректный результат
	
	return new input { inputs, baseResult / result_normalization, _x, baseResult };
}

int main()
{
	system("color 0a");
	cout << "Processing...\n";

	srand(time(NULL));

	// подготовка нормализации и определение размера входных данных нейросети
	double max_input = 1;
	for (int i = 0; i < expertData.size(); i++)
	{
		for (int j = 0; j < expertData[i].size(); j++)
		{
			max_input = max_input < expertData[i][j] ? expertData[i][j] : max_input;
		}
		if (!input_length) {
			input_length = expertData[i].size();
		}
	}
	input_normalization = max_input;

	double max_result = 1;
	for (int i = 0; i < results.size(); i++)
	{
		max_result = max_result < results[i] ? results[i] : max_result;
	}
	result_normalization = max_result;

	for (int it = learnIterations_base; it <= learnIterations_max; it += learnIterations_step)
	{
		time_t start = time(nullptr);
		cout << "it - " << it << "; time start - "<< start;
		string res = "";
		// количество внутренних слоев
		for (int hidden_layer_length = hidden; hidden_layer_length <= hidden_max; hidden_layer_length += 1)
		{
			// отклонение вычислений
			/*double idealParam = 1;
			double jkdeviation = 0;
			string jkdeviations = "";
			double deviation = DBL_MAX;
			*/

			ofstream _file;
			_file.open("layers/layer" + (to_string(hidden)) + ".txt");

			if (_file.is_open())
			{
				// эталон
				_file << printBaseline();

				// цикл определения скорости обучения (константа)
				for (double learn_rate = learn_rate_base; learn_rate <= learn_rate_max; learn_rate += learn_rate_step)
				{
					NeuralNetwork* network = new NeuralNetwork(input_length, hidden_layer_length, it);
					network->delta = learn_rate;
					network->initWeights();

					// обучение нейросети
					for (int i = 0; i < it; i++)
					{
						// формирование массива входных данных
						input* data = prepareInput(preset_learn_input);
						network->input = data->inputs;
						network->learn(data->result, i);
						delete data;
					}

					_file << "\n" << learn_rate << " --> ";
					//jkdeviation = 0;
					for (int i = 0; i < inputData.size(); i += 1)
					{
						input* _input = prepareInput(inputData[i]); // предустановленные входные данные
						network->input = _input->inputs;
						double result = network->solve();
						_file << result * result_normalization << " ";
						res += to_string(result * result_normalization) + ' ';
						//jkdeviation += abs(_input->result- solve);
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
		time_t end = time(nullptr);
		cout << "; time end - " << end << "; difference - " << end - start << " (sec)" << endl;
		cout << "result - " << res << endl;
	}

	cout << endl << "done!" << endl;
	system("pause");
	return 0;
}

