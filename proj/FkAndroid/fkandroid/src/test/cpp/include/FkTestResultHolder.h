/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FKANDROID_FKTESTRESULTHOLDER_H
#define FKANDROID_FKTESTRESULTHOLDER_H

#include <string>
#include <vector>

class FkTestResultHolder {
private:
    FkTestResultHolder();

    FkTestResultHolder(FkTestResultHolder &o) = default;

public:
    static FkTestResultHolder *getInstance();

    void push(std::string msg, bool _passed);

    void pop(std::vector<std::string> &vec);

    void clear();

private:
    static FkTestResultHolder *instance;
    std::vector<std::string> passed;
    std::vector<std::string> failed;

};


#endif //FKANDROID_FKTESTRESULTHOLDER_H
