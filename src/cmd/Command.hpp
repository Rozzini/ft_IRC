/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraspors <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 11:09:42 by alalmazr          #+#    #+#             */
/*   Updated: 2023/06/25 17:55:17 by mraspors         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../server/Server.hpp"
#include "../server/Client.hpp"
#include <numeric>
#include <string>
#include <vector>

class Client;
class Command;
class Server;

class Command
{
protected:
    Server* serv;
    std::vector<std::string> args;
    std::string cmd;

    bool is_valid_mode(char c);

public:
    explicit Command(Server* server);
    virtual ~Command();
    virtual void execute(Client* client, std::vector<std::string> args) = 0;
};

class PASS : public Command
{
public:
    PASS(Server* server);
    void execute(Client* client, std::vector<std::string> args);
    ~PASS();
};

class NICK : public Command
{
public:
    NICK(Server* server);
    void execute(Client* client, std::vector<std::string> args);
    ~NICK();
};

class USER : public Command
{
public:
    USER(Server* server);
    void execute(Client* client, std::vector<std::string> args);
    ~USER();
};

class PM : public Command
{
public:
    PM(Server* server);
    void execute(Client* client, std::vector<std::string> args);
    ~PM();
};

class QUIT : public Command
{
public:
    QUIT(Server* server);
    void execute(Client* client, std::vector<std::string> args);
    ~QUIT();
};

class JOIN : public Command
{
public:
    JOIN(Server* server);
    void execute(Client* client, std::vector<std::string> args);
    ~JOIN();
};

class KICK : public Command
{
public:
    KICK(Server* server);
    void execute(Client* client, std::vector<std::string> args);
    ~KICK();
};

class KILL : public Command
{
public:
    KILL(Server* server);
    void execute(Client* client, std::vector<std::string> args);
    ~KILL();
};

class TOPIC : public Command
{
	public:
	TOPIC(Server *server);
	void execute(Client *client, std::vector<std::string> args);
    ~TOPIC();
};

class MODE : public Command
{
public:
    MODE(Server* server);
    void execute(Client* client, std::vector<std::string> args);
    ~MODE();
};

class INVITE : public Command
{
public:
    INVITE(Server* server);
    void execute(Client* client, std::vector<std::string> args);
    ~INVITE();
};

#endif