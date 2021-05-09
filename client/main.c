#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdbool.h>

static const int port = 5000;

int connectServer() {
	int sock_cli = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_cli == -1){
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in srv_nombre = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = htons(port)
	};

	if(connect(sock_cli, (struct sockaddr *) &srv_nombre, sizeof(struct sockaddr_un)) == -1){
		exit(EXIT_FAILURE);
	}

	return sock_cli;
}

#define AREA_WIDTH 30
#define AREA_HEIGHT 30

struct message {
	char data[AREA_WIDTH][AREA_HEIGHT];
};

void sendToServer(char **data) {
	int sock_cli = connectServer();

    struct message msg = {0};

	for (int i = 0; i < AREA_WIDTH; ++i) {
		for (int j = 0; j < AREA_HEIGHT; ++j) {
			msg.data[i][j] = data[i][j];
		}
	}

	if(send(sock_cli, &msg, sizeof(struct message), 0) == -1){
		exit(EXIT_FAILURE);
	}

	bool recieved = false;
	if(recv(sock_cli, &recieved, sizeof(bool), 0) == -1){
		exit(EXIT_FAILURE);
	}

	close(sock_cli);
}

enum Colors {
	BLACK = 0,
	WHITE = 1,
	BLUE = 2,
	GREEN = 3,
	RED = 4
};

char ** update(char **simulation) {
	char **simulationNext = calloc(AREA_WIDTH, sizeof(char*));
	for (int i = 0; i < AREA_WIDTH; ++i) {
		simulationNext[i] = calloc(AREA_HEIGHT, sizeof(char));
	}

	simulation[5][5] = 2;

	for (int x = 0; x < AREA_WIDTH; ++x) {
		for (int y = 0; y < AREA_HEIGHT; ++y) {
			if(simulation[x][y] == BLUE) {
				int nextX = x;
				int nextY = y;

				if(simulation[x][y + 1] == BLACK) {
					nextX = x;
				} else if(x - 1 >= 0 && simulation[x - 1][y + 1] == BLACK) {
					nextX = x - 1;
				} else if(x + 1 < AREA_WIDTH && simulation[x + 1][y + 1] == BLACK) {
					nextX = x + 1;
				}

				if(y + 1 < AREA_WIDTH && simulation[x][y + 1] == BLACK) {
					nextY = y + 1;
				}

				simulationNext[x][y] = BLACK;
				simulationNext[nextX][nextY] =  BLUE;
			}
		}
	}

	return simulationNext;
}

int main(){
	char **simulation = calloc(AREA_WIDTH, sizeof(char*));
	for (int i = 0; i < AREA_WIDTH; ++i) {
		simulation[i] = calloc(AREA_HEIGHT, sizeof(char));
	}

	simulation[5][5] = 2;

	while(true) {
		char ** nextSimulation = update(simulation);
		sendToServer(nextSimulation);

		for (int i = 0; i < AREA_WIDTH; ++i) {
			free(simulation[i]);
		}
		free(simulation);

		simulation = nextSimulation;
	}
}