#include "chip8.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  unsigned short opcode;
  Chip8 emu;
  bool succ = emu.loadGame(argv[1]);
  for (int i = 0; i < 700; i++)
    emu.executeNextOpcode();
  for (int i = 0; i <= 32 - 1; i++)
  {
    for (int j = 0; j <= 64 - 1; j++)
      cout << static_cast<int>(emu.screen[j][i]);
    cout << endl;
  }
}
