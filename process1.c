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

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget - process1");
        exit(1);
    }

    buf.mtype = 1;
    strcpy(buf.mtext, "Are you hearing me?");
    printf("[Process 1] Enviando: \"%s\"\n", buf.mtext);
    msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0);

    if (msgrcv(msqid, &buf, MSGSZ, 2, 0) < 0) {
        perror("msgrcv - process1");
        exit(1);
    }
    printf("[Process 1] Recibido: \"%s\"\n", buf.mtext);

    buf.mtype = 3;
    strcpy(buf.mtext, "I can hear you too");
    printf("[Process 1] Enviando: \"%s\"\n", buf.mtext);
    msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0);

    exit(0);
}
