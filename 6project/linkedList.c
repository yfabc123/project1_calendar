#include "mySocket.h" //需要转发消息所以导入
#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
LinkedList *head = NULL;
LinkedList *createLinkedList()
{
    head = (LinkedList *)malloc(sizeof(LinkedList));
    head->next = NULL;
    memset(&head->userPacket, 0, sizeof(head->userPacket));
    return head;
}
// 插入
int insertLinkedList(LinkedList *head, Packet *parrUserMessage)
{
    if (!parrUserMessage)
    {
        printf("参数为空!\n");
        return -1;
    }
    LinkedList *temp = (LinkedList *)malloc(sizeof(LinkedList));
    if (!temp)
    {
        perror("LinkedList *temp = (LinkedList *)malloc(sizeof(LinkedList));");
        return -1;
    }
    memcpy(&(temp->userPacket), parrUserMessage, sizeof(Packet));
    temp->next = head->next;
    head->next = temp;
    printf("已插入在线用户信息\n");
    return 1;
}
int deleteLinkedList(LinkedList *head, Packet *paraUserPacket)
{
    LinkedList *ptr = head;
    while (ptr->next)
    {
        if (strcmp(ptr->next->userPacket.user_name, paraUserPacket->user_name) == 0)
        {
            LinkedList *temp = ptr->next;
            ptr->next = temp->next;
            free(temp);
            printf("已删除下线用户信息\n");
            return 1;
        }
        ptr = ptr->next;
    }
    return -1;
}
// 子线程传递 自己的socket,接收到的用户包然后调用链表模块提供的转发函数
int sendAll(LinkedList *head, Packet *pUserPacket, int pchildSocket)
{
    Packet packet;
    memcpy(&packet, pUserPacket, sizeof(Packet));
    LinkedList *ptr = head;
    while (ptr->next)
    {
        mySendTo(pchildSocket, &packet, NULL, 0, sizeof(Packet), &(ptr->next->userPacket.addr));
        printf("%s(%s):%s\n", packet.user_name, packet.time, packet.message);
        ptr = ptr->next;
    }
}
