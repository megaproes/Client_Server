#include "Command.h"
#include <iostream>
#include <string>
#include <algorithm>
void SendMessageCommand::Execute()
{
	std::cout << "From  user: " << message_ << std::endl;
}
void SendMessageCommand::setMessage(const std::string &message)
{
	std::string mes(message);

    // Находим позицию последнего не-мусорного символа
    size_t last_pos = mes.find_last_not_of("\0\x1f\x00");

    if (last_pos != std::string::npos) {
        // Обрезаем строку до последнего не-мусорного символа
        this->message_ = mes.substr(0, last_pos + 1);
    } else {
        // Если все символы мусорные, то просто очищаем строку
        this->message_ = "";
    }
}

void DisconnectCommand::Execute() {}
void DisconnectCommand::setMessage(const std::string &message) {}
