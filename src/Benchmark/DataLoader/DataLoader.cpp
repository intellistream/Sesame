//
// Created by tuidan on 2021/7/5.
//
#include "DataLoader.hpp"

using namespace std;

DataLoader::DataLoader(int n, bool flag, int an) {
    this->pointNumber = n;
    this->isCleaned = flag;
    this->attributeNumber = an;
}

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
bool DataLoader::getIsClustered() {
    return this->isClustered;
}

void DataLoader::setIsClustered(bool flag) {
    this->isClustered = flag;
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
string *DataLoader::readFile(char * absolutePath) {
    string *data = new string[this->getPointNumber()];
    ifstream infile;
    infile.open(absolutePath);
    cout << "Read from the file..." << endl;
    for(int i = 0; i < this->getPointNumber(); i++) getline(infile, data[i]);
    cout << "Complete reading from the file..." << endl;
    infile.close();
    return data;
}

bool DataLoader::dataPreprocessing(Point * points) {
    return true;
}

/**
* @Description: index w1,w2,...,C
* @Param: 
* @Return: 
*/
Point *DataLoader::createPoints(string *dataLine) {
    Point points[this->getPointNumber()];
    for(int i = 0; i < dataLine->length(); i++){
        if(this->getIsClustered()) points[i].Initialization(i + 1,1, this->getAttributeNumber()-2);
        else points[i].Initialization(i + 1, 1, this->getAttributeNumber()-1);
        char * charData  = new char[10000];
        strcpy(charData,dataLine[i].c_str());
        // use c_str() to convert string to char * but it's just a temp pointer we have to use strcpy to store it
        const char *sep = " ";
        char * feature = strtok(charData, sep);
        feature = strtok(nullptr, sep);
        int index = 1;
        while(feature != nullptr){
            if(this->getIsClustered() and index == this->getAttributeNumber()) {
                points[i].setClusteringCenter(atoi(feature));
            }
            else {
                points[i].setFeatureItem(strtod(feature, nullptr), index-1);
                index++;
            }
            feature = strtok(nullptr, sep);
        }
    }
    return points;
}




int main(){
    DataLoader d(15120, false, 56);
    string * data;
    data = d.readFile("/Users/tuidan/code/C++/1/Sesame/src/Benchmark/Workload/new.txt");
    Point *p = d.createPoints(data);
    return 0;
}



