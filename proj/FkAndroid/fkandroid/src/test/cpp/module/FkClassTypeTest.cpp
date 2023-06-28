/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <list>
#include <string>
#include "gtest/gtest.h"
#include "FkCalculatePrt.h"
#include "FkClassType.h"
#include <typeinfo>

FK_SUPER_CLASS(FkObjectA, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkObjectA)

public:
    FkObjectA() {
    };
};
FK_IMPL_CLASS_TYPE(FkObjectA, FkObject)

FK_SUPER_CLASS(FkObjectB, FkObjectA) {
FK_DEF_CLASS_TYPE_FUNC(FkObjectB)

public:
    FkObjectB() : FkObjectA() {
    }
};
FK_IMPL_CLASS_TYPE(FkObjectB, FkObjectA)

FK_SUPER_CLASS(FkObjectC, FkObjectB) {
FK_DEF_CLASS_TYPE_FUNC(FkObjectC)

public:
    FkObjectC() : FkObjectB() {
    }
};
FK_IMPL_CLASS_TYPE(FkObjectC, FkObjectB)

TEST(FkClassHierarchyTest, hierarchy2) {
    auto a = std::make_shared<FkObjectA>();
    auto b = std::make_shared<FkObjectB>();
    auto c = std::make_shared<FkObjectC>();
    std::shared_ptr<FkObjectA> d = b;

    EXPECT_TRUE(a->getClassType().is(FkObjectA_Class::type));
    EXPECT_FALSE(a->getClassType().is(FkObjectB_Class::type));

    EXPECT_TRUE(b->getClassType().is(FkObjectA_Class::type));
    EXPECT_TRUE(b->getClassType().is(FkObjectB_Class::type));
    EXPECT_FALSE(b->getClassType().is(FkObjectC_Class::type));

    EXPECT_TRUE(d->getClassType().is(FkObjectA_Class::type));
    EXPECT_TRUE(d->getClassType().is(FkObjectB_Class::type));
    EXPECT_FALSE(d->getClassType().is(FkObjectC_Class::type));

    EXPECT_TRUE(d->getClassType().is(a->getClassType()));
    EXPECT_TRUE(d->getClassType().is(b->getClassType()));
    EXPECT_FALSE(d->getClassType().is(c->getClassType()));
}

//TEST(FkClassTypeTests, SuperTrack) {
//    FkCalculatePrt prot;
//    EXPECT_STREQ(prot.getClassType().toString().c_str(), "8FkObject:<-10FkProtocol:<-15FkCalculatePrt");
//}
//
//TEST(FkClassTypeTests, HashCode) {
//    FkCalculatePrt prot;
//    EXPECT_EQ(prot.getClassType().hashCode(), -5469057871962402202L);
//}
