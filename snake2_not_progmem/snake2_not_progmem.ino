//e4:65:b8:78:d1:f6
//todo
//in progress - zmiana kolorów snejka
//jeżeli następny ruch to ogon to nie powinno być gameover
//zablokować ruch w przeciwnym kierunku do obecnego DONE
//dodać kolejna matryce która wyświetli punktacje i komunikat gameover
//fud czasami pojawia sie w miejscu gdzie jest snejk przez co jest niewidoczny bo kolor fud nadpisuje sie na kolor bialy
//czy snejk powinien przyśpieszać?

#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <vector>
#include <Bluepad32.h>

#ifndef PSTR
#define PSTR  // Make Arduino Due happy
#endif

TaskHandle_t blinkingBorder;

#define PIN 19          // Define the pin to which your data line is connected
#define NUMPIXELS 1024  // Define the number of LEDs in your strip

#define PIN2 26  // Define the pin to which your data line is connected
//#define VELOCITY 10    // Speed of snake

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix pointsDisplay = Adafruit_NeoMatrix(32, 8, PIN2,
                                                      NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                                                      NEO_GRB + NEO_KHZ800);

const uint16_t colors[] = {
  pointsDisplay.Color(255, 0, 0), pointsDisplay.Color(0, 255, 0), pointsDisplay.Color(0, 0, 255)
};

char currentDirection = ' ';  // Initialize to a non-movement state
char previousDirection = ' ';
int velocity = 0;
bool foodExists = false;
int snakeSize = 0;
int foodPosition = random(0, 1024);
bool gameOver = false;

//screen refresh rate 100 is 10 times a second
unsigned long previousMillis = 0;
const long interval = 100;  // interval in milliseconds (1000 ms = 1 second)

const int sensorPinDown = 16;
const int sensorPinUp = 17;
const int sensorPinLeft = 5;
const int sensorPinRight = 18;

const int snakeIntensity = 25;  // Set Snake's snakeIntensity (brightness) of the LEDs (0-255)
const int borderIntensity = 20;
int red = 256;
int green = 256;
int blue = 256;
int redBrightness;
int greenBrightness;
int blueBrightness;
int z = pointsDisplay.width();
int pass = 0;


