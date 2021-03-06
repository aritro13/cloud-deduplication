import ctypes
import net
import socket
import random
import hashlib
import time
import SQL as sq
import net2
hhash = hashlib.sha256()
import re
import os
from threading import Thread

def intoip(n):
  ips = []
  for i in n:
    ii = ""
    for j in range(4):
      ii = str((int(str(ord(i))) >> 8*j) & 255)+"."+ii
    ips += [ii[:-1]]
  print "chunks_manager = ",ips
  return ips

def INSERT_or_SEARCH(CHECKSUM, IP, C_CODE, FLAG, IP_COUNT):
  C_CODE.INSERT_NODE.restype = ctypes.c_wchar_p
  if FLAG == 1:
    # Insert nodes
    g = ctypes.c_wchar_p(C_CODE.INSERT_NODE(ctypes.c_char_p(IP), ctypes.c_char_p(CHECKSUM), ctypes.c_int(IP_COUNT)))
    # g = ctypes.c_char_p(C_CODE.INSERT_NODE(ctypes.c_char_p(IP), ctypes.c_char_p(CHECKSUM), ctypes.c_int(IP_COUNT)))
    # return 0
    print "\nG: ",g.value
    # print ctypes.c_wchar_p.from_address()
  else:
    # Search nodes
    g = ctypes.c_wchar_p(C_CODE.SEARCH_NODE(ctypes.c_char_p(CHECKSUM)))
    # g = ctypes.c_uint(C_CODE.SEARCH_NODE(ctypes.c_char_p(CHECKSUM)))
  if g.value != None:
    if len(g.value) != 0:
      print "Node ip", FLAG
      return intoip(g.value)
  else:
    return 0

def get_randoms(r, n):
  a = []
  while n != 0:
    k = random.randint(0,r-1)
    if k not in a:
      a += [k]
      n -= 1
  return a

def ssendd(sock, c_checksum, data):
  print "Sending to Server..."
  sock.send("PREPARE*"+c_checksum)
  m = sock.recv(1024)
  if m == "READY":
    net.data_uploadFile(data, sock)
  print "Sended to Server"


#some error can't debug
def STORE_ALL_256_KB_001(socks, sql, m_checksum, C_CODE):
  print "Storing all 256kb chunks "
  seq_no = 0
  L = len(socks[0]) # for generating random nos in the range of L
  with open(m_checksum,'rb') as F:
    print "file opened"
    flag = 0
    while flag!=1:
      data = ''
      b = ''
      for i in range(256):
        b = F.read(1024)
        data += b
        if b=='':
          flag = 1
          print "File read complete"
          break

      if data:
        print "Data: ", type(data) 
        hhash = hashlib.sha256()
        hhash.update(data)
        c_checksum = hhash.hexdigest()
        print "c_checksum: ", c_checksum
        seq_no += 1
        s = []
        if L < 3:
          print "PLEASE MAKE MORE SUB SERVERS.."
          print "Minimum 3 sub servers are required.."
          return
        else:
          s = get_randoms(L,3)
        ips = '.*'.join(socks[1][h] for h in s)
        ips += '.*'
        try:
          print "Inserting node into RAM (hashtable)"
          ips_ram = INSERT_or_SEARCH(c_checksum, ips, C_CODE, 1, 3)
        except Exception as e:
          print "Insert into RAM Failed: ", e
        # print "ips_ram: ", ips_ram
        if ips_ram == 0:
          for j in s:
            ssendd(socks[0][j], c_checksum, data)
        print "inserting,,,",m_checksum,c_checksum
        sql.exeq('insert into 256kb_chunks(m_checksum, c_checksum, seq_no) values("%s", "%s", %s)'%(m_checksum, c_checksum, seq_no))
        sql.exeq('commit')
      else:
        print "Data: ", type(data), data 
        print "No data in file"

def isUP(ip, flag):
  if flag == 1:
    a = os.popen("ping -c 20 -i 0.2 " + ip).read()
    b = re.findall("(\d+\% packet loss)",a)
    c = re.findall("\d+", b[0])[0]
    accuracy = 100-int(c)
    if accuracy > 70:
      return True
    else:
      return False
  else:
    # if flag == 0 then it will not waste time to show output or it will becomes slow, 4 seconds per block
    return True

def RESTORE_ALL_256_KB(socks, cursor, _256kb_chunk_list, MAIN_FILENAME, C_CODE):
  print "RESTORE ALL CHUNKS"
  with open(MAIN_FILENAME, 'wb') as MAIN:
    for i in _256kb_chunk_list:
      try:
        ips = INSERT_or_SEARCH(i[0], "", C_CODE, 0, 0)
      except Exception as e:
        print "node search failed "
      iip = 0
      BUFF = ""
      while True and iip < len(ips):
        if isUP(ips[iip], 1):
          print ips[iip], "is UP!"
          BUFF = net2.dataa_downloadFile(i[0], socks[0][socks[1].index(ips[iip])])
          break
        else:
          print ips[iip], "is Down!, trying other server.."
          iip += 1
      if not iip < len(ips):
        print "3 sub servers are down!!"
      MAIN.write(BUFF)
  print "Main File",MAIN_FILENAME,"is restored!"
