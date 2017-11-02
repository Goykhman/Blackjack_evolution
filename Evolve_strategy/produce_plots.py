from __future__ import print_function
import csv
import matplotlib.pyplot as plt
import numpy as np

###########################################################################

with open('scores.csv', 'r') as file:
    reader = csv.reader(file)
    time_series = list(reader)

time_series=time_series[0]
time_series=[float(x) for x in time_series]

mn=np.sum(time_series[-100:])/100
mn=int(mn*10000)/10000.0

time_series_plot=plt.figure(dpi=300)
plt.plot(range(len(time_series)),time_series)
plt.xlabel("rounds of evolution")
plt.ylabel("mean fit of the 5% of the best strategies")
plt.title("Bet 2 each round, starting from 10000, \n evolved strategy fit saturates to {}".format(mn))
ax = time_series_plot.gca()
ax.set_yticks(np.arange(1.048, 1.056, 0.001))
plt.grid()
plt.show()
time_series_plot.savefig("evolved_fit.pdf",\
                         dpi=300, facecolor='w', edgecolor='w',\
                         orientation='portrait', papertype=None, format=None,\
                         transparent=False, bbox_inches=None, pad_inches=0.1,\
                         frameon=None)

###########################################################################

with open('chrom.csv', 'r') as file:
    reader = csv.reader(file)
    chromosome = list(reader)

chromosome=chromosome[0]
chromosome=[float(x) for x in chromosome]

# splice chromosome into separate strategy vectors

chrom_split=chromosome[:100]
chrom_soft_double_down=chromosome[100:200]
chrom_hard_double_down=chromosome[200:400]
chrom_soft_stand=chromosome[400:600]
chrom_hard_stand=chromosome[600:800]

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
        s=int(100*s)/100.0
        s=str(s)
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
        s=int(100*s)/100.0
        s=str(s)
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
        s=int(100*s)/100.0
        s=str(s)
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
        s=int(100*s)/100.0
        s=str(s)
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
        s=int(100*s)/100.0
        s=str(s)
        while(len(s)<5):
            s+=" "
        print(s,end='')
    print("\n")
