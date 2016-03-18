#include "chip8.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

Chip8::Chip8()
{
  init();
}

void Chip8::init()
{
  PC = 0x200;
  SP = 0;
  I = 0;
  delay_timer = 0;
  sound_timer = 0;

  for (int i = 0; i < 32; i++)
    for (int j = 0; j < 64; j++)
      screen[j][i] = 0;
  
  for (int i = 0; i < 16; i++)
  {
    V[i] = 0;
  }

  for (int i =0; i < 4096; i++)
  {
    memory[i] = 0;
  }

  unsigned char font[16*5] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0,	// 0
		0x20, 0x60, 0x20, 0x20, 0x70,	// 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0,	// 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0,	// 3
		0x90, 0x90, 0xF0, 0x10, 0x10,	// 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0,	// 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0,	// 6
		0xF0, 0x10, 0x20, 0x40, 0x40,	// 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0,	// 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0,	// 9
		0xF0, 0x90, 0xF0, 0x90, 0x90,	// A
		0xE0, 0x90, 0xE0, 0x90, 0xE0,	// B
		0xF0, 0x80, 0x80, 0x80, 0xF0,	// C
		0xE0, 0x90, 0x90, 0x90, 0xE0,	// D
		0xF0, 0x80, 0xF0, 0x80, 0xF0,	// E
		0xF0, 0x80, 0xF0, 0x80, 0x80	// F
	};

  for (int i = 0; i < 80; i++)
  {
    memory[i] = font[i];
  }
}

bool Chip8::loadGame(const char *filename)
{
  ifstream file(filename, ios::in | ios::binary | ios::ate);
  if (file)
  {
    ifstream::pos_type size;
    size = file.tellg();
    file.seekg(0, ios::beg);
    file.read(reinterpret_cast<char*>(&memory[0x200]), size);
    file.close();
    return true;
  }
  return false;
}

void Chip8::drawSprite(unsigned char X, unsigned char Y, unsigned char N)
{
  V[0xF] = 0;
  unsigned char sprite;
  for (int y = 0; y < N; y++)
  {
    sprite = memory[I + y];
    for (int x = 0; x < 8; x++)
    {
      if ((sprite & (0x80 >> x)) != 0)
      {
        if ((V[X] + x) < 64 && (V[Y] + y) < 32 && (V[X] + x) >= 0 && (V[Y] + y) >= 0)
        {
          if (screen[V[X]][V[Y] + y] == 1)
            V[0xF] = 1;
          screen[V[X] + x][V[Y] + y] ^= 1;
        }
      }
    }
  }
}

void Chip8::executeNextOpcode()
{
  opcode = (memory[PC]<<8) + memory[PC+1];
  PC += 2;
  unsigned char tmp;
  short tmp_i;

  switch ((opcode & 0xF000)>>12)
  {
    case 0x1:
      PC = opcode & 0x0FFF;
      break;
    case 0x4:
      if (V[((opcode & 0x0F00)>>8)] != (opcode & 0x00FF))
      {
        PC += 2;
      }
      break;
    case 0x6:
      V[((opcode & 0x0F00)>>8)] = opcode & 0x00FF;
      break;
    case 0x7:
      V[((opcode & 0x0F00)>>8)] += opcode & 0x00FF;
      break;
    case 0x8:
      switch (opcode & 0x000F)
      {
        case 0x4:
          tmp_i = static_cast<int>(V[((opcode & 0x0F00) >> 8)]) + static_cast<int>(V[((opcode & 0x00F0) >> 4)]);
          if (tmp_i > 255)
            V[0xF] = 1;
          else
            V[0xF] = 0;
          V[((opcode & 0x0F00) >> 8)] = tmp_i;
          break;
      }
      break;
    case 0xA:
      I = opcode & 0x0FFF;
      break;
    case 0xD:
      drawSprite(((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), (opcode & 0x000F));
      break;
    case 0xF:
      switch (opcode & 0x00FF)
      {
        case 0x15:
          delay_timer = V[((opcode & 0x0F00) >> 8)];
          break;
      }
      break;
    default:
      cerr << "Unknown opcode: 0x" << hex << opcode << endl;
      cerr << "PC: 0x" << hex << PC << endl;
      exit(1);
  }
}

unsigned short Chip8::test()
{
  opcode = (memory[PC]<<8) + memory[PC+1];
  return opcode;
}
