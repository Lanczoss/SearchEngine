#include "PageLib.h"

#include "Configuration.h"

using namespace tinyxml2;
using std::ifstream;
using std::ofstream;

#include <iostream>
#include <regex>
using std::cerr;

PageLib::PageLib()
    : _dirScanner(), _pages(_dirScanner.getFiles()), _offsetLib() {}

void PageLib::create() {
  // 遍历_pages访问语料
  // 首先用tinyxml2进行清洗，生成新的网页库
  // 然后生成新的网页偏移库
  cerr << "开始构建网页库\n";
  createWebLib();
  cerr << "构建网页库结束\n";

  cerr << "开始构建网页偏移库\n";
  createOffsetLib();
  cerr << "构建网页偏移库结束\n";

  showTwoPage();
}

void PageLib::store(ofstream &ofs, PageMsg &pm, size_t &position) {
  ofs << "<doc>\n";
  ofs << "\t<docid>" << pm._docid << "</docid>\n";
  ofs << "\t<title>" << pm._title << "</title>\n";
  ofs << "\t<link>" << pm._link << "</link>\n";
  ofs << "\t<content>" << pm._content << "</content>\n";
  ofs << "</doc>\n";
  int pageLength = string(
                       "<doc>\n\t<docid></docid>\n\t<title></title>\n\t<link></"
                       "link>\n\t<content></content>\n</doc>\n")
                       .size() +
                   std::to_string(pm._docid).size() + pm._link.size() +
                   pm._content.size() + pm._title.size();
  _offsetLib[pm._docid] = std::make_pair(position, pageLength);
  position += pageLength;
}

void PageLib::createWebLib() {
  // 刷新网页库文件
  ofstream upofs(Configuration::getInstance()->page("save", "page.lib"));
  if (!upofs) {
    cerr << "create/open page.lib failed!\n";
    return;
  }
  upofs.close();

  // 追加模式
  ofstream ofs(Configuration::getInstance()->page("save", "page.lib"),
               std::ios::app);
  if (!ofs) {
    cerr << "create/open page.lib failed!\n";
    return;
  }

  size_t docid = 0;
  size_t position = 0;
  for (size_t idx = 0; idx < _pages.size(); ++idx) {
    PageMsg pm;
    XMLDocument xmlDocument;
    // cerr << "path = " << _pages[idx] << ", idx = " << idx << "\n";
    XMLError error = xmlDocument.LoadFile(_pages[idx].c_str());
    if (error != XML_SUCCESS) {
      std::cout << "读取 xml 失败：" << xmlDocument.ErrorStr() << '\n';
      return;
    }

    XMLElement *node = xmlDocument.FirstChildElement("rss")
                           ->FirstChildElement("channel")
                           ->FirstChildElement("item");
    while (node) {
      pm._docid = docid;
      pm._title = node->FirstChildElement("title")->GetText();
      pm._link = node->FirstChildElement("link")->GetText();
      if (node->FirstChildElement("content")) {
        if (node->FirstChildElement("content")->GetText()) {
          pm._content = node->FirstChildElement("content")->GetText();
        }
      } else if (node->FirstChildElement("description")) {
        if (node->FirstChildElement("description")->GetText()) {
          pm._content = node->FirstChildElement("description")->GetText();
        }
      }

      regexExecute(pm._content);
      regexExecute(pm._title);

      // if (pm._link == "" || pm._title == "" || pm._content == "" ) {
      //   // cerr << "path = " << _pages[idx] << ", idx = " << idx << "\n";
      //   node = node->NextSiblingElement("item");
      //   continue;
      // }

      store(ofs, pm, position);
      node = node->NextSiblingElement("item");
      docid++;
    }
  }
}

void PageLib::regexExecute(string &input) {
  if (input == "") {
    return;
  }
  // 正则表达式匹配 HTML/XML 标签
  std::regex tagRegex("<[^>]*>");
  // 正则表达式匹配 HTML 实体（如 &nbsp;）
  std::regex entityRegex("&.*?;");
  // 小于号
  std::regex ltRegex("<");
  // 大于号
  std::regex gtRegex(">");

  // 使用 regex_replace 去掉标签
  input = std::regex_replace(input, tagRegex, "");
  if (input == "") {
    return;
  }

  // 使用 regex_replace 去掉 HTML 实体
  input = std::regex_replace(input, entityRegex, "");
  if (input == "") {
    return;
  }

  input = std::regex_replace(input, ltRegex, "&lt;");
  input = std::regex_replace(input, gtRegex, "&gt;");
}

void PageLib::createOffsetLib() {
  // 创建偏移库
  ofstream ofs(Configuration::getInstance()->page("save", "offset.lib"));
  if (!ofs) {
    cerr << "create offset.lib failed!\n";
    return;
  }
  // for (auto &offset : _offsetLib) {
  //   ofs << offset.first << " " << offset.second.first << " "
  //       << offset.second.second << '\n';
  // }

  ofs.close();
}

void PageLib::showTwoPage() {
  ifstream ifs(Configuration::getInstance()->page("save", "page.lib"));
  if (!ifs) {
    cerr << "open failed!\n";
    return;
  }
  for (size_t idx = 0; idx < 3; ++idx) {
    ifs.seekg(_offsetLib[idx].first);
    int pageLength = _offsetLib[idx].second;
    char *buff = new char[pageLength + 1]();
    ifs.read(buff, pageLength);
    cerr << buff;
    delete[] buff;
  }

  ifs.close();
}
