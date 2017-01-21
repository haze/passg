#include <stdio.h>
#include <map>
#include <utility>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <functional>
#include <vector>

void copy_to_clipboard(std::string text) {
  // add linux support later.
  system(("echo \"" + text + "\" | pbcopy").c_str());
}

std::map<std::string, std::function<std::string()>> modes;

void lower(std::string& str) { std::transform(str.begin(), str.end(), str.begin(), ::tolower); }

void upper(std::string& str) { std::transform(str.begin(), str.end(), str.begin(), ::toupper); }


std::string ambigious() { return "!@#$%^&*()_+-=,./\\;:[]{}|~"; }
std::string alphabet() { return "abcdefghijklmnopqrstuvwxyz"; }
std::string alphabet_caps() {
  std::string up_alpha = alphabet();
  upper(up_alpha);
  return alphabet() + up_alpha;
}

std::string char_to_string(char ch) {
  std::stringstream ss;
  ss << ch;
  return ss.str();
}

std::string generate_password(std::string mode, int length) {
  std::stringstream password;
  for(int i = 0; i < length; i++) {
    auto func = modes.find(mode)->second;
    password << func();
  }
  return password.str();
}

std::string ambigious_char() {
  std::string ambig = ambigious();
  return char_to_string(ambig.at(std::rand() % ambig.length()));
}

std::string random_num() {
  return std::to_string(std::rand() % 9);
}

std::string random_char() {
  std::string afull = alphabet_caps();
  return char_to_string(afull.at(std::rand() % afull.length()));
}

// God i love programming
template< class T, class V > // todo: type constraints for map
std::vector<T> keys_from(std::map<T, V> map) {
  std::vector<T> keys;
  for(auto it = map.begin(); it != map.end(); ++it)
    keys.push_back(it->first);
  return keys;
}


std::string generate_mode_str() {
  std::stringstream ss;
  auto keys = keys_from(modes);
  ss << "[";
  for(auto it = keys.begin(); it != keys.end(); it++)
    if(std::distance(keys.begin(), it) == keys.size() - 1)
      ss << *it;
    else
      ss << *it << "|";
  ss << "]";
  return ss.str();
}

int main(int argc, char **argv) {
    modes.insert(std::pair<std::string, std::function<std::string()>>(std::string("al"), [](){ return random_char(); }));
    modes.insert(std::pair<std::string, std::function<std::string()>>(std::string("di"), [](){ return random_num(); }));
    modes.insert(std::pair<std::string, std::function<std::string()>>(std::string("am"), [](){ return ambigious_char(); }));
    modes.insert(std::pair<std::string, std::function<std::string()>>(std::string("aldi"), [](){ switch(std::rand() % 2){
            case 0:
              return random_num();
            case 1:
              return random_char();
          }
          return random_char();} ));
    modes.insert(std::pair<std::string, std::function<std::string()>>(std::string("all"), [](){
            switch (std::rand() % 3) {
              case 0:
                return random_num();
              case 1:
                return random_char();
              case 2:
                return ambigious_char();
            }
            std::cout << "i hit here!" << std::endl;
            return random_char(); // default to char.
          }));
  if (argc != 3 && argc != 4) {
    std::cout << "please use this format: ./pwg " << generate_mode_str() << " [0..infinity] {c/C}" << std::endl;
  } else {
    std::srand( std::time(NULL) );
    int length = std::atoi(argv[2]);
    std::string pw_type(argv[1]);
    lower(pw_type);

    if(modes.find(pw_type) == modes.end()) {
      std::cout << "invalid option: \"" << pw_type << "\"" << std::endl;
      return 1;
    } // avoid else !

    std::string password = generate_password(pw_type, length);
    if(argc == 4 && (strcmp(argv[3], "c") == 0 || strcmp(argv[3], "C") == 0)) {
      copy_to_clipboard(password);
    } else if(argc == 3)
      std::cout << password << std::endl;
    else
      std::cout << "invalid parameter count. (4th parameter is not copy-specified!)" << std::endl;
  }
}
