words = open('./Words.txt', "r")
tags = open('./tags.txt', "r")

dictionary = open("./dictionary.txt", "a+")

print("done opening objects")

input()

while True:
    wordline = words.readline()
    tagline = tags.readline()
    
    if not wordline:
        break
        
    curwords = wordline.split()
    curtags = tagline.split()
    
    if len(curwords) != len(curtags):
        pass
    
    
    for i in range(len(curwords)):
        dictionary.seek(0)
        if curwords[i] in dictionary.read():
            if curtags[i] not in dictionary.readline(
            pass
        else:
            print("Found Word: " + curwords[i] + " With Tag: " + curtags[i] + "\n")
            dictionary.write(curwords[i] + " " + curtags[i] + "\n")

