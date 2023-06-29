/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraspors <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:10:46 by alalmazr          #+#    #+#             */
/*   Updated: 2023/06/30 02:47:13 by mraspors         ###   ########.fr       */
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
	this->channels.push_back(channel);
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

std::vector<Channel *> Client::get_channels()
{
	return this->channels;
}

// send msd
void Client::write(const std::string &msg) const
{
    std::string buff = msg + "\r\n";
    if (send(fd, buff.c_str(), buff.length(), 0) < 0)
        throw std::runtime_error("Error while sending a message to a client!");
}

// Reply to the client
void Client::reply(const std::string &msg)
{
	this->write(":" + get_prefix() + " " + msg);
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
	set_channel(channel);

	// Get users on the channel
	std::string users = "";
	std::vector<Client *> clients = channel->getClients();
	for (size_t i = 0; i < clients.size(); i++)
	{
		users.append(clients[i]->get_nick() + " ");
	}
	// Send replies

	reply(NAMREPLY(get_nick(), channel->getName(), users));
    reply(ENDOFNAMES(get_nick(), channel->getName()));
    channel->broadcast(RPLY_JOIN(get_prefix(), channel->getName()));
}

void Client::leaveChannel(Channel *channel)
{
	 if (!channel)
        return;

    leaveChannel(channel->getName());
}

std::string Client::get_prefix() const
{
	std::string username = uname.empty() ? "" : "!" + uname;
	std::string _host = host.empty() ? "" : "@" + this->host;

	std::string result = nick + username + _host;
	return result;
}

//typedef std::vector<Channel *>::iterator    channel_iterator;
//std::vector<Channel *> get_channels();

void Client::leaveChannel(std::string chName)
{
	std::vector<Channel *> channels = get_channels();
	channel_iterator it = channels.begin();
	channel_iterator end = channels.end();
	Channel *channel = NULL;
	
	while (it != end)
	{
		if (!chName.compare((*it)->getName()))
			channel = *it;
		it++;
	}
	const std::string name = channel->getName();

    channel->broadcast(RPLY_PART(get_prefix(), channel->getName()));
	channel->removeClient(this);
	if (channel)
		channels.erase(it);
}

bool Client::isInChannel(std::string chName)
{
	std::vector<Channel *> channels = get_channels();
	channel_iterator it = channels.begin();
	channel_iterator end = channels.end();

	while (it != end)
	{
		if (!chName.compare((*it)->getName()))
			return true;
		it++;
	}
	
	return false;
}

void Client::leaveAllChannels()
{
	std::vector<Channel *> channels = get_channels();

	while (channels.size() > 0)
	{
		leaveChannel(channels.front()->getName());
		channels = get_channels();
	}
}