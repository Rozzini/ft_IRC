/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   admin_Command.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraspors <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:07:38 by alalmazr          #+#    #+#             */
/*   Updated: 2023/06/29 20:17:54 by mraspors         ###   ########.fr       */
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
		client->reply(ERR_MOREPARAMS(client->get_nick(), "KICK"));
		std::cout << "NEED MORE ARGS" << std::endl;
		return;
	}

	std::string name = args[0];
	std::string target = args[1];
	std::string reason = (args.size() >= 3 && args[2][0] != ':') ? args[2] : "No reason specified!";

	Channel *channel = client->get_channel();
	if (!channel || channel->getName() != name)
	{
		client->reply(ERR_NOTONCHANNEL(client->get_nick(), channel->getName()));
		std::cout << "NOT IN CHANNEL/CHANNEL DOESNT EXIST" << std::endl;
		return;
	}
	std::vector<Client *> operators = channel->getOperators();
	for (unsigned long i = 0; i < operators.size(); i++)
	{
		if (operators[i]->get_nick() == client->get_nick())
			break;
		if (i == operators.size() - 1)
		{
			client->reply(ERR_OP_NEEDED(client->get_nick(), channel->getName()));
			std::cout << "DOESNT HAVE RIGHTS TO KICK" << std::endl;
			return;
		}
	}
	Client *dest_client = serv->get_client(target);
	if (!dest_client)
	{
		client->reply(ERR_NO_EXIST(client->get_nick(), dest_client->get_nick()));
		std::cout << "NO SUCH USER" << std::endl;
		return;
	}
	// channel->kick(client, dest_client, reason);
	channel->removeClient(client);
	std::cout << "CLIET REMOVED FROM CHANNEL" << std::endl;
}

// TOPIC <channel> [<topicname>]
void TOPIC::execute(Client *client, std::vector<std::string> args)
{
	if (args.size() < 1 || args.size() > 2)
	{
		client->reply(ERR_MOREPARAMS(client->get_nick(), "TOPIC"));
		std::cout << "NEED MORE PARAMS" << std::endl;
		return;
	}

	std::string channelName = args[0];
	Channel *channel = serv->get_channel(channelName);
	if (!channel)
	{
		//client->reply(ERR_NOSUCHCHANNEL(client->get_nick(), channel));
		std::cout << "NO SUCH CHANNEL" << std::endl;
		return;
	}
	if (channel->check_mode('t') && channel->isOperator(client->get_nick()) == false)
	{
		size_t i = 0;
		std::vector<Client *> ops = channel->getOperators();
		for (i = 0; i < ops.size(); i++)
		{
			if (ops[i]->get_nick() == client->get_nick())
				break;
		}
		if (i < ops.size())
		{
			//client->reply(ERR_OP_NEEDED(client->get_nick(), channel));
			std::cout << "NEED TO BE OPERATOR" << std::endl;
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
		std::cout << "TOPIC SETTED: " << topic << std::endl;
		client->reply(topic);
	}
}

// MODE <nickname> <flags> (user)
// MODE <channel> <flags> [<args>]
void MODE::execute(Client *client, std::vector<std::string> args)
{
	if (args.size() < 2)
	{
		client->reply(ERR_MOREPARAMS(client->get_nick(), "MODE"));
		return;
	}

	std::string target = args.at(0);
	Channel *channel = serv->get_channel(target); // MODE on clients not implemented
	if (!channel)
	{
		client->reply(ERR_NOSUCHCHANNEL(client->get_nick(), target));
		return;
	}
	std::vector<Client *> ops = channel->getOperators();
	size_t i = 0;
	for (i = 0; i < ops.size(); i++)
	{
		if (ops[i]->get_nick() == client->get_nick())
			break;
	}
	if (i < ops.size())
	{
		client->reply(ERR_OP_NEEDED(client->get_nick(), channel->getName()));
		return;
	}

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


//1) - check if client exist (return if no)
//2)check if channel exist (return if no)
//3)check if client already on chaneel (return if yes)
//4)add client to channel
void INVITE::execute(Client *client, std::vector<std::string> args)
{
	Channel *ch;
	Client *cl;

	if (args.empty())
	{
		client->reply("need more args");
		std::cout << "NEED MORE ARGS" << std::endl;
		return;
	}
	if ((cl = serv->get_client(args[1])) != NULL)
	{
		if ((ch = serv->get_channel(args[2])) != NULL)
		{
			if (!(ch->isClientInChannel(cl)))
				ch->addClient(cl);
			else
			{
				std::cout << "This client already exist in this channel!" << std::endl;
				return;
			}
		}
		else
		{
			std::cout << "There isn't exist such channel" << std::endl;
			return;
		}
	}
	else
	{
		std::cout << "There isn't exist such client" << std::endl;
		return;
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