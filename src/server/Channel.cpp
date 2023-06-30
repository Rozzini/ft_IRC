/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraspors <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:41:28 by dkaratae          #+#    #+#             */
/*   Updated: 2023/06/30 21:13:09 by mraspors         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <cstdio>
#include <string>

Channel::Channel (std::string channelName, std::string key, Client *client)
{
    this->name = channelName;
    this->topic = "";
    this->key = key;
    this->inviteOnly = false;
    this->userLimit = -1;
    this->boolTopic = false;
    this->setOperator(client, true);
}

Channel::~Channel() {}

bool Channel::check_mode(char c)
{
    std::string ops = "itkol";
    unsigned long n;
    n = ops.find(c);
    if (n == std::string::npos)
    {
        return false;
    }
    n = this->modes.find(c);
    if (n == std::string::npos)
    {
        return false;
    }
    return true;
}

void Channel::setModeI(char sign)
{
    if (sign == '+')
    {
        inviteOnly = true;
        std::cout << "This Channel only Invite ticket!" << std::endl;
    }
    else if (sign == '-')
    {
        inviteOnly = false;
        std::cout << "Delete only Invite!" << std::endl;
    }
}
void Channel::setModeT(char sign)
{
    if (sign == '+')
    {
        boolTopic = true;
        std::cout << "boolTopic = true;" << std::endl;
    }
    else if (sign == '-')
    {
        boolTopic = false;
        std::cout << "boolTopic = false;" << std::endl;
    }
}

void Channel::setModeK(char sign, std::string key)
{
    if (sign == '+')
    {
        this->key = key;
		this->operators.front()->reply(RPLY_CHAN_PW_SET(this->getName()));
    }
    else if (sign == '-')
    {
        key = "";
        this->operators.front()->reply(RPLY_CHAN_PW_UNSET(this->getName()));
    }
}

void Channel::setModeO(char sign, Client *client)
{
    if (sign == '+')
    {
        setOperator(client, true);
		client->reply(RPLY_USR_IS_OP(client->get_nick(), this->getName()));
    }
    else if (sign == '-')
    {
        setOperator(client, false);
        client->reply(RPLY_USR_NOT_OP(client->get_nick(), this->getName()));
    }
}

void Channel::setModeL(char sign, int limit)
{
    if (sign == '+')
    {
        if (limit < getCountClients())
           this->operators.front()->reply(RPLY_TOO_MUCH_LIMIT(this->getName()));
        else
        {
            setUserLimit(limit);
          this->operators.front()->reply(RPLY_LIMIT_SET(this->getName()));
        }
    }
    else if (sign == '-')
    {
        setUserLimit(-1);
      this->operators.front()->reply(RPLY_LIMIT_UNSET(this->getName()));
    }
}


void Channel::setName(std::string name)
{
    this->name = name;
}

std::string Channel::getName()
{
    return (this->name);
}

std::vector<Client *> Channel::getOperators()
{
    return (this->operators);
}

std::string Channel::getMode()
{
    return mode;
}

std::vector<Client *> Channel::getClients()
{
    return (this->clients);
}
    
void Channel::setTopic(std::string topic)
{
    if (boolTopic)
        this->topic = topic;
}

std::string Channel::getTopic()
{
    return this->topic;
}

void Channel::setKey(std::string key)
{
    this->key = key;
}

std::string Channel::getKey()
{
    return this->key;
}

int Channel::getCountClients()
{
    int countClients = 0;

    for (unsigned long i = 0; i < this->clients.size(); i++)
        countClients++;
    
    return countClients;
}

bool Channel::getInviteOnly()
{
    return this->inviteOnly;
}

bool Channel::getBoolTopic()
{
    return boolTopic;
}

void Channel::setInviteOnly(bool inviteOnly)
{
    this->inviteOnly = inviteOnly;
}

void Channel::setOperator(Client *client, bool isOperator)
{
    if (isOperator)
    {
        this->operators.push_back(client);
    }
    else
    {
        for (unsigned long i = 0; i < this->operators.size(); i++)
        {
            if (this->operators[i]->get_nick() == client->get_nick())
            {
                this->operators.erase(this->operators.begin() + i);
                break;
            }
        }
    }
}

void Channel::setInviteList(Client *client)
{
    this->inviteList.push_back(client);
}

bool Channel::isOperator(std::string nickName)
{
    for (unsigned long i = 0; i < this->operators.size(); i++)
    {
        if (this->operators[i]->get_nick() == nickName)
            return true;
    }
    return false;
}

void Channel::setUserLimit(int limit)
{
    this->userLimit = limit;
}

int Channel::getUserLimit()
{
    return this->userLimit;
}

void Channel::addClient(Client *client)
{
    this->clients.push_back(client);
   client->reply(RPLY_USER_ADDED(client->get_nick(), this->getName()));
}

void Channel::removeClient(Client *client)
{
    for (unsigned long i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i]->get_nick() == client->get_nick())
        {
            this->clients.erase(this->clients.begin() + i);
            this->broadcast(RPLY_KICK(client->get_prefix(), "ch", client->get_nick(), "no reason"));
            break;
        }
    }
}

bool Channel::isClientInChannel(Client *client)
{
    for (unsigned long i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i]->get_nick() == client->get_nick())
            return true;
    }
    return false;
}

void Channel::broadcast(const std::string& message)
{
    std::vector<Client *> clients = getClients();
    client_iterator it_b = clients.begin();
    client_iterator it_e = clients.end();

    while (it_b != it_e)
    {
        (*it_b)->write(message);
        it_b++;
    }
}

void Channel::broadcast(const std::string& message, Client* exclude)
{
    std::vector<Client *> clients = getClients();
    client_iterator it_b = clients.begin();
    client_iterator it_e = clients.end();

    while (it_b != it_e)
    {
        if (*it_b == exclude)
        {
            it_b++;
            continue;
        }

        (*it_b)->write(message);
        it_b++;
    }
}

void Channel::popInivte(Client *client)
{
    client_iterator it_b = this->inviteList.begin();
    client_iterator it_e = this->inviteList.end();
    Client *tmp = NULL;
    int i = 0;
    while (it_b != it_e)
    {
        if (client->get_nick().compare((*it_b)->get_nick()) == 0)
        {
            tmp = *it_b;
            break;
        };
        it_b++;
        i++;
    }
    if (tmp)
	{
		if (this->inviteList.size() > 1)
			this->inviteList.erase(this->inviteList.begin() + i);
		else
			this->inviteList.clear();
	}
}

bool Channel::isInvited(Client *client)
{
    client_iterator it_b = this->inviteList.begin();
    client_iterator it_e = this->inviteList.end();

    while (it_b != it_e)
    {
        if (client->get_nick().compare((*it_b)->get_nick()) == 0)
            return true;
        it_b++;
    }
    return false;
}