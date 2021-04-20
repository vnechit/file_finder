#include <exception>
#include <iostream>
#include <string>
#include "scanner.h"
#include "arg_parser.h"

using namespace std;

void DebugInfo(const arg_property& argpr) {
    if(!argpr.DEBUG) {
        return;
    }
    cout << "=====================INFO=====================" << endl;
    cout << "Начальная директория: " << argpr.Dir << endl;
    cout << "Строка поиска: " << argpr.EXPR << endl;
    cout << "Глубина поиска: " << (argpr.Depth == -1 ? "No set": to_string(argpr.Depth)) << endl;
    cout << "Точное совпадение: " << (argpr.Exact ? "Yes" : "No") << endl;
    cout << "Полный абсолютный путь: " << (argpr.Full_patch  ? "Yes" : "No")  << endl;
    cout << "Относительный путь: " << (argpr.Relative_path ? "Yes" : "No") << endl;
    cout << "Директория или файл: " << (argpr.Type == 't' ? "File" : "Dir") << endl;
    cout << "Учитывать регистр: " << (argpr.Case_insensitive ? "Yes" : "No") << endl;
    cout << "Регулярка: " << (argpr.regex ? "Yes" : "No") << endl;
    cout << "Верхний предел размера: " << (argpr.Size_gt== -1 ? "No set": to_string(argpr.Size_gt)) << endl;
    cout << "Нижний: " << (argpr.Size_lt == -1 ? "No set": to_string(argpr.Size_lt))<< endl;
    cout << "==============================================" << endl;
}

int main(int argc, char **argv){
    // Может быть исключение, например, если папки не существует
    try {
        auto returnarg = arg_parser(argc, argv);
        DebugInfo(returnarg);
        Scanner* scanner = nullptr;
        if(returnarg.Info) {
            scanner = new ScannerFileSystem(returnarg);
        }
        else {
            scanner = new SearchFileOrDirectory(returnarg);
        }
        auto listResult = scanner->scanner();
        for(auto &par : listResult) {
            cout << par << endl;
        }
    }
    catch (exception &e) {
        cout << "Error: " << e.what() << '\n';
    }
    return 0;
}