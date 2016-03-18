#include "chip8.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

ChipEmu::ChipEmu()
{

}

void ChipEmu::init()
{
  PC = 0x200;
  SP = 0;
}

bool ChipEmu::loadGame(const char *filename)
{
  ifstream file(filename, ios::in | ios::binary | ios::ate);
  if (file)
  {
    ifstream::pos_type size;
    size = file.tellg();
    file.seekg(0, ios::beg);
    file.read(&memory[0x200], size);
    file.close();
    return true;
  }
  return false;
}
