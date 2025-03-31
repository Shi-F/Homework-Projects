import re

result = []
temp  = input()
while temp != '':
    match1 = re.search('singer="(.*)">(.*)<',temp)
    if match1:
        singer = match1.group(1)
        song = match1.group(2)
        result.append(f'({singer},{song})')
    temp = input()
print(result)