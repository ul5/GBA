from subprocess import Popen, PIPE
import fcntl
import os
import time
import json
import sys

class EmuParser():
    data = b""

    def __init__(self, bios, file):
        cmd = f"mgba -d -b {bios} {file}".split(" ")
        self.proc = Popen(cmd, stdin = PIPE, stdout = PIPE, stderr = PIPE)
        fcntl.fcntl(self.proc.stdout.fileno(), fcntl.F_SETFL, os.O_NONBLOCK)
        fcntl.fcntl(self.proc.stderr.fileno(), fcntl.F_SETFL, os.O_NONBLOCK)

    def send_cmd(self, cmd):
        self.proc.stdin.write(cmd.encode() + b'\n')
        self.proc.stdin.flush()    

        o = self.proc.stdout.read()
        if o:
            self.data += o

        sys.stdout.flush()

    def interact(self):
        (out, err) = self.proc.communicate(input = b'q\n')
        return (self.data + out).decode()


if __name__ == "__main__":
    p = EmuParser('GBA/bios.gba', 'GBA/emerald.gba')

    for i in range(100000):
        print(i)
        p.send_cmd('n')

    out = p.interact()
    
    a = out.split('\n')

    file = []
    for i in [a[i*6:i*6+6] for i in range(len(a) // 6)]:
        regs = {}
        for line in i[:5]:
            for reg in line.split('  '):
                if reg.strip() == '':
                    continue
                reg_name = reg.strip().split(':')[0]
                reg_value = reg.split(':')[1].strip()
                if reg_name == 'cpsr':
                    reg_value = reg_value.split(' ')[0]
                regs[reg_name] = reg_value
        regs['instr'] = i[5].split('\t')[1].upper()
        file.append(regs)
    with open('test.json', 'w') as f:
        json.dump(file, f)