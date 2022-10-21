#pragma once


class NoPayloadException : public std::exception {
    private:
        std::string message;
    public:
        NoPayloadException(std::string msg) : message(msg) {}
        std::string what () {
            return message;
        }
};
