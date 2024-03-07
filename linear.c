#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int partition(int A[], int p, int r);
int partition_M(int A[], int p, int r, int M);
int Select(int A[], int p, int r, int i);
int linearSelect(int A[], int p, int r, int i);
void heapSort(int A[], int n);
void buildHeap(int A[], int n);
void heapify(int A[], int k, int n);

int A[500];
int main(void) {
   clock_t start_clock_first_algorithm, end_clock_first_algorithm, diff_clock_first_algorithm;
   double ex_time_first_algorithm;

   clock_t start_clock_second_algorithm, end_clock_second_algorithm, diff_clock_second_algorithm;
   double ex_time_second_algorithm;

   srand(time(NULL));
   int i;
   for (i = 0; i < 500; i++) { 
      A[i] = rand();
   }


   for (int cycle = 100; cycle <= 1000; cycle += 100) {
      start_clock_first_algorithm = clock();
      for (int i = 1; i <= 250; i++) {
         for (int j = 0; j < cycle; j++) {
            Select(A, 0, 499, i);
         }
      }
      end_clock_first_algorithm = clock();
      diff_clock_first_algorithm = end_clock_first_algorithm - start_clock_first_algorithm;
      ex_time_first_algorithm = (double)(diff_clock_first_algorithm) / CLOCKS_PER_SEC;
      printf("1번째 알고리즘 %d회 반복 소요 시간 : %lf초\n", cycle, ex_time_first_algorithm);
   }

   printf("\n\n\n");

   for (int cycle = 100; cycle <= 1000; cycle += 100) {
      start_clock_second_algorithm = clock();
      for (int i = 1; i <= 250; i++) {
         for (int j = 0; j < cycle; j++) {
            linearSelect(A, 0, 499, i);
         }
      }
      end_clock_second_algorithm = clock();
      diff_clock_second_algorithm = end_clock_second_algorithm - start_clock_second_algorithm;
      ex_time_second_algorithm = (double)(diff_clock_second_algorithm) / CLOCKS_PER_SEC;
      printf("2번째 알고리즘 %d회 반복 소요 시간 : %lf초\n", cycle, ex_time_second_algorithm);
   }

   return 0;
}

/*
<정렬 잘 되는지 확인용 코드>
int A[100];
int main(void) {
    srand(time(NULL));
    for (int i = 0; i < 100; i++) {
        A[i] = rand()% 1000;
    }

    for (int i = 1; i <= 30; i++) {
        printf("%d\n", linearSelect(A, 0, 49, i));
    }
}
*/

//책의 파티션(분할)함수
int partition(int A[], int p, int r) {
   int x = A[r];
   int i = p - 1;
   int temp;
   for (int j = p; j < r; j++) {
      if (A[j] <= x) {
         i++;
         temp = A[i];
         A[i] = A[j];
         A[j] = temp;
      }
   }
   temp = A[i + 1];
   A[i + 1] = A[r];
   A[r] = temp;
   return i + 1;
}

//(5-2)에 쓸 M을 기준원소로 삼아 전체원소 분할하는 파티션 함수
int partition_M(int A[], int p, int r, int M) { 
   //근데 M을 배열의 마지막 값으로 옮겨야 겠군....
   //그러려면 M의 A에서의 위치(인덱스)를 알아야겠네
   int M_index=-1;
   for (int i = p; i <= r; i++) {
      if (A[i] == M) {
         M_index = i; //M의 인덱스 찾음
         break;
      }
   }

   //M과 r의 위치를 바꿈
   int temp = A[r]; 
   A[r] = A[M_index];
   A[M_index] = temp;

   //이제 M을 기준으로 분할
   int x = A[r];
   int i = p - 1;
   for (int j = p; j < r; j++) {
      if (A[j] <= x) {
         i++;
         temp = A[i];
         A[i] = A[j];
         A[j] = temp;
      }
   }
   temp = A[i + 1];
   A[i + 1] = A[r];
   A[r] = temp;
   return i + 1;
}

//(5-1)평균 선형시간 선택 알고리즘
int Select(int A[], int p, int r, int i) {
   if (p == r) return A[p];
   int q = partition(A, p, r);
   int k = q - p + 1;
   if (i == k) return A[q];
   else if (i < k) return Select(A, p, q - 1, i);
   else return Select(A, q + 1, r, i - k);
}

