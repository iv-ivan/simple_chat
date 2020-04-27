//
//  common.hpp
//  chat
//
//  Created by Ivan Ivashkovskii on 27/04/2020.
//  Copyright © 2020 Ivan Ivashkovskii. All rights reserved.
//

#ifndef common_hpp
#define common_hpp

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

int createSocket(size_t port, bool isServer);

std::string_view nextTok(std::string_view& s, const char del);

#endif /* common_hpp */
