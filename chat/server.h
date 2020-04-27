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

class TServer {
public:
    TServer(size_t port): Port(port) {
    }
    
    void Run();
private:
    void ProcessMessage(int sock);

    void RegisterClient(const std::string& name, size_t port);
    void SendMessage(const std::string& senderName, const std::string& receiverName, const std::string& message);
    void FetchNewMessages(const std::string& name);
private:
    size_t Port;
    
    using TPort = size_t;
    std::unordered_map<std::string, TPort> Clients;
    
    using THistory = std::vector<std::string>;
    std::unordered_map<std::string, THistory> MessageQueue;
};

#endif /* server_hpp */
