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

// getDirStats() computes stats about directory dir_name
// if successful, it return true and stores the results in 'res'
// on failure, it returns false, and res is in undefined state

//(a) largest file in the directory,
//(b) size of the largest file,
//(c) total number of files in the directory,
//(d) total number of subdirectories,
//(e) sum of all file sizes,
//(f) lit of 5 most common file types,
//(g) list of 5 largest groups of duplicate files
bool
getDirStats(const std::string & dir_name, Results & res)
{
  std::string largest_file_path; // path of the largest file in the directory
  long largest_file_size = -1; // size of the largest file
  long n_files = 0; // total number of files in the directory (recursive)
  long n_dirs = 0; // total number of directories in the directory (recursive)
  long all_files_size = 0; // cumulative size of all files
  std::vector<std::string> most_common_types; // 5 most common file types, as reported by the file utility,
  std::vector<std::vector<std::string>> duplicate_files; // list of groups of duplicate files (top 5)

  std::vector<std::string> stack;
  stack.push_back(dir_name.c_str());
  while(!stack.empty()){
    auto dirname = stack.back();
    stack.pop_back();

    printf("%s\n", dirname.c_str());

    DIR * dir = opendir(dirname.c_str());
    if(dir){
      while(1){
        n_dirs =+ 1;
        dirent * de = readdir(dir);
        if (!de) break;
        std::string name = de -> d_name;

        if(name == "." || name == "..") continue;
        std::string path = dirname + "/" + de -> d_name;

        // n_files =+ 1;
        // all_files_size =+ de -> d_name.size();
        // if(de -> d_name.size() > largest_file_size){
        //   largest_file_path = path;
        //   largest_file_size = de -> d_name.size();
        // }

        stack.push_back(path);
      }
      closedir(dir);
    }
  }
return true;

}
