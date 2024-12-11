#include <iostream>
#include <vector>

using namespace std;

#define KEK 3

int SwapNumbers(vector<int> &numbers, int quantity) {
  if (quantity <= 1) {
    return 0;
  }
  int count = 0;

  vector<int> counts(KEK);
  for (int i = 0; i < quantity; ++i) {
    ++counts[numbers[i] - 1];
  }
  counts[2] = counts[1] + counts[0];
  counts[1] = counts[0];
  counts[0] = 0;

  for (int cur = 0; cur < quantity; ++cur) {
    if (cur < counts[1]) {
      if (numbers[cur] == 2) {
        for (int check = counts[1]; check < quantity; ++check) {
          if (numbers[check] == 1) {
            int tmp = numbers[cur];
            numbers[cur] = numbers[check];
            numbers[check] = tmp;
            ++count;
            break;
          }
        }
      } else if (numbers[cur] == 3) {
        for (int check = quantity - 1; check >= counts[1]; --check) {
          if (numbers[check] == 1) {
            int tmp = numbers[cur];
            numbers[cur] = numbers[check];
            numbers[check] = tmp;
            ++count;
            break;
          }
        }
      }
    } else if (cur < counts[2]) {
      if (numbers[cur] == 3) {
        for (int check = counts[2]; check < quantity; ++check) {
          if (numbers[check] == 2) {
            int tmp = numbers[cur];
            numbers[cur] = numbers[check];
            numbers[check] = tmp;
            ++count;
            break;
          }
        }
      }
    }
  }
  return count;
}

int main() {
  int quantity;
  cin >> quantity;
  vector<int> numbers(quantity);
  for (int i = 0; i < quantity; ++i) {
    cin >> numbers[i];
  }
  cout << SwapNumbers(numbers, quantity) << endl;
  return 0;
}