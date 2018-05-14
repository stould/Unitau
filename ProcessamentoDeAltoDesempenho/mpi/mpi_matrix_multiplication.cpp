#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MASTER_ID 0
const int TAG = 1;

int* multiplyChunk(int *a, int *b, int na, int ma, int nb, int mb, int size, int pid){
	int chunkSize = na / size;
	int start = pid * chunkSize, end = start + chunkSize - 1;
	if(pid == size - 1) end = na-1;
	int totalSize = (end - start + 1) * mb;
	int *ans = (int *) malloc(sizeof(int) * totalSize);
	for(int i = 0; i < totalSize; i++){
		ans[i] = 0;
	}
	for(int i = start; i <= end; i++){
		for(int j = 0; j < mb; j++){
			for(int k = 0; k < nb; k++){
				ans[(i - start) * mb + j] += a[i * ma + k] * b[k * mb + j];
			}
		}
	}
	return ans;
}

void groupChunk(int *ans, int *chunk, int na, int ma, int nb, int mb, int size, int pid){
	int chunkSize = na / size;
	int start = pid * chunkSize, end = start + chunkSize - 1;
	if(pid == size - 1) end = na-1;
	int pos = 0;
	for(int i = start; i <= end; i++){
		for(int j = 0; j < mb; j++){
			ans[i * mb + j] = chunk[pos++];
		}
	}
}

/*
This program calculates a matrix multiplication of two matrices
The job is divided among the threads.
*/
int main(int argc, char **argv) {
	int ierr, size, pid;
	ierr = MPI_Init(&argc, &argv);
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (pid == MASTER_ID) {;
		int na,ma;
		//printf("Reading Matrix A dimensions: lines, cols ?\n");
		scanf("%d%d\n", &na, &ma);
		int *a = (int*) malloc(sizeof(int) * (na * ma));

		//printf("Reading in matrix A [%d, %d].\n", na, ma);
		for(int i = 0; i < na; i++){//reaging matrix a
			for(int j = 0; j < ma; j++){
				scanf("%d", &a[i * ma + j]);
			}
		}

		int nb, mb;
		//printf("Reading Matrix B dimensions: lines, cols ?\n");
		scanf("%d%d\n", &nb, &mb);
		int *b = (int*) malloc(sizeof(int) * (nb * mb));

		int nc = na, mc = mb;
		int *answer = (int*) malloc(sizeof(int) * na * mb);//creating matrix c to store the answer

		//printf("Reading in matrix B [%d, %d].\n", nb, mb);
		for(int i = 0; i < nb; i++){//reading matrix b
			for(int j = 0; j < mb; j++){
				scanf("%d", &b[i * mb + j]);
			}
		}

		MPI_Bcast(&na, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
		MPI_Bcast(&ma, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
		MPI_Bcast(&a[0], na * ma, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

		MPI_Bcast(&nb, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
		MPI_Bcast(&mb, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
		MPI_Bcast(&b[0], nb * mb, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

		int* chunk = multiplyChunk(a, b, na, ma, nb, mb, size, pid);
		groupChunk(answer, chunk, na, ma, nb, mb, size, pid);
		free(chunk);

		for(int i = 1; i < size; i++){

			int chunkSize = na / size;
			int start = i * chunkSize, end = start + chunkSize - 1;
			if(i == size - 1) end = na-1;
			int totalSize = (end - start + 1) * mb;

			int* chunk = (int*) malloc(sizeof(int) * totalSize);
			ierr = MPI_Recv(&chunk[0], totalSize, MPI_INT, i, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			groupChunk(answer, chunk, na, ma, nb, mb, size, i);
			free(chunk);
		}
		//printf("The resulting matrix is:\n");
		for(int i = 0; i < na; i++){
			for(int j = 0; j < mb; j++){
				if(j == mb - 1)
					printf("%d", answer[i * mb + j]);
				else
					printf("%d ", answer[i * mb + j]);
			}
			puts("");
		}
	}else {
		int na, ma, nb, mb;
		MPI_Bcast(&na, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
		MPI_Bcast(&ma, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
		int *a = (int*) malloc(sizeof(int) * (na * ma));
		MPI_Bcast(&a[0], na * ma, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

		MPI_Bcast(&nb, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
		MPI_Bcast(&mb, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
		int *b = (int*) malloc(sizeof(int) * (nb * mb));
		MPI_Bcast(&b[0], nb * mb, MPI_INT, MASTER_ID, MPI_COMM_WORLD);

		int* chunk = multiplyChunk(a, b, na, ma, nb, mb, size, pid);
		int chunkSize = na / size;
		int start = pid * chunkSize, end = start + chunkSize - 1;
		if(pid == size - 1) end = na-1;
		int totalSize = (end - start + 1) * mb;

		ierr = MPI_Send(&chunk[0], totalSize, MPI_INT, MASTER_ID, TAG, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
