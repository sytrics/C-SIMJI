
def init() :
    fichier = open("command.asm","r")
    assembly = fichier.read()
    fichier.close()
    ligne = assembly.split("\n")
    for i in ligne :
        i.strip()
    return ligne


# TODO => gerer les labels


def CPL2(nombre, taille=16):
        if (nombre >=0) : 
            return nombre
    
        else :  
            return (1<<(taille-1))-nombre
    
def label(line) : 
    if line == "" : 
        return None 
    args = line.split(" ")
    print(line)
    if args[0][-1] == ":" : 
        return args[0][:-1], " ".join(args[1:])
    else: 
        return None 
    
def searchlabel(lignes) : 
    labels = {}
    for k,ligne in enumerate(lignes) : 
        l = label(ligne)
        if l is not None : 
            labels[l[0]] = k 
    return labels 




def bitstream_gen(lignes, labels) :

    switcher = {
        "add": 1,
        "sub": 2,
        "mult": 3,
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
        "stop": 19
    }

    compteur = 0
    bitstream = ""
    for i in lignes :
        l = label(i)
        if l is not None:
            
            label_var, i = l  #si un label est trouvé on recupère le reste de la ligne dans i 

        instruction = 0
        if len(i)==0 :
            break
        if i[0]!=";" :  #pas un commentaire
            commande_char = i.split()
            commande_int = switcher.get(commande_char[0], 0)

            if commande_int==18 :
                arguments = commande_char[1]
            elif commande_int==19 :
                arguments = 0
            else :
                arguments = commande_char[1].split(",")

            instruction+=commande_int<<27

            if 1<=commande_int<=14 :
                #instructions codeop
                instruction+=int(arguments[0][1])<<22
                if arguments[1][0]=='r':
                    imm=0
                else :
                    imm=1
                instruction+=imm<<21
                if imm==0:
                    instruction+=int(arguments[1][1:])<<5
                else :
                    instruction+=CPL2(int(arguments[1][0:]),16)<<5
                instruction+=int(arguments[2][1])

            elif commande_int==15 :
                #instructions jmp
                if arguments[0][0]=="r":
                    imm=0
                else :
                    imm=1
                instruction+=imm<<26
                if imm==0:
                    #le O n'est pas immédiat 
                    o = int(arguments[0][1:])
                    instruction+=o<<5
                else :
                    #le o est immédiat, ie c'est un label 
                    o = int(labels[arguments[0]])
                    instruction+=o<<5
                instruction+=int(arguments[1][1:])

            elif 16 <= commande_int <= 17:
                # instructions braz,branz
                instruction += int(arguments[0][1])<<22
                instruction += int(arguments[1][1])

            elif commande_int==18 :
                # instruction scall
                instruction +=int(arguments[0])

            elif commande_int==19 :
                # instruction stop
                pass
            iteration = '0x%08X' % compteur+" "+ '0x%08X' % instruction
            print("instruction n°"+str(compteur)+" : " +i+ "----->add line : " + iteration)
            bitstream += (iteration + "\n")
            compteur+=1
    return bitstream

content = init()
bin = open("instruction.bin","w")
labels = searchlabel(content)
lignes = bitstream_gen(content, labels)
bin.write(lignes)
bin.close()





