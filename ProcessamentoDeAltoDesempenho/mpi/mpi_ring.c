#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define MASTER_ID 0

const int throw_tag = 1;

/*
	This program works like a ring, each processor sends the 'ball' called password to the following processor.
	The last processors sends it back to the MASTER.
	Each processor just sums its 2 ^ pid to the current password as well as the MASTER.
*/
int main(){
    MPI_Status status;

    int ierr, size, pid;

    ierr = MPI_Init(NULL, NULL);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(size == 1){
		printf("Password %d matches with %d.\n", 1, (1 << size) - 1);
	}else{
		if(pid == MASTER_ID){
			int initial_password = (1 << 0);//sending the inital password
			ierr = MPI_Send(&initial_password, 1, MPI_INT, pid + 1, throw_tag, MPI_COMM_WORLD);

			int final_password;//receiving the final password
			ierr = MPI_Recv(&final_password, 1, MPI_INT, size - 1, throw_tag, MPI_COMM_WORLD, &status);

			//checking correctness
			if(final_password == (1 << size) - 1) {
				printf("Password %d matches with %d.\n", final_password, (1 << size) - 1);
			}else{
				printf("! - Password %d does't match with %d\n", final_password, (1 << size) - 1);
			}
		}else{
			int curr_password;//receiving the current password
			ierr = MPI_Recv(&curr_password, 1, MPI_INT, pid - 1, throw_tag, MPI_COMM_WORLD, &status);
			printf("curr_password = %d\n", curr_password);

			curr_password |= (1 << pid);// += 2 ^ pid, sending to the next processor
			ierr = MPI_Send(&curr_password, 1, MPI_INT, (pid + 1) % size, throw_tag, MPI_COMM_WORLD);
		}
	}
	return 0;
}
