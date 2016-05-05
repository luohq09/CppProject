//
// Created by LuoHuaqing on 4/14/16.
//

#include "model_files_reader.h"

namespace paradigm4 {

ModelFilesReader::ModelFilesReader(const std::vector<std::string> &files) : _files(files)
{
}

ModelFilesReader::ModelFilesReader(std::vector<std::string> &&files) : _files(std::move(files))
{
}

bool ModelFilesReader::_open_file_if_needed() {
    if (_ifs.is_open()) return true;
    const int file_num = (int)_files.size();
    while (_cur_file < file_num) {
        _ifs.clear();
        _ifs.open(_files[_cur_file].c_str());
        if (_ifs.is_open()) {
            return true;
        } // ToDo: log else case

        ++_cur_file;
    }

    return false;
}

std::vector<std::string> ModelFilesReader::get_lines(int count) {
    std::vector<std::string> ret;
    if (count < 0) return ret;
    while (_open_file_if_needed() && (int)ret.size() < count) {
        if ((int)ret.capacity() < count) {
            ret.reserve(count);
        }

        std::string line;
        while ((int)ret.size() < count && std::getline(_ifs, line)) {
            if (line.length() > 0) {
                ret.push_back(std::move(line));
            }
        }

        if (!_ifs.good()) {
            // ToDo: log the bad case (need to check _ifs's state bits)
            _ifs.clear();
            _ifs.close();
            ++_cur_file;
        }
    }

    return ret;
}

} // namespace paradigm4