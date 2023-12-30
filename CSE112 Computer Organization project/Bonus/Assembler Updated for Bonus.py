instructions = {
    "add": "0000000",
    "sub": "0000100",
    "mov": ["000100", "0001100000"],
    "ld": "001000",
    "st": "001010",
    "mul": "0011000",
    "div": "0011100000",
    "rs": "010000",
    "ls": "010010",
    "xor": "0101000",
    "or": "0101100",
    "and": "0110000",
    "not": "0110100000",
    "cmp": "0111000000",
    "jmp": "011110000",
    "jlt": "111000000",
    "jgt": "111010000",
    "je": "111110000",
    "hlt": "1101000000000000",
    "addf": "1000000",
    "subf": "1000100",
    "movf": "10010",
    "comp": "1010000000",
    "ceil": "1010100000",
    "floor": "1011000000",
    "max": "1011100",
    "min": "1100000"
}

variables = {}
labels = {}
mem_start = 0
var_flag = 0
line_num = -1
error_flag = 0
file_line = 0

overflow_flag, less_than_flag, greater_than_flag, equal_flag = 0, 0, 0, 0

assembly = []


def send_error(error_msg):
    f = open("output.txt", "w")
    f.write(f"{error_msg}\n")
    f.close()
    exit(0)

def reg_to_bin(reg, file_line):
    if reg == "FLAGS":
        send_error(f"Illegal use of FLAGS registered, line {file_line}")
    elif len(reg) == 2 and reg[0] == "R" and reg[1] in "0123456":
        return (bin(int(reg[1]))[2:]).zfill(3)
    else:
        send_error(f"Typos in register name, line {file_line}")

def get_mem_addr(mem):
    return bin(mem)[2:].zfill(7)

def imm_to_bin(num, file_line):
    if len(num) > 1 and num[1:].isnumeric():
        num = int(num[1:])
        if (num >= 0 and num <= 127):
            return bin(num)[2:].zfill(7)
        else:
            send_error(f"Illegal Immediate Values, line {file_line}")
    else:
        send_error(f"Illegal Immediate Values, line {file_line}")

def var_to_bin(var):
    return variables[var]["mem"]

def float_to_bin(num, file_line):
    flag = 1
    for i in num[1:]:
        if i not in "0123456789.":
            flag = 0
    if len(num) > 1 and flag:
        num = num[1:]
        if float(num) > 31.5:
            send_error(f"Illegal Immediate Floating Value, line {file_line}")
        if "." in num and num.count(".") == 1:
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
            print(pow + man)
            return pow + man
        elif "." not in num:
            num = bin(int(num))[2:]
            man = num[1:]
            pow = len(num) + 2
            pow = bin(pow)[2:].zfill(3)
            man = man[:min(len(man), 5)]
            man = man + (5 - len(man)) * '0'
            return pow + man
        else:
            send_error(f"Invalid Immediate Floating Values, line {file_line}")
    else:
        send_error(f"Invalid Immediate Floating Values, line {file_line}")

