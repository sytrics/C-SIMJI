fichier = open("command.asm","r")
ligne = fichier.readline()
switcher = {
            "add": 0,
        }
bin = open("instruction.bin","w")
compteur = 0
while ligne!=0 :
    instruction = 0;
    if fichier.read(1)!=";" :  #pas un commentaire
        commande_char = ligne.split()
        arguments = commande_char[1].split(",")
        commande_int = switcher.get(commande_char[0],0)
        instruction+=commande_int<<27
        if 1<=commande_int<=14 :
            #instructions codeop
            instruction+=int(arguments[0][1])<<22
            if arguments[1][0].isalpha():
                imm=0
            else :
                imm=1
            instruction+=imm<<21
            if imm==0:
                instruction+=int(arguments[1][1])<<5
            else :
                instruction+=int(arguments[1][0])<<5
            instruction+=int(arguments[2][1])
        elif commande_int==15 :
            #instructions jmp
            if arguments[0][0].isalpha():
                imm=0
            else :
                imm=1
            instruction+=imm<<26
            if imm==0:
                instruction+=int(arguments[0][1])<<5
            else :
                instruction+=int(arguments[0][0])<<5
            instruction+=int(arguments[1][1])
        elif 16 <= commande_int <= 17:
            # instructions braz,branz
            instruction += int(arguments[0][1])<<22
            instruction += int(arguments[1][1])
        bin.write(hex(compteur)+" "+hex(instruction)+"\n")
        compteur+=1


fichier.close()
bin.close()





