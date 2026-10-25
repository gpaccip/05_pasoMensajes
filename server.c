#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    long client_id;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 9999;
    message_buf request, response;

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget - server");
        exit(1);
    }

    printf("[Servidor] Iniciado. Esperando peticiones de clientes...\n");

    for (int i = 0; i < 2; i++) {
        if (msgrcv(msqid, &request, sizeof(message_buf) - sizeof(long), 1, 0) < 0) {
            perror("msgrcv - server");
            exit(1);
        }

        printf("[Servidor] Peticion recibida del Cliente %ld: \"%s\"\n", request.client_id, request.mtext);

        response.mtype = request.client_id;
        response.client_id = request.client_id;
        sprintf(response.mtext, "Hola Cliente %ld, tu peticion fue procesada con exito.", request.client_id);

        if (msgsnd(msqid, &response, sizeof(message_buf) - sizeof(long), 0) < 0) {
            perror("msgsnd - server");
            exit(1);
        }
        printf("[Servidor] Respuesta privada enviada a mtype=%ld\n", response.mtype);
    }

    printf("[Servidor] Finalizando y eliminando la cola IPC.\n");
    msgctl(msqid, IPC_RMID, NULL);
    exit(0);
}
