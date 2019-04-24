#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/msg.h>

struct msg{
    long tipo;
    double p;
};
typedef struct msg MSG;

/*---------------------------------------------------------------------*/
double calcula_pi(double nr, double inicio, double salto){
    double x, base, altura;
    double i, pi;

    base = 1.0 / nr;
    pi = 0.0;

    for( i=inicio ; i<=nr ;  i+= salto) {
        x = base * (( double)i - 0.5);
        altura = 4.0 / (1.0 + x*x);
        pi += base * altura;
    }

    return pi;


}
/*---------------------------------------------------------------------*/
int main(int argc, char **argv){

    double pi = 0;
    int pid, msgid;
    double nr;
    double i;
    MSG m;
    int chave = 7;

    if ( argc != 2 ){
        printf("%s <num_retangulos>\n", argv[0]);
        exit(0);
    }

    nr = atof(argv[1]);
    msgid = msgget(chave, 0600 | IPC_CREAT);

    pid = fork();
    if (pid > 0) {

        pi = calcula_pi(nr,2,2);
        msgrcv(msgid, &m, sizeof(m.p),0,0);
        pi += m.p;
        printf("PI= %.15f\n", pi);
        msgctl(msgid, IPC_RMID, 0);

    } else {
        m.tipo = 1;
        m.p = calcula_pi(nr, 1, 2);
        msgsnd(msgid, &m, sizeof(m.p), 0);

    }
}
/*---------------------------------------------------------------------*/
