#include "getDirStats.h"
#include "digester.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

static bool
is_dir( const std::string & path)
{
  struct stat buff;
  if( 0 != stat( path.c_str(), & buff))
    return false;
  return S_ISDIR(buff.st_mode);
}

bool
getDirStats(const std::string & dir_name, Results & res)
{

  res.largest_file_path; // path of the largest file in the directory
  res.largest_file_size = -1; // size of the largest file
  res.n_files = 0; // total number of files in the directory (recursive)
  res.n_dirs = 0; // total number of directories in the directory (recursive)
  res.all_files_size = 0; // cumulative size of all files
  // res.most_common_types; // 5 most common file types, as reported by the file utility,
  // res.duplicate_files; // list of groups of duplicate files (top 5)
  struct stat st;
  // if(stat(dir_name.c_str(), &st) != 0) {
  // }

  std::vector<std::string> stack;
  stack.push_back(dir_name.c_str());
  while(!stack.empty()){
    auto dirname = stack.back();
    stack.pop_back();

    //printf("%s\n", dirname.c_str());

    DIR * dir = opendir(dirname.c_str());
    if(dir){
      while(1){
        dirent * de = readdir(dir);
        if (!de) break;
        std::string name = de -> d_name;

        if(name == "." || name == "..") continue;
        std::string path = dirname + "/" + de -> d_name;
        //printf("%s\n", path.c_str());
        //printf("%s\n", name.c_str());

        if(stat(path.c_str(), &st) == 0){
            if( st.st_mode & S_IFDIR ){ //if directory
              //printf("%s\n", name.c_str());
              res.n_dirs += 1;
            }
            else if( st.st_mode & S_IFREG ){ //if file

            // if (!IsIgnorable(name)){
            //   allFiles.AddRange(Directory.GetFiles(folder, searchPattern, SearchOption.AllDirectories));
            // }
              //printf("%s\n", name.c_str());
              res.n_files += 1;
              int num = stat(path.c_str(), &st);

              if(num == 0){ //if file size
                res.all_files_size += st.st_size;
                //printf("%s\n", st.st_size);

                if(res.largest_file_size < st.st_size){ //if largest file size
                  res.largest_file_path = path.c_str();
                  res.largest_file_size = st.st_size;
                }
              }else{
                //printf("%s\n", "file size error");
              }
            }
            else{
            }
        }

        stack.push_back(path);
      }
      closedir(dir);
    }
  }
return true;

}
