/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraspors <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:10:46 by alalmazr          #+#    #+#             */
/*   Updated: 2023/06/17 14:47:38 by mraspors         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// Default const
Client::Client()
{}

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

//parametirized const
Client::Client(int fd, int port, const std::string &host)
	: fd(fd), port(port), host(host)
{
	state = 0; // handhshake state
}

// destructor
Client::~Client()
{}

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

// GETTERS
int Client::get_fd() const
{
	return fd;
}

int Client::get_port() const
{
	return port;
}

int Client::get_state()
{
	return state;
}

std::string Client::get_nick() const
{
	return nick;
}

std::string Client::get_uname() const
{
	return uname;
}

std::string Client::get_name() const
{
	return name;
}

std::string Client::get_host() const
{
	return host;
}

// send msd
void Client::send(const std::string &msg) const
{
	std::string a = msg;
	//implement send
}

// Reply to the client
void Client::reply(const std::string &msg)
{
	std::string a = msg;
	// implement reply
}

// irc welcome msg
void Client::welcome()
{
	// if _state is not 1 or uname is empty or name is empty or nick is empty:
    //     return

    // welcome msg
}