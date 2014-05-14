//  main.c
//  nQueens
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
void timeval_print(struct timeval *tv);

void rnQueens(int k, int n, int *row, bool *rowUsed, bool *dDiag, bool *uDiag, int *track, int printResults, int printCount);
bool positionOK(int k, int n, int *row, bool *rowUsed, bool *dDiag, bool *uDiag);



int main(int argc, const char * argv[])
{
    // get size n
    int n;
    int printResults, printCount;
    printf("Enter number of queens: ");
    scanf("%d",&n);
    printf("Do you want results printed? 1 for yes, 0 for no\n");
    scanf("%d", &printResults);
    if (printResults==1) {
        printf("Enter max number of print results\n");
        scanf("%d", &printCount);
    }
    
    // begin timestamp
    struct timeval timeBegin, timeEnd, timeDiff;
    gettimeofday(&timeBegin, NULL);
    timeval_print(&timeBegin);
    
    // tracks number of found solutions
    int track = 0;
    
    // create arrays and fill
    int *row = malloc(n*sizeof(int));           //row containing the queen of column x
    bool *rowUsed = malloc(n*sizeof(bool));       //set initially to false
    bool *dDiagUsed = malloc(((2*n)-1)*sizeof(bool));   //if diagonals are occupied
    bool *uDiagUsed = malloc(((2*n)-1)*sizeof(bool));
    for (int i=0; i<n; i++) {
        rowUsed[i] = false;
        row[i] = 0;
    }
    for (int i=0; i<((2*n)-1); i++) {
        dDiagUsed[i] = false;
        uDiagUsed[i] = false;
    }
    
    // run algorithm
    rnQueens(0, n, row, rowUsed, dDiagUsed, uDiagUsed, &track, printResults, printCount);
    
    // print results
    printf("There are %d solutions to the %d-Queen Problem\n", track, n);
    
    // deallocate arrays
    free(row);
    free(rowUsed);
    free(dDiagUsed);
    free(uDiagUsed);
    
    // end timestamp
    gettimeofday(&timeEnd, NULL);
    timeval_print(&timeEnd);
    
    // print difference
    timeval_subtract(&timeDiff, &timeEnd, &timeBegin);
    printf("Ran in %ld.%06d seconds\n", timeDiff.tv_sec, timeDiff.tv_usec);

    return 0;
}

void rnQueens(int k, int n, int *row, bool *rowUsed, bool *dDiag, bool *uDiag, int *track, int printResults, int printCount) {
    for (int i=0; i<n; i++) {
        row[k] = i;
        if (positionOK(k, n, row, rowUsed, dDiag, uDiag)) {     //check if new queen conflicts with existing queens
            rowUsed[row[k]] = true;                 //temporarily place queen
            dDiag[n-k+row[k]] = true;
            uDiag[k+row[k]-1] = true;
            if (k == n-1) {
                if (printResults==1 && *track <= printCount) {
                    for (int j=0; j<n; j++)     //found solution, print
                        printf("%d ", row[j]);
                    printf("\n");
                }
                *track = *track + 1;
            }
            else {
                rnQueens(k+1, n, row, rowUsed, dDiag, uDiag, track, printResults, printCount);
            }
            rowUsed[row[k]] = false;        //if no solution found, reset and continue
            dDiag[n-k+row[k]] = false;
            uDiag[k+row[k]-1] = false;
        }
    }
}

bool positionOK(int k, int n, int *row, bool *rowUsed, bool *dDiag, bool *uDiag) {
    return !(rowUsed[row[k]] || dDiag[n-k+row[k]] || uDiag[k+row[k]-1]);
}

// Return 1 if the difference is negative, otherwise 0.
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    
    return (diff<0);
}

void timeval_print(struct timeval *tv)
{
    char buffer[30];
    time_t curtime;
    
    printf("%ld.%06d", tv->tv_sec, tv->tv_usec);
    curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    printf(" = %s.%06d\n", buffer, tv->tv_usec);
}

