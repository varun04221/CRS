from sys import stdin

opcodes_dict = {"00000": ["add", "A"], "00001": ["sub", "A"], "00010": ["mov", "B"], "00011": ["mov", "C"],
                "00100": ["ld", "D"],
                "00101": ["st", "D"], "00110": ["mul", "A"], "00111": ["div", "C"], "01000": ["rs", "B"],
                "01001": ["ls", "B"],
                "01010": ["xor", "A"], "01011": ["or", "A"], "01100": ["and", "A"], "01101": ["not", "C"],
                "01110": ["cmp", "C"],
                "01111": ["jmp", "E"], "11100": ["jlt", "E"], "11101": ["jgt", "E"], "11111": ["je", "E"],
                "11010": ["hlt", "F"],
                "10000": ["addf", "G"], "10001": ["subf", "G"], "10010": ["movf", "G1"],
                "10100": ["comp", "S"], "10101": ["ceil", "S"], "10110": ["floor", "S"], "10111": ["max", "S1"], "11000": ["min", "S1"]}

reg_dict = {"000": 0, "001": 1, "010": 2, "011": 3, "100": 4, "101": 5, "110": 6, "111": 7}

flags = [0] * 4
reg_list = [0, 0, 0, 0, 0, 0, 0, flags]
halted = False

def check_overflow(reg):
    if (reg < 0):
        reg = 0
        flags[0] = 1
    if (reg > 65535):
        flags[0] = 1
        reg = takelower_16(reg)
    return reg

def takelower_16(n):
    b = bin(n)[2:]
    l = len(b) - 16
    n = int(b[l:], 2)
    return n

# type A
def typeA(instrn, op1, op2):  # add,sub,mul,xor,or,and(arithmetic)
    if instrn == "add":
        res = op1 + op2
        res = check_overflow(res)
        return res
    elif instrn == "sub":
        res = op1 - op2
        res = check_overflow(res)
        return res
    elif instrn == "mul":
        res = op1 * op2
        res = check_overflow(res)
        return res
    elif instrn == "and":
        res = op1 & op2
        return res
    elif instrn == "xor":
        res = op1 ^ op2
        return res
    elif instrn == "or":
        res = op1 | op2
        return res

# type B
def typeB(instrn, imm, reg):  # use imm values
    if instrn == "mov":
        return imm
    elif instrn == "ls":
        return reg << imm
    elif instrn == "rs":
        return reg >> imm

# type C
def typeC(instrn, updated_val, op):  # mov,div,notcmp
    if instrn == "mov":
        if op == flags:
            op = int(convert_flagstobin(), 2)
        reset_flags()
        return op
    elif instrn == "cmp":
        if updated_val == op:  # Equal - E flag set
            flags[3] = 1
        elif updated_val < op:  # Less than - L
            flags[1] = 1
        else:  # Greater than - G
            flags[2] = 1
        return updated_val
    elif instrn == "div":
        reg_list[0] = updated_val // op
        reg_list[1] = updated_val % op
        return updated_val
    elif instrn == "not":
        return op ^ 65535

# type D
def typeD(instrn, mem, reg):  # ld,st(memory instrns)
    if instrn == "ld":
        return int(memory[mem], 2)
    elif instrn == "st":
        memory[mem] = convert_to_16bit_bin(reg)
        return reg

# type E
def typeE(instrn, mem):  # jmp,jlt,jgt,je -jums
    global pc
    if instrn == "jmp":
        pc = mem - 1
    elif instrn == "jlt":
        if flags[1] == 1:
            pc = mem - 1
    elif instrn == "jgt":
        if flags[2] == 1:
            pc = mem - 1
    elif instrn == "je":
        if flags[3] == 1:
            pc = mem - 1
    reset_flags()

# type S
def typeS(instrn, op):
    if instrn == "ceil":
        if op%2 == 0:
            res = op // 2
        else:
            res = op // 2 + 1
    elif instrn == "floor":
        res = op // 2
    elif instrn == "comp":
        res = 127 - op
    return res

#type S1
def typeS1(instrn, op1, op2):
    if instrn == "max":
        res = max(op1, op2)
    elif instrn == "min":
        res = min(op1, op2)
    return res

# type F
def typeF():
    global halted
    halted = True