const int rows = 35;
const int cols = 35;
int twoDArray[rows][cols] = {
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { -1, 1016, 1015, 1000, 999, 984, 983, 968, 967, 952, 951, 936, 935, 920, 919, 904, 903, 888, 887, 872, 871, 856, 855, 840, 839, 824, 823, 808, 807, 792, 791, 776, 775, -1 },
  { -1, 1017, 1014, 1001, 998, 985, 982, 969, 966, 953, 950, 937, 934, 921, 918, 905, 902, 889, 886, 873, 870, 857, 854, 841, 838, 825, 822, 809, 806, 793, 790, 777, 774, -1 },
  { -1, 1018, 1013, 1002, 997, 986, 981, 970, 965, 954, 949, 938, 933, 922, 917, 906, 901, 890, 885, 874, 869, 858, 853, 842, 837, 826, 821, 810, 805, 794, 789, 778, 773, -1 },
  { -1, 1019, 1012, 1003, 996, 987, 980, 971, 964, 955, 948, 939, 932, 923, 916, 907, 900, 891, 884, 875, 868, 859, 852, 843, 836, 827, 820, 811, 804, 795, 788, 779, 772, -1 },
  { -1, 1020, 1011, 1004, 995, 988, 979, 972, 963, 956, 947, 940, 931, 924, 915, 908, 899, 892, 883, 876, 867, 860, 851, 844, 835, 828, 819, 812, 803, 796, 787, 780, 771, -1 },
  { -1, 1021, 1010, 1005, 994, 989, 978, 973, 962, 957, 946, 941, 930, 925, 914, 909, 898, 893, 882, 877, 866, 861, 850, 845, 834, 829, 818, 813, 802, 797, 786, 781, 770, -1 },
  { -1, 1022, 1009, 1006, 993, 990, 977, 974, 961, 958, 945, 942, 929, 926, 913, 910, 897, 894, 881, 878, 865, 862, 849, 846, 833, 830, 817, 814, 801, 798, 785, 782, 769, -1 },
  { -1, 1023, 1008, 1007, 992, 991, 976, 975, 960, 959, 944, 943, 928, 927, 912, 911, 896, 895, 880, 879, 864, 863, 848, 847, 832, 831, 816, 815, 800, 799, 784, 783, 768, -1 },

  { -1, 512, 527, 528, 543, 544, 559, 560, 575, 576, 591, 592, 607, 608, 623, 624, 639, 640, 655, 656, 671, 672, 687, 688, 703, 704, 719, 720, 735, 736, 751, 752, 767, -1 },
  { -1, 513, 526, 529, 542, 545, 558, 561, 574, 577, 590, 593, 606, 609, 622, 625, 638, 641, 654, 657, 670, 673, 686, 689, 702, 705, 718, 721, 734, 737, 750, 753, 766, -1 },
  { -1, 514, 525, 530, 541, 546, 557, 562, 573, 578, 589, 594, 605, 610, 621, 626, 637, 642, 653, 658, 669, 674, 685, 690, 701, 706, 717, 722, 733, 738, 749, 754, 765, -1 },
  { -1, 515, 524, 531, 540, 547, 556, 563, 572, 579, 588, 595, 604, 611, 620, 627, 636, 643, 652, 659, 668, 675, 684, 691, 700, 707, 716, 723, 732, 739, 748, 755, 764, -1 },
  { -1, 516, 523, 532, 539, 548, 555, 564, 571, 580, 587, 596, 603, 612, 619, 628, 635, 644, 651, 660, 667, 676, 683, 692, 699, 708, 715, 724, 731, 740, 747, 756, 763, -1 },
  { -1, 517, 522, 533, 538, 549, 554, 565, 570, 581, 586, 597, 602, 613, 618, 629, 634, 645, 650, 661, 666, 677, 682, 693, 698, 709, 714, 725, 730, 741, 746, 757, 762, -1 },
  { -1, 518, 521, 534, 537, 550, 553, 566, 569, 582, 585, 598, 601, 614, 617, 630, 633, 646, 649, 662, 665, 678, 681, 694, 697, 710, 713, 726, 729, 742, 745, 758, 761, -1 },
  { -1, 519, 520, 535, 536, 551, 552, 567, 568, 583, 584, 599, 600, 615, 616, 631, 632, 647, 648, 663, 664, 679, 680, 695, 696, 711, 712, 727, 728, 743, 744, 759, 760, -1 },

  { -1, 504, 503, 488, 487, 472, 471, 456, 455, 440, 439, 424, 423, 408, 407, 392, 391, 376, 375, 360, 359, 344, 343, 328, 327, 312, 311, 296, 295, 280, 279, 264, 263, -1 },
  { -1, 505, 502, 489, 486, 473, 470, 457, 454, 441, 438, 425, 422, 409, 406, 393, 390, 377, 374, 361, 358, 345, 342, 329, 326, 313, 310, 297, 294, 281, 278, 265, 262, -1 },
  { -1, 506, 501, 490, 485, 474, 469, 458, 453, 442, 437, 426, 421, 410, 405, 394, 389, 378, 373, 362, 357, 346, 341, 330, 325, 314, 309, 298, 293, 282, 277, 266, 261, -1 },
  { -1, 507, 500, 491, 484, 475, 468, 459, 452, 443, 436, 427, 420, 411, 404, 395, 388, 379, 372, 363, 356, 347, 340, 331, 324, 315, 308, 299, 292, 283, 276, 267, 260, -1 },
  { -1, 508, 499, 492, 483, 476, 467, 460, 451, 444, 435, 428, 419, 412, 403, 396, 387, 380, 371, 364, 355, 348, 339, 332, 323, 316, 307, 300, 291, 284, 275, 268, 259, -1 },
  { -1, 509, 498, 493, 482, 477, 466, 461, 450, 445, 434, 429, 418, 413, 402, 397, 386, 381, 370, 365, 354, 349, 338, 333, 322, 317, 306, 301, 290, 285, 274, 269, 258, -1 },
  { -1, 510, 497, 494, 481, 478, 465, 462, 449, 446, 433, 430, 417, 414, 401, 398, 385, 382, 369, 366, 353, 350, 337, 334, 321, 318, 305, 302, 289, 286, 273, 270, 257, -1 },
  { -1, 511, 496, 495, 480, 479, 464, 463, 448, 447, 432, 431, 416, 415, 400, 399, 384, 383, 368, 367, 352, 351, 336, 335, 320, 319, 304, 303, 288, 287, 272, 271, 256, -1 },

  { -1, 0, 15, 16, 31, 32, 47, 48, 63, 64, 79, 80, 95, 96, 111, 112, 127, 128, 143, 144, 159, 160, 175, 176, 191, 192, 207, 208, 223, 224, 239, 240, 255, -1 },
  { -1, 1, 14, 17, 30, 33, 46, 49, 62, 65, 78, 81, 94, 97, 110, 113, 126, 129, 142, 145, 158, 161, 174, 177, 190, 193, 206, 209, 222, 225, 238, 241, 254, -1 },
  { -1, 2, 13, 18, 29, 34, 45, 50, 61, 66, 77, 82, 93, 98, 109, 114, 125, 130, 141, 146, 157, 162, 173, 178, 189, 194, 205, 210, 221, 226, 237, 242, 253, -1 },
  { -1, 3, 12, 19, 28, 35, 44, 51, 60, 67, 76, 83, 92, 99, 108, 115, 124, 131, 140, 147, 156, 163, 172, 179, 188, 195, 204, 211, 220, 227, 236, 243, 252, -1 },
  { -1, 4, 11, 20, 27, 36, 43, 52, 59, 68, 75, 84, 91, 100, 107, 116, 123, 132, 139, 148, 155, 164, 171, 180, 187, 196, 203, 212, 219, 228, 235, 244, 251, -1 },
  { -1, 5, 10, 21, 26, 37, 42, 53, 58, 69, 74, 85, 90, 101, 106, 117, 122, 133, 138, 149, 154, 165, 170, 181, 186, 197, 202, 213, 218, 229, 234, 245, 250, -1 },
  { -1, 6, 9, 22, 25, 38, 41, 54, 57, 70, 73, 86, 89, 102, 105, 118, 121, 134, 137, 150, 153, 166, 169, 182, 185, 198, 201, 214, 217, 230, 233, 246, 249, -1 },
  { -1, 7, 8, 23, 24, 39, 40, 55, 56, 71, 72, 87, 88, 103, 104, 119, 120, 135, 136, 151, 152, 167, 168, 183, 184, 199, 200, 215, 216, 231, 232, 247, 248, -1 },
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
};

