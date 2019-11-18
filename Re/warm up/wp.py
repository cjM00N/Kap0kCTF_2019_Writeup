from collections import Counter
from z3 import *

s = Solver()
flag = [BitVec("a%d"%i,8) for i in range(24)]
cnt = [1,1,0,1,2,1,1,1,2,3,2,3,1,2,1,2]
sum1= [243,208,248,250,212,201,258,169]
pow1= [21317,15754,22206,22218,16456,14909,23540,9523]
sum2= [293,200,251,212,255,211,212,155]
xor2= [97,96,59,106,63,107,106,53]
memcmp=[0,1,0,1,0,1,0,1,0,0,0,1,1,0,1,1,0,1,1,0,0,1,1,1]

for i in range(0,len(flag),3):
    s.add(flag[i]+flag[i+1]+flag[i+2]==sum1[i/3])
    s.add(flag[i]*flag[i]+flag[i+1]*flag[i+1]+flag[i+2]*flag[i+2]==pow1[i/3])
    
for i in range(8):
    s.add(flag[i]+flag[i+8]+flag[i+16]==sum2[i])
    s.add(flag[i]^flag[i+8]^flag[i+16]==xor2[i])

for x in flag:
    s.add(0x30<=x)
    s.add(0x67>x)

for j in range(24):
    if memcmp[j] == 0:
        s.add(flag[j]> 0x60)
        s.add(flag[j]<=0x66)
    else:
        s.add(flag[j]<=0x39)
        s.add(flag[j]> 0x2f)
        
if s.check() == sat:
    m = s.model()
    kk = ""
    for i in range(0,24):
        kk +=  chr(int("%s" % (m[flag[i]])))
    print "kap0k{"+kk+"}"
