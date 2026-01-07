
with open("oc.txt", encoding="utf8") as f:
    ct = f.read()

q = ct.find("data-opc")
while q != -1:
    afopc = ct.find('"', q + 9) + 1
    
    while ct[afopc:afopc+1].isspace():
        ct = ct[1:]

    opc = ct[afopc:afopc+2]
    ct = ct[afopc + 2:]
    
    while ct[0:1].isspace():
        ct = ct[1:]

    name = ct[2:5]
    print(name + ", " + opc)

    #

    q = ct.find("data-opc")
