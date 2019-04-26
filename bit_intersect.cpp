#include "stdio.h"
#include <iostream>
#include "string.h"
#include <vector>
#include "math.h"
#include <algorithm>

typedef unsigned int uint;
typedef unsigned long long uint64;

using namespace std;

uint cp_count = 0;
/* Ввод элементов в битмаску
 * Дополнительно возвращает минимальный и максимальный элемент считываемого множества
 */
void fill_array(vector<uint64> &dest, const uint proposed_size, const string &msg, uint &min_item, uint &max_item, const bool &memsave)
{
	dest = vector<uint64>(proposed_size / 64 * !memsave);
	min_item = UINT32_MAX;
	max_item = 0;

	cout << msg;
	for (int i = 0; i < proposed_size; i++)
	{
		uint elem;
		cin >> elem;
		min_item = min(min_item, elem);
		max_item = max(max_item, elem);
		const uint elem_index = elem / 64;
		if (elem_index >= dest.size()) // Если битмаска мала то расширяем
		{
			for (uint push = dest.size(); push < elem_index; push++) { dest.push_back(0ULL); }
			dest.push_back(1ULL << (elem % 64));
		}
		else {
			dest.at(elem_index) |= 1ULL << (elem % 64);
		}
	}
}

/*
 * Умножаем маску src на маску other, результат пишется в src
 * Следовательно, обязательное условие: src.size() <= other.size()
 */
void intersect(vector<uint64> &src, vector<uint64> &other, uint &min_index, uint &max_index)
{
	for (uint iter = min_index / 64; iter <= max_index / 64; iter++) {
		cp_count ++;
		src.at(iter) &= other.at(iter);
	}
}

vector<uint> to_uint_vector(const vector<uint64> &bits, const uint &start, const uint &end)
{
	vector<uint> dest;
	for (uint dword = start / 64; dword <= end / 64; dword++) {
		for (uint bit = 0; bit < 64; bit++) {
			if ((bits.at(dword) >> bit) & 1ULL)
				dest.push_back(dword * 64 + bit);
		}
	}
	return dest;
}

int main(int argc, char* argv[])
{
	// Logo (самое важное)
	cout << ".___________________________.\n";
	cout << "|                           |\n";
	cout << "|Bitmask Intersection Method|\n";
	cout << "|___________________________|\n";
	cout << endl;
	// Обработка аргументов
	bool silent = (argc > 1 && strcmp(argv[1], "--silent") == 0);
	bool memsave = (argc > 2 && strcmp(argv[2], "--memsave") == 0);
	if (silent) cout << "Entered Silent Mode... Result will not be printed to save execution time\n";
	// Ввод длин числовых массивов
	int m, n;
	cout << "First set length (n): "; cin >> m;
	cout << "Second set length (m): "; cin >> n;
	// Объявление и заполнение массивов + минимума и максимума пересечения
	vector<uint64> arr1;
	vector<uint64> arr2;
	uint first_inter_min, second_inter_min;
	uint first_inter_max, second_inter_max;
	fill_array(arr1, m, "Enter first set: ", first_inter_min, first_inter_max, memsave);
	fill_array(arr2, n, "Enter second set: ", second_inter_min, second_inter_max, memsave);
	uint intersection_min = max(first_inter_min, second_inter_min);
	uint intersection_max = min(first_inter_max, second_inter_max);
	// Пересечение
	cout << "Intersection of these arrays:\n";
	// Проверяем исключительные случаи, сохраняем время
	if (intersection_min > intersection_max) {
		cout << "No elements in intersection\n";
		return 0;
	}
	if (intersection_min == intersection_max) {
		if (silent) cout << "Silent mode but still... intersection is:" << endl;
		cout << intersection_min << endl;
		return 0;
	}
	vector<uint> result;
	if (arr1.size() <= arr2.size()) {
		intersect(arr1, arr2, intersection_min, intersection_max);
		result = to_uint_vector(arr1, intersection_min, intersection_max);
	}
	else {
		intersect(arr2, arr1, intersection_min, intersection_max);
		result = to_uint_vector(arr2, intersection_min, intersection_max);
	}

	if (!result.empty() && !silent) {
		for (auto element : result)
			cout << element << " ";
	}
	else if (silent)
		cout << "Silent mode - Size of intersection: " << result.size();
	else
		cout << "No elements in intersection";
	cout << endl;

	// Диагностика
	for (short i = 0; i <= 32; i++) cout << '*';
	cout << endl;
	const uint consumption1 = sizeof(uint64) * arr1.size();
	const uint consumption2 = sizeof(uint64) * arr2.size();
	cout << "Memory consumed:\n"
	<< "arr1: " << consumption1 << " Bytes (" << consumption1 / pow(10, 6) << " MBytes), " << arr1.size() << " items\n"
	<< "arr2: " << consumption2 << " Bytes (" << consumption2 / pow(10, 6) << " MBytes), " << arr2.size() << " items\n"
	<< "Total:" << consumption1 + consumption2 << " Bytes (" << (consumption1 + consumption2) / pow(10, 6) << " MBytes)\n";
	for (short i = 0; i <= 32; i++) cout << '_'; cout << endl;
	cout << "Control Point Passes: " << cp_count << endl;
	cout << "Desired Control Point Passes: " << ((intersection_max + 1) / 64) + 1 - (intersection_min / 64) << endl;
	return 0;
}
