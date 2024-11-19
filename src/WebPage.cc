#include "WebPage.h"

#include "Configuration.h"
#include "tinyxml2.h"
using namespace tinyxml2;

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
using std::cerr;
using std::unique_ptr;

WebPage::WebPage(const string &pagePath)
    : _doc(),
      _docID(),
      _docTitle(),
      _docURL(),
      _docContent(),
      _pagePath(pagePath) {}

void WebPage::clear() {
  _doc.clear();
  _docID = -1;
  _docTitle.clear();
  _docURL.clear();
  _docContent.clear();
}

void WebPage::processDoc(const int &pos, const int &pageLength) {
  std::ifstream ifs(_pagePath);
  if (!ifs) {
    cerr << "open page.dat failed!\n";
    return;
  }

  ifs.seekg(pos);
  unique_ptr<char[]> buf(new char[pageLength + 1]());
  ifs.read(buf.get(), pageLength);

  _doc = buf.get();
  XMLDocument xmlDocument;
  xmlDocument.Parse(buf.get());
  XMLElement *node = xmlDocument.FirstChildElement("doc");
  if (node->FirstChildElement("docid")) {
    if (node->FirstChildElement("docid")->GetText()) {
      _docID = std::stoi(node->FirstChildElement("docid")->GetText());
    }
  }
  if (node->FirstChildElement("title")) {
    if (node->FirstChildElement("title")->GetText()) {
      _docTitle = node->FirstChildElement("title")->GetText();
    }
  }
  if (node->FirstChildElement("link")) {
    if (node->FirstChildElement("link")->GetText()) {
      _docURL = node->FirstChildElement("link")->GetText();
    }
  }
  if (node->FirstChildElement("content")) {
    if (node->FirstChildElement("content")->GetText()) {
      _docContent = node->FirstChildElement("content")->GetText();
    }
  }
  // cerr << "id = " << _docID << '\n';
  // cerr << "doc = " << _doc << '\n';

  // cerr << "title = " << _docTitle << '\n';

  // cerr << "URL = " << _docURL << '\n';

  // cerr << "content = " << _docContent << '\n';
  ifs.close();
}

string WebPage::getDoc() { return _doc; }

int WebPage::getDocID() { return _docID; }

string WebPage::getDocContent() { return _docContent; }

string WebPage::getUrl() { return _docURL; }

string WebPage::getTitle() { return _docTitle; }
