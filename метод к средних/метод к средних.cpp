// метод к средних.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
using namespace std;
#include <iostream>
const int KK = 10;  //количество кластеров
const int max_iterations = 100;  //максимальное количество итераций

typedef struct {
	double r;
	double g;
	double b;
} rgb;
class K_means
{
private:
	
	vector<rgb> pixcel;
	int q_klaster;
	int k_pixcel;
	vector<rgb> centr;
	void identify_centers();
	inline double compute(rgb k1, rgb k2)
	{
		return sqrt(pow((k1.r - k2.r), 2) + pow((k1.g - k2.g), 2) + pow((k1.b - k2.b), 2));
	}
	inline double compute_s(double a, double b) {
		return (a + b) / 2;
	};
public:
	K_means() : q_klaster(0), k_pixcel(0) {};
	K_means(int n, rgb* mas, int n_klaster);
	K_means(int n_klaster, std::istream& os);
	void clustering(std::ostream& os);
	void print()const;
	~K_means();
	friend std::ostream& operator<<(std::ostream& os, const K_means& k);
};
void K_means::identify_centers()
{
	srand((unsigned)time(NULL));
	rgb temp;
	rgb* mas = new rgb[q_klaster];
	for (int i = 0; i < q_klaster; i++) {
		temp = pixcel[0 + rand() % k_pixcel];
		for (int j = i; j < q_klaster; j++) {
			if (temp.r != mas[j].r && temp.g != mas[j].g && temp.b != mas[j].b) {
				mas[j] = temp;
			}
			else {
				i--;
				break;
			}
		}
	}
	for (int i = 0; i < q_klaster; i++) {
		centr.push_back(mas[i]);
	}
	delete[]mas;
}
K_means::K_means(int n, rgb* mas, int n_klaster)
{
	for (int i = 0; i < n; i++) {
		pixcel.push_back(*(mas + i));
	}
	q_klaster = n_klaster;
	k_pixcel = n;
	identify_centers();
}
K_means::K_means(int n_klaster, std::istream& os) : q_klaster(n_klaster)
{
	rgb temp;
	while (os >> temp.r && os >> temp.g && os >> temp.b) {
		pixcel.push_back(temp);
	}
	k_pixcel = pixcel.size();
	identify_centers();
}
void K_means::clustering(std::ostream& os)
{
	os << "\n\nНачало кластеризации:" << std::endl;
	/*В эти векторы мы будем помещать информацию о принадлежности каждого пикселя к определенному кластеру: в одном векторе будет информация о предыдущей итерации, а в другом - о текущей, и если они равны, то цикл заканчивается, так как каждый пиксель остался на своем месте.*/
	std::vector<int> check_1(k_pixcel, -1);
	std::vector<int> check_2(k_pixcel, -2);
	int iter = 0;
	/*Количество итераций.*/
	while (true)
	{
		os << "\n\n---------------- Итерация №"
			<< iter << " ----------------\n\n";
		{
			for (int j = 0; j < k_pixcel; j++) {
				double* mas = new double[q_klaster];
				/*Первая часть алгоритма: берем пиксель и находим расстояние от него до каждого центра. Данные помещаем в динамический массив размерностью, равной количеству кластеров.*/
				for (int i = 0; i < q_klaster; i++) {
					*(mas + i) = compute(pixcel[j], centr[i]);
					os << "Расстояние от пикселя " << j << " к центру #"
						<< i << ": " << *(mas + i) << std::endl;
				}
				/*Определяем минимальное расстояние и в m_k фиксируем номер центра для дальнейшего пересчета.*/
				double min_dist = *mas;
				int m_k = 0;
				for (int i = 0; i < q_klaster; i++) {
					if (min_dist > *(mas + i)) {
						min_dist = *(mas + i);
						m_k = i;
					}
				}

				os << "Минимальное расстояние к центру #" << m_k << std::endl;
				os << "Пересчитываем центр #" << m_k << ": ";
				centr[m_k].r = compute_s(pixcel[j].r, centr[m_k].r);
				centr[m_k].g = compute_s(pixcel[j].g, centr[m_k].g);
				centr[m_k].b = compute_s(pixcel[j].b, centr[m_k].b);
				os << centr[m_k].r << " " << centr[m_k].g
					<< " " << centr[m_k].b << std::endl;
				delete[] mas;
			}
			/*Классифицируем пиксели по кластерам.*/
			int* mass = new int[k_pixcel];
			os << "\nПроведем классификацию пикселей: " << std::endl;
			for (int k = 0; k < k_pixcel; k++) {
				double* mas = new double[q_klaster];
				/*Находим расстояние до каждого центра.*/
				for (int i = 0; i < q_klaster; i++) {
					*(mas + i) = compute(pixcel[k], centr[i]);
					os << "Расстояние от пикселя №" << k << " к центру #"
						<< i << ": " << *(mas + i) << std::endl;
				}
				/*Определяем минимальное расстояние.*/
				double min_dist = *mas;
				int m_k = 0;
				for (int i = 0; i < q_klaster; i++) {
					if (min_dist > *(mas + i)) {
						min_dist = *(mas + i);
						m_k = i;
					}
				}
				mass[k] = m_k;
				os << "Пиксель №" << k << " ближе всего к центру #" << m_k << std::endl;
			}
			/*Выводим информацию о принадлежности пикселей к кластерам и заполняем вектор для сравнения итераций.*/
			os << "\nМассив соответствия пикселей и центров: \n";
			for (int i = 0; i < k_pixcel; i++) {
				os << mass[i] << " ";
				check_1[i] = *(mass + i);
			}
			os << std::endl << std::endl;

			os << "Результат кластеризации: " << std::endl;
			int itr = KK + 1;
			for (int i = 0; i < q_klaster; i++) {
				os << "Кластер #" << i << std::endl;
				for (int j = 0; j < k_pixcel; j++) {
					if (mass[j] == i) {
						os << pixcel[j].r << " " << pixcel[j].g
							<< " " << pixcel[j].b << std::endl;
						mass[j] = ++itr;
					}
				}
			}

			delete[] mass;
			/*Выводим информацию о новых центрах.*/
			os << "Новые центры: \n";
			for (int i = 0; i < q_klaster; i++) {
				os << centr[i].r << " " << centr[i].g
					<< " " << centr[i].b << " - #" << i << std::endl;
			}
		}
		/*Если наши векторы равны или количество итераций больше допустимого – прекращаем процесс.*/
		iter++;
		if (check_1 == check_2 || iter >= max_iterations) {
			break;
		}
		check_2 = check_1;
	}
	os << "\n\nКонец кластеризации." << std::endl;
}
std::ostream& operator<<(std::ostream& os, const K_means& k)
{
	os << "Начальные пиксели: " << std::endl;
	for (int i = 0; i < k.k_pixcel; i++) {
		os << k.pixcel[i].r << " " << k.pixcel[i].g
			<< " " << k.pixcel[i].b << " - №" << i << std::endl;
	}
	os << std::endl << "Случайные начальные центры кластеризации: " << std::endl;
	for (int i = 0; i < k.q_klaster; i++) {
		os << k.centr[i].r << " " << k.centr[i].g << " "
			<< k.centr[i].b << " - #" << i << std::endl;
	}
	os << "\nКоличество кластеров: " << k.q_klaster << std::endl;
	os << "Количество пикселей: " << k.k_pixcel << std::endl;
	return os;
}

int main()
{
    std::cout << "Hello World!\n";
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
