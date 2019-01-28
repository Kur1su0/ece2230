from __future__ import print_function
import matplotlib.pyplot as plt
import os
import re
import csv
from scipy.interpolate import spline
from scipy.interpolate import interp1d
import numpy as np
def Plot(data, start, end  ):
    x = []
    y = []
    for i in range(start,end):
        #if i!=0:
        x.append(data[i][0])

    for i in range(start,end):
        #if i!=0:
        y.append(data[i][1])
    #print(x)
    x    = np.asarray(x)
    y    = np.asarray(y)
    xnew = np.arange(x[0],x[-1],100) 
    func = interp1d(x, y, kind='cubic')
    ynew = func(xnew)
    #print(xnew)
    plt.plot( xnew ,ynew, label = data[start - 1])
    #plt.axis()
    #plt.show()
    #y.clear()
    #x.clear

def main():
    path = "out/out.log"
    raw = []
    raw2 = []
    x = []
    y = []

#read standard e
    with open(path,'r') as f:
        for line in f.readlines():
            raw.append(line.strip("\n"))
    #print(raw)

    for i in range(len(raw)):
        raw2.append(re.split(r'\t+',raw[i]))

    raw2.pop()
    raw2 = raw2[1:]
    #convert into int.
    for i in range(len(raw2)):
        if(i%6!=0):
            for j in range(3):
                if (j is not 1 ):
                    raw2[i][j] = int(raw2[i][j])
                else:
                    raw2[i][j] = float(raw2[i][j])
        #print(i, raw2[i])
    
    
    plt.figure("Different Sort in the same order")
    plt.subplot(311)
    Plot(raw2,1,6)
    Plot(raw2,19,24)
    Plot(raw2,37,37+5)
    plt.legend(loc=2,prop={'size':12})
    
    
    #plt.figure("ascending list")
    plt.subplot(312)
    Plot(raw2,7,7+5)
    Plot(raw2,25,25+5)
    Plot(raw2,43,43+5)
    
    plt.legend(loc=2,prop={'size':12})
    
    #plt.figure("descending list")
    plt.subplot(313)
    Plot(raw2,13,13+5)
    Plot(raw2,31,31+5)
    Plot(raw2,49,49+5)
    plt.legend(loc=2,prop={'size':12})
    
    plt.figure("Merge Sort")
    Plot(raw2,55,55+5)
    Plot(raw2,61,61+5)
    Plot(raw2,67,67+5)
    plt.legend(loc=2,prop={'size':12})
  
   
    plt.figure("Same sorting in the different order")
    plt.subplot(311)
    Plot(raw2,1,1+5)
    Plot(raw2,7,7+5)
    Plot(raw2,13,13+5)
    plt.legend(loc=2,prop={'size':12})
    
    #plt.figure("recursive Selection")
    plt.subplot(312)
    Plot(raw2,19,19+5)
    Plot(raw2,25,25+5)
    Plot(raw2,31,31+5)
    plt.legend(loc=2,prop={'size':12})

    #iter
    plt.subplot(313)
    Plot(raw2,37,37+5)
    Plot(raw2,43,43+5)
    Plot(raw2,49,49+5)
    plt.legend(loc=2,prop={'size':12})

    plt.show()
    
    csvfile = "result.csv"
    with open(csvfile, "w") as output:
        writer = csv.writer(output,lineterminator=',')
        writer.writerow(raw2)

if __name__ == '__main__':
    main()
