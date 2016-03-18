#include "chip8.h"

int main(int argc, char *argv[])
{
  Chip8 emu;
  bool succ = emu.loadGame(argv[1]);
  cout << succ << endl;
}
