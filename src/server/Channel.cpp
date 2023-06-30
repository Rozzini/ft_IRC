/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkaratae <dkaratae@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:41:28 by dkaratae          #+#    #+#             */
/*   Updated: 2023/06/30 17:46:38 by dkaratae         ###   ########.fr       */
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
        std::cout << "Set password for this Channel!" << std::endl;
    }
    else if (sign == '-')
    {
        key = "";
        std::cout << "Delete password for this Channel!" << std::endl;
    }
}

void Channel::setModeO(char sign, Client *client)
{
    if (sign == '+')
    {
        setOperator(client, true);
        std::cout << client->get_nick() << " is Operator this Channel!" << std::endl;
    }
    else if (sign == '-')
    {
        setOperator(client, false);
        std::cout << client->get_nick() << " isn't Operator this Channel!"<< std::endl;
    }
}

void Channel::setModeL(char sign, int limit)
{
    if (sign == '+')
    {
        if (limit < getCountClients())
            std::cout << "Not possible set limit, because it less then clients in the Channel" << std::endl;
        else
        {
            setUserLimit(limit);
            std::cout << "You set limit "<< limit << " users for this Channel" << std::endl;
        }
    }
    else if (sign == '-')
    {
        setUserLimit(-1);
        std::cout << "You unset limit of users for this Channel" << std::endl;
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
    std::cout << client->get_nick() << " added to Channel!" << std::endl;
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