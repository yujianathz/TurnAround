import os
import md5
import shutil
import urllib2
import sys

IP = '121.196.232.153'
PORT = '2121'
NAME = 'TurnAround'

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
    
def makeMD5(newVersion, folder, contents):
    newPath = os.path.join(NAME, newVersion, folder)
    files = os.listdir(newPath)
    for f in files:
        path = os.path.join(newPath, f)
        if os.path.isdir(path):
            makeMD5(newVersion, os.path.join(folder, f), contents)
        else:
            fin = open(path, 'rb')
            src = fin.read()
            fin.close()
            m = md5.new()
            m.update(src)

            key = os.path.join(folder, f)
            key = key.replace('\\', '/')
            contents.append(','.join([key, m.hexdigest(), '%d'%os.path.getsize(path)]))
            
def genMD5(version, contents):
    contents.insert(0, ','.join([version, '', '']))

    fout = open(os.path.join(NAME, version, 'md5'), 'w+')
    for content in contents:
        print(content)
        fout.write(content + '\n')
    fout.close()

def getMD5FromServer(newVersion):
    contents = []    
    try:
        vers = newVersion.split('.')
        if len(vers) == 3:
            url = 'http://%s/update/%s/%s/md5'%(IP, NAME, '.'.join([vers[0], vers[1]]))
            f = urllib2.urlopen(url)
            lines = f.readlines()
            for line in lines:
                contents.append(line.strip())
        
    except urllib2.URLError,e:
        print e.reason
    except urllib2.HTTPError,e:
        print e.code
        print e.read()

    return contents

def isUploadValid(localVersion, remoteVersion):
    localVers = localVersion.split('.')
    remoteVers = remoteVersion.split('.')
    localVer1 = int(localVers[0])
    localVer2 = int(localVers[1])
    localVer3 = int(localVers[2])
    remoteVer1 = int(remoteVers[0])
    remoteVer2 = int(remoteVers[1])
    remoteVer3 = int(remoteVers[2])
    if localVer1 == remoteVer1:
        if localVer2 == remoteVer2:
            return localVer3 > remoteVer3
        else:
            return localVer2 > remoteVer2
    else:
        return localVer1 > remoteVer1

def upload(newVersion):
    remoteContents = getMD5FromServer(newVersion)
    remoteContentsDict = {}
    remoteVersion = '0.0.0'
    for i in range(len(remoteContents)):
        segments = remoteContents[i].split(',')
        if len(segments) == 3:
            if i == 0:
                remoteVersion = segments[0]
            else:
                remoteContentsDict[segments[0]] = segments[1]            

    fin = open(os.path.join(NAME, newVersion, 'md5'))
    lines = fin.readlines()
    newFiles = []
    localVersion = '0.0.0'
    for i in range(len(lines)):        
        segments = lines[i].split(',')
        if len(segments) == 3:
            if i == 0:
                localVersion = segments[0]
            else:                
                if (not remoteContentsDict.has_key(segments[0])) or remoteContentsDict[segments[0]] != segments[1]:                
                    newFiles.append(segments[0])

    if isUploadValid(localVersion, remoteVersion):
        vers = newVersion.split('.')
        if len(vers) == 3:
            targetFile = os.path.join(NAME, '.'.join([vers[0], vers[1]]), 'md5')
            srcFile = os.path.join(NAME, newVersion, 'md5')            
            os.system('curl -T %s -u wanpishu:wanpishu ftp://%s:%s/update/%s --ftp-create-dirs'%(srcFile.replace('\\', '/'), IP, PORT, targetFile.replace('\\', '/')))
            for newFile in newFiles:
                targetFile = os.path.join(NAME, '.'.join([vers[0], vers[1]]), newFile)
                srcFile = os.path.join(NAME, newVersion, newFile)
                print(targetFile)
                os.system('curl -T %s -u wanpishu:wanpishu ftp://%s:%s/update/%s --ftp-create-dirs'%(srcFile.replace('\\', '/'), IP, PORT, targetFile.replace('\\', '/')))            
    else:
        print("Local version is lower than remote version.   localVersion:%s,remoteVersion:%s"%(localVersion, remoteVersion))
                

# -------------- main --------------
if __name__ == '__main__':
    if len(sys.argv) >= 2 and sys.argv[1] != None:
        IP = sys.argv[1]
    
    newVersion = raw_input('**********************************\n         Do not forget PackAll!!!!!!!!         \n*********************************\n[Publish To %s]\nPlease input new version:'%NAME)
    if not os.path.isdir(NAME):
        os.mkdir(NAME)    
    newFolder = os.path.join(NAME, newVersion)
    if not os.path.isdir(newFolder):
        os.mkdir(newFolder)

    copy_resources('../res', os.path.join(newFolder, 'res'))
    copy_resources('../src', os.path.join(newFolder, 'src'))
    
    os.system('%s %s'%(os.path.join(os.getcwd(), '../../Tools/encryptLua.py'), os.path.join(os.getcwd(), newFolder, 'src')))
    os.system('%s %s'%(os.path.join(os.getcwd(), '../../Tools/encryptRes.py'), os.path.join(os.getcwd(), newFolder, 'res')))

    contents = []
    makeMD5(newVersion, 'res', contents)
    makeMD5(newVersion, 'src', contents)
    genMD5(newVersion, contents)

    upload(newVersion)
