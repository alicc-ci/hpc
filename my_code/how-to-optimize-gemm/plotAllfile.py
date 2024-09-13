import sys
import os
import numpy as np   # pip install numpy
import matplotlib.pyplot as plt

# 自动读取_data目录下的.m文件（不包括output_new.m和output_old.m）绘图
def readFile(filename):
    f = open(filename)
    sizes = []
    gflops = []
    try:

        f.readline()
        f.readline()  # version
        f.readline()
        while True:
            line = f.readline()
            if line:
                slices = line.split(" ")
                if len(slices) <= 2:
                    break
                size = int(slices[0])
                flops = float(slices[1])
                sizes.append(size)
                gflops.append(flops)
    finally:
        f.close()
    return sizes, gflops

if __name__ == '__main__':
    plt.rcParams.update({'font.size': 15})
    plt.xlabel('shape')
    plt.ylabel('gflops')
    l = len(sys.argv)
    
    curves_marker = ['bo-.', 'r-*', 'g-.', 'k--', 'm,', 'c:']

    data_dir = './_data/'
    files = os.listdir(data_dir)
    print(files)
    i = 0
    for filename in files:
        if not filename.endswith('.m') :
            continue
        if 'new' in filename or 'old' in filename:
            continue
        lable = filename.replace('output_', '').replace('.m','')
        x,y = readFile(data_dir + filename)
        plt.plot(x,y, curves_marker[i],label=lable)
        i = i + 1
    
    plt.legend()
    # plt.savefig("all_method.png")
    plt.show()

