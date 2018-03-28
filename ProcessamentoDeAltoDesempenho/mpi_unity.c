#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define MASTER_ID 0
#define ARRAY_SIZE 40

/*
	This program fills an array and divide the work into slaves.
	Each slave process a whole block and send each processed piece to master.
*/

int main(int argc, char **argv){
    
    MPI_Status status;
    
    int ierr, size, pid, i, j;
    
    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunksize = ARRAY_SIZE / size; //size per processor
        
    //------------ MASTER STARTS HERE ------------
    if(pid == MASTER_ID) {    
        float arr[ARRAY_SIZE];
        printf("LOL 1\n");
        //master fills the first parts [0, chunksize - 1]
        for(j = 0; j < chunksize; j++){
            arr[j] = sqrt(j * 1.0);
        }

        for(i = 1; i < size; i++){      
            for (j = 0; j < chunksize; j++) {
                int real_pos = i * chunksize + j;
                ierr = MPI_Recv(&arr[real_pos], 1, MPI_FLOAT, i, real_pos, MPI_COMM_WORLD, &status);            
            }           
        }
        printf("\nMaster and slaves have finished the work, result:\n");
        for(i = 0; i < ARRAY_SIZE; i++){
            printf("%.1f ", arr[i]);
            if((1+i) % chunksize == 0) printf("\n");
        }
        printf("\n");
    }
    //------------ MASTER ENDS HERE ------------
    
    //------------ SLAVES START HERE ------------
    else{
		printf("LOL\n");
        float arr[chunksize];
		
        for(j = 0; j < chunksize; j++){
            int real_pos = pid * chunksize + j;
            arr[j] = sqrt(real_pos * 1.0);
            ierr = MPI_Send(&arr[j], 1, MPI_FLOAT, MASTER_ID, real_pos, MPI_COMM_WORLD);
        }
    
    }
    //------------ SLAVES END HERE ------------
    ierr = MPI_Finalize();
    return 0;
}