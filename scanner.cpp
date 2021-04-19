#include "scanner.h"
using namespace std;

list<string> SearchFileOrDirectory::scanner(){
    list<string> listOfFileOrDir;
    recur_scan_patch(listOfFileOrDir, argProperty.Dir);
    return listOfFileOrDir;
}


void SearchFileOrDirectory::recur_scan_patch(list<string> &listOfFileOrDir, const string &patchToDir, int cIter) {
    if(cIter > argProperty.Depth && argProperty.Depth != -1) {
        return;
    }
    for(const auto &p: fs::directory_iterator(patchToDir)) {
        if(p.is_directory()){
            recur_scan_patch(listOfFileOrDir, p.path(), cIter+1);
        }
        if(p.is_directory() && argProperty.Type != 'd') {
            continue;
        }
        if(!p.is_directory() && argProperty.Type != 't') {
            continue;
        }
        if(!p.is_directory()){
            if( !(p.file_size() >= argProperty.Size_gt || argProperty.Size_gt == -1) || !(p.file_size() <= argProperty.Size_lt || argProperty.Size_lt == -1)){
                continue;
            }
        }
        if(regex_match((string)p.path().filename(), argProperty.REXPR)){
            if (!argProperty.Full_patch){
                string name = p.path();
                auto findDirToPath = name.find(argProperty.Dir);
                auto findSlesh = argProperty.Dir.rfind('/');
                if(findDirToPath != string::npos && findSlesh != string::npos){
                    name.erase(0, findSlesh);
                }
                name = "..." + name;
                listOfFileOrDir.push_back(name);
            }
            else {
                listOfFileOrDir.push_back(p.path());
            }
        }
    }
}



bool ScannerFileSystem::Sr(ScannerFileSystem::DirInfo &lp, ScannerFileSystem::DirInfo &rp){
    return lp.name < rp.name;
}

list<string> ScannerFileSystem::scanner() {
    list<DirInfo> listDirInfo;
    recur_size_dir(listDirInfo, argProperty.Dir, -1);
    listDirInfo.sort(Sr);
    list<string> listDirAndFile;
    if (!listDirInfo.empty()) {
        int count = listDirInfo.front().RecurOut(listDirAndFile);
        listDirAndFile.push_front("РєРѕР»РёС‡РµСЃС‚РІРѕ С„Р°Р№Р»РѕРІ Рё РїР°РїРѕРє: " + to_string(count));
    }
    else {
        listDirAndFile.push_front("Р¤Р°Р№Р»РѕРІ РЅРµ РЅР°Р№РґРµРЅРѕ!");
    }
    return listDirAndFile;
}

int ScannerFileSystem::recur_size_dir(list<DirInfo> &listDirInfo,const string &dir, int depth){
    if(depth > argProperty.Depth && argProperty.Depth != -1) {
        return 0;
    }
    listDirInfo.emplace_back(depth);
    DirInfo &dirInfo = listDirInfo.back();
    auto find = dir.rfind('/');
    if(find != string::npos) {
        dirInfo.AddName(dir.substr(find + 1, dir.size()));
    }
    else {
        dirInfo.AddName(dir);
    }
    for (auto &p : fs::directory_iterator(dir)){
        if (p.is_directory()){
            dirInfo.size += recur_size_dir(dirInfo.listDir, p.path(), depth + 1);
        }
        else {
            if ((argProperty.Huge_files || argProperty.Huge_files_noext) && p.file_size() < 8 * 1024 * 1024) {
                continue;
            }
            if (argProperty.Huge_files_noext || p.path().filename().string().rfind('.') != string::npos) {
                dirInfo.listFile.push_back(p.path().filename().string());
            }
            dirInfo.size += p.file_size();
        }
    }
    return dirInfo.size;
}

int ScannerFileSystem::DirInfo::RecurOut(list<string> &listOut){
    string cos;
    for(int i = 0; i <= this->depth; i++) {
        cos += "-";
    }
    listOut.push_back(cos + Name + " СЂР°Р·РјРµСЂ: " + to_string(this->size));
    int countElement = this->listDir.size() + this->listFile.size();
    for(auto &dir : this->listDir) {
        countElement += dir.RecurOut(listOut);
    }
    cos += "-";
    for(auto &file : this->listFile) {
        listOut.push_back(cos + file);
    }
    return countElement;
}