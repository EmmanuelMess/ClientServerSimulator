#include <execution>
#include <sys/socket.h>
#include <netinet/in.h>
#include <QDebug>
#include "RenderArea.hpp"
#include "Comunication.hpp"

CommunicationThread::CommunicationThread(QObject *parent) : QThread(parent) {
	createServer();
}

struct message {
	char data[RenderArea::AREA_WIDTH][RenderArea::AREA_HEIGHT];
};

void CommunicationThread::createServer() {
	struct sockaddr_in serverName = {0};

	socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if(socketServer == -1){
		exit(EXIT_FAILURE);
	}

	serverName.sin_family = AF_INET;
	serverName.sin_addr.s_addr = INADDR_ANY;
	serverName.sin_port = htons(port);

	if(bind(socketServer, (struct sockaddr *) &serverName, sizeof(struct sockaddr_in)) == -1){
		exit(EXIT_FAILURE);
	}

	if(listen(socketServer, 1) == -1){
		exit(EXIT_FAILURE);
	}
}

void CommunicationThread::run() {
	while(true) {
		std::vector<std::vector<RenderArea::Color>> result;
		struct sockaddr_in clientName = {0};
		socklen_t size;

		int socketClient = accept(socketServer, (struct sockaddr *) &clientName, &size);
		if (socketClient == -1) {
			exit(EXIT_FAILURE);
		}

		struct message msg{};
		if (recv(socketClient, &msg, sizeof(struct message), 0) == -1) {
			exit(EXIT_FAILURE);
		}

		std::vector<std::vector<RenderArea::Color>> simulationNext(RenderArea::AREA_WIDTH,
		                                                           std::vector<RenderArea::Color>(
			                                                           RenderArea::AREA_HEIGHT));

		for (int x = 0; x < RenderArea::AREA_WIDTH; ++x) {
			for (int y = 0; y < RenderArea::AREA_HEIGHT; ++y) {
				simulationNext[x][y] = RenderArea::COLORS[msg.data[x][y]];
			}
		}

		bool recieved = true;

		if (send(socketClient, &recieved, sizeof(bool), 0) == -1) {
			exit(EXIT_FAILURE);
		}
		close(socketClient);

		emit resultReady(simulationNext);
		sleep(1);
	}
}

void CommunicationThread::destroyServer() {
	close(socketServer);
}

QDebug operator<<(QDebug dbg, const std::vector<std::vector<RenderArea::Color>> &display) {
	for (const auto & i : display) {
		for (auto j : i) {
			dbg.nospace() << j;
		}

		dbg << "\n";
	}

	return dbg.maybeSpace();
}
