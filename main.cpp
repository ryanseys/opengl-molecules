#include <stdio.h>
#include <iostream>
#include "pugixml.hpp"

int main(void) {
  pugi::xml_document doc;
  if (!doc.load_file("caffeine.cml")) return -1;
  std::cout << "Loaded molecule: " << doc.child("molecule").child_value("name") << std::endl;
  pugi::xml_node atoms = doc.child("molecule").child("atomArray");
  for (pugi::xml_node atom = atoms.child("atom"); atom; atom = atom.next_sibling("atom")) {
    std::cout << "Element: " << atom.attribute("elementType").value();
    std::cout << ", X3: " << atom.attribute("x3").as_float();
    std::cout << ", Y3: " << atom.attribute("y3").as_float();
    std::cout << ", Z3: " << atom.attribute("z3").as_float();
    std::cout << std::endl;
  }
}
