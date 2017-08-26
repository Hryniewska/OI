import scipy
from collections import *
import re
import csv
import numpy as np
from numpy  import array
from scipy.optimize import minimize, rosen, rosen_der
import shlex, subprocess, os
from subprocess import Popen, PIPE


def optimization_function(x):

    # run solution
    CURRENT_DIR = os.path.dirname(__file__)
    file_path = os.path.join(CURRENT_DIR, 'bin')

    command_line = ('./run --param %d --solution ../../../environment/data/test/images --param2 %d' % (x[0], x[1]))
    print(command_line)
    cmd = subprocess.call(command_line, cwd=file_path, shell=True)


    # run environment
    command_line = './run'
    args = shlex.split(command_line)
    cmd = subprocess.run(args, cwd=os.chdir("../../environment/bin"), shell=True)


    #check accuracy
    try:
        os.getcwd()
        with open('../wyniki_koncowe.txt', 'r') as readfile:
            reader = readfile.read()
            row = (reader.split('\n'))[1]
            row = row.split(': ')
            accuracy = row[1]
            print('poprawnosc {}'.format(accuracy))
    except IOError:
        exit(-1)


    return -int(accuracy)



x = list()

try:
    x.append(40)
    x.append(30)
    # print(x)
    res = minimize(optimization_function, x, method='BFGS', options={'eps': 2e1, 'disp': True})
    # print(res.x)
except Exception as e:
    print('nie dziala', e)