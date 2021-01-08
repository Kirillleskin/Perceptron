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

#define learnIterations 1000000


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

	NeuralNetwork(int _inputLength, int layerCount) {
		input.clear();
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
		delete W;
		delete resultS;
		delete Z;
		delete _sigma;
		delete _sigmaErrors;
	}
};

//// массивы данных по нейронам
//// должны быть отсортированы в порядке возростания
//vector<vector<double>> DATA = {
//	{ 0.1, 0.2, 0.3, 0.4, 0.5 }, // barit // коэффициенты объемов барита
//	{ 0, 1, 2, 3, 4, 5, 6, 7, 8 } // parafinPercentage // процент содержания парафина
//};
//vector<string> DATA_DESCRIPTION = {
//	"Барит",
//	"Парафин"
//};
//
//// соответствующее объему барита давление 
//double baritPressure[5] = { 15, 21, 42, 450, 818 };
//// коэффициенты изменения барита
//double factor[9] = { 1, 0.208, 0.177, 0.173, 0.168, 0.164, 0.16, 0.155, 0.151 };

//double results[10] = { 450, 93.6, 79.65, 77.85, 75.6, 73.8, 72, 69.75, 67.95 }; // barit
//double results[10] = { 167, 286.0437, 310.4643, 316.2778, 315.1151, 313.9524, 312.7897, 311.627, 310.4643 }; // fer

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//vector<vector<double>> DATA = {
//	{ 0, 1 }, // xor
//	{ 0, 1 } // xor
//};
//vector<string> DATA_DESCRIPTION = {
//	"XOR1",
//	"XOR2"
//};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//// массивы данных по нейронам
//// должны быть отсортированы в порядке возростания
//vector<vector<double>> DATA = {
//	{ 0.1, 0.2, 0.3, 0.4, 0.5 }, // barit // коэффициенты объемов барита
//	{ 0, 1, 2, 3, 4, 5, 6 } // состав
//};
//vector<string> DATA_DESCRIPTION = {
//	"Барит",
//	"Состав"
//};
//
//// соответствующее объему барита давление 
//double parafin[3] = { 0.2, 0.5, 0.8 };
//double saja[3] = { 0.7, 1, 1.3 };
//double kerosin[3] = { 0.2, 0.5, 0.8 };
//
//double results[7] = { 64.255, 78.510, 70.638, 91.702, 93.404, 63.191, 80 }; // pressure

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// массивы данных по нейронам
// должны быть отсортированы в порядке возростания
vector<vector<double>> DATA = {
	{ 0.2, 0.5, 0.8 }, // parafin
	{ 0.7, 1, 1.3 }, // saja
	{ 0.2, 0.5, 0.8 } // kerosin
};
vector<string> DATA_DESCRIPTION = {
	"Парафин",
	"Сажа",
	"Керосин",
};

// соответствующее объему барита давление 
double parafin[3] = { 0.2, 0.5, 0.8 };
double saja[3] = { 0.7, 1, 1.3 };
double kerosin[3] = { 0.2, 0.5, 0.8 };

double results[7] = { 64.255, 78.510, 70.638, 91.702, 93.404, 63.191, 80 }; // pressure

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//предустановленные входные данные
//-1 если нужен генератор
vector<double> preset_learn_input; // 0.4
int result_normalization = 100;
int resultLength = 7;
int _layers = 5;
vector<vector<double>> preparedData = {
	{ 0.2, 1, 0.5 },
	{ 0.5, 0.7, 0.5 },
	{ 0.8, 1, 0.5 },
	{ 0.5, 1.3, 0.5 },
	{ 0.5, 1, 0.2 },
	{ 0.5, 1, 0.8 },
	{ 0.5, 1, 0.5 },
};
//double parafin[3] = { 0.2, 0.5, 0.8 };
//double saja[3] = { 0.7, 1, 1.3 };
//double kerosin[3] = { 0.2, 0.5, 0.8 };
//vector<vector<int>> preparedDataIndexes = {
//	{ 3, 0 },
//	{ 3, 1 },
//	{ 3, 2 },
//	{ 3, 3 },
//	{ 3, 4 },
//	{ 3, 5 },
//	{ 3, 6 }
//};
//vector<vector<double>> preparedData = {
//	{ 0, 1, 1 },
//	{ 1, 0, 1 },
//	{ 2, 1, 1 },
//	{ 1, 2, 1 },
//	{ 1, 1, 0 },
//	{ 1, 1, 2 },
//	{ 1, 1, 1 },
//};