def get_ins(s):
    global var_flag, mem_start, line_num, error_flag, file_line
    line_num += 1
    if s[0] == "var" and not var_flag:
        if len(s) != 2:
            send_error(f"Error in variable assignment, line {file_line}")
        elif s[1] not in variables:
            line_num -= 1
            variables[s[1]] = {"mem": None, "value": None}
            if s[1] in ["R1", "R2", "R3", "R4", "R5", "R6", "FLAGS"]:
                send_error(f"Use of register name as variable name, line {file_line}")
            mem_start += 1
        else:
            send_error(f"Variable already declared")
    elif s[0] == "var" and var_flag:
        send_error(f"Variable not declared at the beginning, line {file_line}")
    elif ":" in s[0] and s[0][-1] == ":":
        if s[0][:-1] not in labels:
            labels[s[0][:-1]] = {"Line": bin(line_num)[2:].zfill(7)}
            line_num -= 1
            if len(s) > 1:
                get_ins(s[1:])
            else:
                send_error(f"No instruction, line {file_line}")
        else:
            send_error(f"Label already declared, line {file_line}")
    elif ":" in s[0]:
        send_error(f"No space between colon and instruction, line {file_line}")
    elif len(s) > 1 and ":" in s[1]:
        send_error(f"General Syntax Error, line {file_line}")
    elif s[0] not in instructions:
        send_error(f"Typos in instruction name, line {file_line}")
    elif s[0] == "hlt":
        assembly.append(instructions[s[0]])
        if len(s) != 1:
            send_error(f"General Syntax Error, line {file_line}")
    elif s[0] in instructions:
        ins = instructions[s[0]]
        var_flag = 1
        if s[0] == "mov":
            if len(s) != 3:
                send_error(f"General Syntax Error, line {file_line}")
            if "$" == s[2][0]:
                ins = ins[0] + reg_to_bin(s[1], file_line) + imm_to_bin(s[2], file_line)
                assembly.append(ins)
            else:
                if s[2] == "FLAGS":
                    ins = ins[1] + reg_to_bin(s[1], file_line) + "111"
                    assembly.append(ins)
                else:
                    ins = ins[1] + reg_to_bin(s[1], file_line) + reg_to_bin(s[2], file_line)
                    assembly.append(ins)
        elif s[0] == "movf":
            if len(s) != 3:
                send_error(f"General Syntax Error, line {file_line}")
            if "$" == s[2][0]:
                ins = ins + reg_to_bin(s[1], file_line) + float_to_bin(s[2], file_line)
                assembly.append(ins)
            else:
                send_error(f"General Syntax Error, line {file_line}")
        elif len(ins) == 7:
            if len(s) != 4:
                send_error(f"General Syntax Error, line {file_line}")
            else:
                ins = ins + reg_to_bin(s[1], file_line) + reg_to_bin(s[2], file_line) + reg_to_bin(s[3], file_line)
                assembly.append(ins)
        elif len(ins) == 6:
            if s[0] == "ld":
                if len(s) != 3:
                    send_error(f"General Syntax Error, line {file_line}")
                else:
                    ins = [ins, reg_to_bin(s[1], file_line), s[2], file_line]
                    if s[2] not in variables:
                        send_error(f"Variable not declared at beginning, line {file_line}")
                    assembly.append(ins)
            elif s[0] == "st":
                if len(s) != 3:
                    send_error(f"General Syntax Error, line {file_line}")
                else:
                    ins = [ins, reg_to_bin(s[1], file_line), s[2], file_line]
                    if s[2] not in variables:
                        send_error(f"Variable not declared at beginning, line {file_line}")
                    assembly.append(ins)
            elif s[0] in ["rs", "ls"]:
                if len(s) != 3:
                    send_error(f"General Syntax Error, line {file_line}")
                else:
                    ins = ins + reg_to_bin(s[1], file_line) + imm_to_bin(s[2], file_line)
                    assembly.append(ins)
        elif len(ins) == 10:
            if len(s) != 3:
                send_error(f"General Syntax Error, line {file_line}")
            else:
                ins = ins + reg_to_bin(s[1], file_line) + reg_to_bin(s[2], file_line)
                assembly.append(ins)
        elif len(ins) == 9:
            if len(s) != 2:
                send_error(f"General Syntax Error, line {file_line}")
            else:
                ins = [ins, s[1], file_line]
                assembly.append(ins)
    else:
        send_error(f"General Syntax Error, line {file_line}")

file_name = "input.txt"
with open(file_name, "r") as fin:
    for line in fin:
        file_line += 1
        if line == "\n":
            continue
        if "\n" in line:
            line = line[:-1]
        line = line.split()
        get_ins(line)

f = open("output.txt", "w")

if "1101000000000000" not in assembly:
    f.write("Missing hlt instruction")
elif assembly[-1] != "1101000000000000":
    f.write("hlt not being used as last function\n")
elif len(assembly) + len(variables) > 128:
    f.write("Assembler instruction limit reached\n")
else:
    for var in variables.keys():
        line_num += 1
        variables[var]["mem"] = get_mem_addr(line_num)
    for i in range(0, len(assembly)):
        if len(assembly[i]) == 3:
            if assembly[i][1] not in labels:
                f.write(f"Misuse of Label, line {assembly[i][2]}\n")
                f.close()
                exit(0)
            else:
                assembly[i] = assembly[i][0] + labels[assembly[i][1]]["Line"]
        elif len(assembly[i]) == 4:
            assembly[i] = assembly[i][0] + assembly[i][1] + var_to_bin(assembly[i][2])
    for i in assembly:
        f.write(f"{i}\n")
f.close()
