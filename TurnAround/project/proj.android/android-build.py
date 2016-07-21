#!/usr/bin/python
# android-build.py
# Build android

import sys
import os, os.path
import shutil
from optparse import OptionParser

def getNDKROOT():
    return os.environ['ANDROID_NDK_ROOT']

def getSDKROOT():    
    return os.environ['ANDROID_SDK_ROOT']

def select_toolchain_version():
    ndk_root = getNDKROOT()
    if os.path.isdir(os.path.join(ndk_root,"toolchains/arm-linux-androideabi-4.9")):
        os.environ['NDK_TOOLCHAIN_VERSION'] = '4.9'
        print("The Selected NDK toolchain version was 4.9 !")
    elif os.path.isdir(os.path.join(ndk_root,"toolchains/arm-linux-androideabi-4.8")):
        os.environ['NDK_TOOLCHAIN_VERSION'] = '4.8'
        print("The Selected NDK toolchain version was 4.8 !")
    elif os.path.isdir(os.path.join(ndk_root,"toolchains/arm-linux-androideabi-4.7")):
        os.environ['NDK_TOOLCHAIN_VERSION'] = '4.6'
        print("The Selected NDK toolchain version was 4.6 !")
    else:
        print("Couldn't find the gcc toolchain.")
        exit(1)

def get_num_of_cpu():
    return int(os.environ['NUMBER_OF_PROCESSORS'])       

def do_build(ndk_root, sdk_root, app_android_root, cocos_root, gamebox_root, android_platform, build_mode):
    select_toolchain_version()

    if len(sys.argv) >= 2 and sys.argv[1] == 'debug':
        ndk_path = os.path.join(ndk_root, "ndk-build NDK_DEBUG=1")
    else:
        ndk_path = os.path.join(ndk_root, "ndk-build NDK_DEBUG=0")
    ndk_module_path = 'NDK_MODULE_PATH=%s;%s;%s/cocos;%s/external' % (app_android_root, cocos_root, cocos_root, cocos_root)
    num_of_cpu = get_num_of_cpu()
    command = '%s -j%d -C %s %s' % (ndk_path, num_of_cpu, app_android_root, ndk_module_path)
    print(command)
    if os.system(command) != 0:
        exit(1)
        return
    
    copy_files('libUmeng', 'libs')
    copy_files('libVungle', 'libs')    

    sdk_tool_path = os.path.join(sdk_root, "tools/android")
    cocoslib_path = os.path.join(cocos_root, "cocos/platform/android/java")
    command = '%s update lib-project -t %s -p %s' % (sdk_tool_path,android_platform,cocoslib_path)
    if os.system(command) != 0:
        exit(1)
        return

    gamebox_path = os.path.join(gamebox_root, "proj.android")
    command = '%s update lib-project -t %s -p %s' % (sdk_tool_path,android_platform,gamebox_path)
    if os.system(command) != 0:
        exit(1)
        return

    google_path = os.path.join(cocos_root, "external/google-play-services_lib")
    command = '%s update lib-project -t %s -p %s' % (sdk_tool_path,android_platform,google_path)
    if os.system(command) != 0:
        exit(1)
        return    
    
    command = '%s update project -t %s -p %s -s' % (sdk_tool_path,android_platform,app_android_root)
    if os.system(command) != 0:
        exit(1)
        return
    
    buildfile_path = os.path.join(app_android_root, "build.xml")
    command = 'ant deploy -f %s' % buildfile_path
    if os.system(command) != 0:
        exit(1)
        return

def copy_resources(src, dst):
    if os.path.isdir(dst):
        shutil.rmtree(dst)
    os.mkdir(dst)

    copy_files(src, dst)

def copy_files(src, dst):
    for item in os.listdir(src):
        path = os.path.join(src, item)
        # Android can not package the file that ends with ".gz"
        if not item.startswith('.') and not item.endswith('.gz') and os.path.isfile(path):
            shutil.copy(path, dst)
        if os.path.isdir(path):
            new_dst = os.path.join(dst, item)
            if not os.path.exists(new_dst):
                os.mkdir(new_dst)
            copy_files(path, new_dst)

def compare(a, b):
    aVers = a.split('.')
    bVers = b.split('.')
    aVer1 = int(aVers[0])
    aVer2 = int(aVers[1])
    aVer3 = int(aVers[2])
    bVer1 = int(bVers[0])
    bVer2 = int(bVers[1])
    bVer3 = int(bVers[2])
    if aVer1 == bVer1:
        if aVer2 == bVer2:
            if aVer3 > bVer3:
                return -1
            elif aVer3 < bVer3:
                return 1
            else:
                return 0            
        else:
            if aVer2 > bVer2:
                return -1
            elif aVer2 < bVer2:
                return 1
            else:
                return 0   
    else:
        if aVer1 > bVer1:
            return -1
        elif aVer1 < bVer1:
            return 1
        else:
            return 0   

def copy_res(src, dst):
    files = os.listdir(src)
    versions = []
    for f in files:
        path = os.path.join(src, f)
        if os.path.isdir(path):
            versions.append(f)
    versions = sorted(versions, compare)
    
    path = os.path.join(src, versions[0])
    copy_files(path, dst)
            
def build(android_platform, build_mode):
    ndk_root = getNDKROOT()
    sdk_root = getSDKROOT()

    if os.path.isdir('libs'):
        shutil.rmtree('libs')
    os.mkdir('libs')        
    
    current_dir = os.path.dirname(os.path.realpath(__file__))
    res_dir = os.path.join(current_dir, "../../res")
    src_dir = os.path.join(current_dir, "../../src")
    cocos_root = os.path.join(current_dir, "../../../Cocos2dx")
    gamebox_root = os.path.join(current_dir, "../../../GameBox")
    android_platform = 'android-' + android_platform
    app_android_root = '.'
    copy_resources('../Resources', os.path.join(app_android_root, 'assets'))
    copy_res('../../publish/TurnAround', os.path.join(app_android_root, 'assets'))
    do_build(ndk_root, sdk_root, app_android_root, cocos_root, gamebox_root, android_platform, build_mode)


# -------------- main --------------
if __name__ == '__main__':
    build('23', "release")
