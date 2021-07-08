//
// Created by tuidan on 2021/7/5.
//
#ifndef ONLINEMLBENCHMARK_DATALOADER_HPP
#define ONLINEMLBENCHMARK_DATALOADER_HPP

#endif //ONLINEMLBENCHMARK_DATALOADER_HPP


#include "Preprocessing.hpp"
#include "Point.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

/**
* @Description: store the while input data after data processing, one data point, one dataloader
*/
class DataLoader{
private:
    int pointNumber;
    bool isCleaned;
    int attributeNumber;
public:
    string *readFile(string absolutePath);
    Point createPoints(string * dataLine);
    bool dataPreprocessing(Point * points);
    int getPointNumber();
    void setPointNumber(int number);
    int getAttributeNumber();
    void setAttributeNumber(int number);
    bool getIsCleaned();
    void setIsCleaned(bool flag)
};




