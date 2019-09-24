from subprocess import Popen, PIPE
import fcntl
import os
import time
import json
import sys

class EmuParser():
    data = b""

    def __init__(self):
        cmd = f"./run.sh".split(" ")
        self.proc = Popen(cmd, stdin = PIPE, stdout = PIPE, stderr = PIPE, shell = True)
        fcntl.fcntl(self.proc.stdout.fileno(), fcntl.F_SETFL, os.O_NONBLOCK)
        fcntl.fcntl(self.proc.stderr.fileno(), fcntl.F_SETFL, os.O_NONBLOCK)

    def interact(self):
        return self.proc.communicate()[0].decode()

def diff(mgba_data, my_data):
    for i in range(22, len(my_data)): # First 21 are a little bit wonky with the setup
        for k, v in my_data[i].items():
            if k == 'instr':
                continue
            if k not in mgba_data[i] or mgba_data[i][k] != v:
                if k != 'cpsr':
                    print(f"DIFF AT: Index {i} for the key {k}")
                    print(f"\tReal data: {mgba_data[i]}")
                    print(f"\tNew  data: {my_data[i]}")
                    print("Previous instruction:")
                    print(f"\tReal data: {mgba_data[i-1]}")
                    print(f"\tNew  data: {my_data[i-1]}")
                    input()
                    #return
                else:
                    print("WARN: CPSR DIFFERENT")   
                    print(f"\tReal data: {mgba_data[i]}")
                    print(f"\tNew  data: {my_data[i]}")
                    print("Previous instruction:")
                    print(f"\tReal data: {mgba_data[i-2]}")
                    print(f"\tNew  data: {my_data[i-2]}")
                    print(f"\tReal data: {mgba_data[i-1]}")
                    print(f"\tNew  data: {my_data[i-1]}")
                    input()         
                    continue

        print(f"\t{mgba_data[i]['instr']}")
        #print(f"\tNew  data: {my_data[i]}")

    print("No diff... all ok")

if __name__ == "__main__":
    p = EmuParser()

    out = p.interact().split('\n')

    with open('../test.json', 'r') as f:
        data = json.load(f)[1:]

    data = [i for i in data if i['instr'] != 'BL ']

    
    file = []
    regs = {}
    for line in out[4:]:
        if "Jump " in line:
            continue
        if "[DEBUGGER]" in line:
            for reg in line[10:].split(",")[:-1]:
                reg_name = reg.split('=')[0].strip()
                reg_value = reg.split('=')[1].strip()
                if reg_name == 'SP':
                    reg_name = "r13"
                elif reg_name == 'LR':
                    reg_name = 'r14'
                elif reg_name == 'PC':
                    continue
                elif reg_name == 'SPSR':
                    continue

                regs[reg_name.lower()] = reg_value
            if "SPSR" in line:
                file.append(regs)
                regs = {}

        if "==>" in line:
            file[-1]['instr'] = line[12:]

    diff(data, file)
            