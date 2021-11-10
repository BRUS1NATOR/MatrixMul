#include <iostream>
#include <string>
#include <thread>
#include <vector> 
#include <iomanip>

using namespace std;

void calculate_row(int start, int step, int max, int** aMatrix, int** bMatrix, int** product) {
	for (int row = start; row < max; row += step) {
		for (int col = 0; col < max; col++) {

			product[row][col] = 0;

			for (int inner = 0; inner < max; inner++) {
				product[row][col] += aMatrix[row][inner] * bMatrix[inner][col];
			}
		}
	}
}

double calculate_rnd_matrix(int N, int threads_count = 1) {

	int** aMatrix = new int* [N];
	int** bMatrix = new int* [N];
	int** product = new int* [N];

	for (int i = 0; i < N; ++i) {
		aMatrix[i] = new int[N];
		bMatrix[i] = new int[N];
		product[i] = new int[N];
	}

	srand(time(NULL));

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
		{
			aMatrix[i][j] = rand() % 100 + 1;
			bMatrix[i][j] = rand() % 100 + 1;
		}
	}

	vector<thread> threads;

	unsigned int start_time = clock();

	for (int i = 0; i < threads_count; i++) {
		threads.push_back(thread::thread(calculate_row, i, threads_count, N, aMatrix, bMatrix, product));
	}

	for (int i = 0; i < threads_count; i++) {
		threads[i].join();
	}

	//for (int row = 0; row < n; row++) {
	//	for (int col = 0; col < n; col++) {
	//		cout << product[row][col] << " ";
	//	}
	//	cout << endl;
	//}

	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time; // искомое время

	threads.clear();

	return search_time / (double)CLOCKS_PER_SEC;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	unsigned int n;
	cout << "Введите размерность матрицы : ";
	cin >> n;

	unsigned int threads_hardware = std::thread::hardware_concurrency();
	threads_hardware == 0 ? 4 : threads_hardware;
	cout << threads_hardware << " concurrent threads are supported.\n";

	cout << "Сколько потоков использовать? (макс - " << threads_hardware << ") \n";
	unsigned int threads_count;
	cin >> threads_count;

	if (threads_count == 0 || threads_count > threads_hardware) {
		threads_count = threads_hardware;
	}

	auto glambda = [](int N, double t, int threads){
		cout << "Размерность: " + to_string(N) + ", время: " + to_string(t) + ", число потоков : " + to_string(threads) << endl;
		return 0;
	};

	double time = calculate_rnd_matrix(n, threads_count);
	glambda(n, time, threads_count);

	cout << setw(12) << left << "| N";

	for (int i = 1; i <= threads_hardware; i++) {
		cout << setw(12) << left << "|" + to_string(i);
	}

	cout << endl << setw(12) << "| 500";

	for (int i = 1; i <= threads_hardware; i++) {
		double time = calculate_rnd_matrix(500, i);
		cout << setprecision(4) << setw(12) << left << "|" + to_string(time);
	}

	cout << endl << setw(12) << "| 1000";

	for (int i = 1; i <= threads_hardware; i++) {
		double time = calculate_rnd_matrix(1000, i);
		cout << setprecision(4) << setw(12) << left << "|" + to_string(time);
	}

	cout << endl << setw(12) << "| 2000";

	for (int i = 1; i <= threads_hardware; i++) {
		double time = calculate_rnd_matrix(2000, i);
		cout << setprecision(4) << setw(12) << left << "|" + to_string(time);
	}

	return 0;
}
