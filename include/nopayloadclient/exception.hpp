#pragma once

#include <nlohmann/json.hpp>


class BaseException : public std::exception {
    private:
        int code_ = 1;
        std::string pretext_ = "BaseException: ";
        std::string message_;
    public:
        BaseException(std::string msg) : message_(msg) {}
        BaseException(int code, std::string pretext, std::string msg) {
            code_ = code;
            pretext_ = pretext;
            message_ = msg;
        }
        std::string what () {
            return message_;
        }
        nlohmann::json jsonify() {
            std::string msg = pretext_ + message_;
            return nlohmann::json::object({{"code", code_}, {"msg", msg}});
        }
};


class PayloadException : public BaseException {
    public:
        PayloadException(std::string msg) :
         BaseException(2, "PayloadException: ", msg) {}
};


class DataBaseException : public BaseException {
    private:
        int http_code_;
    public:
        DataBaseException(std::string msg) :
            BaseException(3, "DataBaseException: ", msg) {}
};


class IOVException : public BaseException {
    public:
        IOVException(std::string msg) :
         BaseException(4, "IOVException: ", msg) {}
};

/*
class HttpException : public BaseException {
    private:
        int http_code_;
    public:
        HttpException(std::string msg, int http_code) :
            BaseException(3, "DataBaseException: ", msg) {
                http_code_ = http_code;
        }
        nlohmann::json jsonify() {
            std::string msg = pretext_ + message_ + "(http code: " + http_code_ + ")";
            return nlohmann::json::object({{"code", code_}, {"msg", msg}});
        }
};
*/