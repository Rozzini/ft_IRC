/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   admin_Command.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraspors <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:07:38 by alalmazr          #+#    #+#             */
/*   Updated: 2023/06/29 16:00:23 by mraspors         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "../server/Server.hpp"
#include <cstdio>
#include <string>

// KICK <channel> <client> :[<message>]
void KICK::execute(Client *client, std::vector<std::string> args)
{
	if (args.size() < 2)
	{
		client->reply("need more args");
		return;
	}

	std::string name = args[0];
	std::string target = args[1];
	std::string reason = (args.size() >= 3 && args[2][0] != ':') ? args[2] : "No reason specified!";

	Channel *channel = client->get_channel();
	if (!channel || channel->getName() != name)
	{
		client->reply("u r not in channel/channel doesnt exist");
		return;
	}
	std::vector<Client *> operators = channel->getOperators();
	for (unsigned long i = 0; i < operators.size(); i++)
    {
        if (operators[i]->get_nick() == client->get_nick())
            break;
		if (i == operators.size() - 1)
		{
			client->reply("unauthorized to kick");
			return;
		}
    }
	Client *dest_client = serv->get_client(target);
	if (!dest_client)
	{
		client->reply("user not found");
		return;
	}
	//channel->kick(client, dest_client, reason);
	channel->removeClient(client);
}

// TOPIC <channel> [<topicname>]
void TOPIC::execute(Client *client, std::vector<std::string> args)
{
	if (args.size() < 1 || args.size() > 2)
	{
		client->reply(ERR_MOREPARAMS(client->get_nick(), "TOPIC"));
		return;
	}

	std::string channelName = args[0];
	Channel *channel = serv->get_channel(channelName);
	if (!channel)
	{
		client->reply(ERR_NOSUCHCHANNEL(client->get_nick(), channel));
		return;
	}
	if (channel->check_mode('t') && channel->get_admin() != client)
	{
		std::vector<Client*> ops = channel->getOperators();
		for (size_t i = 0; i < ops.size(); i++)
		{
			if (ops[i]->get_nick == client->get_nick)
				break;
		}
		if (i < ops.size())
		{
			client->reply(ERR_OP_NEEDED(client->get_nick(), channel));
			return;
		}
	}
	else
	{
		std::string topic = args[1];
		channel->setTopic(topic);
	}
	if (args.size() == 1)
	{
		std::string topic = "The topic of " + channelName + " is " + channel->getTopic();
		client->reply(topic);
	}
}

// MODE <nickname> <flags> (user)
// MODE <channel> <flags> [<args>]
void MODE::execute(Client *client, std::vector<std::string> args)
{
	// if (args.size() < 2)
	// {
	// 	client->reply("need more args");
	// 	return;
	(void)client;
	Channel *ch;
	Client *cl;
	bool sign = false;
	int j = 0;
	char sign_ch = '0';
	
	ch = serv->get_channel(args[1]);
	cl = serv->get_client(args[3]);
	
	for (unsigned int i = 0; i < args[2].size(); i++)
	{
		if (args[2][j] == '+' || args[2][j] == '-')
		{
			sign = true;
			sign_ch = args[2][j];
		}
		if (sign == true && args[2][j] == 'i')
			ch->setMode(args[2][j], sign_ch, cl);
		else if (sign == true && args[2][j] == 't')
			ch->setMode(args[2][j], sign_ch, cl);
		else if (sign == true && args[2][j] == 'k')
			ch->setMode(args[2][j], sign_ch, cl);
		else if (sign == true && args[2][j] == 'o')
			ch->setMode(args[2][j], sign_ch, cl);
		else if (sign == true && args[2][j] == 'l')
		{
			ch->setUserLimit(stoi(args[3]));
			ch->setMode(args[2][j], sign_ch, cl);
		}
		
	}
}

///INVITE <NICK> <channel>
// void INVITE::execute(Client* client, std::vector<std::string> args)
// {
// 	if (args.size() != 2)
// 	{
// 		client->reply("args error");
// 		return;
// 	}

// 	std::string nickname = args[0];
// 	Client* target = serv->get_client(nickname);
// 	if (!target)
// 	{
// 		client->reply("user doesnt exist");
// 		return;
// 	}

// 	Channel* channel = serv.get_channel(args[1]);
// 	if (!channel)
// 	{
// 		client->reply("channel doesnt exist");
// 		return;
// 	}

// 	if (channel->get_admin() != client)
// 	{
// 		client->reply("unauthorized to kick");
// 		return;
// 	}

// 	if (channel->get_nick(nickname))
// 	{
// 		client->reply("user already in channel");
// 		return;
// 	}

// 	target->set_channel(channel);//???
// 	//add relevant msg here
// }