# type G
def typeG(instrn, op1, op2):
    if instrn == "addf":
        res = op1 + op2
        if res > 31.5:
            res = 0
            flags[0] = 1
    elif instrn == "subf":
        res = op1 - op2
        if res < 0:
            res = 0
            flags[0] = 1
    return res

def reset_flags():
    for i in range(4):
        flags[i] = 0

def check_and_reset_flags(instrn, type):
    if (instrn == "jlt" or instrn == "jgt" or instrn == "je"):
        return
    if (instrn == "mov" and type == "C"):
        return
    else:
        reset_flags()

def float_to_imm(num):
    pow = int(num[:3], 2) - 3
    man = int(num[3:], 2)
    return (1 + man * 2 ** (-5)) * (2 ** pow)

def execute_instruction(type, line, instrn):
    check_and_reset_flags(instrn, type)
    if type == "A":
        op1 = reg_list[reg_dict[line[10:13]]]
        op2 = reg_list[reg_dict[line[13:]]]
        reg_list[reg_dict[line[7:10]]] = typeA(instrn, op1, op2)
    elif type == "B":
        imm = int(line[9:], 2)
        reg = reg_list[reg_dict[line[6:9]]]
        reg_list[reg_dict[line[6:9]]] = typeB(instrn, imm, reg)
    elif type == "C":
        updated_val = reg_list[reg_dict[line[10:13]]]
        op = reg_list[reg_dict[line[13:]]]
        reg_list[reg_dict[line[10:13]]] = typeC(instrn, updated_val, op)
    elif type == "D":
        mem = int(line[9:], 2)
        reg = reg_list[reg_dict[line[6:9]]]
        reg_list[reg_dict[line[6:9]]] = typeD(instrn, mem, reg)
    elif type == "E":
        mem = int(line[9:], 2)
        typeE(instrn, mem)
    elif type == "S":
        op = reg_list[reg_dict[line[13:]]]
        reg_list[reg_dict[line[10:13]]] = typeS(instrn, op)
    elif type == "S1":
        op1 = reg_list[reg_dict[line[10:13]]]
        op2 = reg_list[reg_dict[line[13:]]]
        reg_list[reg_dict[line[7:10]]] = typeS1(instrn, op1, op2)
    elif type == "G":
        op1 = reg_list[reg_dict[line[10:13]]]
        op2 = reg_list[reg_dict[line[13:]]]
        reg_list[reg_dict[line[7:10]]] = typeG(instrn, op1, op2)
    elif type == "G1":
        imm = float_to_imm(line[8:])
        reg_list[reg_dict[line[5:8]]] = imm
    else:
        typeF()

def convert_to_16bit_bin(num):
    a = bin(int(num))[2:].zfill(16)
    return a

def convert_to_7_bit_bin(num):
    a = bin(int(num))[2:].zfill(7)
    return a

def convert_flagstobin():
    f = 12 * "0" + "".join(str(i) for i in flags)
    return f

def convert_to_16bit_float(num):
    num = str(num)
    num = num.split(".")
    whole, dec = int(num[0]), float(f".{num[1]}")
    whole = bin(whole)[2:]
    d = ''
    for i in range(0, 5):
        dec *= 2
        if (dec >= 1):
            d += '1'
        else:
            d += '0'
        dec = dec % 1
    d = d.rstrip("0")
    pow = len(whole) + 2
    man = whole[1:] + d
    man = man[:min(len(man), 5)]
    man = man + (5 - len(man)) * '0'
    pow = bin(pow)[2:].zfill(3)
    return (pow + man).zfill(16)

def printregs(pc):
    print(convert_to_7_bit_bin(pc), end="        ")
    for i in range(0, 7):
        if "." not in str(reg_list[i]):
            print(convert_to_16bit_bin(reg_list[i]), end=" ")
        else:
            print(convert_to_16bit_float(reg_list[i]), end=" ")
    print(convert_flagstobin())


instructions = stdin.read().splitlines()

memory = ["0" * 16] * 128

i = 0
for line in instructions:
    memory[i] = line
    i += 1
pc = 0

while not halted:
    opcode = instructions[pc][:5]
    instrn = opcodes_dict[opcode][0]
    # type = opcodes_dict[1]
    type = opcodes_dict[opcode][1]
    current = pc
    execute_instruction(type, instructions[pc], instrn)
    printregs(current)
    pc += 1

ctr = 0
# print \ memory dump
[print(i) for i in memory]
