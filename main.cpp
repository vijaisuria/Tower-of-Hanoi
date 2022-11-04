#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

struct Disk {
    char diskIndex;
    char diskWidth;
    char diskColor;
    char towerIndex;
    char towerPosition;
};

const char yDrawStart = 4;
const char xDrawStart = 4;
const char xDrawSpace = 1;
const char defaultColor = 0x07;
const char errorColor = 0x04;
char xDrawWidth = 11;
char disksCount = 4;

bool isGameOver = false;
char selectedTower = 0;
char selectedDisk = -1;
struct Disk disksData[16];
int attemptsCount = 0;

char getLastDisk(char towerIndex) {
    char lastDisk = CHAR_MAX;
    char lastIndex = -1;
    for (int i = 0; i < disksCount; i++) {
        if (disksData[i].towerIndex == towerIndex) {
            if (disksData[i].diskWidth < lastDisk) {
                lastDisk = disksData[i].diskWidth;
                lastIndex = disksData[i].diskIndex;
            }
        }
    }
    return lastIndex;
}

char getDiskIndex(char towerIndex, char towerPosition) {
    for (int i = 0; i < disksCount; i++) {
        if (disksData[i].towerIndex == towerIndex &&
            disksData[i].towerPosition == towerPosition) {
            return disksData[i].diskIndex;
        }
    }
    return -1;
}

bool checkForGameOver() {
    for (int i = 0; i < disksCount; i++) {
        if (disksData[i].towerIndex != 2) {
            return false;
        }
    }
    return true;
}

void setCursor(int x, int y) {
    COORD pos = { x, y };
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

void setTextColor(int color)
{
    static int __BACKGROUND;
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), color + (__BACKGROUND << 4));
}

void clearScreen() {
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  SetConsoleCursorPosition( hStdOut, homeCoords );
}

void init() {
    printf("\n> Tower of Hanoi | by DCT\n");
    int inputDisksNumber = -1;
    while (inputDisksNumber == -1) {
        setTextColor(defaultColor);
        printf("> Enter the number of discs: ");
        scanf("%d", &inputDisksNumber);
        if (inputDisksNumber > 14 || inputDisksNumber < 3) {
            inputDisksNumber = -1;
            setTextColor(errorColor);
            printf("> TRY AGAIN: The number must be between (3 and 14)!\n");
        }
    }
    disksCount = inputDisksNumber;
    xDrawWidth = (disksCount * 2) + 3;
    for(int i = 0; i < disksCount; i++)
    {
        struct Disk disk;
        disk.diskIndex = i;
        disk.diskWidth = ((i + 1) * 2);
        disk.diskColor = i + 1;
        disk.towerIndex = 0;
        disk.towerPosition = (disksCount - i - 1);
        disksData[i] = disk;
    }
    clearScreen();
}

void draw() {
    setCursor(0, 0);
    printf("\n> Tower of Hanoi | by DCT");
    printf("\n> Number of attempts: %d\n\n", attemptsCount);

    for(int i = 0; i < 3; i++)
    {
        for(int y = 0; y < (disksCount + 3); y++)
        {
            char diskIndex = getDiskIndex(i, disksCount - (y - 2) - 1);

            for(int x = 0; x < xDrawWidth; x++)
            {
                char xPos = xDrawStart + (i * xDrawWidth) + (i * xDrawSpace) + x;
                char yPos = yDrawStart + y;
                setCursor(xPos, yPos);

                setTextColor(defaultColor);

                if (y == 0) {
                    if (selectedDisk != -1 && i == selectedTower) {
                        char diskWidth = disksData[selectedDisk].diskWidth + 1;
                        char xBegin = ((xDrawWidth - diskWidth) / 2) - 1;
                        char xEnd = xDrawWidth - xBegin - 1;
                        if (x > xBegin && x < xEnd) {
                            setTextColor(disksData[selectedDisk].diskColor);
                            printf("%c", 0xB1);
                        }
                    } else {
                        printf(" ");
                    }
                } else if (y == 1) {
                    if (x == (xDrawWidth / 2)) {
                        printf("%c", 0xD2);
                    }
                } else if (y == (disksCount + 3) - 1) {
                    if (x == 0) {
                       printf("%c", 0xDA);
                    } else if (x == xDrawWidth - 1) {
                       printf("%c", 0xBF);
                    } else if (x == (xDrawWidth / 2)) {
                       if (i == selectedTower) {
                           printf("%c", 0xFE);
                       } else {
                           printf("%c", 0xD0);
                       }
                    } else  {
                       printf("%c", 0xC4);
                    }
                } else {
                    if (diskIndex != -1 && diskIndex != selectedDisk) {
                        char diskWidth = disksData[diskIndex].diskWidth + 1;
                        char xBegin = ((xDrawWidth - diskWidth) / 2) - 1;
                        char xEnd = xDrawWidth - xBegin - 1;
                        if (x > xBegin && x < xEnd) {
                            if (x == xBegin + (diskWidth / 2) + 1) {
                                printf("%c", 0xD7);
                            } else {
                                setTextColor(disksData[diskIndex].diskColor);
                                printf("%c", 0xDB);
                            }
                        }
                    } else {
                        if (x == (xDrawWidth / 2)) {
                            printf("%c", 0xD7);
                        } else {
                            printf(" ");
                        }
                    }
                }
            }
        }
    }

    if (isGameOver) {
        printf("\n\n> Well done!!! You have completed the game in %d moves.", attemptsCount);
        getch();
    }
}

bool update(char c) {
    bool canUpdate = false;

    if (c == 0x4B || c == 0x61 || c == 0x41) {
        if (selectedTower - 1 >= 0) {
            selectedTower -= 1;
            canUpdate = true;
        }
    } else if (c == 0x4D || c == 0x64 || c == 0x44) {
        if (selectedTower + 1 <= 2) {
            selectedTower += 1;
            canUpdate = true;
        }
    } else if (c == 0x20 || c == 0x0D) {
        if (selectedDisk == -1) {
            selectedDisk = getLastDisk(selectedTower);
        } else {
           char lastTowerDiskIndex = getLastDisk(selectedTower);
           if (lastTowerDiskIndex != -1) {
               if (disksData[lastTowerDiskIndex].diskWidth > disksData[selectedDisk].diskWidth) {
                    disksData[selectedDisk].towerIndex = selectedTower;
                    disksData[selectedDisk].towerPosition = disksData[lastTowerDiskIndex].towerPosition + 1;
                    selectedDisk = -1;
                    attemptsCount += 1;
               } else if (disksData[lastTowerDiskIndex].diskWidth == disksData[selectedDisk].diskWidth) {
                    selectedDisk = -1;
               }
           } else {
               disksData[selectedDisk].towerIndex = selectedTower;
               disksData[selectedDisk].towerPosition = 0;
               selectedDisk = -1;
               attemptsCount += 1;
           }
        }
        canUpdate = true;
        isGameOver = checkForGameOver();
    }

    return canUpdate;
}

int main()
{
    SetConsoleCP(437);
    SetConsoleOutputCP(437);
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);

    init();
    draw();

    while(!isGameOver) {
        char c = getch();
        bool canUpdate = update(c);
        if (canUpdate) {
            draw();
        }
    }

    return 0;
}
