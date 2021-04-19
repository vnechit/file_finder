//
// Created by   User on 19.04.2021.
//

#pragma once
#include <string>
#include <memory>
#include <filesystem>
#include <list>
#include <algorithm>
#include <utility>
#include <regex>
#include <QFileInfo>
#include "arg_property.h"


using namespace std;
namespace fs = filesystem;

class Scanner {
protected:
    arg_property argProperty;
public:
    explicit Scanner(arg_property prop): argProperty(move(prop)){}

    virtual list<string> scanner() = 0;
};

class SearchFileOrDirectory: public Scanner{
    void recur_scan_patch(list<string> &, const string&, int = 0);

public:
    explicit SearchFileOrDirectory(arg_property pr): Scanner(move(pr)){}

    list<string> scanner() override;
};

class ScannerFileSystem: public Scanner {
    struct DirInfo {
        int depth;
        int size;
        string Name;
        string name;
        list<DirInfo> listDir;
        list<string> listFile;

        explicit DirInfo(int d = 0, int s = 0, string n = ""): depth(d), size(s), Name(move(n)){
            transform(Name.begin(), Name.end(), name.begin(), ::tolower);
        }

        void AddName(string N){
            Name = move(N);
            name = Name;
            transform(name.begin(), name.end(), name.begin(), ::tolower);
        }

        int RecurOut(list<string> &);
    };

    int recur_size_dir(list<DirInfo> &, const string &, int = 0);

    static bool Sr(DirInfo &lp, DirInfo &rp);

public:
    explicit  ScannerFileSystem(arg_property pr): Scanner(move(pr)){}

    list<string> scanner() override;
};