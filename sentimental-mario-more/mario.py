
def rowA(rowa):
    print("#"*(h+1))

def rowB():
    print("  ",end="")

def rowC(rowc,hgt):
    print(" "*(hgt-rowc-1),end="")
    print("#"*(h+1),end="")

while True:
    height = int(input(" Define height: "))
    if height >= 1:
        break

for h in range(height):
    rowC(h,height)
    rowB()
    rowA(h)
    h += 1
