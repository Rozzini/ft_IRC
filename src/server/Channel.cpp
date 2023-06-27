/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkaratae <dkaratae@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:41:28 by dkaratae          #+#    #+#             */
/*   Updated: 2023/06/27 18:40:36 by dkaratae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel (std::string channelName, std::string key, Client *client)
{
    this->name = channelName;
    this->topic = "";
    this->key = key;
    this->inviteOnly = false;
    this->userLimit = -1;
    operators[0] = client;
}

Channel::~Channel() {}


void Channel::setMod(char m)
{
    std::string a(1, m);
    if (this->check_mode(m))
        this->modes.append(a);
}

bool Channel::check_mode(char c)
{
    std::string ops = "itkol";
    unsigned long n;
    n = ops.find(c);
    if (n == std::string::npos)
    {
        std::cout << "dolboeb" << std::endl;
        return false;
    }
    n = this->modes.find(c);
    if (n == std::string::npos)
    {
        std::cout << "ne nashol" << std::endl;
        return false;
    }
    return true;
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

std::vector<Client *> Channel::getClients()
{
    return (this->clients);
}
    
void Channel::setTopic(std::string topic)
{
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

void Channel::setInviteOnly(bool inviteOnly)
{
    this->inviteOnly = inviteOnly;
}

bool Channel::isInviteOnly()
{
    return this->inviteOnly;
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
}

void Channel::removeClient(Client *client)
{
    for (unsigned long i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i]->get_nick() == client->get_nick())
        {
            this->clients.erase(this->clients.begin() + i);
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