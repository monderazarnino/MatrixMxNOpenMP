///**
// * file mxn.cpp
// */
//#include <sys/time.h>
//#include <time.h>
//#include <cstdio>
//#include <stdlib.h>
//#include <pthread.h>
//
//#include <omp.h>
//
//using namespace std;
///* matrix-omp.cpp */
//const int size = 1600;
//
//float a[size][size];
//float b[size][size];
//float c[size][size];
//int i, j, k;
//float sum;
//int main() {
//	clock_t before, after;
//	float time_serial;
//	timespec startTime, currentTime;
//	// Initialize buffers.
//	for (int i = 0; i < size; ++i) {
//		for (int j = 0; j < size; ++j) {
//			a[i][j] = (float) i + j;
//			b[i][j] = (float) i - j;
//			c[i][j] = 0.0f;
//		}
//	}
//
//	before = clock();
//	long elapsed;
//	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &startTime);
//
//	// Compute matrix multiplication.
//	// C <- C + A x B
//
//
//omp_set_num_threads(2);
//#pragma omp parallel shared(a,b,c) private(i,j,k)
//	{
//#pragma omp for
//		for (i = 0; i < size; ++i) {
//			for (j = 0; j < size; ++j) {
//				for (k = 0; k < size; ++k) {
//					c[i][j] += a[i][k] * b[k][j];
//				}
//			}
//		}
//	}
//	after = clock();
//
//	time_serial = (float) (after - before) / CLOCKS_PER_SEC;
//
//	printf("result time in clock() mxm = %f seconds\n", time_serial);
//	clock_gettime(CLOCK_REALTIME, &currentTime);
//	elapsed = currentTime.tv_nsec - startTime.tv_nsec;
//
//	printf("result time in clock_gettime() : %f second \n", elapsed / 1000000);
//
//	return 0;
//}
/**
 * file mxn.cpp
 */

#include <sys/time.h>
#include <time.h>
#include <cstdio>
#include <stdlib.h>
#include <pthread.h>

#include <omp.h>

using namespace std;
/*
 * 16 ,32, 64, 100 ,200, 400, 800,1600
 */
const int N[] = { 16, 32, 64, 100, 200, 400, 800, 1600 };
float *a, *b, *c, *rc;

void initialize(float *a, float *b, int n) {

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			a[i * n + j] = (float) i + j;
			b[i * n + j] = (float) i * j;
		}
	}
}

int i, k, j, ordo;
float sum;

void mxn(float *c, float *a, float *b, int n) {
#pragma omp parallel shared(a,b,c) private(i,j,k,sum)
	{
#pragma omp parallel for
		for (i = 0; i < n; ++i) {

			for (j = 0; j < n; ++j) {
				//float sum = 0;
				for (k = 0; k < n; ++k) {
					sum += a[i * n + k] * b[k * n + j];

				}
				c[i * n + j] = sum;
			}
			//printf(" i  = %d,j= %d,k= %d \n",c[i * n + 1]);
		}
	}
}

int main() {
	clock_t before, after;
	float time_serial;
	timespec startTime, currentTime;
	struct timeval tim;
	time_t start, end;
	int jumlahThread = 4;

	long elapsed;

//#pragma omp parallel shared(a,b,c)
//	{

//mxn(c, a, b, N);

//int ordo[] = { 16, 32, 64, 100, 200, 400, 800, 1600 };
	double t1, t2;
	int rest1, rest2;
	//int f = 7;
	for (int f = 0; f < 8; f++) {
		a = (float*) malloc(N[f] * N[f] * sizeof(float));
		b = (float*) malloc(N[f] * N[f] * sizeof(float));
		c = (float*) malloc(N[f] * N[f] * sizeof(float));
		//	rc = (float*) malloc(N * N * sizeof(float));

		initialize(a, b, N[f]);
		before = clock();
		start = time(NULL);
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &startTime);
		gettimeofday(&tim, NULL);
		t1 = tim.tv_sec + (tim.tv_usec / 1000000.0);
		//omp_set_dynamic(true);
		omp_set_num_threads(jumlahThread);
		printf("\n\n\n");
		printf(" ordo %d ....Sedang dalam proses......\n", N[f]);
		printf(
				"================================================================\n");
		ordo = N[f];

#pragma omp parallel shared(a,b,c) private(i,j,k,sum)
		{
#pragma omp for
			for (i = 0; i < ordo; ++i) {
				for (j = 0; j < ordo; ++j) {
					for (k = 0; k < ordo; ++k) {
						sum += a[i * ordo + k] * b[k * ordo + j];
					}
					c[i * ordo + j] = sum;
				}
			}
		}

//#pragma omp

		after = clock();

		time_serial = (float) (after - before) / CLOCKS_PER_SEC;

		//printf("result time in clock() mxm = %f seconds\n", time_serial);

		end = time(NULL);
		gettimeofday(&tim, NULL);
		t2 = tim.tv_sec + (tim.tv_usec / 1000000.0);

		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &currentTime);
		elapsed = currentTime.tv_nsec - startTime.tv_nsec;

		//printf()
		rest1 = end / 3600;
		rest2 = start / 3600;

		rest1 = rest2 - rest1;
		printf(
				"=========================Selesai================================\n");
		printf("Jumlah Thread yang digunakan : %d \n", jumlahThread);
		printf(
				"================================================================\n");
		printf("							Time \n");
		printf(
				"================================================================\n");
		printf("result time in clock()        : %f second \n", time_serial);
		printf("result time in time()         : %f second \n", rest1);
		printf("result time in gettimeofday() : %.6lf seconds \n", t2 - t1);
		printf("result time in clock_gettime(): %f second \n",
				elapsed / 1000000);
		free(a);
		free(b);
		free(c);
		//free(rc);
	}
printf("hello github!");
	return 0;

}

