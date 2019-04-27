#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define N 50

/*---------------------------------------------------------------------*/
double calcula_pi(double nr, double inicio, double salto){
    double x, base, altura;
    double i, pi;

    base = 1.0 / nr;
    pi = 0.0;

    for( i=inicio ; i<=nr ;  i+= salto ) {
        x = base * (( double)i - 0.5);
        altura = 4.0 / (1.0 + x*x);
        pi += base * altura;
    }

    return pi;


}
/*---------------------------------------------------------------------*/
int main(int argc, char **argv){

    double pi = 0, aux;
    int pid, fd1, fd2, fd3;
    double nr;
    double i;
    char fifo1[N] = "fifo1", fifo2[N] = "fifo2", fifo3[N] = "fifo3";

    if ( argc != 2 ){
        printf("%s <num_retangulos>\n", argv[0]);
        exit(0);
    }


    nr = atof(argv[1]);

    pid = fork();
    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);
    mkfifo(fifo3, 0666);

    if (pid > 0) {
        pid = fork();
        if (pid > 0) {
            pid = fork();
            if (pid > 0) {
                pi = calcula_pi(nr, 4, 4); 
                fd1 = open(fifo1, O_RDONLY);
                read(fd1, &aux, sizeof(double));
                pi += aux;
                fd2 = open(fifo2, O_RDONLY);
                read(fd2, &aux, sizeof(double));
                pi += aux;
                fd3 = open(fifo3, O_RDONLY);
                read(fd3, &aux, sizeof(double));
                pi += aux;
                printf("Pi = %.15f\n",pi);
                unlink(fifo1);
                unlink(fifo2);
                unlink(fifo3);
            } else {
                fd3 = open(fifo3, O_WRONLY);
                pi = calcula_pi(nr, 3, 4);
                write(fd3, &pi, sizeof(double));   
                close(fd3);
            }
        } else {
            fd2 = open(fifo2, O_WRONLY);
            pi = calcula_pi(nr, 2, 4);
            write(fd2, &pi, sizeof(double));   
            close(fd2);
        }
    } else {
        fd1 = open(fifo1, O_WRONLY);
        pi = calcula_pi(nr, 1, 4);
        write(fd1, &pi, sizeof(double));   
        close(fd1);  
    }

}
/*---------------------------------------------------------------------*/
