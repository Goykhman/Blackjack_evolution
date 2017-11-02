'''
1. Creates the chromosome file "chrom_evolved.csv" from
the mean chromosome by assigning each entry either 1 or 0,
depending on whether the corresponding mean value is
larger or equal than "a", where we chose a=0.95.

2. Prints out resulting de-serialized strategy matrices
into console.
'''

from __future__ import print_function
import csv
import matplotlib.pyplot as plt

###########################################################################

with open('chrom.csv', 'r') as file:
    reader = csv.reader(file)
    chromosome = list(reader)

chromosome=chromosome[0]
chromosome=[float(x) for x in chromosome]

improved_chromosome=[]

a=0.95

for x in chromosome:
    if x>=a:
        improved_chromosome+=[1]
    else:
        improved_chromosome+=[0]

with open("strategy_chromosome.csv", "wb") as f:
    writer = csv.writer(f)
    writer.writerow(improved_chromosome)

###########################################################################

# splice chromosome into separate strategy vectors

chrom_split=improved_chromosome[:100]
chrom_soft_double_down=improved_chromosome[100:200]
chrom_hard_double_down=improved_chromosome[200:400]
chrom_soft_stand=improved_chromosome[400:600]
chrom_hard_stand=improved_chromosome[600:800]

# de-serialize chromosome vectors into tables

table_split=[]
for i in range(10):
    row=[]
    for j in range(10):
        row+=[chrom_split[10*i+j]]
    table_split+=[row]

table_soft_double_down=[]
for i in range(10):
    row=[]
    for j in range(10):
        row+=[chrom_soft_double_down[10*i+j]]
    table_soft_double_down+=[row]

table_hard_double_down=[]
for i in range(20):
    row=[]
    for j in range(10):
        row+=[chrom_hard_double_down[10*i+j]]
    table_hard_double_down+=[row]

table_soft_stand=[]
for i in range(20):
    row=[]
    for j in range(10):
        row+=[chrom_soft_stand[10*i+j]]
    table_soft_stand+=[row]

table_hard_stand=[]
for i in range(20):
    row=[]
    for j in range(10):
        row+=[chrom_hard_stand[10*i+j]]
    table_hard_stand+=[row]

card={0:'A',1:'2',2:'3',3:'4',4:'5',5:'6',6:'7',7:'8',8:'9',9:'T'}

#print split
print("*****************************************************************")
print("                             Mean split                          ")
print("*****************************************************************")
print("***** ",end="")
print("  A  ",end='')
print("  2  ",end='')
print("  3  ",end='')
print("  4  ",end='')
print("  5  ",end='')
print("  6  ",end='')
print("  7  ",end='')
print("  8  ",end='')
print("  9  ",end='')
print("  T  ")
for i in range(10):
    si=card[i]
    print("  "+ si + "  ",end='')
    for j in range(10):
        s=table_split[i][j]
        s=int(s/a)
        s=str(s)
        s="   "+s
        while(len(s)<5):
            s+=" "
        print(s,end='')
    print("\n")

#print soft double down
print("*****************************************************************")
print("                        Mean soft double down                    ")
print("*****************************************************************")
print("***** ",end="")
print("  A  ",end='')
print("  2  ",end='')
print("  3  ",end='')
print("  4  ",end='')
print("  5  ",end='')
print("  6  ",end='')
print("  7  ",end='')
print("  8  ",end='')
print("  9  ",end='')
print("  T  ")
for i in range(10):
    si=card[i]
    print("  "+ si + "  ",end='')
    for j in range(10):
        s=table_soft_double_down[i][j]
        s=int(s/a)
        s=str(s)
        s="   "+s
        while(len(s)<5):
            s+=" "
        print(s,end='')
    print("\n")

#print hard double down
print("*****************************************************************")
print("                        Mean hard double down                    ")
print("*****************************************************************")
print("***** ",end="")
print("  A  ",end='')
print("  2  ",end='')
print("  3  ",end='')
print("  4  ",end='')
print("  5  ",end='')
print("  6  ",end='')
print("  7  ",end='')
print("  8  ",end='')
print("  9  ",end='')
print("  T  ")
for i in range(20):
    si=str(i+2)
    print("  "+ si + "  ",end='')
    if i<8:
        print(" ",end='')
    for j in range(10):
        s=table_hard_double_down[i][j]
        s=int(s/a)
        s=str(s)
        s="  "+s
        while(len(s)<5):
            s+=" "
        print(s,end='')
    print("\n")

#print soft stand
print("*****************************************************************")
print("                          Mean soft stand                        ")
print("*****************************************************************")
print("***** ",end="")
print("  A  ",end='')
print("  2  ",end='')
print("  3  ",end='')
print("  4  ",end='')
print("  5  ",end='')
print("  6  ",end='')
print("  7  ",end='')
print("  8  ",end='')
print("  9  ",end='')
print("  T  ")
for i in range(20):
    si=str(i+2)
    print("  "+ si + "  ",end='')
    if i<8:
        print(" ",end='')
    for j in range(10):
        s=table_soft_stand[i][j]
        s=int(s/a)
        s=str(s)
        s="  "+s
        while(len(s)<5):
            s+=" "
        print(s,end='')
    print("\n")

#print hard stand
print("*****************************************************************")
print("                          Mean hard stand                        ")
print("*****************************************************************")
print("***** ",end="")
print("  A  ",end='')
print("  2  ",end='')
print("  3  ",end='')
print("  4  ",end='')
print("  5  ",end='')
print("  6  ",end='')
print("  7  ",end='')
print("  8  ",end='')
print("  9  ",end='')
print("  T  ")
for i in range(20):
    si=str(i+2)
    print("  "+ si + "  ",end='')
    if i<8:
        print(" ",end='')
    for j in range(10):
        s=table_hard_stand[i][j]
        s=int(s/a)
        s=str(s)
        s="  "+s
        while(len(s)<5):
            s+=" "
        print(s,end='')
    print("\n")
