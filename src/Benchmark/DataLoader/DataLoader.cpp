//
// Created by tuidan on 2021/7/5.
//
#include "DataLoader.hpp"
using namespace std;

int DataLoader::getPointNumber() {
    return this->pointNumber;
}

void DataLoader::setPointNumber(int n) {
    this->pointNumber = n;
}

bool DataLoader::getIsCleaned() {
    return isCleaned;
}

void DataLoader::setIsCleaned(bool flag) {
    this->isCleaned = flag;
}

int DataLoader::getAttributeNumber() {
    return this->attributeNumber;
}

void DataLoader::setAttributeNumber(int n) {
    this->attributeNumber = n;
}

/**
* @Description: store the data file per line in the string array, every line is a string element
* @Param: data file path
* @Return: line data
*/
string *DataLoader::readFile(string absolutePath) {
    string *data = new string[this->pointNumber];
    ifstream infile;
    infile.open(absolutePath);
    cout << "Read from the file..." << endl;
    for(int i = 0; i < this->pointNumber; i++) getline(infile, data[i]);
    cout << "Complete reading from the file..." << endl;
    infile.close();
    return data;
}

bool DataLoader::dataPreprocessing(int *points) {return true}

DataLoader::c



