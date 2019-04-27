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
    int pid, fd;
    double nr;
    double i;
    char fifo[N] = "fifo";

    if ( argc != 2 ){
        printf("%s <num_retangulos>\n", argv[0]);
        exit(0);
    }


    nr = atof(argv[1]);

    pid = fork();
    mkfifo(fifo, 0666);

    if (pid > 0) {
        pi = calcula_pi(nr, 2, 2); 
        fd = open(fifo, O_RDONLY);
        read(fd, &aux, sizeof(double));
        pi += aux;
        printf("Pi = %.15f\n",pi);
        unlink(fifo);
    } else {
        fd = open(fifo, O_WRONLY);
        pi = calcula_pi(nr, 1, 2);
        write(fd, &pi, sizeof(double));   
        close(fd);  
    }

}
/*---------------------------------------------------------------------*/
