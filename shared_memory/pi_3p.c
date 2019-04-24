#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>

/*---------------------------------------------------------------------*/
double calcula_pi(double nr, double inicio, double salto){
    double x, base, altura;
    double i, pi;

    base = 1.0 / nr;
    pi = 0.0;

    for( i= inicio ; i<=nr ;  i+= salto ) {
        x = base * (( double)i - 0.5);
        altura = 4.0 / (1.0 + x*x);
        pi += base * altura;
    }

    return pi;


}
/*---------------------------------------------------------------------*/
int main(int argc, char **argv){

    double *memoria;
    double pi = 0;
    int pid, chave = 5, shmid;
    double nr;
    double i;

    if ( argc != 2 ){
        printf("%s <num_retangulos>\n", argv[0]);
        exit(0);
    }


    nr = atof(argv[1]);

    if ( ( shmid = shmget (chave, 2*sizeof(double) ,IPC_CREAT | 0600)) < 0 ){
        printf("Erro na criacao da memoria compartilhada");
    }
 
    if ( ( memoria = shmat(shmid, 0, 0 )) < 0 ){
        printf("Erro na alocacao");
    }

    pid = fork();

    if (pid > 0) {
        pid = fork();

        if (pid > 0) {
            pi = calcula_pi(nr,3,3);
            wait(NULL);
            wait(NULL);
            pi += memoria[0];
            pi += memoria[1];
            shmdt(memoria);
            shmctl(shmid, IPC_RMID, 0);
            printf("Pi = %.15f\n",pi);
        } else {
            memoria[1] = calcula_pi(nr,2,3);
        }
    } else {
        memoria[0] = calcula_pi(nr,1,3);
    }
}
/*---------------------------------------------------------------------*/
