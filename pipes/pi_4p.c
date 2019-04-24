#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

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
    int pid;
    double nr;
    double i;
    int p[2], p1[2], p2[2];

    if ( argc != 2 ){
        printf("%s <num_retangulos>\n", argv[0]);
        exit(0);
    }


    nr = atof(argv[1]);

    pipe(p);
    pid = fork();

    if (pid > 0) {
        pid = fork();
        if (pid > 0) {
            if (pid > 0) {
                close(p[1]);
                close(p1[1]);
                close(p2[1]);
                pi = calcula_pi(nr,4,4);
                read(p[0], &aux, sizeof(double));
                pi += aux;
                read(p1[0], &aux, sizeof(double));
                pi += aux;
                read(p2[0], &aux, sizeof(double));
                pi += aux;
                close(p[0]);
                close(p1[0]);
                close(p2[0]);
                printf("Pi = %.15f\n",pi);
            } else {
                close(p[0]);
                close(p[1]);
                close(p2[0]);
                close(p1[0]);
                close(p1[1]);
                pi = calcula_pi(nr,3,4);
                write(p2[1], &pi, sizeof(double));
                close(p2[1]);
            }
        } else {
            close(p[0]);
            close(p[1]);
            close(p1[0]);
            close(p2[0]);
            close(p2[1]);
            pi = calcula_pi(nr,2,4);
            write(p1[1], &pi, sizeof(double));
            close(p1[1]);
        }
    } else {
        close(p[0]);
        close(p1[0]);
        close(p2[0]);
        close(p1[1]);
        close(p2[1]);
        pi = calcula_pi(nr,1,4);
        write(p[1], &pi, sizeof(double));
        close(p[1]);
    }

}
/*---------------------------------------------------------------------*/