int edgeValues[128] = { 7, 8, 23, 24, 39, 40, 55, 56, 71, 72, 87, 88, 103, 104, 119, 120, 135, 136, 151, 152, 167, 168, 183, 184, 199, 200, 215, 216, 231, 232, 247, 248,
                        1015, 1000, 999, 984, 983, 968, 967, 952, 951, 936, 935, 920, 919, 904, 903, 888, 887, 872, 871, 856, 855, 840, 839, 824, 823, 808, 807, 792, 791, 776, 775,
                        1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023, 512, 513, 514, 515, 516, 517, 518, 519, 504, 505, 506, 507, 508, 509, 510, 511, 0, 1, 2, 3, 4, 5, 6, 7,
                        775, 774, 773, 772, 771, 770, 769, 768, 767, 766, 765, 764, 763, 762, 761, 760, 263, 262, 261, 260, 259, 258, 257, 256, 255, 254, 253, 252, 251, 250, 249, 248 };

char movmentQueue[2] = { 'x', 'y' };
char pressedDirection = 'x';

std::vector<int> sLengthControl = { 11, 8, 12, 8, 13, 8 };
std::vector<int> sResetValues = { 11, 8, 12, 8, 13, 8 };  // Define sLengthControl as a vector
std::vector<int> matrixCopy;                              // Define sLengthControl as a vector

int x = sLengthControl[0];  // Snake's head position column
int y = sLengthControl[1];  // Snake's head position row



void snakeSizeChecker() {
  snakeSize = sLengthControl.size();
}

// Function to calculate brightness based on current value of i
int calculateBorderBrightness(int i) {
  return 256 * i / 255;  // Calculate brightness value between 0 and 255
}

void calculateSnakeBrightness() {
  redBrightness = red * snakeIntensity / 255;
  greenBrightness = green * snakeIntensity / 255;
  blueBrightness = blue * snakeIntensity / 255;
}

