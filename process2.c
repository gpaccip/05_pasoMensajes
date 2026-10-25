#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main() {
    int msqid;
    key_t key = 5678;
    message_buf buf;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget - process2");
        exit(1);
    }

    // 1. Recibir: "Are you hearing me?" (mtype = 1)
    if (msgrcv(msqid, &buf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv - process2");
        exit(1);
    }
    printf("[Process 2] Recibido: \"%s\"\n", buf.mtext);

    // 2. Responder: "Loud and clear" (mtype = 2)
    buf.mtype = 2;
    strcpy(buf.mtext, "Loud and clear");
    printf("[Process 2] Enviando: \"%s\"\n", buf.mtext);
    msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0);

    // 3. Recibir réplica: "I can hear you too" (mtype = 3)
    if (msgrcv(msqid, &buf, MSGSZ, 3, 0) < 0) {
        perror("msgrcv - process2");
        exit(1);
    }
    printf("[Process 2] Recibido: \"%s\"\n", buf.mtext);

    msgctl(msqid, IPC_RMID, NULL);
    exit(0);
}
