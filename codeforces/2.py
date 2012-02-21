i, j = raw_input().split()
i = int(i)
j = int(j)
record = [];
patten = ['aabb', 'abab', 'abba', 'aaaa']
for a in range(0, i):
	cnt = 0;
	line1 = raw_input()
	line2 = raw_input()
	line3 = raw_input()
	line4 = raw_input()
	for letter in range(len(line1)-1, 0, -1):
		if line1[letter] in ['a', 'e', 'i', 'o', 'u']:
			cnt += 1;
		if(cnt == j):
			break;
	if(line1[-cnt-1:] == line2[-cnt-1:] and line3[-cnt-1:] == line4[-cnt-1:] and line2[-cnt-1:] ==
			line3[-cnt-1:]):
		record.append(3)
	elif(line1[-cnt-1:] == line2[-cnt-1:] and line3[-cnt-1:] == line4[-cnt-1:]):
		record.append(0)
	elif(line1[-cnt-1:] == line3[-cnt-1:] and line2[-cnt-1:] == line4[-cnt-1:]):
		record.append(1)
	elif(line1[-cnt-1:] == line4[-cnt-1:] and line3[-cnt-1:] == line2[-cnt-1:]):
		record.append(2)
	else:
		record.append(-1)

if -1 in record:
	print 'NO'
else:
	for t in range(0, len(record)):
		if record[t] != 3:
			temp = record[t]
			break;
		else:
			temp = 3
	if temp == 3:
		print patten[3]
	else:
		flag = 0;
		for t in range(0, len(record)):
			if temp != record[t] and record[t] != 3:
				print 'NO'
				flag = 1;
				break;
		if flag == 0:
			print patten[temp]
