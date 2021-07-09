//
// Created by tuidan on 2021/7/5.
//

#ifndef ONLINEMLBENCHMARK_PREPROCESSING_HPP
#define ONLINEMLBENCHMARK_PREPROCESSING_HPP

enum AttributeType {digital, text, category, timeDate, decimal};

void dataNormalization();
void dataTransformation();
void fillNullValue();

#endif //ONLINEMLBENCHMARK_PREPROCESSING_HPP
