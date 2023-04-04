#pragma once
#include <string>

class Command
{
public:
	virtual ~Command() {}
	virtual void Execute() = 0;
	virtual void setMessage(const std::string &message) = 0;
};

class SendMessageCommand : public Command
{
private:
	std::string message_;

public:
	SendMessageCommand(const std::string &message) : message_(message) {}
	void Execute() override;
	void setMessage(const std::string &message) override;
};

class DisconnectCommand : public Command
{
public:
	void Execute() override;
		void setMessage(const std::string &message) override;

};