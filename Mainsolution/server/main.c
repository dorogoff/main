#include <sys/types.h>
#include <winsock2.h>
#include <stdio.h>
#include <errno.h>
#include <string.h> 


#pragma comment(lib, "Ws2_32.lib")

int main( void )
{


	struct sockaddr_in local;
	int s;
	int s1;
	int rc;
	char buf[ 1 ];
	char str;
	int err;
	WSADATA wsa_data;
	err = WSAStartup (MAKEWORD(2,2), &wsa_data);

	local.sin_family = AF_INET;
	local.sin_port = htons( 5555 );
	local.sin_addr.s_addr = htonl( INADDR_ANY );
	s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( s < 0 ){
		perror( "socket call failed" );
		exit( 1 );
	}

	rc = bind( s, ( struct sockaddr * )&local, sizeof( local ) );
	if ( rc < 0 ){
		perror( "bind call failure" );
		exit( 1 );
	}
	
	while(1){
		// слушаем сокет
		rc = listen( s, 5 );
		if ( rc ){
			perror( "listen call failed" );
			exit( 1 );
		}
		// принимаем соединение
		s1 = accept( s, NULL, NULL );
		if ( s1 < 0 ){
			perror( "accept call failed" );
			exit( 1 );
		}
		else{
			// соединение устновлено

			//rc = send( s1, "OK!", 3, 0 );
			printf("Connect!\n");
		}

		//обработка входящих сообщений
		// прием 27, отправка 80
		// прием адреса младший байт
		// прием адреса старший байт
		// прием кода команды

		while(1){
		// прием 27
			rc = recv( s1, buf, 1, 0 );
			printf( "\nesc: %c", buf[ 0 ] );
		// выдача кода 80
			str=0x80;
			rc = send(s1, &str, 1, 0);
		// прием адреса младший байт
			rc = recv( s1, buf, 1, 0 );
			printf( "\nlo addr: %c", buf[ 0 ] );
		// прием адреса старший байт
			rc = recv( s1, buf, 1, 0 );
			printf( "\nhi addr: %c", buf[ 0 ] );
		// прием кода команды
			rc = recv( s1, buf, 1, 0 );
			printf( "\ncommand: %c", buf[ 0 ] );
		// отправка результата
			str=0xFC;
			rc = send(s1, &str, 1, 0);
			break;

		}
	}
	exit( 0 );
}
