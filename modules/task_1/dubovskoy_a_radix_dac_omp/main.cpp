#include <iostream>
#include "stdlib.h"
#include <chrono>
using namespace std;

/* Äëÿ qsort */
int compare(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}
int* get_randomized_array(int size, int s)
{
	int* array = new int[size];
	int range = size;
	for (int i = 0; i < size; i++)
	{
		array[i] = rand() % s + 1;
	}
	return array;
}
void print_array(int* array, int length)
{
	cout << " Array: ";
	for (int i = 0; i < length; i++)
	{
		cout << " " << array[i];
	}
	cout << endl;
}
int get_max(int* a, int n)
{
	int max = a[0];
	for (int i = 1; i < n; i++)
	{
		if (a[i] > max)
			max = a[i];
	}
	return max;
}
int* get_num(int* a, int size, int rang)
{
	int *count = new int[rang];
	for (int g = 0; g < rang; g++)
	{
		count[g] = 0;
	}
	for (int g = 0; g < rang; g++)
	{
		int r = 1;
		for (int k = 0; k < g; k++)
		{
			r *= 10;
		}
		for (int j = 0; j < size; j++)
		{
			if (a[j] / r < 10 && a[j] / r > 0)
			{
				count[g]++;
			}
		}
	}

	return count;
}
void get_arr(int** arr, int* a, int size, int rang)
{
	for (int g = 0; g < rang; g++)
	{
		int r = 1;
		for (int k = 0; k < g; k++)
		{
			r *= 10;
		}
		int N = 0;
		for (int j = 0; j < size; j++)
		{
			if (a[j] / r < 10 && a[j] / r > 0)
			{
				arr[g][N] = a[j];
				N++;
			}
		}
	}
}
void sort(int** arr, int* count, int rang)
{
	for (int i = 0; i < rang; i++)
	{
		qsort(arr[i], count[i], sizeof(int), compare);
	}
}
int main(int argc, char *argv[])
{

	int size = atoi(argv[1]);
	int s = atoi(argv[2]);
	int *a = new int[size];

	a = get_randomized_array(size, s);
	if (size < 20)
		print_array(a, size);

	int max = get_max(a, size);

	if (max == 0)
	{
		cout << "Array = {0}";
		return -1;
	}


	int rang = 0;
	while (max > 0)
	{
		max /= 10;
		rang++;
	}
	cout << "Max rang " << rang << endl;

	int* count = new int[rang];
	count = get_num(a, size, rang);
	cout << "Rangs ";
	print_array(count, rang);


	int** arr;
	arr = new int*[rang];
	for (int i = 0; i < rang; i++)
		arr[i] = new int[count[i]];
	get_arr(arr, a, size, rang);

	if (size < 20)
	{
		cout << "ARR:  ";
		for (int i = 0; i < rang; i++)
			for (int j = 0; j < count[i]; j++)
			{
				cout << arr[i][j] << " ";
			}
		cout << endl;
	}

	sort(arr, count, rang);

	if (size < 20)
	{
		cout << "SORTED ARR:  ";
		for (int i = 0; i < rang; i++)
			for (int j = 0; j < count[i]; j++)
			{
				cout << arr[i][j] << " ";
			}
		cout << endl;
	}


	int b = 0;
	for (int i = 0; i < rang; i++)
	{
		for (int j = 0; j < count[i]; j++)
		{

			a[b + j] = arr[i][j];
		}
		b += count[i];
	}

	for (int i = 1; i < size; i++)
	{
		if (a[i] < a[i - 1])
		{
			cout << "Array not sorted";
			return -1;
		}
	}
	cout << "Array sorted" << endl;	
}