import random
import re

h=24
w=159

COLOR_TRANS=0x00
COLOR_BLACK=0x01
COLOR_MEDGREEN=0x02
COLOR_LTGREEN=0x03
COLOR_DKBLUE=0x04
COLOR_LTBLUE=0x05
COLOR_DKRED=0x06
COLOR_CYAN=0x07
COLOR_MEDRED=0x08
COLOR_LTRED=0x09
COLOR_DKYELLOW=0x0A
COLOR_LTYELLOW=0x0B
COLOR_DKGREEN=0x0C
COLOR_MAGENTA=0x0D
COLOR_GRAY=0x0E
COLOR_WHITE=0x0F

tiles=[
        {'id':1,'name':'ground','pat':"0000000000000000",'fg':COLOR_LTGREEN,'bg':COLOR_DKGREEN},
        {'id':1,'name':'groundspec','pat':"00000600006003000",'fg':COLOR_LTGREEN,'bg':COLOR_DKGREEN},
        {'id':1,'name':'groundcap','pat':"FFFFFFEFE5A10000",'bg':COLOR_DKGREEN,'fg':COLOR_DKRED},
        {'id':1,'name':'stem','pat':"18185A5E7E381818",'bg':COLOR_LTBLUE,'fg':COLOR_DKGREEN},
        {'id':1,'name':'flower','pat':"0000183C7E7E3C18",'bg':COLOR_LTBLUE,'fg':COLOR_LTYELLOW},
        {'id':1,'name':'weed1','pat':"0000203012525494",'bg':COLOR_LTBLUE,'fg':COLOR_DKGREEN},
        {'id':1,'name':'weed2','pat':"0000203012525494",'bg':COLOR_LTBLUE,'fg':COLOR_DKGREEN},
        {'id':1,'name':'sky','pat':"0000000000000000",'fg':COLOR_DKGREEN,'bg':COLOR_LTBLUE}
      ]

sprites=[
        {'name':'DINOHEADLEFT','pat':"0000000306070707"},
        {'name':'DINOHEADRIGHT','frames':["000000E0F0F080E0","000000E0F0F080E0","000000E0F0F080E0","000000E0F0F080E0","000000E0F0F0E080","000000E0F0F0E080","000000E0F0F0E080","000000E0F0F0E080"]},
        {'name':'DINOBODY','frames':["878FFFFF7E3C2436","878FFFFF7E3C2436","878FFFFF7E3C2436","878FFFFF7E3C4680","878FFFFF7E3C4680","878FFFFF7E3C4680","878FFFFF7E3C140E","878FFFFF7E3C140E"]},
        {'name':'BARREL','frames':["3C7EFFFFBFDF6E3C","3C7EFFFFBFDF6E3C","3C7EFFFFFDFB763C","3C7EFFFFFDFB763C","3C76FBFDFFFF7E3C","3C76FBFDFFFF7E3C","3C6EDFBFFFFF7E3C","3C6EDFBFFFFF7E3C"]},
        ]


def getTile(name):
    for t in tiles:
        if t['name']==name:
            return t
    return None

def makeSingleRow(name):
    result=[]

    for i in range(0,w):
        result.append(getTile(name))

    return result

def makePlantRow():
    result1=[]
    result2=[]
    pop=[getTile('weed1'),getTile('weed2'),getTile('stem'),getTile('sky')]
    result2=random.choices(pop,weights=[20,20,10,50],k=w)
    return result2

def makeGround():
    result=[]
    pop=[getTile('ground'),getTile('groundspec')]
    result=random.choices(pop,weights=[30,70],k=w)
    return result

def makeMap():
    m=[]
    for r in range(0,15):
        m.append(makeSingleRow('sky'));

    m.append(makePlantRow());
    m.append(makeSingleRow('groundcap'))

    for r in range(0,8):
        m.append(makeGround())
    return m

def tilePatBA(p):
    p=re.findall('..',p)
    ba=[]
    for b in p:
      ba.append(int(b,16))
    return ba

