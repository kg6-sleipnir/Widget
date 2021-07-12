words = open('./Words.txt', "r")
tags = open('./tags.txt', "r")

tagDict = {
    'EX': {},
    '$': {},
    'AS': {},
    'SO': {},
    ':': {},
    ';': {},
    'SYM': {},
    'TO': {},
    'WDT': {},
    'WP': {},
    'WP$': {},
    'WRB': {}
    }
    


while True:
    wordline = words.readline()
    tagline = tags.readline()

    curwords = wordline.split()
    curtags = tagline.split()

    if not wordline:
        break

    if len(curwords) != len(curtags):
        pass

    for i in range(len(curwords)):
        if curtags[i] in tagDict:
            if curwords[i] in tagDict[curtags[i]]:
                tagDict[curtags[i]][curwords[i]] += 1
            else:
                tagDict[curtags[i]][curwords[i]] = 1

for i in tagDict:
    for n in tagDict[i]:
        print(i + ': ' + n + ': ' + str(tagDict[i][n]))
    print('')


            
