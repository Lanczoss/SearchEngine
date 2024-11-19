#ifndef WEB_PAGE_H_
#define WEB_PAGE_H_

#include <string>
using std::string;

class WebPage {
 public:
  WebPage(const string &pagePath);
  int getDocID();
  string getDoc();
  string getDocContent();
  void clear();
  size_t getPageLength();
  string getUrl();
  string getTitle();

  // 处理录入的文章
  void processDoc(const int &pos, const int &pageLength);

 private:
  // 整篇文章
  string _doc;
  // docid
  int _docID;
  // 标题
  string _docTitle;
  // 链接
  string _docURL;
  // 内容
  string _docContent;

  string _pagePath;
};

#endif  // !WEB_PAGE_H_
