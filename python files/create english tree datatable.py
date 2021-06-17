datatable = open("datatable.txt", "w+")

words = open('./Words.txt', "r")
tags = open('./tags.txt', "r")

taglist = [
        "NUL",
	"CC",
	"CD",
	"DT",
	"EX",
	"FW",
	"IN",
	"JJ",
	"JJR",
	"JJS",
	"LS",
	"MD",
	"NN",
	"NNS",
	"NNP",
	"NNPS",
	"PDT",
	"POS",
	"PRP",
	"PRP$",
	"RB",
	"RBR",
	"RBS",
	"RP",
	"SYM",
	"TO",
	"UH",
	"VB",
	"VBD",
	"VBG",
	"VBN",
	"VBP",
	"VBZ",
	"WDT",
	"WP",
	"WP$",
	"WRB",
	",",
	".",
	"$",
	":",
	";",
	"LQU",
	"RQU",
	"LRB",
	"RRB",
	"AS",
	"SO"
        ]

def get_features(words, tags, num):
    return {
        'word':words[num], #this line is ignored and is the word itself
        
        'isFirst':num == 0, #true if first word in sentence
        'isLast':num == (len(words) - 1), #true if last word in sentence
        'hyphen':'-' in words[num], #true if word contains hyphen
        'prevtag':0 if num == 0 else taglist.index(tags[num - 1]), #previous tag index
        'nexttag':0 if num == len(words) - 1 or tags[num + 1] not in ['TO', ',', '.', '$', ';', 'LQU', 'RQU', 'LRB', 'RRB', 'CD'] else taglist.index(tags[num + 1]),
        'pre1':ord(words[num][0]), #first letter in word
        'pre2':ord(words[num][1:2] if words[num][1:2] else '|'), #second letter in word or '|' if word is shorter than 2 letters
        'pre3':ord(words[num][2:3] if words[num][2:3] else '|'), #third letter in word or '|' if word is shorter than 3 letters
        'suf1':ord(words[num][-3:-2] if words[num][-3:-2] else '|'), #third to last letter in word or '|' if word is shorter
        'suf2':ord(words[num][-2:-1] if words[num][-2:-1] else '|'), #second to last letter in word or '|' if word is shorter
        'suf3':ord(words[num][-1:len(words[num])]), #last letter in word
        'tag':taglist.index(tags[num]) #tag index of word
        }

linenum = 0

while True:
    linenum += 1

    wordline = words.readline()
    tagline = tags.readline()

    curwords = wordline.split()
    curtags = tagline.split()

    if not wordline:
        break

    if len(curwords) != len(curtags):
        pass
    
    for i in range(len(curwords)):
        try:
            features = get_features(curwords, curtags, i)
        except Exception as e:
            print('Current Word Amount: ' + str(len(curwords)))
            print(curwords)
            print('')
            print('Current Tags Amount: ' + str(len(curtags)))
            print(curtags)
            print('')
            print(e)
            exit(-1)
        
        temp = ''


       
        for n in list(features)[1:]:
            temp += str(features[n]) + ' ' if type(features[n]) != bool else str(int(features[n])) + ' '
        

        
        print(str(linenum) + ': ' + str(i) + ': '+ temp)
        datatable.write(temp + '\n')

words.close()
tags.close()
