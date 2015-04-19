/*
* Copyright 2014 Chatsiri Rattana.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*  Titles																								Authors					 Date
 *
 */

#include "boost/filesystem.hpp"

#include "file_handler.hpp"

namespace utils
{

    template<typename FileType>
    bool file_handler<FileType>::file_read()
    {

        if(stat(file_path, &file_status) != 0 || S_ISDIR(file_status.st_mode)) {
            throw std::runtime_error("File cannot check status");
            return false;
        }

        p_file = fopen(file_path,"r");

        if(p_file == NULL) {
            throw std::runtime_error("File cannot open");
            return false;
        }

        return true;
    }
    template<typename FileType>
    bool file_handler<FileType>::file_read_mapped()
    {

        if(stat(file_path, &file_status) != 0 || S_ISDIR(file_status.st_mode)) {
            throw std::runtime_error("File cannot check status");
            return false;
        }

        p_open_file = open(file_path,O_RDONLY);

        if(p_open_file == NULL) {
            throw std::runtime_error("File cannot open");
            return false;
        }

        return true;
    }

    template<typename FileType>
    typename FileType::file_open_ptr&   file_handler<FileType>::get_popen_file()
    {
        return p_open_file;
    }


    template<typename FileType>
    struct stat    *file_handler<FileType>::get_file_status() {
        return &file_status;
    }


    template<typename FileType>
    bool file_handler<FileType>::set_filepath(char const *file_path)
    {
        this->file_path = file_path;

        if(this->file_path != NULL)
            return true;

        return false;
    }

    template<typename FileType>
    typename FileType::file_ptr *file_handler<FileType>::get_file() const
    {
        return p_file;
    }

    template<typename FileType>
    bool file_handler<FileType>::get_fdetail_create()
    {
        return false;
    }

    template<typename FileType>
    bool file_handler<FileType>::close_file()
    {
        close(p_open_file);
        return true;
    }

    template<typename FileType>
    bool file_handler<FileType>::file_full_path(const char *dir_path)
    {
        //boot file system.
        if(boost::filesystem::exists(dir_path)) {
            //regular file
            if(boost::filesystem::is_regular_file(dir_path))
                file_path_vec.push_back(dir_path);
            else if(boost::filesystem::is_directory(dir_path)) {
                boost::filesystem::directory_iterator dir_end;
                typename boost::filesystem::directory_iterator iter_dir;

                //get all path from dir
                for(boost::filesystem::directory_iterator iter_dir(dir_path);
                        iter_dir != dir_end;
                        ++iter_dir) {
                    file_path_vec.push_back(iter_dir->path().string());
                    logger->write_info("Get File dir", iter_dir->path().string());
                }//for
            }//else if
            else {
                logger->write_info("Not file on path or empty file");
            }//else
        } else {
            logger->write_info("File not exist");
        }//else

    }

    template<typename FileType>
    std::vector<std::string> file_handler<FileType>::get_full_path()
    {
        return file_path_vec;
    }

    template<typename FileType>
    bool file_handler<FileType>::write_file(std::vector<char>& buffer_vec)
    {
        std::ofstream outfile(file_path, std::ios::out | std::ios::binary);
        outfile.write(&buffer_vec[0], buffer_vec.size());
    }

    template class file_handler<common_filetype>;

    //____________________________ File stream handler ________________________________________________
    //file_strem
    template<typename FileType>
    bool file_stream_handler<FileType>::file_read()
    {
        file_stream_read.open(this->file_path);

        if(file_stream_read.good()) {

        }

        return true;
    }

    template<typename FileType>
    bool file_stream_handler<FileType>::set_filepath(char const *file_path)
    {
        this->file_path = file_path;

        if(this->file_path != NULL)
            return true;

        return false;
    }

		template<typename FileType>
    typename FileType::file_ptr *file_stream_handler<FileType>::get_file()const 
    {
        return 0;
    }

    template<typename FileType>
    bool file_stream_handler<FileType>::read_all_line(std::vector<std::string> & str_line_vec)
    {
        std::string line;

        str_line_vec.clear();

        while(std::getline(file_stream_read,line)) {
            str_line_vec.push_back(line);
        }
				
				if(str_line_vec.empty())
					return false;						

        return true;
    }

    template class file_stream_handler<common_filetype>;



}
