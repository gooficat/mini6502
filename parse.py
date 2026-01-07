f : str = open("oc2.txt", "rt", encoding="utf8").read();
lines : list[str] = f.splitlines();

opcode : int = 0;

class Profile: #{
    def __init__(self, opcode : int, spec : str): #{
        self.opcode : int = opcode;
        self.spec : str = spec;
    #}
#}

class Opcode: #{
    def __init__(self, name : str): #{
        self.name : str = name;
        self.profs : list[Profile] = [];
    #}
#}

def find_spec(inp : str) -> str:
    print(f"'{inp}'")
    match(inp):
        case 'impl':
            return 'ADDR_IMPL'
        case 'abs':
            return 'ADDR_ABS'
        case 'abs,X':
            return 'ADDR_ABS_X'
        case 'abs,Y':
            return 'ADDR_ABS_Y'
        case 'zpg':
            return 'ADDR_ZPG'
        case 'zpg,X':
            return 'ADDR_ZPG_X'
        case 'zpg,Y':
            return 'ADDR_ZPG_Y'
        case '#':
            return 'ADDR_IMM'
        case 'rel':
            return 'ADDR_REL'
        case 'A':
            return 'ADDR_A'
        case 'ind':
            return 'ADDR_IND'
        case 'X,ind':
            return 'ADDR_X_IND'
        case 'ind,Y':
            return 'ADDR_IND_Y'
        case _:
            print("Error!")
            exit(1)

opcodes : list[Opcode] = []

for line in lines: #{
    if line != '---':
        name = line[0:line.find(' ')].lower()
        # print(f"{name} {hex(opcode)}");
    
        opl = None

        for op in opcodes:
            if op.name == name:
                opl = op;
        
        if opl == None:
            opcodes.append(Opcode(name));
            opl = opcodes[len(opcodes) - 1];
        
        pfl = Profile(opcode, find_spec(line[line.find(' ') + 1:]))

        opl.profs.append(pfl)
    #}

    opcode += 1;
#}

for op in opcodes:
    print(f"{op.name}:")
    for pf in op.profs:
        print('\t{' + str(pf.opcode) + ', ' + pf.spec + '},')

out = open("spec.c", "wt")

i : int = 0;
out.write('{\n')
for op in opcodes:
    out.write(f'\t{'{'}"{op.name}", {i}, {len(op.profs)} {'}'},\n')
    i += len(op.profs)

out.write('}')
