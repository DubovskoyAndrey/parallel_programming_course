#include <iostream>

using namespace std;

void radixsort(int *list, int n)
{
	int *bucket = new int[n];
	int digpos = 1;
	int max = list[0];

	for (int i = 1; i < n; i++) {
		if (list[i] > max)
			max = list[i];
	}

	while (max / digpos > 0) {

		int digit[10] = { 0 };

		for (int i = 0; i < n; i++)
			digit[(list[i] / digpos) % 10]++;

		for (int i = 1; i < 10; i++)
			digit[i] += digit[i - 1];

		for (int i = n - 1; i >= 0; i--)
			bucket[--digit[(list[i] / digpos) % 10]] = list[i];

		for (int i = 0; i < n; i++)
			list[i] = bucket[i];
		digpos *= 10;
	}
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

int* get_randomized_array(int size)
{
	int* array = new int[size];
	int range = size;
	for (int i = 0; i < size; i++)
	{
		array[i] = rand() % range + 1;
	}
	return array;
}
int* get_part_of_array(int* arr, int start_pos, int end_pos)
{
	int* part_arr = new int[end_pos - start_pos];
	int j = 0;
	for (int i = start_pos; i < end_pos; i++)
	{
		part_arr[j] = arr[i];
		j++;
	}
	return part_arr;
}
void sort(int* arr, int size, int counts)
{
	bool trg;
	if ((size%counts) == 0)
		trg = false;
	else
		trg = true;
	int part = size / counts;
	int** a = new int*[counts];
	if (trg)
	{
		for (int i = 0; i < counts - 1; i++)
		{
			a[i] = new int[part];
			a[i] = get_part_of_array(arr, part*i, part*(i + 1));
			radixsort(a[i], part);
			for (int j = i * part; j < (i + 1)*part; j++)
			{
				arr[j] = a[i][j - i * part];
			}
		}
		a[counts - 1] = new int(part + size % counts);
		a[counts - 1] = get_part_of_array(arr, part*(counts - 1), size);
		radixsort(a[counts - 1], part + size % counts);
		for (int j = (counts - 1) * part; j < size; j++)
		{
			arr[j] = a[counts - 1][j - (counts - 1) * part];
		}
	}
	else
	{
		for (int i = 0; i < counts; i++)
		{
			a[i] = new int[part];
			a[i] = get_part_of_array(arr, part*i, part*(i + 1));
			radixsort(a[i], part);
			for (int j = i * part; j < (i + 1)*part; j++)
			{
				arr[j] = a[i][j - i * part];
			}
		}
	}
}
int main(int argc, char *argv[])
{
	int size = atoi(argv[1]);
	int counts = atoi(argv[2]);

	int* arr = new int[size];
	arr = get_randomized_array(size);
	if (size < 20)
		print_array(arr, size);

	sort(arr, size, counts);

	if (size < 20)
		print_array(arr, size);

	radixsort(arr, size);

	if (size < 20)
		print_array(arr, size);
	for (int i = 1; i < size; i++)
	{
		if (arr[i] < arr[i - 1])
		{
			cout << "Array not sorted";
			return -1;
		}
	}
	cout << "Array sorted" << endl;

}