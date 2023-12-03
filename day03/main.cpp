// Your First C++ Program

#include <iostream>
#include <fstream>

#define IS_NUM(c) ((c)>='0' && (c)<='9')
#define IS_BLANK(c) ((c)=='.')
#define IS_SYMBOL(c) (!IS_NUM(c) && !IS_BLANK(c))

class Engine
{
private:
    char *buffer;
    size_t fileLength;
    size_t engineWidth;
    size_t engineHeight;
    char CharAt(size_t x, size_t y);
    uint32_t ValidateNum(int32_t x1, int32_t x2, int32_t y);
public:
    Engine(std::string fileName);
    ~Engine();
    int Examine();
};

Engine::Engine(std::string fileName)
{
    std::ifstream infile(fileName, std::ios_base::binary);
    infile.seekg(0, std::ios::end);
    this->fileLength = infile.tellg();
    infile.seekg(0, std::ios::beg);
    this->buffer = (char*) malloc(this->fileLength+1);
    buffer[this->fileLength] = 0;
    infile.read(buffer, this->fileLength);
    for (size_t i=0; i < this->fileLength && this->buffer[i]!='\n'; i++) {
        this->engineWidth = i+1;
    }
    this->engineHeight = (this->fileLength+1) / (this->engineWidth+1);
    printf("engineDimensions = %zux%zu\n", engineWidth, this->engineHeight);  
}

Engine::~Engine()
{
    free(this->buffer);
}

char Engine::CharAt(size_t x, size_t y) {
    if (x<0 || x>=this->engineWidth) return '.';
    if (y<0 || y>=this->engineHeight) return '.';
    return this->buffer[y*(this->engineWidth+1)+x];
}

uint32_t Engine::ValidateNum(int32_t x1, int32_t x2, int32_t y) {
    bool isValid = false;
    for (int32_t i = x1-1; i <= x2+1 ; i++)
    {
        char above = this->CharAt(i,y-1);
        char below = this->CharAt(i,y+1);
        if (IS_SYMBOL(above) || IS_SYMBOL(below)) {
            isValid = true;
            break;
        }
    }
    char left = this->CharAt(x1-1,y);
    char right = this->CharAt(x2+1,y);
    if (IS_SYMBOL(left) || IS_SYMBOL(right)) {
        isValid=true;
    }
    if (!isValid) return 0;
    uint32_t num = 0;
    for (int32_t i = x1; i <= x2 ; i++) {
        num = num*10 + (this->CharAt(i,y)-'0');
    }
    return num;
}

int Engine::Examine() {
    bool inNum = false;
    uint32_t total = 0;

    for (int32_t y = 0; y < this->engineHeight; y++) {
        inNum = false;
        int32_t numStart = 0, numEnd=0;
        // NOTE: x goes beyond the line-ends to ensure a number at the end of a line is properly terminated
        for (int32_t x = 0; x < this->engineWidth+1; x++) {
            char c = this->CharAt(x,y);

            if (inNum) {
                if (!IS_NUM(c)) {
                    numEnd = x-1;
                    uint32_t num = this->ValidateNum(numStart, numEnd, y);
                    if (num) {
                        printf("Num: %d\n", num);
                    }
                    total += num;
                    inNum = false;
                }
            } else {
                if (IS_NUM(c)) {
                    numStart = x;
                    inNum = true;
                }
            }
        }
    }
    
    printf("\nSum: %d\n", total);
    return 0;
}

int main(int argc, char *argv[]) {
    if(argc<2) exit(1);
    Engine *engine = new Engine(argv[1]);

    engine->Examine();

    delete engine;
    return 0;
}