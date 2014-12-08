#ifndef RYAN_MOLECULE
#define RYAN_MOLECULE

#include <algorithm>
#include "ryan_atom.h"
#include "ryan_bond.h"
#include "pugixml.hpp"

bool BothAreSpaces(char lhs, char rhs) {
  return (lhs == rhs) && (lhs == ' ');
}

std::string trim(std::string const& str) {
  std::size_t first = str.find_first_not_of(' ');
  std::size_t last  = str.find_last_not_of(' ');
  std::string newStr = str.substr(first, last-first+1);

  std::string::iterator new_end = std::unique(newStr.begin(), newStr.end(), BothAreSpaces);
  newStr.erase(new_end, newStr.end());
  return newStr;
}

class Molecule {
protected:
  std::vector<Atom> atom_list;
  std::vector<Bond> bond_list;
  const GLfloat ATOM_RADIUS = 0.7;
  GLfloat rotateXAmt = 0;
  GLfloat rotateYAmt = 0;
public:
  Molecule(std::string filename) {
    pugi::xml_document doc;
    if (!doc.load_file(filename.c_str())) {
      printf("Error: Could not load molecule file: %s\n", filename.c_str());
      std::exit(-1);
    }
    atom_list.clear();
    bond_list.clear();
    std::string name = doc.child("molecule").child_value("name");

    if(name.length() > 0) {
      glutSetWindowTitle(name.c_str());
    } else {
      glutSetWindowTitle(filename.c_str());
    }

    std::map<std::string, Atom> atom_map;

    // parse all atoms
    pugi::xml_node atoms = doc.child("molecule").child("atomArray");
    for (pugi::xml_node atom = atoms.child("atom"); atom; atom = atom.next_sibling("atom")) {
      GLfloat x = atom.attribute("x3").as_float();
      GLfloat y = atom.attribute("y3").as_float();
      GLfloat z = atom.attribute("z3").as_float();
      std::string id = atom.attribute("id").as_string();

      std::string atomType = atom.attribute("elementType").as_string();

      Atom tempAtom(ATOM_RADIUS, x, y, z, atomType);
      atom_map.insert(std::pair<std::string, Atom>(id, tempAtom));
      atom_list.push_back(tempAtom);
    }

    // parse all bonds
    pugi::xml_node bonds = doc.child("molecule").child("bondArray");
    for (pugi::xml_node bond = bonds.child("bond"); bond; bond = bond.next_sibling("bond")) {
      std::string refAtoms = trim(bond.attribute("atomRefs2").as_string());

      std::string delimiter = " ";

      int end = refAtoms.find(delimiter, 0);
      std::string token1 = refAtoms.substr(0, end);
      std::string token2 = refAtoms.substr(end+1, refAtoms.find(delimiter, refAtoms.length()-1));

      int order = bond.attribute("order").as_int();

      Bond tempBond(atom_map.find(token1)->second, atom_map.find(token2)->second, order);

      bond_list.push_back(tempBond);
    }
    printf("Loaded molecule: %s\n", filename.c_str());
  }

  ~Molecule() {
    // destructor
  }

  void rotateX(GLfloat amount) {
    rotateXAmt = amount;
  }

  void rotateY(GLfloat amount) {
    rotateYAmt = amount;
  }

  void draw(GLuint shaderProg) {
    for(std::vector<Atom>::iterator atom = atom_list.begin(); atom != atom_list.end(); ++atom) {
      atom->rotateY(rotateYAmt, 0);
      atom->rotateX(rotateXAmt, 0);
      atom->draw(shaderProg);
    }

    for(std::vector<Bond>::iterator bond = bond_list.begin(); bond != bond_list.end(); ++bond) {
      bond->rotateY(rotateYAmt, 0);
      bond->rotateX(rotateXAmt, 0);
      bond->draw(shaderProg);
    }
  }
};

#endif
