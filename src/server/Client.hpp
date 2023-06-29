/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalmazr <alalmazr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:10:46 by alalmazr          #+#    #+#             */
/*   Updated: 2023/06/29 15:19:01 by alalmazr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/poll.h>
#include <sys/socket.h>
#include <string>
#include <vector>
#include "Server.hpp"

class Channel;

#include <iostream>
#include <string>

#include <cstdlib>
#include <cstdio>
#include <time.h>

/* Error Responses */

#define ERR_NO_REG(source)                       "451 " + source + " :You have not registered"
#define ERR_ALREADY_REG(source)                   "462 " + source + " :You may not register"
#define ERR_PW(source)                      "464 " + source + " :Password is incorrect"
#define ERR_NO_NICK(source)                     "431 " + source + " :Nickname not given"
#define ERR_NICK_USED(source)                       "433 " + source + " " + source  + " :Nickname is already in use"

#define ERR_UNKNOWN_COMMAND(source, command)             "421 " + source + " " + command + " :Unknown command"
#define ERR_MOREPARAMS(source, command)             "461 " + source + " " + command + " :Not enough parameters"

#define ERR_TOOMANYCHANNELS(source, channel)            "405 " + source + " " + channel + " :You have joined too many channels"
#define ERR_NOTONCHANNEL(source, channel)               "442 " + source + " " + channel + " :You're not on that channel"
#define ERR_NOSUCHCHANNEL(source, channel)              "403 " + source + " " + channel + " :No such channel"
#define ERR_CHANNELKEY(source, channel)              "475 " + source + " " + channel + " :Cannot join channel (+k)"
#define ERR_CHANNEL_FULL(source, channel)              "471 " + source + " " + channel + " :Cannot join channel (+l)"
#define ERR_CHAN(source, channel)           "404 " + source + " " + channel + " :Cannot send to channel"
#define ERR_OP_NEEDED(source, channel)           "482 " + source + " " + channel + " :You're not channel operator"

#define ERR_NO_EXISTING_NICK(source, nickname)                "401 " + source + " " + nickname + " :No such nick/channel"
#define ERR_USERNOTINCHANNEL(source, nickname, channel) "441 " + source + " " + nickname + " " + channel + " :They aren't on that channel"

#define WELCOME(source)                             "001 " + source + " :Welcome " + source + " to the ft_irc network"
#define NAMREPLY(source, channel, users)            "353 " + source + " = " + channel + " :" + users
#define ENDOFNAMES(source, channel)                 "366 " + source + " " + channel + " :End of /NAMES list."

#define RPLY_PM(source, target, message)            ":" + source + " PRIVMSG " + target + " :" + message
#define RPLY_PART(source, channel)                       ":" + source + " PART :" + channel                     ":" + source + " PONG :" + token
#define RPLY_QUIT(source, message)                       ":" + source + " QUIT :Quit: " + message
#define RPLY_JOIN(source, channel)                       ":" + source + " JOIN :" + channel
#define RPLY_NOTICE(source, target, message)             ":" + source + " NOTICE " + target + " :" + message
#define RPLY_MODE(source, channel, modes, args)          ":" + source + " MODE " + channel + " " + modes + " " + args
#define RPLY_KICK(source, channel, target, reason)       ":" + source + " KICK " + channel + " " + target + " :" + reason


/* Log Response */

static inline void log(const std::string& message) 
{
    time_t      rawtime;
    struct tm   *timeinfo;
    char        buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    std::string str(buffer);

    (void)message;
    std::cout << "\033[0;34m[" << str << "]\033[0m ";
    std::cout << message << std::endl;
}


class Client
{
private:
	int fd;
	int port;
	std::string nick;
	std::string uname;
	std::string name;
	std::string host;
	Channel *channel;
	// 0handshake 1login 2registed -1disconnected
	int state;

	// constructor
	Client();
	// cpy constructor
	Client(const Client &src);

public:
	Client(int fd, int port, const std::string &host);
	~Client();

	// setters
	void set_state(int state);
	void set_nick(const std::string &nick);
	void set_uname(const std::string &uname);
	void set_name(const std::string &name);
	void set_channel(Channel *channel);
	// getters
	std::string get_prefix() const;
	int get_fd() const;
	int get_port() const;
	int get_state();
	Channel *get_channel();

	std::string get_nick() const;
	std::string get_uname() const;
	std::string get_name() const;
	std::string get_host() const;

	// send recieve actions
	void send(const std::string &msg) const;
	void reply(const std::string &msg);
	// welcome msg as in irc
	void welcome();
	// client actions; join/leave
	void join(Channel *channel);
	void leave();
};

#endif