/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   admin_Command.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraspors <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:07:38 by alalmazr          #+#    #+#             */
/*   Updated: 2023/06/30 22:54:15 by mraspors         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "../server/Server.hpp"
#include <cstddef>
#include <cstdio>
#include <string>

// KICK <channel> <client> :[<message>]
void KICK::execute(Client *client, std::vector<std::string> args)
{
	if (args.size() < 2)
	{
		client->reply(ERR_MOREPARAMS(client->get_nick(), "KICK"));
		return;
	}

	std::string name = args[0];
	std::string target = args[1];
	std::string reason = (args.size() >= 3 && args[2][0] != ':') ? args[2] : "No reason specified!";

	Channel *channel = serv->get_channel(name);
	if (!channel || channel->getName() != name)
	{
		client->reply(ERR_NOTONCHANNEL(client->get_nick(), channel->getName()));
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
			return;
		}
	}
	Client *dest_client = serv->get_client(target);
	if (dest_client->get_nick().compare(client->get_nick()) == 0)
	{
		client->reply(ERR_NO_EXIST(client->get_nick(), dest_client->get_nick()));
		return;
	}
	if (!dest_client)
	{
		client->reply(ERR_NO_EXIST(client->get_nick(), dest_client->get_nick()));
		return;
	}
	channel->removeClient(dest_client);
	dest_client->leaveChannel(channel);
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
	if (channel == NULL)
	{
		client->reply(ERR_NOSUCHCHANNEL(client->get_nick(), channel->getName()));
		return;
	}
	if (channel->getBoolTopic() == true)
	{
		client->reply(ERR_OP_NEEDED(client->get_nick(), channel->getName()));
		return;
	}
	else
	{
		std::string topic = args[1];
		channel->setTopic(topic);
		client->reply(RplTopic(channel->getName(), topic));
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
	Channel *ch;
	Client *cl;
	char sign;
	
	//Check count of arguments
	if (args.size() < 1 || args.size() > 3)
	{
		client->reply(ERR_MOREPARAMS(client->get_nick(), "MODE"));
		return;
	}
	
	//Check the Channel name should start with #
	if (args[0][0] != '#')
	{
		client->reply(RPLY_USE_HASH(args[0]));
		return;
	}
	//Check the Channel 
	if ((ch = serv->get_channel(args[0])) == NULL)
	{
		client->reply(ERR_NOSUCHCHANNEL(client->get_nick(), ch->getName()));
		return;
	}
	
	//Check the Client is operator or not in the Channel
	std::string str = args[0].substr(1, args[0].size());
	if (!(ch->isOperator(client->get_nick())))
	{
		client->reply(ERR_OP_NEEDED(client->get_nick(), ch->getName()));
		return;
	}
	
	if (args[1].size() == 2)
	{
		if ((args[1][0] != '+') && (args[1][0] != '-'))
		{
			client->reply(ERR_MOREPARAMS(client->get_nick(), "MODE"));
			return;
		}
		sign = args[1][0];
		if (args[1][1] == 'i')
		{
			if (args.size() == 2)	
				ch->setModeI(sign);
			else
			{
				client->reply(ERR_MOREPARAMS(client->get_nick(), "MODE"));
				return;
			}
		}
		else if (args[1][1] == 't')
		{
			if (args.size() == 2)	
				ch->setModeT(sign);
			else
			{
				client->reply(ERR_MOREPARAMS(client->get_nick(), "MODE"));
				return;	
			}
		}
		else if (args[1][1] == 'k')
		{
			std::cout << "KKK" << std::endl;
			if ((sign == '+' && args.size() == 3) || (sign == '-' && args.size() == 2))	
			{
				ch->setModeK(sign, args[2]);
			}
			else
			{
				client->reply(ERR_MOREPARAMS(client->get_nick(), "MODE"));
				return;
			}
		}
		else if (args[1][1] == 'o')
		{
			if ((sign == '+' && args.size() == 3) || (sign == '-' && args.size() == 2))
			{
				if ((cl = serv->get_client(args[2])) == NULL)
				{
					client->reply(ERR_MOREPARAMS(client->get_nick(), "MODE"));
						return;
				}
				if (ch->isClientInChannel(cl) == false)
				{
					client->reply(ERR_MOREPARAMS(client->get_nick(), "MODE"));
						return;
				}
				ch->setModeO(sign, cl);
			}
			else
				client->reply(ERR_MOREPARAMS(client->get_nick(), "MODE"));
		}
		else if (args[1][1] == 'l')
		{
			if (sign == '+' && args.size() == 3)
				ch->setModeL(sign, stoi(args[2]));
			else if (sign == '-' && args.size() == 2)
				ch->setModeL(sign, -1);
			else
				client->reply(ERR_MOREPARAMS(client->get_nick(), "MODE"));
		}
		else
			client->reply(ERR_MODE(args[1]));
	}
	else
	{
		client->reply(ERR_MODE(args[1]));
		return;	
	}
}

///INVITE <NICK> <channel>
void INVITE::execute(Client *client, std::vector<std::string> args)
{
	Channel *ch = NULL;
	Client *cl;

	if (args.empty())
	{
		client->reply(ERR_MOREPARAMS(client->get_nick(), "INVITE"));
		return;
	}
	if ((cl = serv->get_client(args[0])) != NULL)
	{
		if (args[1][0] == '#')
		{
			if ((ch = serv->get_channel(args[1])) != NULL)
			{
				if (!(ch->isClientInChannel(cl)))
				{
					if (ch->isInvited(cl) == false)
					{
						ch->setInviteList(cl);
						cl->reply(RPL_INVITING(ch->getName(), cl->get_name()));
					}
				}
				else
				{
					client->reply(ERR_ALREADY_IN_CH(cl->get_nick()));
					return;
				}
			}
			else
			{
				client->reply(ERR_NOSUCHCHANNEL(cl->get_nick(), ch->getName()));
				return;
			}
		} else {
			client->reply(RPLY_USE_HASH(ch->getName()));
			return;
		}
	}
	else
	{
		client->reply(ERR_NO_EXIST(cl->get_nick(), ch->getName()));
		return;
	}
}
