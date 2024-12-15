#include <iostream>

using ll = long long;

ll minimum_of_3(ll a, ll b, ll c) {
  if (a < b) {
    if (a < c)
      return a;
    else
      return c;
  } else {
    if (b < c)
      return b;
    else
      return c;
  }
}

int main() {
  int n, m;
  std::cin >> n >> m;
  if (n > 1000 || n < 2 || m > 1000 || m < 2) {
    return 0;
  }
  long int *A = (long int *)malloc(sizeof(long int) * n * m);
  long long int *B = (long long int *)malloc(sizeof(long long int) * n * m);
  int i = 0, j = 0, k = 0;
  for (i = 0; i < n; ++i) {
    for (j = 0; j < m; ++j) {
      std::cin >> A[i * m + j];
    }
  }
  for (j = 0; j < m; ++j) {
    B[(n - 1) * m + j] = A[(n - 1) * m + j];
  }
  for (i = n - 2;; --i) {
    B[i * m] =
        minimum_of_3(B[(i + 1) * m], B[(i + 1) * m], B[(i + 1) * m + 1]) +
        A[i * m];
    for (j = 1; j < m - 1; ++j) {
      B[i * m + j] = minimum_of_3(B[(i + 1) * m + j - 1], B[(i + 1) * m + j],
                                  B[(i + 1) * m + j + 1]) +
                     A[i * m + j];
    }
    B[i * m + m - 1] =
        minimum_of_3(B[(i + 1) * m + m - 2], B[(i + 1) * m + m - 1],
                     B[(i + 1) * m + m - 1]) +
        A[i * m + m - 1];
    if (!i)
      break;
  }
  for (j = 1; j < m; ++j) {
    if (B[j] < B[k])
      k = j;
  }
  std::cout << B[k] << '\n' << "(1," << k + 1 << ")";
  for (i = 1; i < n; ++i) {
    if (k && B[i * m + k - 1] < B[i * m + k]) {
      if (k < m - 1 && B[i * m + k - 1] > B[i * m + k + 1])
        ++k;
      else
        --k;
    } else {
      if (k < m - 1 && B[i * m + k + 1] < B[i * m + k])
        k++;
    }
    std::cout << " " << "(" << i + 1 << "," << k + 1 << ")";
  }
  std::cout << std::endl;
  free(B);
  free(A);
  return 0;
}