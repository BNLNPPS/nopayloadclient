raw_logs_works = []
raw_logs_fails = []


#with open('log_works.txt', 'r') as f:
with open('unpack_works.txt', 'r') as f:
    for line in f:
        try:
            line = line.split(" ")[1:]
            raw_logs_works.append(line)
        except:
            print('couldnt split line:')
            print(line)

        
#with open('log_fails.txt', 'r') as f:
with open('unpack_fails.txt', 'r') as f:
    for line in f:
        try:
            line = line.split(" ")[1:]
            raw_logs_fails.append(line)
        except:
            print('couldnt split line:')
            print(line)

        
for line in raw_logs_works:
    if line not in raw_logs_fails:
        print(line)
        

for line in raw_logs_fails:
    if line not in raw_logs_works:
        print(line)
