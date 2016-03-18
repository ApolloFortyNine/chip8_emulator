#ifndef CHIP_EMU_H
#define CHIP8_H
class Chip8
{
private:
  unsigned char V[16];
  unsigned char memory[4096];
  unsigned short stack[16];
  unsigned char delay_timer;
  unsigned char sound_timer;
  unsigned short I;
  unsigned short PC;
  unsigned short opcode;
  unsigned short SP;

public:
  unsigned bool key[16];
  unsigned char screen[64][32];
  Chip8();
  void init();
  void executeNextOp();
  bool loadGame(const char *filename);
  void decreaseTimers();

}
#endif
