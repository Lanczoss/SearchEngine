#ifndef DIR_SCANNER_H_
#define DIR_SCANNER_H_

#include <string>
#include <vector>
using std::string;
using std::vector;

class DirScanner {
 public:
  DirScanner();
  vector<string>& getFiles();
  void traverse(const string& dir);

 private:
  vector<string> _files;
};

#endif  // !DIR_SCANNER_H_
