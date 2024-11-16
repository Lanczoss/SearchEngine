#include <iostream>

#include "PageLib.h"

int main() {
  // // 使用tinyxml2
  // // 遍历用的节点
  // XMLDocument xmlDocument;
  // XMLError error = xmlDocument.LoadFile("../yuliao/xml/auto.xml");
  // if (error != XML_SUCCESS) {
  //   std::cout << "读取 xml 失败：" << xmlDocument.ErrorStr() << endl;
  //   return EXIT_FAILURE;
  // }
  // XMLElement *node = xmlDocument.FirstChildElement("rss")
  //                        ->FirstChildElement("channel")
  //                        ->FirstChildElement("item");
  // if (node) return EXIT_SUCCESS;
  //
  PageLib pl;
  pl.create();
}
