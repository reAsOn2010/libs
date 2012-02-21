n = raw_input()
count = 0;
lenth = 0;
count_len = 0;
for i in range(0, len(n)):
	for j in range(i, len(n)+1):
		new = n[i:j]
		count = 0
		for letter in new:
			if letter == 'A' or letter == 'a':
				count += 1
			elif letter == 'E' or letter == 'e':
				count += 1
			elif letter == 'I' or letter == 'i':
				count += 1
			elif letter == 'o' or letter == 'O':
				count += 1
			elif letter == 'U' or letter == 'u':
				count += 1
		other = len(new) - count
		if count <= 2*other:
			if lenth < len(new):
				lenth = len(new)
				count_len = 1
			elif lenth == len(new):
				count_len += 1

print lenth
print count_len
