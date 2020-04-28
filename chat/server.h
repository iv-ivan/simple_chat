//
//  server.hpp
//  chat
//
//  Created by Ivan Ivashkovskii on 27/04/2020.
//  Copyright © 2020 Ivan Ivashkovskii. All rights reserved.
//

#ifndef server_hpp
#define server_hpp

#include <iostream>
#include <unordered_map>
#include <vector>
#include <future>
#include <mutex>

class TServer {
public:
    TServer(size_t port): Port(port) {
    }
    
    void Run();
private:
    void ProcessMessage(int sock);
    void ProcessCommand(const std::string& commandMessage, int sock);

    void RegisterClient(const std::string& name, int sock);
    void SendMessage(const std::string& senderName, const std::string& receiverName, const std::string& message);
    void FetchNewMessages(const std::string& name);
private:
    size_t Port;
    
    using TSocket = int;
    std::unordered_map<TSocket, std::future<void>> ClientConnections;
    std::unordered_map<std::string, TSocket> Clients;
    std::mutex ClientsMutex;
    
    using THistory = std::vector<std::string>;
    std::unordered_map<std::string, THistory> MessageQueue;
};

#endif /* server_hpp */
