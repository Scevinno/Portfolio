
def Sentences(x):
    n = 0
    for i in x:
        if(i == '.' or i =='!' or i =='?'):
            n+=1
    return n

def Words(x):
    n = 0
    for i in x:
        if(i == ' '):
            n+=1
    return n

def Letters(x):
    n = 0
    for i in x:
        if(i != '.' and i !='!' and i !='?' and i != ' '):
            n+=1
    return n

i = input("Enter sentence: ")
s = Sentences(i)
w = Words(i)
l = Letters(i)
li = l/(w/100)
si = s/(w/100)
index = 0.0588 * li - 0.296 * si - 15.8
if (index < 1):
    print("Before Grade 1")
elif (index >= 16):
    print("Grade 16+")
else:
    print("Grade:", int(index))
