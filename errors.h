//
// Created by   User on 19.04.2021.
//

#pragma once
#include <stdexcept>
#include <string>

inline void error(const std::string& msg)
{
    throw std::runtime_error(msg);
}