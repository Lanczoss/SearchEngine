#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
using namespace std;

int main() {
  DIR *dirp = opendir("../yuliao/art/");
  if (dirp == nullptr) {
    perror("opendir");
    return -1;
  }
  struct dirent *dp;
  while ((dp = readdir(dirp)) != nullptr) {
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
    } else {
      cout << "filename = " << dp->d_name << "\n";
    }
  }
  closedir(dirp);
}
