/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:10:46 by alalmazr          #+#    #+#             */
/*   Updated: 2023/06/29 14:31:18 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// Default const
Client::Client()
{
}

// cpy const
Client::Client(const Client &src)
{
	fd = src.fd;
	port = src.port;
	nick = src.nick;
	uname = src.uname;
	name = src.name;
	host = src.host;
	state = src.state;
}

// parametirized const
Client::Client(int fd, int port, const std::string &host)
	: fd(fd), port(port), host(host)
{
	state = 0; // handhshake state
}

// destructor
Client::~Client()
{
}

// SETTERS
void Client::set_state(int state)
{
	this->state = state;
}

void Client::set_nick(const std::string &nick)
{
	this->nick = nick;
}

void Client::set_uname(const std::string &uname)
{
	this->uname = uname;
}

void Client::set_name(const std::string &name)
{
	this->name = name;
}

void Client::set_channel(Channel *channel)
{
	this->channel = channel;
}

// GETTERS
int Client::get_fd() const
{
	return this->fd;
}

int Client::get_port() const
{
	return this->port;
}

int Client::get_state()
{
	return this->state;
}

std::string Client::get_nick() const
{
	return this->nick;
}

std::string Client::get_uname() const
{
	return this->uname;
}

std::string Client::get_name() const
{
	return this->name;
}

std::string Client::get_host() const
{
	return this->host;
}

Channel *Client::get_channel()
{
	return NULL;
}
// send msd
void Client::send(const std::string &msg) const
{
	std::string a = msg;
	// implement send
}

// Reply to the client
void Client::reply(const std::string &msg)
{
	this->write(":" + get_prefix() + " " + reply);
	this->write(":" + get_prefix() + " " + reply);
}

// irc welcome msg
void Client::welcome()
{
	if (state != 1 || uname.empty() || name.empty() || nick.empty())
		return;
	state = 2; // registered
	this->reply("welcome " + nick);
	char message[100];
	sprintf(message, "%s:%d is now known as %s.", host.c_str(), port, nick.c_str());
	log(message);
}

void Client::join(Channel *channel)
{
	channel->addClient(this);
	this->channel = channel;

	// Get users on the channel
	std::string users = "";
	std::vector<Client *> clients = channel->getClients();
	for (size_t i = 0; i < clients.size(); i++)
	{
		users.append(clients[i]->get_nick() + " ");
	}
	// Send replies

	reply("reply 1 for join");
	reply("reply 2 for join");
	// channel->(RPL_JOIN(get_prefix(), channel->get_name()));
	//we need BROADCAST FOR CHANNEL
	// log

	std::string message = nick + " has joined to the channel " + channel->get_name();
	log(message);
}

void Client::leave()
{
	 if (!channel)
        return;

    // const std::string name = channel->getName();

    // channel->broadcast(RPL_PART(get_prefix(), _channel->get_name()));
    channel->removeClient(this);

    std::string message = nick + " has left the channel " + channel->getName();
    log(message);
}

std::string Client::get_prefix() const
{
	std::string uname = uname.empty() ? "" : "!" + uname;
	std::string host = host.empty() ? "" : "@" + host;

	std::string result = nick + uname + host;
	return result;
}