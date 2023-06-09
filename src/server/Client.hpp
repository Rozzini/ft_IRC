/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraspors <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:10:46 by alalmazr          #+#    #+#             */
/*   Updated: 2023/06/30 20:17:59 by mraspors         ###   ########.fr       */
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

#define ERR_NO_REG(source) "451 " + source + " :You have not registered"
#define ERR_ALREADY_REG(source) "462 " + source + " :You may not register"
#define ERR_PW(source) "464 " + source + " :Password is incorrect"
#define ERR_NO_NICK(source) "431 " + source + " :Nickname not given"
#define ERR_NICK_USED(source) "433 " + source + " " + source + " :Nickname is already in use"

#define ERR_UNKNOWN_COMMAND(source, command) "421 " + source + " " + command + " :Unknown command"
#define ERR_MOREPARAMS(source, command) "461 " + source + " " + command + " :Not enough parameters"

#define ERR_ALREADYJOINEDCHANNEL(source, channel) "405 " + source + " " + channel + " :You have joined this channel already"
#define ERR_ALREADY_IN_CH(source) "443 : " + source + " already joined this channel"
#define ERR_NOTONCHANNEL(source, channel) "442 " + source + " " + channel + " :You're not on the channel"
#define ERR_NOSUCHCHANNEL(source, channel) "403 " + source + " " + channel + " :No such channel"
#define ERR_CHANNELKEY(source, channel) "475 " + source + " " + channel + " :Cannot join channel (+k)"
#define ERR_CHANNEL_FULL(source, channel) "471 " + source + " " + channel + " :Cannot join channel (+l)"
#define ERR_CHAN(source, channel) "404 " + source + " " + channel + " :Cannot send to channel"
#define ERR_OP_NEEDED(source, channel) "482 " + source + " " + channel + " :You're not channel operator"
#define ERR_INV_ONLY_CH(channel) "473 " + channel + ": Cannot join Channel (invite only)"
#define ERR_NO_EXIST(source, nickname) "401 " + source + " " + nickname + " :No such nick"
#define ERR_USERNOTINCHANNEL(source, nickname, channel) "441 " + source + " " + nickname + " " + channel + " :Not on channel"
#define ERR_MODE(source) "472: " + source + " is not a recognised channel mode."

#define WELCOME(source) "001 " + source + " :Welcome " + source + " to the ft_irc network"
// #define NAMREPLY(source, channel, users)            "353 " + source + " = " + channel + " :" + users
// #define ENDOFNAMES(source, channel)                 "366 " + source + " " + channel + " :End of /NAMES list."

#define RPLY_USE_HASH(source) ":" + source + " Must use '#' at beginning of channel name"
#define RPLY_PM(source, target, message) ":" + source + " PRIVMSG " + target + " :" + message
#define RPLY_PART(source, channel) ":" + source + " PART :" + channel
#define RPLY_QUIT(source, message) ":" + source + " QUIT :Quit: " + message
#define RPLY_JOIN(source, channel) ":" + source + " JOIN :" + channel
#define RPLY_NOTICE(source, target, message) ":" + source + " NOTICE " + target + " :" + message
#define RPLY_MODE(source, channel, modes, args) ":" + source + " MODE " + channel + " " + modes + " " + args
#define RPLY_KICK(source, channel, target, reason) ":" + source + " KICK " + channel + " " + target + " :" + reason
#define RPLY_CHANGE_MODE(source, channel) ":" + source + " changed " + channel + " mode"
#define RPLY_CHAN_PW_SET(channel) ":" + channel + " password is set"
#define RPLY_CHAN_PW_UNSET(channel) ":" + channel + " password is unset"
#define RPLY_USR_IS_OP(source, channel) ":" + source + "is now an operator of channel " + channel
#define RPLY_USR_NOT_OP(source, channel) ":" + source + "is not an operator of channel " + channel
#define RPLY_TOO_MUCH_LIMIT(channel) ": limit is less than current users for channel " + channel
#define RPLY_LIMIT_SET(channel) ": user limit set for " + channel + " channel"
#define RPLY_LIMIT_UNSET(channel) ": limit has been unset for " + channel + " channel"
// #define RplUniqOpIs         = 325,
// #define RplChannelModeIs    = 324,                  // <channel> <mode>
// #define RplChannelUrl       = 328,                  // <channel> :url                                                                   DALNET
#define RplChannelCreated(channel) "329 " + channel + " was created"
#define RPLY_USER_ADDED(user, channel) ": " + user + " has been added to " + channel
// #define RplNoTopic          = 331,                  // <channel> :No topic is set.
#define RplTopic(channel, topic) "329 " + channel + " topic is " + topic
#define RplTopicSetBy(channel, client) "329 " + channel + " topic was set by " + client
#define NAMREPLY(source, channel, users) "353 " + source + " = " + channel + " :" + users
#define ENDOFNAMES(source, channel) "366 " + source + " " + channel + " :End of /NAMES list."

#define RPL_INVITING(channel, nick)    "341 " + channel + " " + nick
/* Log Response */

static inline void log(const std::string &message)
{
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

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
	typedef std::vector<Channel *>::iterator channel_iterator;

private:
	int fd;
	int port;
	std::string nick;
	std::string uname;
	std::string name;
	std::string host;
	std::vector<Channel *> channels;
	int state; // 0handshake 1login 2registed -1disconnected

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
	std::vector<Channel *> get_channels();
	std::string get_nick() const;
	std::string get_uname() const;
	std::string get_name() const;
	std::string get_host() const;

	bool isInChannel(std::string chName);
	// send recieve actions
	void write(const std::string &msg) const;
	void reply(const std::string &msg);
	// welcome msg as in irc
	void welcome();
	// client actions; join/leave
	void join(Channel *channel);
	void leaveChannel(Channel *channel);
	void leaveChannel(std::string chName);
	void leaveAllChannels();
};

#endif