import sys #used for asm argument 

def init() :
    fichier = open("asm/"+ sys.argv[1],"r")
    assembly = fichier.read()
    fichier.close()
    ligne = assembly.split("\n")
    for i in ligne :
        i.strip()
    return ligne  # reading file in one go and storing everything in memory


def CPL2(nombre, taille=16):
    #converts signed int into two's complement binary representation
        if (nombre >=0) : 
            return nombre
    
        else :  
            return (1<<(taille-1))-nombre
    
def label(line) : # search a label in a line and returns it 
    if line == "" : 
        return None 
    args = line.split(" ")
    if args[0][-1] == ":" : 
        return args[0][:-1], " ".join(args[1:])
    else: 
        return None 
    
def searchlabel(lignes) : # find all labels of the file and creates a dictionary of labels 
    labels = {}
    for k,ligne in enumerate(lignes) : 
        l = label(ligne)
        if l is not None : 
            labels[l[0]] = k 
    return labels 




def bitstream_gen(lignes, labels) : # generate the bitstream of instructions 

    switcher = {  # MiniMIPS instruction set 
        "add": 1,
        "sub": 2,
        "mul": 3,
        "div": 4,
        "and": 5,
        "or": 6,
        "xor": 7,
        "shl": 8,
        "shr": 9,
        "slt": 10,
        "sle": 11,
        "seq": 12,
        "load": 13,
        "store": 14,
        "jmp": 15,
        "braz": 16,
        "branz": 17,
        "scall": 18,
        "stop": 0
    }

    compteur = 0  
    bitstream = ""
    for i in lignes :
        l = label(i)
        if l is not None:
            
            label_var, i = l  #if a label is found, it modifies i to remove the label

        instruction = 0
        if len(i)==0 :
            break
        if i[0]!=";" :  #not a commment line (DOES NOT WORK, messing with label indexes)
            commande_char = i.split()
            commande_int = switcher.get(commande_char[0], 0)
            # parsing instructions into command and arguments 
            if commande_int==18 :
                arguments = commande_char[1]
            elif commande_int==0 :
                arguments = 0
            else :
                arguments = commande_char[1].split(",")

            instruction+=commande_int<<27

            if 1<=commande_int<=14 :
                #instructions codeop
                instruction+=int(arguments[0][1:])<<22
                if arguments[1][0]=='r':
                    imm=0
                else :
                    imm=1
                instruction+=imm<<21
                if imm==0:
                    instruction+=int(arguments[1][1:])<<5
                else :
                    instruction+=CPL2(int(arguments[1][0:]),16)<<5
                instruction+=int(arguments[2][1:])

            elif commande_int==15 :
                #instructions jmp
                if arguments[0][0]=="r":
                    imm=0
                else :
                    imm=1
                instruction+=imm<<26
                if imm==0:
                    #o is not immediate 
                    o = int(arguments[0][1:])
                    instruction+=o<<5
                else :
                    #o is immediate, it's a label 
                    o = int(labels[arguments[0]])
                    instruction+=o<<5
                instruction+=int(arguments[1][1:])

            elif 16 <= commande_int <= 17:
                # instructions braz,branz
                instruction += int(arguments[0][1:])<<22
                instruction += int(labels[arguments[1]])

            elif commande_int==18 :
                # instruction scall
                instruction +=int(arguments[0])

            elif commande_int==0 :
                # instruction stop
                pass
            iteration = '0x%08X' % compteur+" "+ '0x%08X' % instruction
            print("instruction n°"+str(compteur)+" : " +i+ "----->add line : " + iteration)
            bitstream += (iteration + "\n")
            compteur+=1
    return bitstream


content = init() #reading the asm file
bin = open("bin/instruction.bin","w") #opening the instruction file to be written
labels = searchlabel(content) # searching labels into asm 
lignes = bitstream_gen(content, labels) # creating instructions corresponding to asm
bin.write(lignes) # writting instructions in one go
bin.close()