//(5-2)최악의 경우에도 선형시간을 보장하는 알고리즘
int linearSelect(int A[], int p, int r, int i) {
   int n = r - p + 1; //원소의 총 수
   if (n <= 5) return Select(A, p, r, i);

   int groupNum = (n + 4) / 5; //그룹 개수

   int* m = (int*)calloc(groupNum, sizeof(int)); //중앙값들 저장할 1차원 배열 
   int** groups = (int**)malloc(groupNum*sizeof(int*)); //그룹들 저장할 2차원 배열

/* (수정사항 1)
    그룹들에 무조건 공간을 5개씩 할당하면 안된다. 
    이 코드를 사용하게 되면 분할된 그룹 중 적합한 쪽을 택해 재귀문을 돌릴 때 
    한 그룹에 원소가 1개만 들어가면 나머지 빈 4개의 공간에 
    쓰레기 값이 할당되게 된다.

   for (int j = 0; j < groupNum; j++) {
      groups[j] = (int*)malloc(5 * sizeof(int)); //각 그룹들에 5개씩 공간 할당
   }
*/

//그래서 위의 코드를 이렇게 수정했다.
for (int i = 0; i < groupNum; i++) { //각각의 그룹에 원소 수만큼 배열크기 할당
        if (i < groupNum - 1) {
            groups[i] = (int*)calloc(5, sizeof(int));
        }
        else { //마지막 그룹만 나머지만큼 크기 할당
            groups[i] = (int*)calloc(n % 5, sizeof(int));
        }
}

/*(수정사항 2)
    1번 오류의 연장선으로 같은 오류가 발생하는 원인이 된다. 
   for (int j = 0; j < groupNum; j++) { //그룹들에 원소 할당
      for (int k = 0; k < 5; k++) { //5개씩 원소 할당
         groups[j][k] = A[p + j * 5 + k];  //전체 배열 A의 원소들을 만들어 놓은 groups들에 할당
      }
      heapSort(groups[j], 5);
   }
   */

  //그래서 위의 코드를 이렇게 수정했다.
  //각 배열의 크기만큼만 원소를 할당하게 length변수 선언
   for (int j = 0; j < groupNum; j++) { //그룹들에 원소 할당
        int length = sizeof(groups[j]) / sizeof(*groups[j]);
        for (int k = 0; k < length; k++) { //5개씩 원소 할당
            groups[j][k] = A[p + j * 5 + k];  //전체 배열 A의 원소들을 만들어 놓은 groups들에 할당
        }
        heapSort(groups[j], length);
    }


/*(수정사항 3)
   for (int j = 0; j < groupNum; j++) {
      m[j] = groups[j][2]; //각 그룹들의 중앙값들을 m에 저장
   }
   */

  //그래서 위의 코드를 이렇게 수정했다.
    for (int j = 0; j < groupNum; j++) {
        int length = sizeof(groups[j]) / sizeof(*groups[j]); //*각 배열의 길이에 대한
        int index = length / 2; //*중앙값을 도출하도록 위에서 썼던 length사용
        m[j] = groups[j][index]; //각 그룹들의 중앙값들을 m에 저장
    }


   int M = linearSelect(m, 0, groupNum - 1, (groupNum + 1) / 2); //중앙값들의 중앙값
   int q= partition_M(A, p, r, M); //중앙값들의 중앙값을 기준으로 분할;
   int k= q - p + 1; //기준원소가 전체에서 k번째 작은 원소임을 의미
   

/*
   if (i < 0 || i > r - p + 1) {
      printf("오류 검출용.\n");
      return -1;
   }
   */

     if (i == k) {
     free(m);
     for (int s = 0; s < groupNum; s++) free(groups[s]);
     free(groups);
		return A[q];
	}
    else if (i < k) {
	free(m);
     for (int s = 0; s < groupNum; s++) free(groups[s]);
     free(groups);
		return linearSelect(A, p, q - 1, i);
	}
    else{
     free(m);
     for (int s = 0; s < groupNum; s++) free(groups[s]);
     free(groups);
		return linearSelect(A, q + 1, r, i - k);
	}
}


void heapSort(int A[], int n) {
   buildHeap(A, n);

   for (int i = n - 1; i > 0; i--) {
      int temp = A[0];
      A[0] = A[i];
      A[i] = temp;

      heapify(A, 0, i);
   }
}

void buildHeap(int A[], int n) {
   for (int i = n / 2-1 ; i >= 0; i--) {
      heapify(A, i, n);
   }
}

void heapify(int A[], int k, int n) {
   int left = 2 * k + 1;
   int right = 2 * k + 2;
   int larger = k;

   if (left < n && A[left] > A[larger]) {
      larger = left;
   }
   if (right < n && A[right] > A[larger]) {
      larger = right;
   }

   if (larger != k) {
      int temp = A[k];
      A[k] = A[larger];
      A[larger] = temp;
      heapify(A, larger, n);
   }
}