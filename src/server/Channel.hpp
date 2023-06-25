/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraspors <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:41:22 by dkaratae          #+#    #+#             */
/*   Updated: 2023/06/25 19:04:59 by mraspors         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"

class Client;

class Channel
{
    public:
        Channel(std::string channelName, std::string key, Client *client);
        ~Channel();


        void setMod(char m);
        bool check_mode(char c);
    
        void setTopic(std::string topic);
        void setKey(std::string key);

        std::string getTopic();
        std::string getKey();

        void setInviteOnly(bool inviteOnly);
        bool isInviteOnly();

        void setOperator(Client *client, bool isOperator);
        bool isOperator(std::string nickName);

        void setUserLimit(int limit);
        int getUserLimit();

        void addClient(Client *client);
        void removeClient(Client *client); 
        bool isClientInChannel(Client *client);

        void setName(std::string name);
        std::string getName();
        
        std::vector<Client *> getOperators();
        std::vector<Client *> getClients();
    private:
        std::string name;
        std::string topic;
        std::string modes;
        std::string key;
        bool inviteOnly;
        std::vector<Client *> clients;
        std::vector<Client *> operators;
        int userLimit;
};

#endif