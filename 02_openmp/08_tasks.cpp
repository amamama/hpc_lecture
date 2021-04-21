#include <cstdio>

int fib_ser(int n)
{
	if (n < 2)
		return n;
	else
		return fib_ser(n-1) + fib_ser(n-2);
}

int fib(int n)
{
	int i, j;

	if (n <= 20)
		return fib_ser(n);
	else
	{
#pragma omp task shared(i)
		i = fib(n-1) % 0x12345678;
#pragma omp task shared(j)
		j = fib(n-2) % 0x12345678;
#pragma omp taskwait
		return i+j;
	}
}
int fib_(int n) {
  int i,j;
  if (n<2) return n;
#pragma omp task shared(i) if (n > 20)
  i = fib_(n-1) % 0x12345678;
#pragma omp task shared(j) if (n > 20)
  j = fib_(n-2) % 0x12345678;
#pragma omp taskwait
  return i+j;
}

int main() {
  int n = 50;
#pragma omp parallel
  for (int i=1; i<n; i++) {
#pragma omp single
    printf("%d ",fib(i) % 0x12345678);
  }
  printf("\n");
}
