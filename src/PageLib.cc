#include "PageLib.h"

#include "Configuration.h"

using namespace tinyxml2;

#include <fstream>
#include <iostream>
#include <regex>
using std::cerr;

PageLib::PageLib()
    : _dirScanner(), _pages(_dirScanner.getFiles()), _offsetLib() {}

void PageLib::create() {
  // 遍历_pages访问语料
  // 首先用tinyxml2进行清洗，生成新的网页库
  // 然后生成新的网页偏移库
  // for (auto &path : _pages) {
  //   cerr << "path = " << path << "\n";
  // }
  cerr << "开始构建网页库\n";
  createWebLib();
  cerr << "构建网页库结束\n";
}

void PageLib::store(PageMsg &pm) {
  std::ofstream ofs(Configuration::getInstance()->page("save", "page.lib"),
                    std::ios::app);
  if (!ofs) {
    cerr << "create/open page.lib failed!\n";
    return;
  }
  ofs << "<doc>\n";
  ofs << "\t<docid>" << pm._docid << "</docid>\n";
  ofs << "\t<title>" << pm._title << "</title>\n";
  ofs << "\t<link>" << pm._link << "</link>\n";
  ofs << "\t<content>" << pm._content << "</content>\n";
  ofs << "</doc>\n";
  ofs.close();
}

void PageLib::createWebLib() {
  std::ofstream ofs(Configuration::getInstance()->page("save", "page.lib"));
  if (!ofs) {
    cerr << "create/open page.lib failed!\n";
    return;
  }
  ofs.close();

  size_t docid = 0;
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
      if (pm._link == "" || pm._title == "" || pm._content == "") {
        // cerr << "path = " << _pages[idx] << ", idx = " << idx << "\n";
        node = node->NextSiblingElement("item");
        continue;
      }

      regexExecute(pm._content);
      regexExecute(pm._title);

      if (pm._link == "" || pm._title == "" || pm._content == "") {
        // cerr << "path = " << _pages[idx] << ", idx = " << idx << "\n";
        node = node->NextSiblingElement("item");
        continue;
      }

      store(pm);
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
