/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkaratae <dkaratae@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:41:28 by dkaratae          #+#    #+#             */
/*   Updated: 2023/06/30 02:33:52 by dkaratae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
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
        inviteOnly = false;
        std::cout << "inviteOnly = false;" << std::endl;
    }
    else if (sign == '-')
    {
        inviteOnly = true;
        std::cout << " inviteOnly = true;" << std::endl;
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
        std::cout << "this->key = key;" << std::endl;
    }
    else if (sign == '-')
    {
        key = "";
        std::cout << "key = "";" << std::endl;
    }
}

void Channel::setModeO(char sign, Client *client)
{
    if (sign == '+')
    {
        setOperator(client, true);
        std::cout << "etOperator(client, true);" << std::endl;
    }
    else if (sign == '-')
    {
        setOperator(client, false);
        std::cout << "setOperator(client, false);" << std::endl;
    }
}

void Channel::setModeL(char sign, int limit)
{
    if (sign == '+')
    {
        if (limit > getCountClients())
            std::cout << "Not possible because the limit less then clients in the Channel" << std::endl;
        else
        {
            setUserLimit(limit);
            std::cout << "setUserLimit(limit);" << std::endl;
        }
    }
    else if (sign == '-')
    {
        setUserLimit(-1);
        std::cout << "s setUserLimit(-1);" << std::endl;
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

// bool Channel::isInviteOnly()
// {
//     return this->inviteOnly;
// }

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