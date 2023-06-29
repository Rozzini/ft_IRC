/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   admin_Command.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkaratae <dkaratae@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:07:38 by alalmazr          #+#    #+#             */
/*   Updated: 2023/06/30 02:32:40 by dkaratae         ###   ########.fr       */
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
	Channel *ch;
	Client *cl;
	char sign;
	
	//Check count of arguments
	if (args.size() < 1 || args.size() > 3)
	{
		// client->reply("need more args");
		std::cout << "YOU NEED MORE OR LESS ARG!!!" << std::endl;
		return;
	}
	
	//Check the Channel name should start with #
	if (args[0][0] != '#')
	{
		std::cout << "Your channel should start with #" << std::endl;
		return;
	}
	//Check the Channel 
	if ((ch = serv->get_channel(args[0])) == NULL)
	{
		std::cout << "There isn't such channel!!!" << std::endl;
		return;
	}
	
	//Check the Client is operator or not in the Channel
	std::string str = args[0].substr(1, args[0].size());
	if (!(ch->isOperator(client->get_nick())))
	{
		std::cout << "The Client isn't operator in this Channel!!!" << std::endl;
		return;
	}
	
	if (args[1].size() == 2)
	{
		if ((args[1][0] != '+') && (args[1][0] != '-'))
		{
			std::cout << "The Flag should be start the + or - " << std::endl;
			return;
		}
		sign = args[1][0];
		if (args[1][1] == 'i')
		{
			if (args.size() == 2)	
				ch->setModeI(sign);
			else
			{
				std::cout << "You should use MODE #flag +/-i!!!" << std::endl;
				return;
			}
			
		}
		else if (args[1][1] == 't')
		{
			if (args.size() == 2)	
				ch->setModeT(sign);
			else
			{
				std::cout << "You should use MODE #flag +/-t!!!" << std::endl;
				return;	
			}
		}
		else if (args[1][1] == 'k')
		{
			if (args.size() == 3)	
				ch->setModeK(sign, args[2]);
			else
			{
				std::cout << "You should use MODE #flag +/-k keyPass!!!" << std::endl;
				return;
			}
		}
		else if (args[1][1] == 'o')
		{
			if (args.size() == 3)
			{
				if ((cl = serv->get_client(args[2])) == NULL)
				{
					std::cout << "There isn't such client!!!" << std::endl;
					return;
				}
				ch->setModeO(sign, cl);
			}
			else
				std::cout << "You should use MODE #flag +/-0 clientName!!!" << std::endl;
		}
		else if (args[1][1] == 'l')
		{
			if (args.size() == 3)	
				ch->setModeL(sign, stoi(args[2]));
			else
				std::cout << "You should use MODE #flag +/-l limitUser!!!" << std::endl;
		}
		else
			std::cout << "You should use these commands +/-i, +/-t, +/-k, +/-o, +/-l," << std::endl;
	}
	else
	{
		std::cout << "You should use these commands +/-i, +/-t, +/-k, +/-o, +/-l," << std::endl;
		return;	
	}
}

///INVITE <NICK> <channel>
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
	std::cout << "CLIENT = "<< args[0] << std::endl;
	if ((cl = serv->get_client(args[0])) != NULL)
	{
		if (args[1][0] == '#')
		{
			if ((ch = serv->get_channel(args[1])) != NULL)
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
		} else {
			std::cout << "You should write Channel name with # " << std::endl;
			return;
		}
	}
	else
	{
		std::cout << "There isn't exist such client" << std::endl;
		return;
	}
}
