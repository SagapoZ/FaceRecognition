#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os.path
import sqlite3

# This is a tiny script to help you creating a CSV file from a face
# database with a similar hierarchie:
#
#  philipp@mango:~/facerec/data/at$ tree
#  .
#  |-- README
#  |-- s1
#  |   |-- 1.pgm
#  |   |-- ...
#  |   |-- 10.pgm
#  |-- s2
#  |   |-- 1.pgm
#  |   |-- ...
#  |   |-- 10.pgm
#  ...
#  |-- s40
#  |   |-- 1.pgm
#  |   |-- ...
#  |   |-- 10.pgm
#
#def processFile():

#用户名 用户id 打卡次数 标签  name id label attence

if __name__ == "__main__":
    conn = sqlite3.connect('../face.db')
    c = conn.cursor()
    print("Opened database successfully")

    BASE_PATH = "../video/att_faces/"
    SEPARATOR = ";"
    fh = open("../video/att_faces/at.txt",'w')
    label = 0
    for dirname, dirnames, filenames in os.walk(BASE_PATH):
      for subdirname in dirnames:
          subject_path = os.path.join(dirname, subdirname)

          #print("subdirname: ",subdirname)
          c.execute("INSERT OR IGNORE INTO user (name,label) \
               VALUES ('{}',{})".format(subdirname,label))

          for filename in os.listdir(subject_path):
              abs_path = "%s/%s" % (subject_path, filename)
              print("%s%s%d" % (abs_path, SEPARATOR, label))
              fh.write(abs_path)
              fh.write(SEPARATOR)
              fh.write(str(label))
              fh.write("\n")
          label = label + 1

    conn.commit()
    print("Records created successfully")
    fh.close()
    conn.close()
