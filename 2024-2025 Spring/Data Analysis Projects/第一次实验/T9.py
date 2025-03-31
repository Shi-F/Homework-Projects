import re

result = []
pattern = r'http[s]?://(?:[a-zA-Z]|[0-9]|[$-_@.&+]|[!*\\(\\),]|(?:%[0-9a-fA-F][0-9a-fA-F]))+'
temp  = input()
while temp != '':
    urls = re.findall(pattern,temp)
    result += urls
    temp = input()
print(result)