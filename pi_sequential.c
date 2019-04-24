#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*---------------------------------------------------------------------*/
double calcula_pi(double nr){
    double x, base, altura;
    double i, pi;

    base = 1.0 / nr;
    pi = 0.0;

    for( i=1 ; i<=nr ;  i++ ) {
        x = base * (( double)i - 0.5);
        altura = 4.0 / (1.0 + x*x);
        pi += base * altura;
    }

    return pi;


}
/*---------------------------------------------------------------------*/
int main(int argc, char **argv){

    double pi = 0;

    double nr;
    double i;

    if ( argc != 2 ){
        printf("%s <num_retangulos>\n", argv[0]);
        exit(0);
    }


    nr = atof(argv[1]);

    pi = calcula_pi(nr);

    printf("Pi = %.15f\n",pi);
}
/*---------------------------------------------------------------------*/
