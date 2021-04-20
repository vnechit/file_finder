#include "arg_parser.h"
#include "errors.h"
#include <QCoreApplication>
#include <QCommandLineParser>

arg_property arg_parser(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QCommandLineParser par;
    par.addHelpOption();
    //Поиск файлов
    par.addOption({{"depth", "d"}, "максимальная глубина поиска", "DEPTH"});
    par.addOption({{"exact", "e"}, "проверять точное совпадение по имени файла, вместо поиска подстроки."});
    par.addOption({{"full-path", "f"}, "в результатах поиска печатать полный абсолютный путь до файла"});
    par.addOption({{"relative-path", "p"}, "в результатах поиска печатать относительный путь до файла."});
    par.addOption({{"type", "t"}, "что искать — директорию (d) или файл (f)", "TYPE"});
    par.addOption({{"case-insensitive", "i"}, "при поиске не учитывать регистр"});
    //*
    par.addOption({{"regex", "r"}, "рассматривать EXPR как регулярное выражение."});
    par.addOption({{"size-gt", "g"}, "искать файлы с размером больше либо равным SIZE", "SIZE"});
    par.addOption({{"size-lt", "s"}, "искать файлы с размером меньше либо равным SIZE", "SIZE"});
    par.addOption({{"DEBUG", "D"}, "DEBUG"});
    // Анализ Файловой системы
    par.addOption({"info", "выводит информацию о текущей директории"});
    par.addOption({"analyze-huge-files", "ищет файлы с размером больше 100 МБ"});
    par.addOption({"analyze-huge-files-noext", "ищет файлы с размером больше 100 МБ и без расширения"});
    //*
    par.process(app);

    if (!par.parse(QCoreApplication::arguments())) {
        error(par.errorText().toStdString());
    }
    arg_property retarg;
    if (argc >= 2) {
        retarg.Dir = argv[1];
    }
    if (argc >= 3) {
        if (argv[2][0] != '-') {
            retarg.EXPR = argv[2];
        }
    }
    retarg.Full_patch = par.isSet("full-path");
    retarg.Relative_path = par.isSet("relative-path");
    if (retarg.Full_patch == retarg.Relative_path && retarg.Full_patch){
        error("Установлеы два формата вывода, ожидался один");
    }
    if(retarg.Full_patch == retarg.Relative_path && !retarg.Full_patch) {
        retarg.Full_patch = true;
    }
    if(par.isSet("size-lt")) {
        retarg.Size_lt = par.value("size-lt").toInt();
    }
    if(par.isSet("size-gt")) {
        retarg.Size_gt = par.value("size-gt").toInt();
    }
    if(par.isSet("depth")) {
        retarg.Depth = par.value("depth").toInt();
    }
    retarg.Exact = par.isSet("exact");
    if(par.isSet("type")) {
        retarg.Type = par.value("type").toStdString()[0];
    }
    if(retarg.Type != 'd' && retarg.Type != 't') {
        error("Не правильный тип поиска файл или папка");
    }
    retarg.Case_insensitive = par.isSet("case-insensitive");
    retarg.regex = par.isSet("regex");
    retarg.DEBUG = par.isSet("DEBUG");
    if(!retarg.Exact) {
        retarg.EXPR = ".*" + retarg.EXPR + ".*";
    }
    auto flag = regex_constants::collate;
    if(retarg.Case_insensitive) {
        flag |= regex_constants::icase;
    }
    retarg.REXPR = regex(retarg.EXPR, flag);
    retarg.Info = par.isSet("info");
    retarg.Huge_files = par.isSet("analyze-huge-files");
    retarg.Huge_files_noext = par.isSet("analyze-huge-files-noext");

    return retarg;
}