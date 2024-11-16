#include "DirScanner.h"

#include <dirent.h>

#include "Configuration.h"

DirScanner::DirScanner() : _files() {
  traverse(Configuration::getInstance()->page("yuliao", "directory"));
}

void DirScanner::traverse(const string &dir) {
  DIR *dirp = opendir(dir.c_str());
  if (dirp == nullptr) {
    perror("opendir");
    return;
  }
  struct dirent *dp;
  while ((dp = readdir(dirp)) != nullptr) {
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
    } else {
      // cout << "filename = " << dp->d_name << "\n";
      //
      if (dir.back() == '/') {
        _files.push_back(dir + string(dp->d_name));
      } else {
        _files.push_back(dir + '/' + string(dp->d_name));
      }
    }
  }
  closedir(dirp);
}

vector<string> &DirScanner::getFiles() { return _files; }
