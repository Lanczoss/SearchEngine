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

WebPage::WebPage() : _doc(), _docID(), _docTitle(), _docURL(), _docContent() {}

void WebPage::clear() {
  _doc.clear();
  _docID = -1;
  _docTitle.clear();
  _docURL.clear();
  _docContent.clear();
}

void WebPage::processDoc(const int &pos, const int &pageLength) {
  std::ifstream ifs(
      Configuration::getInstance()->page("save", "page.lib").c_str());
  if (!ifs) {
    cerr << "open page.dat failed!\n";
    return;
  }

  ifs.seekg(pos);
  unique_ptr<char[]> buf(new char[pageLength + 1]());
  ifs.read(buf.get(), pageLength);

  XMLDocument xmlDocument;
  xmlDocument.Parse(buf.get());
  XMLElement *node = xmlDocument.FirstChildElement("doc");
  if (node->FirstChildElement("docid")->GetText() &&
      node->FirstChildElement("title")->GetText() &&
      node->FirstChildElement("link")->GetText() &&
      node->FirstChildElement("content")->GetText()) {
    _docID = std::stoi(node->FirstChildElement("docid")->GetText());
    // cerr << "id = " << _docID << '\n';
    _doc = buf.get();
    // cerr << "doc = " << _doc << '\n';
    _docTitle = node->FirstChildElement("title")->GetText();
    // cerr << "title = " << _docTitle << '\n';
    _docURL = node->FirstChildElement("link")->GetText();
    // cerr << "URL = " << _docURL << '\n';
    _docContent = node->FirstChildElement("content")->GetText();
    // cerr << "content = " << _docContent << '\n';
  }
  ifs.close();
}

string WebPage::getDoc() { return _doc; }

int WebPage::getDocID() { return _docID; }

string WebPage::getDocContent() { return _docContent; }
