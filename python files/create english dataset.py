#this file was meant to read over the giant amount of files from one of the CONLL 
#conferences to extract all of the training data for part of speech training from the files.
import glob

#create files for writing the words and tags from each dataset to
data = open("Words.txt", "w")
tags = open("tags.txt", "w")

#find all files in the dataset, each file ends in .tags
trainingData = (glob.glob('./**/*.tags', recursive=True))

#loop over dataset files
for i in trainingData:
    readfile = open(i, "r")
    
    sentence = ""
    tagList = ""
    
    #read each line in each file
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
        
        #there are two columns in each file one is the words and the other is the tags
        for i in line:
            #check if the token is the tag of the period in the end of the sentence
            #and write all of the words and tags to their respective files, then 
            #move on to the next file
            if i == "." and count == 1:
                tagList += i
                print(sentence + "\n" + tagList)
                data.write(sentence + "\n")
                tags.write(tagList + "\n")
                sentence = ""
                tagList = ""
                count = 0
                break
                
            #add the word to the sentence if the whitespace character
            #separating the word and the tag is encountered
            elif i == "\t" and count == 0:
                sentence += word + " "
                word = ""
                count += 1
                
            #add the tag to the taglist if the whitespace after the tag is encountered
            elif i == "\t" and count == 1:
                tagList += tag + " "
                tag = ""
                count = 0
                break
            
            #if the whitespace has not been encountered, add the current character to the current word
            elif count == 0:
                word += i
                
            #if the whitespace has been encountered, add the current character to the current tag
            else:
                tag += i
        
        
                
                
    
