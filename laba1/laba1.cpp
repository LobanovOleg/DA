#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>

using namespace std;

struct KeyValue {
  long keyInNum;
  char *key;
  char *value;
};

class TVector {
private:
  KeyValue *arr;
  long long vector_size_;
  long long capacity;

public:
  TVector(long long size, long long capacity);
  long long Size();
  void Print();
  void PushBack(KeyValue elem);
  KeyValue *First();
  KeyValue *Last();
  KeyValue &operator[](size_t ix);
  TVector &operator=(TVector &vector);
  ~TVector();
};

const size_t MAX_LEN = 65;
const size_t PHONES_BITS = 45;

TVector::TVector(long long size, long long capacity) {
  vector_size_ = size;
  this->capacity = capacity;
  arr = new KeyValue[capacity];
}

long long TVector::Size() { return this->vector_size_; }

void TVector::PushBack(KeyValue elem) {
  if (vector_size_ == capacity) {
    capacity *= 30;
    arr = (KeyValue *)realloc(arr, capacity * sizeof(KeyValue));
  }
  arr[vector_size_] = elem;
  vector_size_++;
}

void TVector::Print() {
  for (int j = 0; j < vector_size_; ++j) {

    cout << arr[j].key << "\t";
    for (size_t i = 0; i < MAX_LEN - 1; ++i) {
      if (arr[j].value[i] != '\0')
        cout << arr[j].value[i];
    }
    cout << "\n";
  }
}

KeyValue *TVector::First() {
  if (vector_size_ == 0)
    return NULL;
  return &arr[0];
}

KeyValue *TVector::Last() {
  if (vector_size_ == 0)
    return NULL;
  return &arr[vector_size_];
}

KeyValue &TVector::operator[](size_t ix) { return arr[ix]; }

TVector &TVector::operator=(TVector &vector) {
  delete[] arr;
  arr = nullptr;
  if (vector.arr != nullptr) {
    arr = new KeyValue[vector.capacity];
    for (int i = 0; i < vector.Size(); i++) {
      arr[i] = vector.arr[i];
    }
    capacity = vector.capacity;
    vector_size_ = vector.Size();
  }
  return *this;
}

TVector::~TVector() { delete[] arr; }

size_t Log(size_t b, size_t a) { return (int)(log(b) / log(a)); }

int GetDigit(long n, int r, int numSys, int i) {
  return (n >> (r * i)) & (numSys - 1);
}

int GetSortPar(long n) {
  if (PHONES_BITS < Log(n, 2)) {
    return PHONES_BITS;
  } else {
    return (int)Log(n, 2);
  }
}

void countingSort(TVector &vec, int index, int r, int numSys) {
  long *tmp_array = new long[numSys];

  TVector result(vec.Size(), vec.Size());

  for (int i = 0; i < numSys; ++i) {
    tmp_array[i] = 0;
  }

  for (int indexI = 0; indexI < vec.Size(); ++indexI) {
    tmp_array[GetDigit(vec[indexI].keyInNum, r, numSys, index)]++;
  }

  for (int i = 1; i <= numSys; ++i) {
    tmp_array[i] += tmp_array[i - 1];
  }

  for (int j = vec.Size() - 1; j >= 0; --j) {
    KeyValue tmp_data = vec[j];
    result[tmp_array[GetDigit(vec[j].keyInNum, r, numSys, index)] - 1] =
        tmp_data;
    tmp_array[GetDigit(vec[j].keyInNum, r, numSys, index)]--;
  }

  vec = result;

  delete[] tmp_array;
}

void radixSort(TVector &vec) {

  if (vec.Size() < 2) {
    return;
  }

  int r = GetSortPar(vec.Size());
  int numSys = pow(2, r);
  int phoneDigits = ceil((float)PHONES_BITS / r);

  for (int index = 0; index < phoneDigits; ++index) {
    countingSort(vec, index, r, numSys);
  }
}

int main() {

  TVector vec(0, 100);
  KeyValue elem;

  elem.key = new char[17];
  while (scanf("%s", elem.key) != EOF) {

    elem.value = new char[MAX_LEN];
    char *tmp = new char[MAX_LEN];
    scanf("%s", tmp);
    size_t tmp_size = strlen(tmp);
    for (size_t j = 0; j < MAX_LEN - 1 - tmp_size; ++j) {
      elem.value[j] = '\0';
    }
    for (size_t j = MAX_LEN - 1 - tmp_size, val_j = 0; j < MAX_LEN;
         ++j, ++val_j) {
      elem.value[j] = tmp[val_j];
    }

    elem.keyInNum = 0;
    tmp_size = strlen(elem.key);
    for (size_t j = 0; j < tmp_size; ++j) {
      if (elem.key[j] != '+' && elem.key[j] != '-') {
        elem.keyInNum = elem.keyInNum * 10 + elem.key[j] - '0';
      }
    }

    vec.PushBack(elem);

    elem.key = new char[17];
  }
  radixSort(vec);
  vec.Print();

  return 0;
}
