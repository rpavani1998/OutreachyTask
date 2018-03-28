import sys
import os
import hashlib
from nose.tools import assert_raises, assert_equal, with_setup
import fcntl
import rados
import cephfs as libcephfs

cephfs = None

#setting up cephfs
def setup_cephfs():
		global cephfs
		cephfs = libcephfs.LibCephFS(conffile='ceph/build/ceph.conf')
		cephfs.mount()

#creating file in filesystem
def create_file(file_name):
    if not isinstance(file_name, bytes):
      file_name = bytes(file_name, encoding="ascii")
    fd = cephfs.open(file_name, 'w', 0o755)   
    data = bytes(input("\nEnter data into the file\n"), encoding="ascii")
    cephfs.write(fd, data, 0)
    
#accessing file in filesystem
def get_file(file_name):
    fd = -1
    if not isinstance(file_name, bytes):
      file_name = bytes(file_name, encoding="ascii")
    cephfs.chdir(b'/')
    d = cephfs.opendir(b"/")
    dent = cephfs.readdir(d)
    try :
       fd = cephfs.open(file_name,'r', 0o755) 
       return fd 
    except : #handling file exception
       print( "\nFile not found\n") 
       choice = input("Do you want to create the file? (y/n) : ");
       if choice == 'y' or choice == 'Y' :
         create_file(file_name)
         fd = cephfs.open(file_name,'r', 0o755)
       return fd
 
#finding sha1sum of the file       
def sha1sum(file_name) :
    fd = get_file(file_name)
    if fd == -1 :
      return ""
    BUF_SIZE = 65536
    sha1 = hashlib.sha1()
    while True:
      data = cephfs.read(fd, BUF_SIZE, 1024)
      if not data:
        break
      sha1.update(data)
    return format(sha1.hexdigest())

#initiating
setup_cephfs()
result = sha1sum(sys.argv[1])
if result :
   print("\nSHA1 of", sys.argv[1],"is",result)
