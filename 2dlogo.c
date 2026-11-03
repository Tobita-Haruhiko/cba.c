#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define WIDTH  101
#define HEIGHT 50
#define PI 3.14159265f

int main() {
  char output[HEIGHT][WIDTH];
  float distanceBuffer[HEIGHT][WIDTH];

  int centerIndexW = (WIDTH - 1) / 2;
  int centerIndexH = HEIGHT / 2;

  float rotX = 0;
  float rotZ = 0;
  float deltaRotX = 0;
  float deltaRotZ = 0.03;


  float zoom = 200;
  float camZPos = 5;

  float lx = 0;
  float ly = 1 / sqrt(2);
  float lz = 1 / sqrt(2);

  float dr = 0.01;
  float dt = 0.02;
  float dz = 0.01;

  printf("\x1b[2J");
  for (;;) {
    memset(output, ' ', sizeof output);
    for (int i = 0; i < HEIGHT; i++) {
      output[i][WIDTH - 1] = '\n';
    }

    for (int i = 0; i < HEIGHT; i++) {
      for (int j = 0; j < WIDTH; j++) {
        distanceBuffer[i][j] = 200;
      }
    }

    rotX += deltaRotX;
    rotZ += deltaRotZ;
    float cosRotX = cos(rotX);
    float sinRotX = sin(rotX);
    float cosRotZ = cos(rotZ);
    float sinRotZ = sin(rotZ);

    float t1; float t2; float r1; float r2; float centerX; float centerY;
    for (int arcType = 0; arcType < 4; arcType++) {
      switch (arcType) {
      case 0: t1 = 0.524; t2 = 5.603; r1 = 0.93; r2 = 1.07; centerX = 0; centerY = 0; break;
      case 1: t1 = 0.519; t2 = 5.798; r1 = 0.7325; r2 = 0.8175; centerX = 0; centerY = 0; break;
      case 2: t1 = 4.704; t2 = 6.479; r1 = 0.8105; r2 = 0.9215; centerX = -1.022; centerY = 0.587; break;
      case 3: t1 = -2.234; t2 = -1.653; r1 = 0.4765; r2 = 0.5615; centerX = 0.985; centerY = 0.012; break;
      }

      for (float t = t1; t < t2; t += dt) {
        for (float r = r1; r < r2; r += dr) {
          float x = r * cos(t) + centerX;
          float y = r * sin(t) + centerY;

          float nX = cos(t) / sqrt(2);
          float nY = sin(t) / sqrt(2);
          float nZ = 1 / sqrt(2);

          for (float z = 0; z < 0.12; z += dz) {
            float yRot1 = y * cosRotX - z * sinRotX; 
            float zRot1 = y * sinRotX + z * cosRotX; 

            float xRot2 = x * cosRotZ - yRot1 * sinRotZ; 
            float yRot2 = x * sinRotZ + yRot1 * cosRotZ;

            float distance = camZPos - zRot1;

            int i = (int)(centerIndexH - 0.5 * yRot2 / distance * zoom);
            int j = (int)(xRot2 / distance * zoom + centerIndexW);

            float nYRot1 = nY * cosRotX - nZ * sinRotX; 
            float nZRot1 = nY * sinRotX + nZ * cosRotX; 

            float nXRot2 = nX * cosRotZ - nYRot1 * sinRotZ; 
            float nYRot2 = nX * sinRotZ + nYRot1 * cosRotZ;

            float lightStrength = lx * nXRot2 + ly * nYRot2 + lz * nZRot1;
            int charIndex = (int) (lightStrength > 0 ? lightStrength * 8 : 0);

            if (0 <= i && i < HEIGHT && 0 <= j && j < (WIDTH - 1)) {
              if (distance < distanceBuffer[i][j]) {
                distanceBuffer[i][j] = distance;
                output[i][j] = ".:-=+*#%@"[charIndex];
              }
            }
          }
        }
      }
    }    

    printf("\x1b[H"); 
    for (int i = 0; i < HEIGHT; i++) {
      for (int j = 0; j < WIDTH; j++)
      {
        putchar(output[i][j]);
      }
    }

    usleep(30000);
  }
}