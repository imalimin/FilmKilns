import os
import argparse
from string import Template
import socket
import time

def write(file, content):
    print("Generate: %s" % file)
    with open(file, "w") as f:
        f.write(content)

def fillTemplate(template):
    authorStr = socket.gethostname()
    with os.popen("git config user.email",'r',-1) as f:
        authorStr = f.readline()
    return template.substitute(
        moduleName=args.module,
        className=args.name,
        tagHeader=(args.module.replace("Fk", "Fk_") + "_" + args.name + "_H").upper(),
        author=authorStr,
        time=time.strftime("%Y-%m-%d %H:%M:%S"),
        email="aliminabc@gmail.com"
    )

def gen(args, templateDir):
    with open(templateDir + "/FkCPP.txt", "r") as f:
        content = f.read()
        template = Template(content.replace("\#", "#"))
        result = fillTemplate(template)
        write("./src/%s/%s/%s.cpp" % (args.module, args.pkt, args.name), result)

    with open(templateDir + "/FkHeader.txt", "r") as f:
        content = f.read()
        template = Template(content.replace("\#", "#"))
        result = fillTemplate(template)
        write("./src/%s/include/%s.h" % (args.module, args.name), result)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-module',
        type=str,
        default='',
        help='Module name'
    )
    parser.add_argument(
        '-pkt',
        type=str,
        default='',
        help='Package path'
    )
    parser.add_argument(
        '-name',
        type=str,
        default='',
        help='Class name'
    )

    args = parser.parse_args()
    if args.module == '':
        print("Invalid module name")
        exit()
    if args.pkt == '':
        print("Invalid package path")
        exit()
    if args.name == '':
        print("Invalid class name")
        exit()
    gen(args, "./proj/FkAndroid/fkandroid/templates")
    print("Generate cpp class finish.")