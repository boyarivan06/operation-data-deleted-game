#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>
#include <string>


class ObjectParameterException : public std::exception {
    std::string message_;
public:
    explicit ObjectParameterException(const char* message) : message_(message) {}

    [[nodiscard]] const char* what() const noexcept override {
        return message_.c_str();
    }
};

class FieldException : public std::exception {
    std::string message_;
public:
    explicit FieldException(const char* message) : message_(message) {}

    [[nodiscard]] const char* what() const noexcept override {
        return message_.c_str();
    }
};

class WayImpassableException : public std::exception {
    std::string message_ = "Маршрут непроходим";
public:
    explicit WayImpassableException(const char* message) : message_(message) {}
    WayImpassableException() = default;

    [[nodiscard]] const char* what() const noexcept override {
        return message_.c_str();
    }
};

class WeaponEmptyError : public std::exception {
    std::string message_ = "Оружие разряжено";
public:
    explicit WeaponEmptyError(const char* message) : message_(message) {}
    WeaponEmptyError() = default;

    [[nodiscard]] const char* what() const noexcept override {
        return message_.c_str();
    }
};
#endif