void food() {

  if (twoDArray[y][x] == foodPosition) {
    foodExists = false;
  }

  while (!foodExists) {
    foodPosition = random(0, 1024);
    for (int i = 0; i < 128; i++) { //jeśli wylosuje jedzenie na ramce to losuje jeszcze raz aż do skutku
      if (foodPosition == edgeValues[i]) { 
        foodPosition = random(0, 1024);
        i = 0;
      }
    }
    foodExists = true;
    strip.setPixelColor(foodPosition, strip.Color(redBrightness, 0, 0));

  }
}

void createSnake() {
  snakeSizeChecker();  //przy pierwszy przebiegu sprawdza wielkość snake'a (na starcie ma 0 więc warto to aktualizować przed wywołaniem dalszej komendy)

  // Dekrementuje i o 2 poniewaz setuje dwa pixele do matrixa z tabeli ktore sa zawsze obok siebie i oraz i-1
  for (int i = snakeSize - 1; i >= 0; i -= 2) {
    if (i > 1) {
      strip.setPixelColor(twoDArray[sLengthControl[i]][sLengthControl[i - 1]], strip.Color(redBrightness, greenBrightness, blueBrightness));
    } else {
      strip.setPixelColor(twoDArray[sLengthControl[i]][sLengthControl[i - 1]], strip.Color(redBrightness, greenBrightness, blueBrightness));
      //po co to jest dwa razy????????????????????????????????????????????????????????????
    }
  }
}

void turnOffLed(int axisX, int axisY) {
  strip.setPixelColor(twoDArray[axisY][axisX], strip.Color(0, 0, 0));
}

void move() {
  snakeSizeChecker();
  int axisX = sLengthControl[snakeSize - 2];         //zapisuje kolumnę ogona do zmiennej axisX
  int axisY = sLengthControl[snakeSize - 1];         //zapisuje rząd ogona do zmiennej axisY
  sLengthControl.insert(sLengthControl.begin(), y);  //wstawia nowy rząd głowy po wykonanym ruchu do zmeinnej y
  sLengthControl.insert(sLengthControl.begin(), x);  //wstawia nową kolumnę głowy po wykonanym ruchu do zmeinnej x

  if (twoDArray[y][x] == foodPosition) {  //sprawdza, czy głowa = jedzenie. jeśli tak to nie gasi diody tylko powiększa snejka
    snakeSizeChecker();

  } else {
    twoDArray[axisY][axisX] = matrixCopy.back();  //przywraca wartość w matrixie z -1 na oryginalna
    turnOffLed(axisX, axisY);
    sLengthControl.pop_back();  //usuwa y z kontrolnej tabeli dlugosci
    sLengthControl.pop_back();  //usuwa x z kontrolnej tabeli dlugosci
    matrixCopy.pop_back();      //usuwa ostatnią wartość z kopii tymczasowej ponieważ już została przywrócona do matrixa
    snakeSizeChecker();
  }


  food();  //checks if theres food on the board and if not creates it

  strip.setPixelColor(twoDArray[y][x], strip.Color(redBrightness, greenBrightness, blueBrightness));  //zapala kolejna diode
  //strip.show();

  matrixCopy.insert(matrixCopy.begin(), twoDArray[y][x]);  //kopia wartości z matrixa do tablicy tymczasowej
  twoDArray[y][x] = -1;                                    //zmiana w matrixie pozycji glowy na -1
}

bool checkForGameOver() {
  snakeSizeChecker();
  if (twoDArray[y][x] == -1) {
    Serial.println("game over");
    gameOver = true;
    currentDirection = ' ';

    return true;
  }
  return false;
}

