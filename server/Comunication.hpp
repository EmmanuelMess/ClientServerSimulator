#ifndef SERVER_COMUNICATION_HPP
#define SERVER_COMUNICATION_HPP

#include <QThread>
#include "RenderArea.hpp"

class CommunicationThread : public QThread
{
Q_OBJECT

public:
	explicit CommunicationThread(QObject *parent = nullptr);
	void run() override;

signals:
	void resultReady(const std::vector<std::vector<RenderArea::Color>> &s);

private:
	static const int port = 5000;
	int socketServer{};

	void createServer();
	void destroyServer();
};

QDebug operator<<(QDebug dbg, const std::vector<std::vector<RenderArea::Color>> &display);

#endif //SERVER_COMUNICATION_HPP
