
//  server_chat.c

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>           /* 'atoi'を利用するために定義 */
#include <time.h>             /* time関数 */

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         100
#define QUEUE_SIZE          5


int main(int argc, char* argv[])
{
  int hSocket,hServerSocket;   /* handle to socket */
  struct hostent* pHostInfo;   /* holds info about a machine */
  struct sockaddr_in Address;  /* Internet socket address stuct */
  int nAddressSize=sizeof(struct sockaddr_in);  /* Address size*/
  char pBuffer[BUFFER_SIZE];   /* 文字列格納場所 */
  int nHostPort;               /* ポート番号を格納する変数 */
  
  time_t timer;                /* time_t型に指定 */
  struct tm *date;             /* 時間を表す構造体 struct tm を定義 */
  char str[100];               /* 時刻等を格納する変数 */
  char str2[50];               /* コメントを格納する変数 */
  
  if(argc < 2) {
    printf("\nUsage: server host-port\n");
    return 0;
  } else {
    /* 文字列で表現された数値をint型の数値に変換(ポート番号が格納) */
    nHostPort=atoi(argv[1]);
  }
  
  printf("\nStarting server");
  printf("\nMaking socket"); /* make a socket */
  hServerSocket=socket(AF_INET,SOCK_STREAM,0);
  if(hServerSocket == SOCKET_ERROR) {
    printf("\nCould not make a socket\n");
    return 0;
  }
  
  Address.sin_addr.s_addr=INADDR_ANY; /* fill address struct */
  Address.sin_port=htons(nHostPort);  /* ポート番号 */
  Address.sin_family=AF_INET;
  
  printf("\nBinding to port %d",nHostPort);    /* bind to a port */
  if(bind(hServerSocket,(struct sockaddr*)&Address,sizeof(Address)) == SOCKET_ERROR) {
    printf("\nCould not connect to host\n");
    return 0;
  }
  
  /* getsocknameの第３引数をint型からsocklen_t型に変更 */
  getsockname( hServerSocket, (struct sockaddr *) &Address,(socklen_t *)&nAddressSize); /*  get port number */
  printf("opened socket as fd (%d) on port (%d) for stream i/o\n",hServerSocket, ntohs(Address.sin_port));
  printf("Server\n  sin_family = %d\n sin_addr.s_addr = %d\n sin_port = %d\n"
         ,Address.sin_family, Address.sin_addr.s_addr, ntohs(Address.sin_port));
  
  printf("\nMaking a listen queue of %d elements",QUEUE_SIZE); /* establish listen queue */
  if(listen(hServerSocket,QUEUE_SIZE) == SOCKET_ERROR) {
    printf("\nCould not listen\n");
    return 0;
  }
  
  for(;;) {
    printf("\nWaiting for a connection\n");  /* get the connected socket */
    /* accept関数の第３引数をsocklen_t型と定義 */
    hSocket=accept(hServerSocket,(struct sockaddr*)&Address,(socklen_t *)&nAddressSize);
    
    printf("\nGot a connection");
    
    timer=time(NULL);          /* timerに経過時刻を取得 */
    date=localtime(&timer);    /* 経過時刻を表す構造体に変換 */

    // 変数dateに格納している時刻を、指定の表現方法に従って書式化し,文字列strに書き込む。
    strftime(str, 63, "%Y, %B, %d, %A %p%I:%M:%S\n", date);
    
    /* コメントを送る */
    printf("\n文字を入力してください");
    printf("\n>");
    scanf("%s", str2);
    
    // 文字列の追加
    strcat(str, str2);
    /* str の値を pBuffer にコピー */
    strcpy(pBuffer,str);
    
    printf("\nSending \"%s\" to client",str);
    
    write(hSocket,pBuffer,strlen(pBuffer)+1); /*write what we received back to the server */
    read(hSocket,pBuffer,BUFFER_SIZE);  /* read from socket into buffer */
    
    // 文字列の比較
    if(strcmp(pBuffer,str) == 0)
      printf("\nThe messages match");
    else
      printf("\nSomething was changed in the message");
    
    if(close(hSocket) == SOCKET_ERROR) {
      printf("\nCould not close socket\n");
      return 0;
    }
    
    printf("\nClosing the socket"); /* close socket */
    return 0;
  }
}
