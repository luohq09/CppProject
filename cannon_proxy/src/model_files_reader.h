//
// Created by LuoHuaqing on 4/14/16.
//

#ifndef CANNON_PROXY_MODEL_FILES_READER_H
#define CANNON_PROXY_MODEL_FILES_READER_H

#include <vector>
#include <string>
#include <fstream>

namespace paradigm4 {

class ModelFilesReader {
public:
    ModelFilesReader(const std::vector<std::string> &files);
    ModelFilesReader(std::vector<std::string> &&files);

    std::vector<std::string> get_lines(int count);

private:
    bool _open_file_if_needed();

private:
    std::vector<std::string> _files;
    std::ifstream _ifs;
    int _cur_file {0};
};

} // namespace paradigm4

#endif //CANNON_PROXY_MODEL_FILES_READER_H
