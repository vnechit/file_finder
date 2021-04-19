//
// Created by   User on 19.04.2021.
//

#pragma once

#include <string>
#include <regex>

using namespace std;

struct arg_property{
    //Поиск файлов
    string Dir = "";
    string EXPR = "";
    regex REXPR;
    int Depth = -1;
    bool Exact = false;
    bool Full_patch = false;
    bool Relative_path = false;
    char Type = 'd';// or 'f'
    bool Case_insensitive = false;
    //*
    bool regex = false;
    int Size_gt = -1;
    int Size_lt = -1;
    bool DEBUG = false;
    //Анализ файловой системы
    bool Info = false;
    bool Huge_files = false;
    bool Huge_files_noext = false;
};