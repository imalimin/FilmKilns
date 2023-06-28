import os
import argparse

if __name__ == '__main__':
    cppOut = "./src/FkImage/pb"
    javaOut = "./proj/FkAndroid/fkandroid/src/main/java"
    srcDir = "./src/FkImage/pb/proto"
    srcStr = ''
    for name in sorted(os.listdir(srcDir)):
        srcStr += srcDir + "/" + name + " "
    os.system("protoc --proto_path=./src/FkImage/pb/proto --cpp_out=%s --java_out=%s %s" % (cppOut, javaOut, srcStr))