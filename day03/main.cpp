// Your First C++ Program

#include <iostream>
#include <fstream>
#include <map>
#include <vector>


#define IS_NUM(c) ((c)>='0' && (c)<='9')
#define IS_BLANK(c) ((c)=='.')
#define IS_GEAR(c) ((c)=='*')
#define IS_SYMBOL(c) (!IS_NUM(c) && !IS_BLANK(c))

class Engine
{
private:
    char *buffer;
    size_t fileLength;
    size_t engineWidth;
    size_t engineHeight;
    std::map<uint32_t, std::vector<uint32_t> > gears;
    char CharAt(size_t x, size_t y);
    uint32_t ValidateNum(int32_t x1, int32_t x2, int32_t y);
    void AddToGearAt(int32_t x, int32_t y, uint32_t num);
    bool ConsiderBorderPoint(int32_t x, int32_t y, uint32_t num);
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

void Engine::AddToGearAt(int32_t x, int32_t y, uint32_t num) {
    uint32_t gearId = y*this->engineWidth+x;
    if (this->gears.find(gearId) == this->gears.end()) {
        std::vector<uint32_t> firstEntry;
        this->gears[gearId] = firstEntry;
    }
    this->gears[gearId].push_back(num);
}

bool Engine::ConsiderBorderPoint(int32_t x, int32_t y, uint32_t num) {
    char c = this->CharAt(x,y);
    bool isValid = IS_SYMBOL(c);
    if (isValid && IS_GEAR(c)) {
        this->AddToGearAt(x,y, num);
    }
    return isValid;
}

uint32_t Engine::ValidateNum(int32_t x1, int32_t x2, int32_t y) {
    bool isValid = false;

    uint32_t num = 0;
    for (int32_t i = x1; i <= x2 ; i++) {
        num = num*10 + (this->CharAt(i,y)-'0');
    }
    for (int32_t i = x1-1; i <= x2+1 ; i++)
    {
        isValid = this->ConsiderBorderPoint(i,y-1, num) || isValid;
        isValid = this->ConsiderBorderPoint(i,y+1, num) || isValid;
    }
    isValid = this->ConsiderBorderPoint(x1-1,y, num) || isValid;
    isValid = this->ConsiderBorderPoint(x2+1,y, num) || isValid;
    if (!isValid) return 0;
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

    u_int32_t totalGearRatio = 0;
    std::map<uint32_t, std::vector<uint32_t> >::iterator it;

    for (it = this->gears.begin(); it != this->gears.end(); it++) {
        if (it->second.size() == 2) {
            totalGearRatio += it->second[0] * it->second[1];
        }
    }
    
    printf("\nSum: %d\n", total);
    printf("\ntotalGearRatio: %d\n", totalGearRatio);
    return 0;
}

int main(int argc, char *argv[]) {
    if(argc<2) exit(1);
    Engine *engine = new Engine(argv[1]);

    engine->Examine();

    delete engine;
    return 0;
}