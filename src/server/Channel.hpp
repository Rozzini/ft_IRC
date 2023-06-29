/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraspors <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:41:22 by dkaratae          #+#    #+#             */
/*   Updated: 2023/06/30 03:03:41 by mraspors         ###   ########.fr       */
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
    typedef std::vector<Client *>::iterator client_iterator;

    public:
        Channel(std::string channelName, std::string key, Client *client);
        ~Channel();


        // void setMod(char m);
        bool check_mode(char c);
    
        void setTopic(std::string topic);
        void setKey(std::string key);
        void setModeI(char sign);
        void setModeT(char sign);
        void setModeK(char sign, std::string key);
        void setModeO(char sign, Client *client);
        void setModeL(char sign, int limit);

        std::string getTopic();
        std::string getKey();
        std::string getMode();
        int getCountClients();
        bool getInviteOnly();
        bool getBoolTopic();

        void setInviteOnly(bool inviteOnly);
        // bool isInviteOnly();

        void setOperator(Client *client, bool isOperator);
        bool isOperator(std::string nickName);

        void setUserLimit(int limit);
        int getUserLimit();

        void addClient(Client *client);
        void removeClient(Client *client); 
        bool isClientInChannel(Client *client);

        void setName(std::string name);
        std::string getName();
        
        void broadcast(const std::string& message);
        void broadcast(const std::string& message, Client* exclude);
        
        std::vector<Client *> getOperators();
        std::vector<Client *> getClients();
    private:
        std::string name;
        std::string topic;
        std::string modes;
        std::string key;
        std::string mode;
        bool boolTopic;
        bool inviteOnly;
        std::vector<Client *> clients;
        std::vector<Client *> operators;
        int userLimit;
};

#endif