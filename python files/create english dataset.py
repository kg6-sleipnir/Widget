import glob

data = open("Words.txt", "w")
tags = open("tags.txt", "w")

trainingData = (glob.glob('./**/*.tags', recursive=True))

for i in trainingData:
    readfile = open(i, "r")
    
    sentence = ""
    tagList = ""
    
    while True:
        try:
            line = readfile.readline()
        except:
            pass
        if not line:
            break
            
        word = ""
        tag = ""
        
        count = 0
        for i in line:
            if i == "." and count == 1:
                tagList += i
                print(sentence + "\n" + tagList)
                data.write(sentence + "\n")
                tags.write(tagList + "\n")
                sentence = ""
                tagList = ""
                count = 0
                break
            elif i == "\t" and count == 0:
                sentence += word + " "
                word = ""
                count += 1
            elif i == "\t" and count == 1:
                tagList += tag + " "
                tag = ""
                count = 0
                break
            elif count == 0:
                word += i
            else:
                tag += i
        
        
                
                
    