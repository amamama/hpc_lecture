#include <cstdio>
#include <cstdlib>
#include <vector>

template<class T>
void merge_ser(std::vector<T>& vec, int begin, int mid, int end) {
  std::vector<T> tmp(end-begin+1);
  int left = begin;
  int right = mid+1;
  for (int i=0; i<tmp.size(); i++) { 
    if (left > mid)
      tmp[i] = vec[right++];
    else if (right > end)
      tmp[i] = vec[left++];
    else if (vec[left] <= vec[right])
      tmp[i] = vec[left++];
    else
      tmp[i] = vec[right++]; 
  }
  size_t s = tmp.size();
  for (int i=0; i < s; i++) 
    vec[begin+i] = tmp[i];
}

template<class T>
void merge(std::vector<T>& vec, int begin, int mid, int end) {
  std::vector<T> tmp(end-begin+1);
  int left = begin;
  int right = mid+1;
  for (int i=0; i<tmp.size(); i++) { 
    if (left > mid)
      tmp[i] = vec[right++];
    else if (right > end)
      tmp[i] = vec[left++];
    else if (vec[left] <= vec[right])
      tmp[i] = vec[left++];
    else
      tmp[i] = vec[right++]; 
  }
  size_t s = tmp.size();
#pragma omp parallel for
  for (int i=0; i < s; i++) 
    vec[begin+i] = tmp[i];
}

template<class T>
void merge_sort_ser(std::vector<T>& vec, int begin, int end) {
  if(begin < end) {
    int mid = (begin + end) / 2;
    merge_sort_ser(vec, begin, mid);
    merge_sort_ser(vec, mid+1, end);
    merge_ser(vec, begin, mid, end);
  }
}


template<class T>
void merge_sort(std::vector<T>& vec, int begin, int end) {
  if(begin < end) {
    int mid = (begin + end) / 2;
	if(end - begin > 0x2000) {
#pragma omp task shared(vec)
      merge_sort(vec, begin, mid);
#pragma omp task shared(vec)
      merge_sort(vec, mid+1, end);
#pragma omp taskwait
      merge(vec, begin, mid, end);
	} else merge_sort_ser(vec, begin, end);
  }
}

int main() {
  int n = 0x10000000;
  std::vector<int> vec(n);
  for (int i=0; i<n; i++) {
    vec[i] = rand() % (10 * n);
    //printf("%d ",vec[i]);
  }
  printf("\n");
#pragma omp parallel
#pragma omp single
  merge_sort(vec, 0, n-1);
  int last = vec[0];
  for (int i=0; i<n; i++) {
	  if(last > vec[i]) printf("not sorted\n");
    //printf("%d ",vec[i]);
  }
  printf("\n");
}