void gameReset() {
  int j = 0;
  for (int i = 0; i <= snakeSize - 1; i += 2) {
    twoDArray[sLengthControl[i + 1]][sLengthControl[i]] = matrixCopy[j];                             //przywracamy współrzędne do tablicy 2d z -1 na właściwe
    strip.setPixelColor(twoDArray[sLengthControl[i + 1]][sLengthControl[i]], strip.Color(0, 0, 0));  //gasimy stare diody
    j++;
  }

  matrixCopy.clear();             //czyścimy kopię starych współrzędnych
  sLengthControl = sResetValues;  //przywracamy początkową tabelę snejka
  createSnake();                  //tworzymy snejka na początkowych wartościach
  food();                         //tworzymy jedzonko

  for (int i = 0; i <= snakeSize - 1; i += 2) {  //podmieniamy wspórzędne pozycji snejka na matrixie na -1 i kopiujemy te wartości przed podmianą do matrixCopy
    matrixCopy.push_back(twoDArray[sLengthControl[i + 1]][sLengthControl[i]]);
    twoDArray[sLengthControl[i + 1]][sLengthControl[i]] = -1;  //przez zmiane wartosci na -1 przestanie dzialac turn off - trzeba poprawic
  }

  x = sLengthControl[0];  // Snake's head position column
  y = sLengthControl[1];  // Snake's head position row

  gameOver = false;
  currentDirection == ' ';
}

void move_right() {
  strip.setPixelColor(twoDArray[y][x], strip.Color(redBrightness, greenBrightness, blueBrightness));
  x += 1;
  if (checkForGameOver()) {

  } else move();
}

void move_left() {
  strip.setPixelColor(twoDArray[y][x], strip.Color(redBrightness, greenBrightness, blueBrightness));
  x -= 1;
  if (checkForGameOver()) {

  } else move();
}

void move_up() {
  strip.setPixelColor(twoDArray[y][x], strip.Color(redBrightness, greenBrightness, blueBrightness));
  y -= 1;
  if (checkForGameOver()) {

  } else move();
}

void move_down() {
  strip.setPixelColor(twoDArray[y][x], strip.Color(redBrightness, greenBrightness, blueBrightness));
  y += 1;
  if (checkForGameOver()) {

  } else move();
}

void setDirection(char incomingByte) {
  switch (incomingByte) {
    case 'U':
      if (currentDirection == 'D') {
        break;
      }
      currentDirection = 'U';
      break;
    case 'D':
      if (currentDirection == 'U') {
        break;
      }
      currentDirection = 'D';
      break;
    case 'R':
      if (currentDirection == 'L') {
        break;
      }
      currentDirection = 'R';
      break;
    case 'L':
      if (currentDirection == 'R') {
        break;
      }
      currentDirection = 'L';
      break;
      // Add cases for 'L' and 'R' for Left and Right arrows
  }
}

void performMovement() {
  switch (currentDirection) {
    case 'U':
      move_up();
      break;
    case 'D':
      move_down();
      break;
    case 'R':
      move_right();
      break;
    case 'L':
      move_left();
      break;
    // Add cases for 'L' and 'R' for Left and Right arrows
    default:
      // Stop movement or perform other actions for the default state
      break;
  }
}

void printMatrix() {
  for (int i = 0; i < 34; i++) {
    Serial.println();
    for (int j = 0; j < 34; j++) {
      if (j == 0) {
        Serial.print(twoDArray[i][j]);
        Serial.print("   ");
      } else if (j == 33) {
        Serial.print("   ");
        Serial.print(twoDArray[i][j]);
        Serial.print(";");
      } else {
        Serial.print(twoDArray[i][j]);
        if (j != 32) {
          Serial.print(", ");
        }
      }
    }
    if (i % 8 == 0) {
      Serial.println();
    }
  }
}



//controller settings
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println("CALLBACK: Controller connected, but could not found empty slot");
  }
}


void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

  if (!foundController) {
    Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
  }
}

void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
    "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
    "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
    ctl->index(),        // Controller Index
    ctl->dpad(),         // D-pad
    ctl->buttons(),      // bitmask of pressed buttons
    ctl->axisX(),        // (-511 - 512) left X Axis
    ctl->axisY(),        // (-511 - 512) left Y axis
    ctl->axisRX(),       // (-511 - 512) right X axis
    ctl->axisRY(),       // (-511 - 512) right Y axis
    ctl->brake(),        // (0 - 1023): brake button
    ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
    ctl->miscButtons(),  // bitmask of pressed "misc" buttons
    ctl->gyroX(),        // Gyro X
    ctl->gyroY(),        // Gyro Y
    ctl->gyroZ(),        // Gyro Z
    ctl->accelX(),       // Accelerometer X
    ctl->accelY(),       // Accelerometer Y
    ctl->accelZ()        // Accelerometer Z
  );
}

