#!/usr/bin/python3

import subprocess
import os
import re

app="./main"

root = '.'
for vid in os.listdir(root):
	if len(re.findall('.mp4$', vid)) < 1:
		continue

	result = subprocess.run([app, os.path.join(root,vid)], stdout=subprocess.PIPE, cwd='/home/fgrando/Downloads/object-counter/main/build')
	output = result.stdout.decode('utf-8')
	result = re.findall('(?<=result \{).*(?=\}\n)', output, re.DOTALL)
	if len(result) > 0:
		data = result[0].split('\n')
		score = {}
		for d in data:
			if len(d) < 2:
				continue
			pair = d.split()
			score[pair[0]]=int(pair[1])

#		print(score)
		if score['por1'] + score['por2'] + score['c2'] + score['gar2'] > 3:
			print("something detected")
