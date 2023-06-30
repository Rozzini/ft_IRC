/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkaratae <dkaratae@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 18:03:16 by mraspors          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/06/30 17:46:16 by dkaratae         ###   ########.fr       */
=======
/*   Updated: 2023/06/30 16:39:23 by mraspors         ###   ########.fr       */
>>>>>>> 56442938e37bdf533f977a1cd853a581316df28f
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "../server/Server.hpp"
#include <cstddef>

class Client;
// base class

Command::Command(Server *server, bool auth) : serv(server), auth(auth) {}

void Command::execute(Client *client, std::vector<std::string> args)
{
	(void)client;
	(void)args;
}

bool    Command::need_auth() const
{
	return this->auth; 
}

// constructors
JOIN::JOIN(Server *server) : Command(server) {}
TOPIC::TOPIC(Server *server) : Command(server) {}
MODE::MODE(Server *server) : Command(server) {}
INVITE::INVITE(Server *server) : Command(server) {}
KILL::KILL(Server *server) : Command(server) {}
KICK::KICK(Server *server) : Command(server) {}
PM::PM(Server *server) : Command(server) {}
USER::USER(Server *server, bool auth) : Command(server, auth) {}
NICK::NICK(Server *server, bool auth) : Command(server, auth) {}
PASS::PASS(Server *server, bool auth) : Command(server, auth) {}
QUIT::QUIT(Server *server, bool auth) : Command(server, auth) {}

// child class

// PASS <password>
void PASS::execute(Client *client, std::vector<std::string> args)
{
	if (args.empty())
	{
		client->reply(ERR_MOREPARAMS(client->get_nick(), "PASS"));
		std::cout << "MORE PARAMS PLS" << std::endl;
		return;
	}

	if (client->get_state() == 2) //== registered )
	{
		client->reply(ERR_ALREADY_REG(client->get_nick()));
		std::cout << "ALREADY REGISTERED" << std::endl;
		return;
	}

	std::string password = args[0];
	if (password[0] == ':')
		password = password.substr(1);

	if (serv->get_password() != password)
	{
		client->reply(ERR_PW(client->get_nick()));
		std::cout << "WRONG SERV PASS" << std::endl;
		return;
	}

	client->set_state(1); // 1 = registered
	std::cout << "LOGED IN" << std::endl;
}

// NICK <nickname>
void NICK::execute(Client *client, std::vector<std::string> args)
{
	if (args.empty() || args[0].empty())
	{
		client->reply(ERR_NO_NICK(client->get_nick()));
		std::cout << "MORE PARAMS PLS" << std::endl;
		return;
	}

	std::string nickname = args[0];
	if (serv->get_client(nickname))
	{
		client->reply(ERR_NICK_USED(client->get_nick()));
		return;
	}
	client->set_nick(nickname);
	client->welcome();
}

//  USER <username> <hostname> <servername> <realname>
void USER::execute(Client *client, std::vector<std::string> args)
{
	if (client->get_state() == 2)
	{
		client->reply(ERR_ALREADY_REG(client->get_nick()));
		std::cout << "REGISTERED" << std::endl;
		return;
	}
	if (args.size() < 4)
	{
		client->reply(ERR_MOREPARAMS(client->get_nick(), "USER"));
		std::cout << "MORE PARAMS PLS" << std::endl;
		return;
	}
	client->set_uname(args[0]);
	client->set_name(args[3]);
	client->welcome();
}

// PRIVMSG <msgtarget> :<message>
void PM::execute(Client *client, std::vector<std::string> args)
{
	if (args.size() < 2 || args[0].empty() || args[1].empty())
	{
		client->reply(ERR_MOREPARAMS(client->get_nick(), "PRIVMSG"));
		return;
	}
	std::string target = args[0];
	
	std::string message;
	std::vector<std::string>::iterator it = args.begin() + 1;
    std::vector<std::string>::iterator end = args.end();
    while (it != end)
    {
        message.append(*it + " ");
        it++;
    }
	if (message.at(0) == ':')
        message = message.substr(1);

    if (target.at(0) == '#')//channel notice
    {
		std::cout << "AAA1" << std::endl;
		Channel *channel =  serv->get_channel(target.substr(1, target.size()));
		std::cout << "AAA2" << std::endl;
        if (!client->isInChannel(channel->getName()))
        {
			std::cout << "ERR" << std::endl;
            client->reply(ERR_CHAN(client->get_nick(), target));
			return;
        }
		std::cout << "AAA3" << std::endl;
        channel->broadcast(RPLY_PM(client->get_prefix(), target, message), client);
		std::cout << "AAA4" << std::endl;
        return;
    }
    // else if notice is for a client
    Client  *dest = serv->get_client(target);
    if (!dest)
    {
        client->reply(ERR_NO_EXIST(client->get_nick(), target));
		return;
    }
    dest->write(RPLY_PM(client->get_prefix(), target, message));
}


// QUIT [<message>]
void QUIT::execute(Client *client, std::vector<std::string> args)
{
	// if args empty set reason to leaving, if not empty check at [0][0] if ':' then take string without first char
	std::string reason = args.empty() ? "leaving!!!" : (args.at(0)[0] == ':' ? args.at(0).substr(1) : args.at(0));
	client->write("leaving because " + reason);
	serv->on_client_disconnect(client->get_fd());
}

// JOIN <channels> [<keys>]
void JOIN::execute(Client *client, std::vector<std::string> args)
{
	if (args.empty())
	{
		client->reply(ERR_MOREPARAMS(client->get_nick(), "JOIN"));
		return;
	}
	std::string name = args[0];
	std::string pass = args.size() > 1 ? args[1] : "";

	if (client->isInChannel(name))
	{
		client->reply(ERR_ALREADYJOINEDCHANNEL(client->get_nick(), name));
		return;
	}

	Channel *channel = serv->get_channel(name);
	if (!channel)
		channel = serv->create_channel(name, pass, client);

	// checks channel has a user limit and if current number of users is more than limit
	if (channel->getUserLimit() > 0 && channel->getCountClients() >= channel->getUserLimit())
	{
		client->reply(ERR_CHANNEL_FULL(client->get_nick(), name));
		return;
	}
	if (channel->getKey() != pass)
	{
		client->reply(ERR_CHANNELKEY(client->get_nick(), name));
		return;
	}
	if (channel->getMode() == "+i")
	{
		client->reply(ERR_INV_ONLY_CH(channel->getName()));
	}

	client->join(channel);
}

void KILL::execute(Client *client, std::vector<std::string> args)
{
	(void)client;
	(void)args;
}

// destructors 
Command::~Command() {}
PASS::~PASS() {}
INVITE::~INVITE() {}
NICK::~NICK() {}
USER::~USER() {}
PM::~PM() {}
QUIT::~QUIT() {}
JOIN::~JOIN() {}
KICK::~KICK() {}
TOPIC::~TOPIC() {}
MODE::~MODE() {}
KILL::~KILL() {}