def printMap(m,ct,pairs):
    #l=list(pairs.keys())
    #l.sort()
    print("#ifndef _MAP_H")
    print("#define _MAP_H")
    print()
    print("#define MAPCOLS "+str(w))
    print("#define MAPROWS "+str(h))
    print()
    
    byteno=0
    patno=0
    nametopatno={}
    bytenotocolor={}

    output=[]

    print("/**")
    for c in ct:
        print("* "+str(byteno)+" "+c);
        for p in ct[c]:
            print("* "+str(byteno)+"   "+str(patno)+"  0x%x"%patno+" "+p)
            output.append((byteno,patno,p))
            bytenotocolor[byteno]=(pairs[p]['tiles'][0]['fg'],pairs[p]['tiles'][0]['bg'])
            nametopatno[p]=patno
            patno=patno+1
            if (patno % 8)==0:
              byteno=byteno+1
              bytenotocolor[byteno]=(pairs[p]['tiles'][0]['fg'],pairs[p]['tiles'][0]['bg'])
        if patno<(byteno+1)*8:
            for i in range(patno,(byteno+1)*8):
                print("* "+str(byteno)+"   "+str(i)+" blank")
                output.append((byteno,i,"blank"))
        byteno=byteno+1
        patno=byteno*8
    for s in sprites:
        print("* -   "+str(patno)+"  0x%x"%patno+" "+s['name'])
        s['patno']=patno
        patno=patno+1

                

    print("**/\n\n")

    print("#define CTLEN "+str(byteno))
    for s in sprites:
        print("#define "+s['name']+' '+str(s['patno']))

    print()
    print("unsigned const char ct[CTLEN] = {")
    ctstr=""
    for c in bytenotocolor:
        ctstr=ctstr+"0x%x,\n" % (bytenotocolor[c][0]<<4 | bytenotocolor[c][1])
    print(ctstr[:-2])
    print("};")
    print()

    #print(output)
    #print(nametopatno)

            
    print("unsigned const char map[%d][%d] = {" % (h,w))
    for i in range(0,h):
        rs=""
        for j in range(0,w-1):
            rs=rs+"0x%x," % (nametopatno[m[i][j]])
        print( "{ ",end='')
        print(rs[:-1],end='')
        print("}",end='')
        print("," if i!=(h-1) else '')
    print("};")

    print()

    patlen=8*patno
    print("#define PATLEN "+str(patlen))
    print()
    c1=None
    for i in range(0,8):
      print("unsigned const char pat"+str(i)+"["+str(patlen)+"] = {")

      pats=""
      for k in output:
          if k[2]=='blank':
              for j in range(0,8):
                pats=pats+"0x20,"
          else:
              #pats=pats+k[2]+"\n"
              p=pairs[k[2]]
              for d in p['pat'][i]:
                pats=pats+"0x{0:x}".format(d)+","
          pats=pats+"\n"
      for s in sprites:

        if 'pat' in s:
          c1=tilePatBA(s['pat'])
        else:
          c1=tilePatBA(s['frames'][i])
        for d in c1:
          pats=pats+"0x{0:x}".format(d)+","
        pats=pats+"\n"
      pats=pats[:-2]
      print(pats)
      print("};")
      print()

    print("#endif")

def arrayShift(c1, c2, shift):
  p=[]
  for i in range(0,8):
    p.append((c1[i]<<shift & 0xff) | (c2[i] >> (8-shift)) & 0xff);
  return p

def printBin(c1,c2):
  for i in range(0,8):
    print("{0:08b}".format(c1[i]),end='')
    print(" ",end='')
    print("{0:08b}".format(c2[i]),end='')
    print()

def makeMergedPatterns(tile):
  c1=tilePatBA(tile['tiles'][0]['pat'])
  c2=tilePatBA(tile['tiles'][1]['pat'])
  #printBin(c1, c2)

  tile['pat']=[]
  for i in range(0,8):
    c3=arrayShift(c1,c2,i)
    tile['pat'].append(c3)
    #for j in range(0,8):
    #  print("{0:08b}".format(c3[j]).replace('0',' ').replace('1','#'))

def makePatterns(l,pairs):
  r=l.copy()
  r.reverse()
  l2=[]

  for i in range(0,len(r)-1):
    k=r[i]['name']+':'+r[i+1]['name']
    if not k in pairs:
      pairs[k]={'tiles':(r[i],r[i+1])}
    l2.append(k)

  l=list(pairs.keys())
  l.sort()

  for k in l:
    tile=pairs[k]
    if 'pat' not in tile:
      makeMergedPatterns(tile)

  return l2
  
    
pairs={}
m=makeMap()
am=[]
for r in m:
  am.append(makePatterns(r,pairs))


ct={}
for p in pairs:
    #print(pairs[p])
    a=str(pairs[p]['tiles'][0]['fg'])+":"+str(pairs[p]['tiles'][0]['bg'])
    b=str(pairs[p]['tiles'][1]['fg'])+":"+str(pairs[p]['tiles'][1]['bg'])
    if not a in ct:
        ct[a]=[]
    ct[a].append(p)

#print(ct)

#for i in ct:
#    print(i)
#    print(len(ct[i]))
#    print(ct[i])


#print(pairs)
printMap(am,ct,pairs)
