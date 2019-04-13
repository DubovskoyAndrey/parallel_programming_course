// Copyright 2019 Dubovskoy Andrey

#include <omp.h>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <random>

unsigned int* get_randomized_array(int size) {
  unsigned int* array = new unsigned int[size];
  std::default_random_engine generator;
  std::uniform_int_distribution<unsigned int> distribution(0, 10000);
  for (int i = 0; i < size; i++) {
    array[i] = distribution(generator);
  }
  return array;
}

void print_array(unsigned int* array, int size) {
  if (size > 25) {
    return;
  }
  std::cout << " Array: ";
  for (int i = 0; i < size; i++) {
    std::cout << " " << array[i];
  }
  std::cout << std::endl;
}
void copy_array(unsigned int* array1, unsigned int* array2, int size) {
  for (int i = 0; i < size; i++) {
    array2[i] = array1[i];
  }
}

void countSort(unsigned int* arr, int n, int exp) {
  unsigned int *output = new unsigned int[n];
  int i;
  unsigned int count[10] = { 0 };

  for (i = 0; i < n; i++)
    count[(arr[i] / exp) % 10]++;
  for (i = 1; i < 10; i++)
    count[i] += count[i - 1];
  for (i = n - 1; i >= 0; i--) {
    output[count[(arr[i] / exp) % 10] - 1] = arr[i];
    count[(arr[i] / exp) % 10]--;
  }

  for (i = 0; i < n; i++)
    arr[i] = output[i];
}

void radix_sort(unsigned int *arr, int n) {
  unsigned int max = arr[0];
  for (int i = 1; i < n; i++)
    if (arr[i] > max)
      max = arr[i];
  for (int exp = 1; max / exp > 0; exp *= 10)
    countSort(arr, n, exp);
}

void check_result(unsigned int * array, int size) {
  for (int i = 1; i < size; i++)
    if (array[i] < array[i - 1]) {
      std::cout << "Array not sorted" << std::endl;
      return;
    }
  std::cout << "Array sorted" << std::endl;
}

unsigned int * Splitter(unsigned int * arr1, unsigned int* arr2, int size1, int size2) {
  unsigned int * tmp = new unsigned int[size1 + size2];
  int a = 0, b = 0, i = 0;
  while (a < size1 && b < size2) {
    if (arr1[a] < arr2[b]) {
      tmp[i++] = arr1[a++];
    } else {
      tmp[i++] = arr2[b++];
    }
  }
  while (a < size1) {
    tmp[i++] = arr1[a++];
  }
  while (b < size2) {
    tmp[i++] = arr2[b++];
  }
  return tmp;
}
int BinSearch(unsigned int *mas, int l, int r, unsigned int x) {
  if (l == r) return l;
  if (l + 1 == r) {
    if (x < mas[l]) return l;
    else
      return r;
  }

  int m = (l + r) / 2;
  if (x < mas[m]) {
    r = m;
  } else {
    if (x > mas[m]) l = m;
    else
      return m;
  }

  return BinSearch(mas, l, r, x);
}

void dac_sort(unsigned int * array, int size, int threads) {
  omp_set_num_threads(threads);
  int * piece_mas = new int[threads];
  int piece = size / threads;
  int remainder = threads % size;
  for (int i = 0; i < threads; i++) {
    piece_mas[i] = size / threads;
  }
  if (size / threads != 0) {
    piece_mas[threads - 1] = piece_mas[threads - 1] + remainder;
  }
#pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < threads; i++)
    radix_sort(array + i * piece, piece_mas[i]);
  print_array(array, size);


  int counter = static_cast<int>(std::log(threads) / std::log(2));
  std::cout << " Counter = " << counter << std::endl;


  for (int c = 0; c < counter; c++) {
    if (c != 0) {
      piece = piece * 2;
    }
    std::cout << " Piece = " << piece << std::endl;
    int* r1 = new int[size / piece / 2];
    int* l1 = new int[size / piece / 2];
    int* r2 = new int[size / piece / 2];
    int* l2 = new int[size / piece / 2];
    for (int j = 0; j < size / piece / 2; j++) {
      l1[j] = j * piece * 2;
      r1[j] = j * piece * 2 + piece - 1;
      l2[j] = j * piece * 2 + piece;
      r2[j] = BinSearch(array, l2[j], j * piece * 2 + piece * 2 - 1, array[r1[j]]);
    }
    r2[size / piece / 2 - 1] = BinSearch(array, l2[size / piece / 2 - 1], size - 1, array[r1[size / piece / 2 - 1]]);
    for (int j = 0; j < size / piece / 2; j++) {
      std::cout << " l1 = " << l1[j] << " r1 = " << r1[j] << std::endl;
      std::cout << " l2 = " << l2[j] << " r2 = " << r2[j] << std::endl;
    }
#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < size / piece / 2; i++) {
      unsigned int * tmp = new unsigned int[r1[i] - l1[i] + 1 + r2[i] - l2[i] + 1];
      tmp = Splitter(array + l1[i], array + l2[i], r1[i] - l1[i] + 1, r2[i] - l2[i] + 1);
      int j = l1[i], g = 0;
      while (j <= r2[i]) {
        array[j] = tmp[g++];
        j++;
      }
    }
    print_array(array, size);
    std::cout << std::endl;
  }
}


int main() {
  int size = 20, threads = 4;
  std::cout << " Size = " << size << " Threads = " << threads << std::endl;
  double time1, time2;
  unsigned int *a = new unsigned int[size];
  a = get_randomized_array(size);
  unsigned int *b = new unsigned int[size];
  copy_array(a, b, size);
  print_array(a, size);

  std::cout << std::endl;

  time1 = omp_get_wtime();
  dac_sort(a, size, threads);
  time2 = omp_get_wtime();
  std::cout << std::endl;
  print_array(a, size);
  std::cout << " Parralel time " << time2 - time1 << std::endl;
  check_result(a, size);
  std::cout << std::endl;

  time1 = omp_get_wtime();
  radix_sort(b, size);
  time2 = omp_get_wtime();
  print_array(b, size);
  std::cout << " Linear time " << time2 - time1 << std::endl;
  check_result(b, size);
}
