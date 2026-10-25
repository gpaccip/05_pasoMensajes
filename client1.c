#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128
#define CLIENT_ID 10

typedef struct msgbuf {
    long mtype;
    long client_id;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 9999;
    message_buf req, res;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget - client1");
        exit(1);
    }

    req.mtype = 1;
    req.client_id = CLIENT_ID;
    strcpy(req.mtext, "Solicitud de datos del Cliente 1");
    printf("[Cliente 1] Enviando peticion al Servidor...\n");
    msgsnd(msqid, &req, sizeof(message_buf) - sizeof(long), 0);

    if (msgrcv(msqid, &res, sizeof(message_buf) - sizeof(long), CLIENT_ID, 0) < 0) {
        perror("msgrcv - client1");
        exit(1);
    }

    printf("[Cliente 1] Respuesta PRIVADA recibida: \"%s\"\n", res.mtext);
    exit(0);
}