void processGamepad(ControllerPtr ctl) {
  // There are different ways to query whether a button is pressed.
  // By query each button individually:
  //  a(), b(), x(), y(), l1(), etc...
  if (ctl->a()) {
    static int colorIdx = 0;
    // Some gamepads like DS4 and DualSense support changing the color LED.
    // It is possible to change it by calling:
    switch (colorIdx % 3) {
      case 0:
        // Red
        ctl->setColorLED(255, 0, 0);
        break;
      case 1:
        // Green
        ctl->setColorLED(0, 255, 0);
        break;
      case 2:
        // Blue
        ctl->setColorLED(0, 0, 255);
        break;
    }
    colorIdx++;
  }

  if (ctl->b()) {
    // Turn on the 4 LED. Each bit represents one LED.
    static int led = 0;
    led++;
    // Some gamepads like the DS3, DualSense, Nintendo Wii, Nintendo Switch
    // support changing the "Player LEDs": those 4 LEDs that usually indicate
    // the "gamepad seat".
    // It is possible to change them by calling:
    ctl->setPlayerLEDs(led & 0x0f);
  }

  if (ctl->x()) {
    // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S, Stadia support rumble.
    // It is possible to set it by calling:
    // Some controllers have two motors: "strong motor", "weak motor".
    // It is possible to control them independently.
    ctl->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */,
                        0x40 /* strongMagnitude */);
  }

  // Another way to query controller data is by getting the buttons() function.
  // See how the different "dump*" functions dump the Controller info.

  if (ctl->dpad() == 0x02) {
    pressedDirection = 'D';
    printf("DOWN ");
  } else if (ctl->dpad() == 0x01) {
    pressedDirection = 'U';
    printf("UP ");
  } else if (ctl->dpad() == 0x04) {
    pressedDirection = 'R';
    printf("RIGHT ");
  } else if (ctl->dpad() == 0x08) {
    pressedDirection = 'L';
    printf("LEFT ");
  } else pressedDirection = ' ';
  //dumpGamepad(ctl);
}

void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
        processGamepad(myController);
      } else {
        Serial.println("Unsupported controller");
      }
    }
  }
}

void setup() {
  pinMode(sensorPinDown, INPUT);
  pinMode(sensorPinUp, INPUT);
  pinMode(sensorPinLeft, INPUT);
  pinMode(sensorPinRight, INPUT);
  strip.begin();
  Serial.begin(115200);

  //Initialize points display
  pointsDisplay.begin();
  pointsDisplay.show();
  pointsDisplay.setTextWrap(false);
  pointsDisplay.setBrightness(40);
  pointsDisplay.setTextColor(colors[0]);
  calculateSnakeBrightness();

  BP32.setup(&onConnectedController, &onDisconnectedController);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But it might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();

  xTaskCreatePinnedToCore(
    blinkingBorderCode, /* Task function. */
    "blinkingBorder",   /* name of task. */
    10000,              /* Stack size of task */
    NULL,               /* parameter of the task */
    1,                  /* priority of the task */
    &blinkingBorder,    /* Task handle to keep track of created task */
    0);                 /* pin task to core 0 */

  //incjalizacja ramki
  for (int j = 0; j < 128; j++) {
    strip.setPixelColor(edgeValues[j], strip.Color((red * borderIntensity) / 255, 0, 0));
  }

  // Create snake
  createSnake();
  strip.show();  //Initialize all pixels
  //podmiana pierwszych współrzędnych snejka na -1
  for (int i = 0; i <= snakeSize - 1; i += 2) {
    matrixCopy.push_back(twoDArray[sLengthControl[i + 1]][sLengthControl[i]]);
    twoDArray[sLengthControl[i + 1]][sLengthControl[i]] = -1;  //przez zmiane wartosci na -1 przestanie dzialac turn off - trzeba poprawic
  }
}

