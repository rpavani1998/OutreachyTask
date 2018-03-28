# OutreachyTask
Program that uses the libcephfs library and computes the sha1sum of a file in CephFS.

## Task 1 : Creating a ceph cluster

* Clone the [Ceph GitRepo](https://github.com/ceph/ceph)
* Install all the dependencies
* Build Ceph
* Run a test cluster

[Commands for creating a cluster](https://github.com/ceph/ceph/blob/master/README.md)

## Task 2 : Using libcephfs library

 After running a test cluster, follow the following steps to compute sha1sum of a file in CephFS
 
#### Using Python

 ```python3 sha1sum.py <file-name>```
 
 
#### Using C
 
 Compiling the sha1sum.c file
 
  ``` gcc sha1sum.c -lssl -lcrypto ceph/build/lib/libcephfs.so```
  
  ```./a.out <file-name>```
  

 