// эксперт
// x - входные значения
double getResult(vector<double> _x) {
	//return baritPressure[_x[0]] * factor[_x[1]]; // /900
	//return _x[0] ^ _x[1];
	//auto it = search(preparedDataIndexes.begin(), preparedDataIndexes.end(), _x.begin(), _x.end());
	auto it = find(preparedData.begin(), preparedData.end(), _x);
	int index = it - preparedData.begin();
	return it != preparedData.end() ? results[index] : -1;
}

string printBaseline() {
	string str = "";
	for (int i = 0; i <= DATA.size() - 1; i += 1)
	{
		str += DATA_DESCRIPTION[i] + ": ";
		for (int j = 0; j <= DATA[i].size()-1; j += 1)
		{
			str += to_string(DATA[i][j]) + " ";
		}
		str += "\n";
	}

	/*str += "Эталон: ";
	
	for (int j = 0; j < resultLength; j += 1)
	{
		str += " " + to_string(getResult(preparedData[j])).substr(0, 7);
	}*/

	return str;
}

// подготовка массива индексов входных данных и результата
input *prepareInput(vector<double> _x) {
	vector<double> inputs;
	vector<double> base;
	// входные данные
	for (int i = 0; i <= DATA.size() - 1; i++)
	{
		int valIndex;
		int size = _x.size() - 1;
		int dataSize = DATA[i].size() - 1;
		if (size >= i && _x[i] != -1) {
			auto it = find(DATA[i].begin(), DATA[i].end(), _x[i]);
			valIndex = it - DATA[i].begin();
			//valIndex = _x[i];
		}
		else {
			int c = DATA[i].size();
			valIndex = rand() % c;
		}

		double lastValue = DATA[i][dataSize];
		inputs.push_back(DATA[i][valIndex] / (lastValue > 1 ? lastValue : 1)); // нормализация
		base.push_back(DATA[i][valIndex]);
		if (size < i) {
			_x.push_back(DATA[i][valIndex]);
		}
	}

	double baseResult = getResult(_x);
	
	return new input { inputs , baseResult / result_normalization, base, baseResult };
}

int main()
{
	system("color 0a");
	cout << "Processing...\n";

	srand(time(NULL));

	// шаг по оси Ох
	double pIncrease = 1;

	// количество внутренних слоев
	for (int layer = _layers; layer <= _layers; layer += 1)
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
			_file << printBaseline();
		
			// цикл определения скорости обучения (константа)
			for (double _delta = 0.5; _delta <= 0.5; _delta += 0.3)
			{
				NeuralNetwork* network = new NeuralNetwork(DATA.size(), layer);
				network->delta = _delta;
				network->initWeights();

				// обучение нейросети
				for (int i = 0; i < learnIterations; i++)
				{
					// формирование массива входных нейронов
					input* data = prepareInput(preset_learn_input);
					if (data->baseResult == -1) {
						if (i > 0) i--;
						delete data;
						continue;
					}
					network->input = data->inputs;
					network->learn(data->result, i);
					delete data;
				}

				_file << "\n" << _delta << " --> ";

				//jkdeviation = 0;
				// barit
				// TODO recursion
				for (int i = 0; i < resultLength; i += pIncrease)
				{
					input* _input = prepareInput(preparedData[i]); // предустановленные входные данные
					network->input = _input->inputs;
					double solve = network->solve() * result_normalization;
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

	cout << "done!";

	return 0;
}

