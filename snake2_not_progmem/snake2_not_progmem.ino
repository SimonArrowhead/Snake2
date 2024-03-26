#include <Adafruit_NeoPixel.h>
#include <vector>

#define PIN 19          // Define the pin to which your data line is connected
#define NUMPIXELS 1024  // Define the number of LEDs in your strip
//#define VELOCITY 10    // Speed of snake

char currentDirection = ' ';  // Initialize to a non-movement state
int velocity = 0;
bool foodExists = false;
int snakeSize = 0;
int foodPosition = random(0, 1024);

const int sensorPinDown = 16;
const int sensorPinUp = 17;
const int sensorPinLeft = 5;
const int sensorPinRight = 18;

// const int sensorPinDown = 3;
// const int sensorPinUp = 2;
// const int sensorPinLeft = 8;
// const int sensorPinRight = 9;

int intensity = 50;  // Set the intensity (brightness) of the LEDs (0-255)
int red = 256;
int green = 256;
int blue = 256;


const int rows = 35;
const int cols = 35;
const int twoDArray[rows][cols] = {
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

char movmentQueue[2] = { 'x', 'y' };
char direction = 'x';

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
std::vector<int> sLengthControl = { 11, 8, 12, 8, 13, 8, 14, 8 }; // Define sLengthControl as a vector


int x = sLengthControl[0];  // Snake's head position column
int y = sLengthControl[1];  // Snake's head position row

void setup() {
  pinMode(sensorPinDown, INPUT);
  pinMode(sensorPinUp, INPUT);
  pinMode(sensorPinLeft, INPUT);
  pinMode(sensorPinRight, INPUT);
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
  Serial.begin(115200);
  // Create snake
  createSnake();
}

void loop() {
  int sensorValueDown = digitalRead(sensorPinDown);
  int sensorValueUp = digitalRead(sensorPinUp);
  int sensorValueLeft = digitalRead(sensorPinLeft);
  int sensorValueRight = digitalRead(sensorPinRight);


  // Turn on one LED at a time with random color and specified intensity
  // Check if the sensor pin is HIGH
  if (sensorValueDown == HIGH) {
    // Print a message to the Serial Monitor
    Serial.println("High signal detected on pin Down!");
    direction = 'D';
    delay(15);  // Add a delay to avoid repeated messages (adjust as needed)
  } else if (sensorValueUp == HIGH) {
    // Print a message to the Serial Monitor
    Serial.println("High signal detected on Up!");
    direction = 'U';
    delay(15);  // Add a delay to avoid repeated messages (adjust as needed)
  } else if (sensorValueLeft == HIGH) {
    // Print a message to the Serial Monitor
    Serial.println("High signal detected on Left!");
    direction = 'L';
    delay(15);  // Add a delay to avoid repeated messages (adjust as needed)
  } else if (sensorValueRight == HIGH) {
    // Print a message to the Serial Monitor
    Serial.println("High signal detected on Right!");
    direction = 'R';
    delay(15);
  }

  //przechodzi 20 razy zanim zacznie resetować pozycje x dlatego wchodzi zawsze w else if zanim wywoła if z movementem

  if (movmentQueue[0] == 'x') {
    movmentQueue[0] = direction;
  } else if (movmentQueue[0] != direction) {
    movmentQueue[1] = direction;
  }

  //to jest tragiczne..................................... o7



  if (velocity % 5 == 0) {

    if (movmentQueue[0] != 'x') {
      setDirection(movmentQueue[0]);
      performMovement();
      movmentQueue[0] = 'x';
    }

    if (movmentQueue[1] != 'y') {
      setDirection(movmentQueue[1]);
      performMovement();
      movmentQueue[1] = 'y';
    }
    velocity = 0;
  }

  velocity++;
  delay(5);  // Adjust the delay as needed

  // if (x >= lenght) {

  // Turn off the first LED if GROUP_SIZE LEDs are already on
  //   Serial.print("Clear: ");
  //   Serial.println(twoDArray[y][x - lenght]);
  //   strip.setPixelColor(twoDArray[y][x - lenght] , strip.Color(0, 0, 0));  // Black color turns off the LED
  //   delay(100);
  // }

  // Turn off all LEDs
  // for (int i = 0; i < NUMPIXELS; i++) {
  //   strip.setPixelColor(i, strip.Color(0, 0, 0));  // Black color turns off the LED
  // }
}

void createSnake() {
  snakeSizeChecker();
  // Dekrementuje i o 2 poniewaz setuje dwa pixele do matrixa z tabeli ktore sa zawsze obok siebie i oraz i-1
  for (int i = snakeSize - 1; i >= 0; i -= 2) {
    strip.setPixelColor(twoDArray[sLengthControl[i]][sLengthControl[i - 1]], strip.Color((red * intensity) / 255, (green * intensity) / 255, (blue * intensity) / 255));
    // Serial.println(twoDArray[sLengthControl[i]][sLengthControl[i-1]]);
    strip.show();

    if (twoDArray[y][x] == foodPosition) {
      foodExists = false;

    
    }
    food();
  }
}

void snakeSizeChecker() {
 snakeSize = sLengthControl.size();
}

void food() {

  if (!foodExists) {
    foodPosition = random(0, 1024);
    strip.setPixelColor(foodPosition, strip.Color((red * intensity) / 255, 0, 0));
    // Serial.println(twoDArray[sLengthControl[i]][sLengthControl[i-1]]);
    strip.show();
    foodExists = true;
    }
}

void move() {
  int axisX = 0;
  int axisY = 0;

  // Ustalenie rozmiaru tabeli
  snakeSizeChecker();
  // Przesuniecie tabeli o dwie pozycje
  for (int i = snakeSize - 1; i > 1; --i) {
    if (i == snakeSize - 1) {
      axisX = sLengthControl[i - 1];  // Zapisuje wspolrzedne x diody do zgaszenia
      axisY = sLengthControl[i];      // Zapisuje wspolrzedne y diody do zgaszenia
    }

    sLengthControl[i] = sLengthControl[i - 2];
  }

  checkForGameOver();     // Musi być przed ustaleniem nowych wsporzednych glowy bo inaczej nie zadziala
  sLengthControl[0] = x;  // Przypisuje nowe wspolrzedne x dla glowy
  sLengthControl[1] = y;  // Przypisuje nowe wspolrzedne y dla glowy
  if (twoDArray[y][x] == foodPosition) { //sprawdza czy głowa = jedzenie. jeśli tak to nie gasi diody tylko powiększa snejka
    sLengthControl.push_back(axisX);
    sLengthControl.push_back(axisY);
    snakeSizeChecker();
  }
  else turnOffLed(axisX, axisY);

  createSnake();
}

//poprawić kierunki bo po odwróceniu matrixa nazwy metod nie odpowiadają kierunkom na matrycy
void move_right() {
  x += 1;
  move();
}

void move_left() {
  x -= 1;
  move();
}

void move_up() {
  y -= 1;
  move();
}

void move_down() {
  y += 1;
  move();
}

void checkForGameOver() {
  snakeSizeChecker();
  for (int i = snakeSize - 1; i > 0; i -= 2) {
    if (y == sLengthControl[i] && x == sLengthControl[i - 1])
      Serial.println("game over");
  }
  if (twoDArray[y][x] == -1) {
    Serial.println("game over");
  }
}

void turnOffLed(int axisX, int axisY) {
  // int arraySize = sizeof(sLengthControl) / sizeof(sLengthControl[0]);
  // for (int i = arraySize - 1; i >= 0; i--) {
  //   Serial.print("Wartość na pozycji: ");
  //   Serial.print(i);
  //   Serial.print(" wynosi: ");
  //   Serial.println(sLengthControl[i]);
  // }
  // Serial.print("Wygaszanie diody na pozycji x: ");
  // Serial.print(axisX);
  // Serial.print(" or y: ");
  // Serial.println(axisY);
  // Serial.print("O numerze: ");
  // Serial.println(twoDArray[axisY][axisX]);

  strip.setPixelColor(twoDArray[axisY][axisX], strip.Color(0, 0, 0));
}

void setDirection(char incomingByte) {
  switch (incomingByte) {
    case 'U':
      currentDirection = 'U';
      break;
    case 'D':
      currentDirection = 'D';
      break;
    case 'R':
      currentDirection = 'R';
      break;
    case 'L':
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