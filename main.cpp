#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <execution>
#include <future>
#include <algorithm>


void print_vect(const std::vector<int>& vect);
void selection_sort(std::vector<int> vect, std::promise<std::vector<int>> prom);

int main() {
	setlocale(LC_ALL, "ru");
	std::cout << "\tСортировка выбором\n\n" << std::endl;

	const int size = 20;
	std::vector<int> vect(size);
	std::mt19937 gen;
	std::uniform_int_distribution<int> dist(0, size);
	std::generate(std::execution::par, vect.begin(), vect.end(), [&gen, &dist]() {return dist(gen); });

	std::cout << "Вектор до сортировки: ";
	print_vect(vect);
	std::cout << std::endl << std::endl;

	std::promise<std::vector<int>> prom;
	std::future ft = prom.get_future();
	std::thread th(selection_sort, vect, std::move(prom));
	vect = std::move(ft.get());
	th.join();

	std::cout << "Вектор после сортировки: ";
	print_vect(vect);
	std::cout << std::endl;

	system("pause > nul");
	return 0;
}

void print_vect(const std::vector<int>& vect) {
	for (auto it = vect.cbegin(); it != vect.cend(); ++it) {
		std::cout << *it << " ";
	}
}

void selection_sort(std::vector<int> vect, std::promise<std::vector<int>> prom) {
	for (size_t i = 0; i < vect.size(); ++i) {
		auto it = vect.begin();
		std::advance(it, i);

		auto ft_min = std::async(std::launch::async, [&vect, it]() {return std::min_element(it, vect.end()); });
		auto it_min = ft_min.get();

		int temp = *it;
		*it = *it_min;
		*it_min = temp;
	}
	prom.set_value(vect);
}