//blinkingBorder: blinks a border
void blinkingBorderCode(void* pvParameters) {
  Serial.print("Border running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    for (int i = 0; i < borderIntensity; i++) {
      int borderBrightness = calculateBorderBrightness(i);
      for (int j = 0; j < 128; j++) {
        strip.setPixelColor(edgeValues[j], strip.Color(borderBrightness, 0, 0));
      }
      vTaskDelay(pdMS_TO_TICKS(100));  // Add delay using FreeRTOS function
      // strip.show();
    }
    for (int i = 20; i > 0; i--) {
      int borderBrightness = calculateBorderBrightness(i);
      for (int j = 0; j < 128; j++) {
        strip.setPixelColor(edgeValues[j], strip.Color(borderBrightness, 0, 0));
      }
      vTaskDelay(pdMS_TO_TICKS(100));  // Add delay using FreeRTOS function
      // strip.show();
    }
  }
}

bool newDirection(char previousValue, char currentValue) {
  if (previousValue != currentValue) {
    return true;
  } else return false;
}

void loop() {
  // Serial.print("loop() running on core ");
  // Serial.println(xPortGetCoreID());
  int sensorValueDown = digitalRead(sensorPinDown);
  int sensorValueUp = digitalRead(sensorPinUp);
  int sensorValueLeft = digitalRead(sensorPinLeft);
  int sensorValueRight = digitalRead(sensorPinRight);

  strip.show();
  previousDirection = pressedDirection;
  bool dataUpdated = BP32.update();
  if (dataUpdated)
    processControllers();

  if (gameOver) {
    printf("press button to reset");
    delay(200);  //delay aby w razie spamowania przycisków nie resetować przypadkowo po gameover
    while (currentDirection == ' ') {
      dataUpdated = BP32.update();
      if (dataUpdated)
        processControllers();
      setDirection(pressedDirection);
      printf("game over while loop");
      pointsDisplay.fillScreen(0);
      pointsDisplay.setCursor(z, 0);
      pointsDisplay.print(F("Game Over - press any button"));
      if (--z < -144) {
        z = pointsDisplay.width();
        if (++pass >= 3) pass = 0;
        pointsDisplay.setTextColor(colors[pass]);
      }
      pointsDisplay.show();
      delay(100);
    }
    gameReset();
  }


  if (newDirection(pressedDirection, previousDirection) && pressedDirection != ' ') {
    setDirection(pressedDirection);
    printf("setting new direction");
  }

  if (velocity % 3 == 0) {
    performMovement();
  }


  velocity++;
  // // Turn on one LED at a time with random color and specified snakeIntensity
  // // Check if the sensor pin is HIGH
  // if (sensorValueDown == HIGH) {
  //   // Print a message to the Serial Monitor
  //   Serial.println("High signal detected on pin Down!");
  //   pressedDirection = 'D';
  //   delay(15);  // Add a delay to avoid repeated messages (adjust as needed)
  // } else if (sensorValueUp == HIGH) {
  //   // Print a message to the Serial Monitor
  //   Serial.println("High signal detected on Up!");
  //   pressedDirection = 'U';
  //   delay(15);  // Add a delay to avoid repeated messages (adjust as needed)
  // } else if (sensorValueLeft == HIGH) {
  //   // Print a message to the Serial Monitor
  //   Serial.println("High signal detected on Left!");
  //   pressedDirection = 'L';
  //   delay(15);  // Add a delay to avoid repeated messages (adjust as needed)
  // } else if (sensorValueRight == HIGH) {
  //   // Print a message to the Serial Monitor
  //   Serial.println("High signal detected on Right!");
  //   pressedDirection = 'R';
  //   delay(15);
  // }

  //przechodzi 20 razy zanim zacznie resetować pozycje x dlatego wchodzi zawsze w else if zanim wywoła if z movementem
  //   if (movmentQueue[0] == 'x') {
  //     movmentQueue[0] = pressedDirection;
  //   } else if (movmentQueue[0] != pressedDirection) {
  //     movmentQueue[1] = pressedDirection;
  //   }

  //   //to jest tragiczne..................................... o7
  //   if (velocity % 5 == 0) {

  //     if (movmentQueue[0] != 'x') {
  //       setDirection(movmentQueue[0]);
  //       performMovement();
  //       movmentQueue[0] = 'x';
  //     }

  //     if (movmentQueue[1] != 'y') {
  //       setDirection(movmentQueue[1]);
  //       performMovement();
  //       movmentQueue[1] = 'y';
  //     }
  //     velocity = 0;
  //   }

  //   velocity++;
}
