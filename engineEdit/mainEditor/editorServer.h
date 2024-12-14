#if !defined(EDITORSERVER_H)
#define EDITORSERVER_H
#include "editorCommand.h"
#include <stdio.h>
#include <thread>
#include <winsock2.h>
class EditorServer {
  private:
    int socketSv;
    int socketCl;
    int port = 9000;
    sockaddr_in addressSv;
    sockaddr_in addressCl;
    int addrClLen;
    char buffer[1024] = {0};
    bool bRun = true;

  public:
    static EditorServer server;
    EditorServer(/* args */);
    ~EditorServer();
    void connect();
    void loop();
    void close();
    void sendMesssage(const std::string &msg);
    std::thread *th;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// method//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline EditorServer EditorServer::server;
EditorServer::EditorServer(/* args */) {
    WSADATA data;
    int ret = WSAStartup(MAKEWORD(1, 1), &data);
    socketSv = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int opt = 1;
    setsockopt(socketSv, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt,
               sizeof opt);

    addressSv.sin_family = AF_INET;
    addressSv.sin_addr.s_addr = htonl(INADDR_ANY);
    addressSv.sin_port = htons(port);
    bind(socketSv, (sockaddr *)&addressSv, sizeof(addressSv));
    if (!listen(socketSv, 1))
        printf("listening port 9000\n");
    th = new std::thread(&EditorServer::loop, this);
}

EditorServer::~EditorServer() {
    closesocket(socketSv);
    closesocket(socketCl);
    th->join();
    delete th;
}

inline void EditorServer::connect() {
    addrClLen = sizeof(addressCl);
    socketCl = accept(socketSv, (sockaddr *)&addressCl, &addrClLen);
}

inline void EditorServer::loop() {
    connect();
    while (bRun) {
        int bytesRead = recv(socketCl, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            printf("recieved:%s\n", buffer);
            // const char *response = "server get message success";
            // send(socketCl, response, strlen(response), 0);
            sendMesssage("server get message success");
            editorCommand::edc.parseString(buffer);
            editorCommand::edc.executeCommand();
        } else if (bytesRead == 0) {
            printf("Client disconnected.\n");
            socketCl = accept(socketSv, (sockaddr *)&addressCl, &addrClLen);
        } else {
            printf("recv failed: %ld\n", WSAGetLastError());
            socketCl = accept(socketSv, (sockaddr *)&addressCl, &addrClLen);
        }
    }
}

inline void EditorServer::close() { bRun = false; }

inline void EditorServer::sendMesssage(const std::string &msg) {
    send(socketCl, msg.c_str(), strlen(msg.c_str()), 0);
}

#endif // EDITORSERVER_H
