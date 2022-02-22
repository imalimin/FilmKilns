//
// Created by Alimin on 2022/2/22.
//

#ifndef FKANDROID_FKTESTDEFINE_H
#define FKANDROID_FKTESTDEFINE_H

#define FK_NEW_INSTANCE(name, type, alias)  \
auto name = std::make_shared<type>(alias); \
EXPECT_EQ(name->create(), FK_OK);       \
EXPECT_EQ(name->start(), FK_OK);        \

#define FK_DELETE_INSTANCE(name)  \
EXPECT_EQ(name->stop(), FK_OK);       \
EXPECT_EQ(name->destroy(), FK_OK);        \

#endif //FKANDROID_FKTESTDEFINE_H
